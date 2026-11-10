#!/usr/bin/env python3
"""The accepted ledger: proof-backed matches that cannot be lost or faked without the project noticing.

Four versioned files, written only by tools/hstpromote.py:

  data/target-manifest.json     TARGET LOCK. The game release; for each of the six decrypted modules its SHA-256, size,
                                ELF program headers and sections, the load segments the matcher maps, and the reviewed
                                function inventory (count, bytes, digest over the list). `target_digest` covers all of
                                it. Changed only by `hstpromote.py retarget --reason`.
  data/toolchain-manifest.json  TOOLCHAIN LOCK. SHA-256 of everything that can change compiler output or the verdict:
                                mwccpsp build 219 (and its DLL) and wibo, every file of both Sony psp-gcc kits, the PSPDEV
                                psp-ld/psp-objcopy/psp-objdump (and their ldscripts), the external specs directory the
                                psp-gcc driver searches, the compiler environment inside WSL, include/, the compiler
                                profiles and flags, and the comparison code (COMPARISON_CODE). Hashes and kit-relative
                                names only, no local paths. `core_digest` covers it. Changed only by
                                `hstpromote.py migrate-toolchain --reason`, which reverifies every accepted proof.
  data/accepted-ledger.jsonl    ACCEPTED LEDGER. A header line, then one proof per line, sorted by module and start.
                                Ranges never overlap: each byte has one owner, so coverage counts it once.
  data/accepted-journal.jsonl   Append-only, hash-chained record of every change to the ledger (seed, promote,
                                migrate-toolchain, retarget, retract). The ledger header names the journal line that
                                produced it, so a hand edit of either file is detected.

A PROOF exists only because a verification run of tools/hstpromote.py produced it: module, claimed byte range, symbol,
source path and SHA-256, the dependency record (headers it includes, and the address every undefined name it links got:
the symbol lines it uses) with its digest, the toolchain and target digests, the sections compared with their SHA-256,
the object SHA-256, status EXACT, the hstgate result, the run id.

Symbol maps are bound PER PROOF rather than in the toolchain lock (symbol lines are appended with most harvests): a proof
whose recorded resolutions no longer hold is stale, and the next promotion reverifies it. Everything else that can change
output is in the toolchain lock.

    python tools/hstproof.py status                  digests and a ledger summary
    python tools/hstproof.py check [--base REV] [--staged] [--quick]
        Fast audit, no compiling: the manifests against the live target and toolchain, ledger and journal integrity and
        binding, every proof against the tree (source SHA-256, dependencies, gate, inventory alignment). --base REV: the
        ledger at REV is a subset of this one (except recorded retractions), its journal a prefix, and any manifest change
        since REV has its migration event. --staged: the git index holds exactly the sources the proofs name.
        --quick skips hashing the toolchain (for status displays only; never in the push procedure).
    python tools/hstproof.py coverage [--json FILE]  coverage counted from accepted ranges only (each byte once)
    python tools/hstproof.py spans MODULE            the merged accepted spans of one module

API (other tools call these; they read and validate the ledger and journal, not the tree):
    accepted_proofs(module=None) -> list[dict]       proofs, sorted by (module, start)
    accepted_spans(module) -> list[(start, end)]      merged union of the accepted ranges of `module`
    proven_sources() -> dict[str, dict]              source path -> proof
    accepted_coverage() -> dict                      per module and total: scope, inventory, accepted bytes/functions
    ledger_present() -> bool
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
import re
import struct
import subprocess
import sys
import time
from dataclasses import dataclass, field
from pathlib import Path

TOOLS = Path(__file__).resolve().parent
ROOT = TOOLS.parent
sys.path.insert(0, str(TOOLS))
import hstgate  # noqa: E402
import hstmatch as M  # noqa: E402

DATA = ROOT / "data"
TARGET_PATH = DATA / "target-manifest.json"
TOOLCHAIN_PATH = DATA / "toolchain-manifest.json"
LEDGER_PATH = DATA / "accepted-ledger.jsonl"
JOURNAL_PATH = DATA / "accepted-journal.jsonl"
LEDGER_FILES = ("data/target-manifest.json", "data/toolchain-manifest.json", "data/accepted-ledger.jsonl",
                "data/accepted-journal.jsonl")

TARGET_SCHEMA = "hst-decomp.target-manifest.v1"
TOOLCHAIN_SCHEMA = "hst-decomp.toolchain-manifest.v1"
LEDGER_SCHEMA = "hst-decomp.accepted-ledger.v1"
JOURNAL_SCHEMA = "hst-decomp.accepted-journal.v1"
PROOF_SCHEMA = "hst-decomp.proof.v1"

# The code whose behaviour decides a verdict. Any change to one of these files is a toolchain change.
COMPARISON_CODE = ("tools/hstmatch.py", "tools/hstgate.py", "tools/hstconfig.py", "tools/hstproof.py",
                   "tools/hstpromote.py",
                   # where cc=gcc404x-* is allowed (tools/hstgate.py reads it; owner decision 2026-11-09)
                   "data/sdk-gcc4-units.json")
SOURCE_SUFFIXES = (".c", ".cpp")
DEFAULT_CC = "mwcc219"
# where the Sony psp-gcc driver looks for its pspspecs/pspfixup helpers (it warns that they are absent); a file appearing
# there could change what the driver does, so the toolchain lock records the directory's state
PSPGCC_SPECS_DIR = "/usr/local/psp/devkit/lib"
# compiler-relevant environment variables inside WSL (values are recorded as hashes)
COMPILER_ENV = re.compile(r"^(GCC_\w+|COMPILER_PATH|LIBRARY_PATH|CPATH|C_INCLUDE_PATH|CPLUS_INCLUDE_PATH|"
                          r"OBJC_INCLUDE_PATH|DEPENDENCIES_OUTPUT|SUNPRO_DEPENDENCIES|MWC\w*|SCE\w*|PSP\w*|WIBO\w*|"
                          r"LD_\w+|TMPDIR|LANG|LC_\w+)=", re.M)

PROOF_KEYS = ("schema", "module", "start", "end", "symbol", "source", "source_sha256", "cc", "lang", "deps",
              "deps_sha256", "toolchain", "target", "compared", "object_sha256", "status", "gate", "run",
              "verified_utc", "proof_sha256")
HEX8 = re.compile(r"0x[0-9a-f]{8}")
SHA = re.compile(r"[0-9a-f]{64}")


class ProofError(Exception):
    """A refusal. `code` is a short kind (target-drift, toolchain-drift, ledger, journal, mismatch, ...)."""

    def __init__(self, code: str, message: str):
        super().__init__(f"{code}: {message}")
        self.code = code
        self.message = message


# --------------------------------------------------------------------------------------------------------------
# primitives

def canon(obj) -> bytes:
    return json.dumps(obj, sort_keys=True, separators=(",", ":"), ensure_ascii=True).encode("ascii")


def sha256(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def digest(obj) -> str:
    return sha256(canon(obj))


def file_sha256(path: Path) -> str:
    return sha256(path.read_bytes())


def hx(value: int) -> str:
    return f"0x{value:08x}"


def unhx(text: str) -> int:
    if not isinstance(text, str) or not HEX8.fullmatch(text):
        raise ProofError("ledger", f"bad address {text!r}")
    return int(text, 16)


def utc_now() -> str:
    return time.strftime("%Y-%m-%dT%H:%M:%SZ", time.gmtime())


def rel(path: Path, root: Path = ROOT) -> str:
    return path.resolve().relative_to(root.resolve()).as_posix()


def to_wsl(path: Path) -> str:
    """Host path -> /mnt/<drive>/... (this module's own conversion; hstmatch has its own)."""
    p = str(path.resolve()).replace("\\", "/")
    if len(p) < 3 or p[1] != ":":
        raise ProofError("path", f"not a drive path: {p}")
    return f"/mnt/{p[0].lower()}{p[2:]}"


def run_wsl_script(text: str, workdir: Path, name: str) -> str:
    """Run a POSIX sh script inside WSL (like hstmatch does) and return its standard output."""
    workdir.mkdir(parents=True, exist_ok=True)
    script = workdir / f"{name}.{os.getpid()}.sh"
    script.write_text("#!/bin/sh\n" + text + "\n", encoding="utf-8", newline="\n")
    try:
        r = subprocess.run(["wsl.exe", "-e", "sh", to_wsl(script)], capture_output=True, check=False)
    finally:
        script.unlink(missing_ok=True)
    if r.returncode != 0:
        raise ProofError("wsl", f"{name} script failed ({r.returncode}): {r.stderr.decode(errors='replace')[-400:]}")
    return r.stdout.decode("utf-8", errors="replace")


# --------------------------------------------------------------------------------------------------------------
# a minimal ELF32 little-endian reader of our own (independent of hstmatch's)

@dataclass(frozen=True)
class Section:
    index: int
    name: str
    type: int
    flags: int
    addr: int
    offset: int
    size: int
    link: int
    info: int
    align: int = 1


@dataclass(frozen=True)
class Symbol:
    index: int
    name: str
    value: int
    size: int
    info: int
    shndx: int


@dataclass(frozen=True)
class Reloc:
    offset: int
    type: int
    sym: int


SHT_PROGBITS, SHT_SYMTAB, SHT_RELA, SHT_NOBITS, SHT_REL = 1, 2, 4, 8, 9
STT_SECTION, STT_FILE = 3, 4
SHF_ALLOC = 2
PT_LOAD, PF_X = 1, 1


class Elf:
    def __init__(self, data: bytes, what: str = "ELF file"):
        self.what = what
        if len(data) < 52 or data[:4] != b"\x7fELF" or data[4] != 1 or data[5] != 1:
            raise ProofError("elf", f"{what} is not a 32-bit little-endian ELF file")
        self.data = data
        try:
            phoff, shoff = struct.unpack_from("<II", data, 0x1C)
            phentsize, phnum, shentsize, shnum, shstrndx = struct.unpack_from("<HHHHH", data, 0x2A)
            raw = [struct.unpack_from("<10I", data, shoff + i * shentsize) for i in range(shnum)] if shoff else []
            self.phdrs = [struct.unpack_from("<8I", data, phoff + i * phentsize) for i in range(phnum)] if phoff else []
            names = raw[shstrndx] if raw else None
            self.sections = []
            for i, s in enumerate(raw):
                nm = ""
                if names is not None:
                    at = names[4] + s[0]
                    nm = data[at:data.index(b"\0", at)].decode("latin-1")
                self.sections.append(Section(i, nm, s[1], s[2], s[3], s[4], s[5], s[6], s[7], max(s[8], 1)))
        except (struct.error, ValueError, IndexError) as e:
            raise ProofError("elf", f"{what}: malformed ELF ({e})") from None
        self._symbols: list[Symbol] | None = None

    def section(self, name: str) -> Section | None:
        hits = [s for s in self.sections if s.name == name]
        if len(hits) > 1:
            raise ProofError("elf", f"{self.what} has {len(hits)} sections named {name}")
        return hits[0] if hits else None

    def named(self, name: str, kind: int | None = None) -> list[Section]:
        """Every section called `name`, in file order (MWCC emits one `.text` per function)."""
        return [s for s in self.sections if s.name == name and (kind is None or s.type == kind)]

    def placements(self, name: str, base: int) -> list[tuple[Section, int]]:
        """Where a linker that starts at `base` puts the object's input sections called `name`: in file order, each
        at the next address aligned to its sh_addralign (ld's rule for one input file)."""
        out, addr = [], base
        for s in self.named(name, SHT_PROGBITS):
            addr = (addr + s.align - 1) // s.align * s.align
            out.append((s, addr))
            addr += s.size
        return out

    def contents(self, sec: Section) -> bytes:
        if sec.type == SHT_NOBITS:
            return b""
        if sec.offset + sec.size > len(self.data):
            raise ProofError("elf", f"{self.what}: section {sec.name} runs past the end of the file")
        return self.data[sec.offset:sec.offset + sec.size]

    def symbols(self) -> list[Symbol]:
        if self._symbols is None:
            tabs = [s for s in self.sections if s.type == SHT_SYMTAB]
            out: list[Symbol] = []
            for tab in tabs:
                strtab = self.sections[tab.link]
                for i in range(tab.size // 16):
                    st_name, value, size, info, _other, shndx = struct.unpack_from("<IIIBBH", self.data, tab.offset + i * 16)
                    at = strtab.offset + st_name
                    out.append(Symbol(i, self.data[at:self.data.index(b"\0", at)].decode("latin-1"), value, size, info, shndx))
            self._symbols = out
        return self._symbols

    def relocations(self, target: Section) -> list[Reloc]:
        out = []
        for s in self.sections:
            if s.type == SHT_RELA and s.info == target.index:
                raise ProofError("relocation", f"{self.what}: RELA relocations for {target.name} (not produced by these compilers)")
            if s.type == SHT_REL and s.info == target.index:
                for k in range(s.size // 8):
                    off, info = struct.unpack_from("<II", self.data, s.offset + k * 8)
                    out.append(Reloc(off, info & 0xFF, info >> 8))
        return out


# the bits each MIPS relocation type may change in the word it applies to
RELOC_FIELD = {0: 0x00000000,   # R_MIPS_NONE
               2: 0xFFFFFFFF,   # R_MIPS_32
               4: 0x03FFFFFF,   # R_MIPS_26
               5: 0x0000FFFF,   # R_MIPS_HI16
               6: 0x0000FFFF,   # R_MIPS_LO16
               7: 0x0000FFFF,   # R_MIPS_GPREL16
               10: 0x0000FFFF}  # R_MIPS_PC16


def masked_difference(obj: bytes, linked: bytes, relocs: list[Reloc]) -> int | None:
    """Offset of the first byte where `linked` differs from `obj` outside the relocation fields (None: no difference).
    Proves the linked bytes are the object's own bytes with only relocation fields filled in."""
    if len(obj) != len(linked):
        return min(len(obj), len(linked))
    keep = bytearray(b"\xff" * len(obj))
    for r in relocs:
        if r.type not in RELOC_FIELD:
            raise ProofError("relocation", f"relocation type {r.type} at +0x{r.offset:x} is not one this check knows")
        if r.offset % 4 or r.offset + 4 > len(obj):
            raise ProofError("relocation", f"relocation at +0x{r.offset:x} is not an aligned word of the section")
        word = struct.unpack_from("<I", keep, r.offset)[0] & ~RELOC_FIELD[r.type] & 0xFFFFFFFF
        struct.pack_into("<I", keep, r.offset, word)
    for i, (a, b, k) in enumerate(zip(obj, linked, keep)):
        if (a ^ b) & k:
            return i
    return None


# --------------------------------------------------------------------------------------------------------------
# ranges

def merge(ranges) -> list[tuple[int, int]]:
    out: list[list[int]] = []
    for s, e in sorted(ranges):
        if out and s <= out[-1][1]:
            out[-1][1] = max(out[-1][1], e)
        else:
            out.append([s, e])
    return [(s, e) for s, e in out]


def subtract(a, b) -> list[tuple[int, int]]:
    """Bytes of the ranges `a` not covered by the ranges `b`, merged."""
    b = merge(b)
    out = []
    for s, e in merge(a):
        cur = s
        for bs, be in b:
            if be <= cur or bs >= e:
                continue
            if bs > cur:
                out.append((cur, bs))
            cur = max(cur, be)
            if cur >= e:
                break
        if cur < e:
            out.append((cur, e))
    return out


def length(ranges) -> int:
    return sum(e - s for s, e in merge(ranges))


def intersect_length(a, b) -> int:
    a, b = merge(a), merge(b)
    i = j = n = 0
    while i < len(a) and j < len(b):
        lo, hi = max(a[i][0], b[j][0]), min(a[i][1], b[j][1])
        if lo < hi:
            n += hi - lo
        if a[i][1] < b[j][1]:
            i += 1
        else:
            j += 1
    return n


# --------------------------------------------------------------------------------------------------------------
# target lock

_campaign_cache: dict = {}


def campaign_ledger() -> dict:
    if "ledger" not in _campaign_cache:
        path = M.campaign_root() / "ledger" / "campaign.json"
        _campaign_cache["ledger"] = json.loads(path.read_text(encoding="utf-8-sig"))
    return _campaign_cache["ledger"]


def module_image(name: str) -> bytes:
    """The decrypted module exactly as tools/hstmatch.py reads it."""
    data, _seg = M.module_bytes(_campaign_cache.setdefault("images", {}), name)
    return data


def read_functions_file(path: Path) -> list[tuple[int, int]]:
    out = []
    for line in path.read_text(encoding="utf-8").splitlines():
        line = line.split("#", 1)[0].strip()
        if line:
            s, e = line.split()
            out.append((int(s, 16), int(e, 16)))
    return sorted(out)


def inventory_digest(functions) -> str:
    return sha256("".join(f"{s:08x} {e:08x}\n" for s, e in sorted(functions)).encode("ascii"))


def live_inventory(module: str) -> list[tuple[int, int]]:
    m = next((m for m in campaign_ledger()["modules"] if m["name"] == module), None)
    if m is None:
        raise ProofError("target-drift", f"the campaign ledger has no module {module}")
    return sorted((f["start"], f["end"]) for f in m["functions"])


def live_target(root: Path = ROOT) -> dict:
    """The target identity as it is now: data/modules.json, the campaign ledger, the decrypted modules the matcher
    reads, and data/functions-*.txt. Raises ProofError when these sources disagree with each other."""
    doc = json.loads((root / "data" / "modules.json").read_text(encoding="utf-8"))
    game_modules = {m["name"]: m for m in doc["modules"]}
    led = campaign_ledger()
    problems = []
    modules = []
    for m in sorted(led["modules"], key=lambda x: x["name"]):
        data = module_image(m["name"])
        digest_ = sha256(data)
        fns = sorted((f["start"], f["end"]) for f in m["functions"])
        segs = [{k: s[k] for k in ("start", "file_offset", "file_end", "memory_end", "flags")} for s in m["load_segments"]]
        if m.get("input_sha256") and m["input_sha256"] != digest_:
            problems.append(f"{m['name']}: the image read ({digest_[:16]}) is not the ledger's input ({m['input_sha256'][:16]})")
        gm = game_modules.pop(m["name"], None)
        if gm:
            if gm["sha256"] != digest_ or gm["size"] != len(data):
                problems.append(f"{m['name']}: the image read is not the one data/modules.json names")
            if read_functions_file(root / gm["functions_file"]) != fns:
                problems.append(f"{gm['functions_file']} differs from the campaign ledger's function list")
            if gm.get("function_count") != len(fns):
                problems.append(f"data/modules.json function_count of {m['name']} is {gm.get('function_count')}, the list has {len(fns)}")
            if [{k: s[k] for k in ("start", "file_offset", "file_end", "memory_end", "flags")} for s in gm["load_segments"]] != segs:
                problems.append(f"{m['name']}: load segments differ between data/modules.json and the campaign ledger")
        elf = Elf(data, m["name"])
        modules.append({
            "name": m["name"], "sha256": digest_, "size": len(data), "load_segments": segs,
            "program_headers": [dict(zip(("type", "offset", "vaddr", "paddr", "filesz", "memsz", "flags", "align"), ph))
                                for ph in elf.phdrs],
            "sections": [{"name": s.name, "type": s.type, "flags": s.flags, "addr": s.addr, "offset": s.offset, "size": s.size}
                         for s in elf.sections if s.index],
            "inventory": {"count": len(fns), "bytes": sum(e - s for s, e in fns), "sha256": inventory_digest(fns),
                          "source": gm["functions_file"] if gm else "campaign ledger (reviewed boundaries)"},
        })
    if game_modules:
        problems.append(f"data/modules.json names modules the campaign ledger lacks: {', '.join(game_modules)}")
    if problems:
        raise ProofError("target-drift", "; ".join(problems))
    game = {k: doc[k] for k in ("title", "title_id", "region", "disc_version", "disc_image_sha256")}
    return {"schema": TARGET_SCHEMA, "game": game, "modules": modules,
            "scope_bytes": sum(m["size"] for m in modules),
            "inventory_functions": sum(m["inventory"]["count"] for m in modules),
            "inventory_bytes": sum(m["inventory"]["bytes"] for m in modules)}


def target_identity(manifest: dict) -> dict:
    return {k: v for k, v in manifest.items() if k not in ("target_digest", "history")}


def load_target_manifest(path: Path = TARGET_PATH) -> dict:
    if not path.exists():
        raise ProofError("target-manifest", f"{path.name} does not exist (create the locks with tools/hstpromote.py lock)")
    doc = json.loads(path.read_text(encoding="utf-8"))
    if doc.get("schema") != TARGET_SCHEMA:
        raise ProofError("target-manifest", f"{path.name}: unknown schema {doc.get('schema')!r}")
    if digest(target_identity(doc)) != doc.get("target_digest"):
        raise ProofError("target-manifest", f"{path.name} was edited: its content does not match its target_digest")
    return doc


def describe_target_diff(old: dict, new: dict) -> str:
    out = []
    if old.get("game") != new.get("game"):
        out.append("game release fields differ")
    om = {m["name"]: m for m in old.get("modules", [])}
    nm = {m["name"]: m for m in new.get("modules", [])}
    for name in sorted(set(om) | set(nm)):
        a, b = om.get(name), nm.get(name)
        if a is None or b is None:
            out.append(f"module {name} {'added' if a is None else 'removed'}")
            continue
        for key in ("sha256", "size", "load_segments", "program_headers", "sections", "inventory"):
            if a.get(key) != b.get(key):
                if key == "inventory":
                    out.append(f"{name} inventory: {a[key]['count']} functions / {a[key]['bytes']} bytes locked, "
                               f"{b[key]['count']} / {b[key]['bytes']} now")
                else:
                    out.append(f"{name} {key} differs")
    for key in ("scope_bytes", "inventory_functions", "inventory_bytes", "schema"):
        if old.get(key) != new.get(key):
            out.append(f"{key}: locked {old.get(key)}, now {new.get(key)}")
    return "; ".join(out) or "identity differs"


def verify_target(manifest: dict, live: dict | None = None) -> dict:
    live = live if live is not None else live_target()
    if digest(live) != manifest["target_digest"]:
        raise ProofError("target-drift", "the target is not the locked one: " + describe_target_diff(target_identity(manifest), live))
    return live


def inventory(module: str, manifest: dict) -> list[tuple[int, int]]:
    """The live function list of `module`, checked against the locked inventory digest."""
    fns = live_inventory(module)
    locked = next((m for m in manifest["modules"] if m["name"] == module), None)
    if locked is None:
        raise ProofError("target-drift", f"{module} is not in the target manifest")
    if inventory_digest(fns) != locked["inventory"]["sha256"]:
        raise ProofError("target-drift", f"{module}: the function inventory is not the locked one")
    return fns


def image_range(module: str, start: int, end: int, manifest: dict, executable: bool) -> bytes:
    """Retail bytes [start, end) of `module`, mapped through the module's own ELF program headers (independent of the
    ledger's segment records), from the image whose SHA-256 the target manifest locks."""
    data = module_image(module)
    locked = next((m for m in manifest["modules"] if m["name"] == module), None)
    checked = _campaign_cache.setdefault("checked", {})
    if locked is None or checked.get((module, id(data))) != locked["sha256"]:
        if locked is None or sha256(data) != locked["sha256"]:
            raise ProofError("target-drift", f"{module}: the image is not the locked one")
        checked[(module, id(data))] = locked["sha256"]   # hashed once per image and lock
    for ph in locked["program_headers"]:
        if ph["type"] != PT_LOAD or (executable and not ph["flags"] & PF_X):
            continue
        if ph["vaddr"] <= start and end <= ph["vaddr"] + ph["filesz"]:
            off = ph["offset"] + start - ph["vaddr"]
            return data[off:off + end - start]
    raise ProofError("range", f"{module} [{hx(start)}, {hx(end)}) is not inside one file-backed "
                              f"{'executable ' if executable else ''}load segment")


# --------------------------------------------------------------------------------------------------------------
# toolchain lock

def _kit_relative(path: str, home: str) -> str:
    home = home.rstrip("/") + "/"
    if not path.startswith(home):
        raise ProofError("toolchain", f"profile path {path!r} is not under the configured kit directory")
    return path[len(home):]


def live_profiles() -> dict:
    profiles = {DEFAULT_CC: {"compiler": "mwcc", "build": M.PROFILE["build"], "flags": M.PROFILE["flags"],
                             "lang_option": "-lang", "include_env": "MWCIncludes=<repository>/include"}}
    home = M.gcc_home()
    for name, g in sorted(M.gcc_profiles().items()):
        if g.get("kit"):            # a secondary kit (gcc402x-*): paths relative to its own directory setting
            if g["driver"] is None:
                raise ProofError("toolchain", f"profile {name} needs the {g['kit']} setting (hst-config.json)")
            profiles[name] = {"compiler": "psp-gcc", "kit": g["kit"],
                              "driver": _kit_relative(g["driver"], M.gcc4_home()), "B": None, "flags": g["flags"]}
            continue
        profiles[name] = {"compiler": "psp-gcc", "driver": _kit_relative(g["driver"], home),
                          "B": _kit_relative(g["B"], home) if g["B"] else None, "flags": g["flags"]}
    return profiles


HASH_SCRIPT = r"""
set -u
hashset() {
  g=$1; base=$2; shift 2
  if [ ! -d "$base" ]; then echo "X $g"; return 0; fi
  cd "$base" || { echo "X $g"; return 0; }
  for s in "$@"; do
    if [ -e "$s" ] || [ -L "$s" ]; then
      find "$s" \( -type f -o -type l \) -print0 | sort -z | xargs -0 -r sha256sum | sed "s|^|F $g |"
      find "$s" -type l -printf "L $g %p %l\n"
    else
      echo "M $g $s"
    fi
  done
}
"""


def link_target(target: str, roots: dict[str, str]) -> str:
    """A symbolic link's target as the lock records it: inside a configured root (the deepest one that holds it) it is
    `<name>/rest`, so the lock names no local directory layout; any other target is kept as it is (`/usr/bin/ar`).
    A link that points somewhere else, or a root whose contents change, still changes the digest."""
    best = ""
    for name, base in roots.items():
        base = base.rstrip("/")
        if base and (target == base or target.startswith(base + "/")) and len(base) > len(roots.get(best, "").rstrip("/")):
            best = name
    if not best:
        return target
    return f"<{best}>" + target[len(roots[best].rstrip("/")):]


def live_toolchain(root: Path = ROOT, workdir: Path | None = None) -> dict:
    """The toolchain core as it is now, hashed at the configured locations (hst-config.json / HST_* variables)."""
    native, home, pspdev = M.native_dir(), M.gcc_home(), M.pspdev_bin()
    gcc4 = M.gcc4_home()
    if gcc4 is None:
        raise ProofError("toolchain", "the gcc4_home setting is not configured (the gcc402x-*/gcc404x-* kits are part "
                                      "of the lock)")
    build = M.PROFILE["build"]
    script = HASH_SCRIPT + "\n".join([
        f'hashset mwcc "{native}" wibo builds/{build}',
        f'hashset psp-gcc-1.3.1 "{home}/psp-gcc-1.3.1" .',
        f'hashset psp-gcc-1.7.1 "{home}/psp-gcc-1.7.1" .',
        f'hashset psp-gcc-4.0.2x "{gcc4}" psp-gcc-4.0.2x sony-as',
        f'hashset psp-gcc-4.0.4x "{gcc4}" psp-gcc-4.0.4x',
        f'hashset binutils "{pspdev}" psp-ld psp-objcopy psp-objdump',
        f'hashset binutils-ldscripts "{pspdev}/.." psp/lib/ldscripts',
        f'hashset pspgcc-specs "{PSPGCC_SPECS_DIR}" .',
        # the configured roots as the shell expands them, for link_target
        f'printf "R pspdev_bin %s\\n" "{pspdev}"', f'printf "R gcc4_home %s\\n" "{gcc4}"',
        f'printf "R gcc_home %s\\n" "{home}"', f'printf "R mwcc_native %s\\n" "{native}"',
        'printf "R HOME %s\\n" "$HOME"',
        'echo "E-BEGIN"', "env | sort", 'echo "E-END"'])
    out = run_wsl_script(script, workdir or root / "build" / "proof", "toolchain-hash")
    groups: dict[str, dict] = {g: {"present": True, "files": {}, "links": {}, "missing": []}
                               for g in ("mwcc", "psp-gcc-1.3.1", "psp-gcc-1.7.1", "psp-gcc-4.0.2x", "psp-gcc-4.0.4x",
                                         "binutils",
                                         "binutils-ldscripts",
                                         "pspgcc-specs")}
    env_lines: list[str] = []
    roots: dict[str, str] = {}
    links: list[tuple[str, str, str]] = []
    in_env = False
    for line in out.splitlines():
        if line == "E-BEGIN":
            in_env = True
            continue
        if line == "E-END":
            in_env = False
            continue
        if in_env:
            env_lines.append(line)
            continue
        kind, _, rest = line.partition(" ")
        g, _, rest = rest.partition(" ")
        if kind == "R":
            roots[g] = rest
            continue
        if g not in groups:
            continue
        if kind == "X":
            groups[g]["present"] = False
        elif kind == "M":
            groups[g]["missing"].append(rest)
        elif kind == "F":
            h, _, p = rest.partition("  ")
            groups[g]["files"][p.removeprefix("./")] = h.lstrip("\\")
        elif kind == "L":
            p, _, target = rest.partition(" ")
            links.append((g, p.removeprefix("./"), target))
    for g, p, target in links:
        groups[g]["links"][p] = link_target(target, roots)
    env = {}
    for line in env_lines:
        if COMPILER_ENV.match(line):
            k, _, v = line.partition("=")
            env[k] = sha256(v.encode("utf-8"))
    for g, rec in groups.items():
        rec["sha256"] = digest({"files": rec["files"], "links": rec["links"], "missing": rec["missing"],
                                "present": rec["present"]})
    must = {"mwcc": ["wibo", f"builds/{build}/mwccpsp.exe"], "binutils": ["psp-ld", "psp-objcopy", "psp-objdump"],
            "psp-gcc-1.3.1": ["bin/psp-gcc"], "psp-gcc-1.7.1": [],
            "psp-gcc-4.0.2x": ["psp-gcc-4.0.2x/bin/psp-gcc", "psp-gcc-4.0.2x/libexec/gcc/psp/4.0.2/cc1", "sony-as"],
            "psp-gcc-4.0.4x": ["psp-gcc-4.0.4x/bin/psp-gcc", "psp-gcc-4.0.4x/libexec/gcc/psp/4.0.4/cc1",
                               "psp-gcc-4.0.4x/libexec/gcc/psp/4.0.4/cc1plus"]}
    for g, files in must.items():
        if not groups[g]["present"] or any(f not in groups[g]["files"] for f in files):
            raise ProofError("toolchain-drift", f"toolchain component {g} is missing at its configured location")
    include = {rel(p, root): file_sha256(p) for p in sorted((root / "include").rglob("*")) if p.is_file()}
    code = {}
    for name in COMPARISON_CODE:
        p = root / name
        code[name] = file_sha256(p) if p.exists() else None
    return {"programs": groups, "environment": env, "profiles": live_profiles(), "include": include,
            "comparison_code": code}


def load_toolchain_manifest(path: Path = TOOLCHAIN_PATH) -> dict:
    if not path.exists():
        raise ProofError("toolchain-manifest", f"{path.name} does not exist (create the locks with tools/hstpromote.py lock)")
    doc = json.loads(path.read_text(encoding="utf-8"))
    if doc.get("schema") != TOOLCHAIN_SCHEMA:
        raise ProofError("toolchain-manifest", f"{path.name}: unknown schema {doc.get('schema')!r}")
    if digest(doc.get("core")) != doc.get("core_digest"):
        raise ProofError("toolchain-manifest", f"{path.name} was edited: its core does not match its core_digest")
    return doc


def describe_toolchain_diff(old: dict, new: dict) -> list[str]:
    out = []
    for key in ("profiles", "environment"):
        if old.get(key) != new.get(key):
            a, b = old.get(key) or {}, new.get(key) or {}
            names = sorted(k for k in set(a) | set(b) if a.get(k) != b.get(k))
            out.append(f"{key}: {', '.join(names)}")
    for key in ("include", "comparison_code"):
        a, b = old.get(key) or {}, new.get(key) or {}
        for name in sorted(set(a) | set(b)):
            if a.get(name) != b.get(name):
                out.append(f"{name} {'added' if name not in a else 'removed' if name not in b else 'changed'}")
    og, ng = old.get("programs") or {}, new.get("programs") or {}
    for g in sorted(set(og) | set(ng)):
        a, b = og.get(g) or {}, ng.get(g) or {}
        if a.get("sha256") == b.get("sha256"):
            continue
        fa, fb = a.get("files") or {}, b.get("files") or {}
        changed = sorted(f for f in set(fa) | set(fb) if fa.get(f) != fb.get(f))
        what = ", ".join(changed[:6]) + (f" and {len(changed) - 6} more" if len(changed) > 6 else "")
        if a.get("present") != b.get("present"):
            what = "present" if b.get("present") else "absent"
        out.append(f"{g}: {what or 'links/layout'}")
    return out


def verify_toolchain(manifest: dict, live: dict | None = None) -> dict:
    live = live if live is not None else live_toolchain()
    if digest(live) != manifest["core_digest"]:
        raise ProofError("toolchain-drift", "the toolchain is not the locked one (a qualified migration is required: "
                                            "hstpromote.py migrate-toolchain --reason ...): "
                         + "; ".join(describe_toolchain_diff(manifest["core"], live)))
    return live


# --------------------------------------------------------------------------------------------------------------
# dependencies of one source

INCLUDE_RX = re.compile(r"^[ \t]*(?:#|%:|\?\?=)[ \t]*(include_next|include|import)\b[ \t]*(.*)$", re.M)
PRAGMA_INCLUDE = re.compile(r"^[ \t]*#[ \t]*pragma[ \t]+\w*(?:include|prefix)\w*", re.M | re.I)
INCLUDE_WORDS = re.compile(r"include|import|pragma", re.I)
HEADER_SUFFIXES = (".h", ".hpp", ".hh", ".hxx", ".inc", ".inl", ".ipp", ".tcc")


def include_closure(path: Path, root: Path = ROOT) -> dict:
    """Every repository file `path` can include, transitively (a superset: conditionals are ignored), with SHA-256s.
    `<x>` resolves in include/ (MWCIncludes); `"x"` in the source's directory, then include/. A name found in neither is
    a compiler header (the psp-gcc kits, hashed by the toolchain lock). A computed include or an include pragma makes
    the set undeterminable: the record then also binds the whole include/ tree and every header beside the source."""
    seen: dict[str, str] = {}
    unresolved: set[str] = set()
    determinable = True
    stack = [path]
    while stack:
        f = stack.pop()
        raw = f.read_text(encoding="utf-8", errors="replace")
        if not INCLUDE_WORDS.search(raw):
            continue        # no directive can name a file without one of these words
        text = hstgate.strip_comments(raw)
        if PRAGMA_INCLUDE.search(text):
            determinable = False
        for m in INCLUDE_RX.finditer(text):
            arg = m.group(2).strip()
            mm = re.match(r'"([^"\n]+)"|<([^>\n]+)>', arg)
            if not mm:
                determinable = False
                continue
            name = mm.group(1) or mm.group(2)
            options = ([f.parent / name] if mm.group(1) else []) + [root / "include" / name]
            hit = next((c for c in options if c.is_file()), None)
            if hit is None:
                unresolved.add(name)
                continue
            key = rel(hit, root)
            if key not in seen:
                seen[key] = file_sha256(hit)
                stack.append(hit)
    rec = {"headers": sorted([k, v] for k, v in seen.items()), "unresolved_includes": sorted(unresolved)}
    if not determinable:
        tree = sorted(p for p in (root / "include").rglob("*") if p.is_file())
        tree += sorted(p for p in path.parent.iterdir() if p.is_file() and p.suffix.lower() in HEADER_SUFFIXES)
        rec["include_tree_sha256"] = digest([[rel(p, root), file_sha256(p)] for p in tree])
    return rec


SYMBOL_LINE = re.compile(r"(\S+)\s*=\s*(0x[0-9A-Fa-f]+)\s*;?")
DEFAULT_NAME = re.compile(r"(?:FUN|DAT|LAB|PTR|jtbl)_([0-9a-fA-F]{8})")


def symbol_map(module: str, root: Path = ROOT) -> dict[str, int]:
    """symbols/<module>.txt, parsed by this module (the same rules as the matcher)."""
    out: dict[str, int] = {}
    path = root / "symbols" / f"{module}.txt"
    if not path.exists():
        return out
    for line in path.read_text(encoding="utf-8").splitlines():
        line = line.split("#", 1)[0].strip()
        if not line:
            continue
        m = SYMBOL_LINE.fullmatch(line)
        if not m:
            raise ProofError("symbols", f"bad symbol line in {path.name}: {line}")
        value = int(m.group(2), 16)
        if out.get(m.group(1), value) != value:
            raise ProofError("symbols", f"conflicting symbol {m.group(1)} in {path.name}")
        out[m.group(1)] = value
    return out


def resolve(name: str, symmap: dict[str, int]) -> int | None:
    if name in symmap:
        return symmap[name]
    m = DEFAULT_NAME.fullmatch(name)
    return int(m.group(1), 16) if m else None


def deps_record(source_sha: str, closure: dict, symbols: list) -> dict:
    return {"source_sha256": source_sha, **closure, "symbols": symbols}


# --------------------------------------------------------------------------------------------------------------
# ledger and journal

def proof_digest(p: dict) -> str:
    return digest({k: v for k, v in p.items() if k != "proof_sha256"})


def proof_range(p: dict) -> tuple[str, int, int]:
    return p["module"], unhx(p["start"]), unhx(p["end"])


def ranges_digest(proofs: list[dict]) -> str:
    return digest([[p["module"], p["start"], p["end"]] for p in proofs])


def ledger_body(proofs: list[dict]) -> bytes:
    return b"".join(canon(p) + b"\n" for p in proofs)


def bytes_by_module(proofs: list[dict]) -> dict[str, int]:
    out: dict[str, int] = {}
    for p in proofs:
        _m, s, e = proof_range(p)
        out[p["module"]] = out.get(p["module"], 0) + e - s
    return dict(sorted(out.items()))


def validate_proofs(proofs: list[dict]) -> None:
    """Structure, digests, order and ownership (no two proofs share a byte) of a proof list."""
    seen_sources = set()
    last: dict[str, int] = {}
    prev_key = None
    for i, p in enumerate(proofs):
        where = f"proof {i + 1}"
        missing = [k for k in PROOF_KEYS if k not in p]
        if missing:
            raise ProofError("ledger", f"{where} lacks {', '.join(missing)}")
        if p["schema"] != PROOF_SCHEMA or p["status"] != "EXACT" or p.get("gate", {}).get("result") != "pass":
            raise ProofError("ledger", f"{where} ({p.get('source')}) is not an EXACT, gate-passing proof")
        if p["proof_sha256"] != proof_digest(p):
            raise ProofError("ledger", f"{where} ({p['source']}) was edited: its proof_sha256 does not match")
        if p["deps_sha256"] != digest(p["deps"]) or p["deps"].get("source_sha256") != p["source_sha256"]:
            raise ProofError("ledger", f"{where} ({p['source']}): inconsistent dependency record")
        for k in ("source_sha256", "object_sha256", "toolchain", "target"):
            if not SHA.fullmatch(str(p[k])):
                raise ProofError("ledger", f"{where}: bad {k}")
        module, s, e = proof_range(p)
        if not s < e:
            raise ProofError("ledger", f"{where} ({p['source']}): empty range")
        key = (module, s)
        if prev_key is not None and key <= prev_key:
            raise ProofError("ledger", f"{where} ({p['source']}) is out of order or duplicates a start")
        prev_key = key
        if s < last.get(module, -1):
            raise ProofError("ledger", f"{where} ({p['source']}) overlaps the range of an earlier proof: "
                                       "a byte may have only one owner")
        last[module] = e
        if p["source"] in seen_sources:
            raise ProofError("ledger", f"{where}: {p['source']} owns two ranges")
        seen_sources.add(p["source"])


@dataclass
class Ledger:
    header: dict
    proofs: list[dict]
    journal: list[dict]
    journal_lines: list[bytes] = field(repr=False)

    def by_source(self) -> dict[str, dict]:
        return {p["source"]: p for p in self.proofs}


def read_journal(path: Path = JOURNAL_PATH, data: bytes | None = None) -> tuple[list[dict], list[bytes]]:
    data = path.read_bytes() if data is None else data
    if data and not data.endswith(b"\n"):
        raise ProofError("journal", "the journal does not end with a newline (truncated?)")
    lines = data.split(b"\n")[:-1] if data else []
    events, prev = [], None
    for i, raw in enumerate(lines):
        try:
            ev = json.loads(raw)
        except ValueError:
            raise ProofError("journal", f"journal line {i + 1} is not JSON") from None
        if canon(ev) != raw:
            raise ProofError("journal", f"journal line {i + 1} is not in canonical form (edited by hand?)")
        if ev.get("schema") != JOURNAL_SCHEMA or ev.get("seq") != i + 1 or ev.get("prev_sha256") != prev:
            raise ProofError("journal", f"journal line {i + 1} breaks the hash chain")
        prev = sha256(raw)
        events.append(ev)
    return events, lines


def parse_ledger(data: bytes, journal_data: bytes) -> Ledger:
    if not data.endswith(b"\n"):
        raise ProofError("ledger", "the ledger does not end with a newline (truncated?)")
    lines = data.split(b"\n")[:-1]
    if not lines:
        raise ProofError("ledger", "the ledger is empty")
    try:
        header = json.loads(lines[0])
        proofs = [json.loads(x) for x in lines[1:]]
    except ValueError:
        raise ProofError("ledger", "the ledger is not valid JSON lines") from None
    if header.get("schema") != LEDGER_SCHEMA:
        raise ProofError("ledger", f"unknown ledger schema {header.get('schema')!r}")
    body = b"".join(x + b"\n" for x in lines[1:])
    if any(canon(p) != x for p, x in zip(proofs, lines[1:])):
        raise ProofError("ledger", "a proof line is not in canonical form (edited by hand?)")
    if header.get("body_sha256") != sha256(body):
        raise ProofError("ledger", "the ledger body does not match its header (proofs added, removed or edited by hand)")
    validate_proofs(proofs)
    if header.get("count") != len(proofs) or header.get("bytes") != bytes_by_module(proofs) \
            or header.get("ranges_sha256") != ranges_digest(proofs):
        raise ProofError("ledger", "the ledger header's counts do not match its proofs")
    events, jlines = read_journal(data=journal_data)
    if not events:
        raise ProofError("journal", "the journal is empty")
    if header.get("journal_seq") != events[-1]["seq"] or header.get("journal_sha256") != sha256(jlines[-1]):
        raise ProofError("journal", "the ledger was not written by the journal's last event (ledger or journal edited, "
                                    "or a write was interrupted: restore both from git)")
    if events[-1].get("ledger_body_sha256") != header["body_sha256"]:
        raise ProofError("journal", "the journal's last event names a different ledger body")
    return Ledger(header, proofs, events, jlines)


def load_ledger(path: Path = LEDGER_PATH, journal: Path = JOURNAL_PATH, required: bool = True) -> Ledger | None:
    if not path.exists():
        if journal.exists():
            raise ProofError("ledger", f"{path.name} is missing but its journal exists (the ledger was deleted)")
        if required:
            raise ProofError("ledger", f"{path.name} does not exist (seed it with tools/hstpromote.py seed)")
        return None
    if not journal.exists():
        raise ProofError("journal", f"{journal.name} is missing (the ledger cannot be trusted without it)")
    return parse_ledger(path.read_bytes(), journal.read_bytes())


def check_binding(led: Ledger, toolchain_digest: str, target_digest: str) -> None:
    if led.header.get("toolchain") != toolchain_digest or led.header.get("target") != target_digest:
        raise ProofError("binding", "the ledger is bound to a different toolchain or target than the locked ones")
    for p in led.proofs:
        if p["toolchain"] != toolchain_digest or p["target"] != target_digest:
            raise ProofError("binding", f"{p['source']}: its proof is bound to a different toolchain or target")


def symbols_snapshot(root: Path = ROOT) -> dict[str, str]:
    return {p.name: file_sha256(p) for p in sorted((root / "symbols").glob("*.txt"))}


def ledger_header(proofs: list[dict], toolchain_digest: str, target_digest: str, event_line: bytes, seq: int,
                  root: Path = ROOT) -> dict:
    return {"schema": LEDGER_SCHEMA, "toolchain": toolchain_digest, "target": target_digest, "count": len(proofs),
            "bytes": bytes_by_module(proofs), "ranges_sha256": ranges_digest(proofs),
            "body_sha256": sha256(ledger_body(proofs)), "journal_seq": seq, "journal_sha256": sha256(event_line),
            "symbols_sha256": symbols_snapshot(root)}


def write_ledger(proofs: list[dict], event: dict, toolchain_digest: str, target_digest: str,
                 ledger_path: Path = LEDGER_PATH, journal_path: Path = JOURNAL_PATH, root: Path = ROOT) -> dict:
    """The only writer of the ledger: validates the new proof list, appends a journal event and replaces the ledger.
    Both files are written to temporaries first and then moved into place (journal first)."""
    proofs = sorted(proofs, key=lambda p: (p["module"], unhx(p["start"])))
    validate_proofs(proofs)
    for p in proofs:
        if p["toolchain"] != toolchain_digest or p["target"] != target_digest:
            raise ProofError("binding", f"{p['source']}: refusing to write a proof bound to another toolchain or target")
    events, jlines = read_journal(journal_path) if journal_path.exists() else ([], [])
    body = ledger_body(proofs)
    ev = {"schema": JOURNAL_SCHEMA, "seq": len(events) + 1, "prev_sha256": sha256(jlines[-1]) if jlines else None,
          "utc": utc_now(), **event, "ledger_body_sha256": sha256(body), "ledger_count": len(proofs),
          "ranges_sha256": ranges_digest(proofs), "bytes": bytes_by_module(proofs), "toolchain": toolchain_digest,
          "target": target_digest}
    line = canon(ev)
    header = ledger_header(proofs, toolchain_digest, target_digest, line, ev["seq"], root)
    jtmp, ltmp = journal_path.with_suffix(".tmp"), ledger_path.with_suffix(".tmp")
    jtmp.write_bytes(b"".join(x + b"\n" for x in jlines) + line + b"\n")
    ltmp.write_bytes(canon(header) + b"\n" + body)
    parse_ledger(ltmp.read_bytes(), jtmp.read_bytes())   # never move a ledger into place that would not load
    os.replace(jtmp, journal_path)
    os.replace(ltmp, ledger_path)
    return ev


def write_json(path: Path, doc: dict) -> None:
    tmp = path.with_suffix(".tmp")
    tmp.write_text(json.dumps(doc, indent=1, sort_keys=True) + "\n", encoding="utf-8", newline="\n")
    os.replace(tmp, path)


# --------------------------------------------------------------------------------------------------------------
# a proof against the tree

def tree_sources(root: Path = ROOT) -> dict[str, Path]:
    return {rel(p, root): p for p in sorted((root / "src").rglob("*"))
            if p.suffix in SOURCE_SUFFIXES and p.is_file()}


def current_symbols(proof: dict, symmap: dict[str, int]) -> list:
    out = []
    for name, addr in proof["deps"]["symbols"]:
        now = resolve(name, symmap)
        out.append([name, None if addr is None and now is None else (hx(now) if now is not None else "unresolved")])
    return out


def proof_problems(proof: dict, root: Path, symmaps: dict[str, dict], inventories: dict[str, list]) -> list[str]:
    """Why `proof` no longer holds for the tree at `root` ([] when it still does). Cheap: no compiling."""
    path = root / proof["source"]
    if not path.is_file() or path.is_symlink():
        return ["source missing"]
    data = path.read_bytes()
    if sha256(data) != proof["source_sha256"]:
        return ["source changed"]
    out = []
    try:
        h = M.parse_header(path)
    except ValueError as e:
        return [f"header: {e}"]
    if (h["module"], hx(h["start"]), hx(h["end"]), h["symbol"]) != (proof["module"], proof["start"], proof["end"],
                                                                      proof["symbol"]):
        out.append("header differs from the proof")
    closure = include_closure(path, root)
    if {k: proof["deps"].get(k) for k in closure} != closure or set(proof["deps"]) - set(closure) - {"source_sha256", "symbols"}:
        out.append("included headers changed")
    symmap = symmaps.setdefault(proof["module"], symbol_map(proof["module"], root))
    now = current_symbols(proof, symmap)
    changed = [n for (n, a), (_n, b) in zip(proof["deps"]["symbols"], now) if a != b]
    if changed:
        out.append("symbol resolution changed: " + ", ".join(changed[:5]) + (" ..." if len(changed) > 5 else ""))
    # (the gate result cannot change while the source is unchanged: tools/hstgate.py is in the toolchain lock)
    fns = inventories.get(proof["module"])
    if fns is not None:
        starts, ends = boundaries(fns)
        _m, s, e = proof_range(proof)
        if s not in starts or e not in ends:
            out.append("range is not aligned to the function inventory")
        if any(hs not in starts or he not in ends for _n, hs, he in h.get("helpers") or []):
            out.append("a helper (helpers=) is not aligned to the function inventory")
    return out


_bounds: dict[int, tuple[list, set[int], set[int]]] = {}


def boundaries(fns: list[tuple[int, int]]) -> tuple[set[int], set[int]]:
    """(function starts, function ends) of an inventory list, computed once per list."""
    hit = _bounds.get(id(fns))
    if hit is None or hit[0] is not fns:
        hit = (fns, {s for s, _ in fns}, {e for _, e in fns})
        _bounds[id(fns)] = hit
    return hit[1], hit[2]


# --------------------------------------------------------------------------------------------------------------
# coverage API

def ledger_present(root: Path = ROOT) -> bool:
    return (root / "data" / LEDGER_PATH.name).exists()


def _ledger(led: Ledger | None, root: Path = ROOT) -> Ledger | None:
    if led is not None:
        return led
    return load_ledger(root / "data" / LEDGER_PATH.name, root / "data" / JOURNAL_PATH.name, required=False)


def accepted_proofs(module: str | None = None, led: Ledger | None = None, root: Path = ROOT) -> list[dict]:
    led = _ledger(led, root)
    if led is None:
        return []
    return [p for p in led.proofs if module is None or p["module"] == module]


def accepted_spans(module: str, led: Ledger | None = None, root: Path = ROOT) -> list[tuple[int, int]]:
    """Merged, sorted [start, end) spans the accepted ledger proves for `module`: every byte appears once."""
    return merge((unhx(p["start"]), unhx(p["end"])) for p in accepted_proofs(module, led, root))


def proven_sources(led: Ledger | None = None, root: Path = ROOT) -> dict[str, dict]:
    return {p["source"]: p for p in accepted_proofs(None, led, root)}


def accepted_coverage(led: Ledger | None = None, manifest: dict | None = None, root: Path = ROOT) -> dict:
    """Coverage counted from accepted ranges only. Per module: scope bytes (the decrypted image), inventory functions and
    bytes (locked), accepted range bytes (union), accepted function bytes (union intersected with the inventory) and
    accepted functions (wholly inside an accepted span)."""
    led = _ledger(led, root)
    manifest = manifest or load_target_manifest(root / "data" / TARGET_PATH.name)
    rows = {}
    total = {"scope_bytes": 0, "inventory_functions": 0, "inventory_bytes": 0, "accepted_proofs": 0,
             "accepted_range_bytes": 0, "accepted_function_bytes": 0, "accepted_functions": 0}
    for m in manifest["modules"]:
        fns = inventory(m["name"], manifest)
        spans = accepted_spans(m["name"], led, root) if led else []
        n_proofs = len(accepted_proofs(m["name"], led, root)) if led else 0
        covered = 0
        j = 0
        for s, e in fns:
            while j < len(spans) and spans[j][1] <= s:
                j += 1
            if j < len(spans) and spans[j][0] <= s and e <= spans[j][1]:
                covered += 1
        row = {"scope_bytes": m["size"], "inventory_functions": len(fns), "inventory_bytes": m["inventory"]["bytes"],
               "accepted_proofs": n_proofs, "accepted_range_bytes": length(spans),
               "accepted_function_bytes": intersect_length(spans, fns), "accepted_functions": covered}
        rows[m["name"]] = row
        for k in total:
            total[k] += row[k]
    for r in [*rows.values(), total]:
        r["percent_of_inventory"] = round(100 * r["accepted_function_bytes"] / r["inventory_bytes"], 3) if r["inventory_bytes"] else 0.0
        r["percent_of_scope"] = round(100 * r["accepted_function_bytes"] / r["scope_bytes"], 3) if r["scope_bytes"] else 0.0
    return {"modules": rows, "total": total, "ledger": None if led is None else
            {"count": led.header["count"], "journal_seq": led.header["journal_seq"],
             "body_sha256": led.header["body_sha256"]}}


# --------------------------------------------------------------------------------------------------------------
# git helpers (read-only)

def git(root: Path, *args: str, check: bool = True) -> bytes:
    r = subprocess.run(["git", *args], cwd=root, capture_output=True, check=False)
    if check and r.returncode != 0:
        raise ProofError("git", f"git {' '.join(args)}: {r.stderr.decode(errors='replace').strip()[:300]}")
    return r.stdout


def git_file(root: Path, rev: str, path: str) -> bytes | None:
    r = subprocess.run(["git", "show", f"{rev}:{path}"], cwd=root, capture_output=True, check=False)
    return r.stdout if r.returncode == 0 else None


def base_problems(led: Ledger, rev: str, root: Path = ROOT) -> list[str]:
    """The ledger at `rev` must be a subset of this one, except ranges an explicit retract event removed since; the
    journal at `rev` must be a prefix of this one; a manifest change since `rev` needs its migration event."""
    old_l, old_j = git_file(root, rev, "data/accepted-ledger.jsonl"), git_file(root, rev, "data/accepted-journal.jsonl")
    if old_l is None:
        return []
    out = []
    try:
        old = parse_ledger(old_l, old_j or b"")
    except ProofError as e:
        return [f"the ledger at {rev} does not load: {e}"]
    if led.journal_lines[:len(old.journal_lines)] != old.journal_lines:
        out.append(f"the journal was rewritten since {rev} (it must only grow)")
    later = led.journal[len(old.journal):]
    retracted: dict[str, list] = {}
    for ev in later:
        if ev.get("event") == "retract":
            for r in ev.get("removed", []):
                retracted.setdefault(r["module"], []).append((unhx(r["start"]), unhx(r["end"])))
    modules = {p["module"] for p in old.proofs}
    for module in sorted(modules):
        was = merge(proof_range(p)[1:] for p in old.proofs if p["module"] == module)
        now = accepted_spans(module, led) + retracted.get(module, [])
        lost = subtract(was, now)
        if lost:
            out.append(f"{module}: {length(lost)} accepted bytes at {rev} are no longer accepted, first at {hx(lost[0][0])} "
                       f"(only an explicit `hstpromote.py retract` may remove accepted bytes)")
    events = {ev.get("event") for ev in later}
    for name, key, needed in (("data/toolchain-manifest.json", "core_digest", "migrate-toolchain"),
                              ("data/target-manifest.json", "target_digest", "retarget")):
        was = git_file(root, rev, name)
        if was is None:
            continue
        now_path = root / name
        old_d = json.loads(was).get(key)
        new_d = json.loads(now_path.read_text(encoding="utf-8")).get(key) if now_path.exists() else None
        if old_d != new_d and needed not in events:
            out.append(f"{name} changed since {rev} without a {needed} event in the journal")
    return out


def staged_problems(led: Ledger, root: Path = ROOT) -> list[str]:
    """The index must hold exactly what the proofs name: every proof's source tracked and identical to the work tree,
    and the ledger files themselves staged as they are on disk."""
    tracked = set(git(root, "ls-files", "-z").decode("utf-8").split("\0"))
    unstaged = set(git(root, "diff", "--name-only", "-z").decode("utf-8").split("\0"))
    out = []
    for p in led.proofs:
        if p["source"] not in tracked:
            out.append(f"{p['source']}: proven but not in the index")
        elif p["source"] in unstaged:
            out.append(f"{p['source']}: the index differs from the proven work-tree file")
    for name in LEDGER_FILES:
        if (root / name).exists() and (name not in tracked or name in unstaged):
            out.append(f"{name}: not staged as it is on disk")
    return out


# --------------------------------------------------------------------------------------------------------------
# CLI

def full_check(root: Path = ROOT, base: str | None = None, staged: bool = False, quick: bool = False) -> tuple[list[str], list[str]]:
    problems: list[str] = []
    warnings: list[str] = []
    data = root / "data"
    try:
        target = load_target_manifest(data / TARGET_PATH.name)
        verify_target(target, live_target(root))
    except ProofError as e:
        return [str(e)], warnings
    try:
        tool = load_toolchain_manifest(data / TOOLCHAIN_PATH.name)
        if quick:
            warnings.append("toolchain not hashed (--quick)")
        else:
            verify_toolchain(tool, live_toolchain(root))
    except ProofError as e:
        return [str(e)], warnings
    try:
        led = load_ledger(data / LEDGER_PATH.name, data / JOURNAL_PATH.name)
        check_binding(led, tool["core_digest"], target["target_digest"])
    except ProofError as e:
        return [str(e)], warnings
    symmaps: dict = {}
    inventories = {m["name"]: inventory(m["name"], target) for m in target["modules"]}
    for p in led.proofs:
        for why in proof_problems(p, root, symmaps, inventories):
            problems.append(f"{p['source']} [{p['module']} {p['start']}-{p['end']}]: {why}")
    proven = led.by_source()
    unproven = [s for s in tree_sources(root) if s not in proven]
    if unproven:
        warnings.append(f"{len(unproven)} source(s) in src/ have no proof (not counted by proof-based coverage): "
                        + ", ".join(unproven[:8]) + (" ..." if len(unproven) > 8 else ""))
    if led.header.get("symbols_sha256") != symbols_snapshot(root):
        warnings.append("symbols/ changed since the last ledger write (proofs that use a changed line are listed above)")
    if base:
        problems += base_problems(led, base, root)
    if staged:
        problems += staged_problems(led, root)
    return problems, warnings


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="command", required=True)
    sub.add_parser("status")
    c = sub.add_parser("check")
    c.add_argument("--base", help="a git revision whose ledger must be a subset of this one")
    c.add_argument("--staged", action="store_true", help="also check that the git index holds what the proofs name")
    c.add_argument("--quick", action="store_true", help="skip hashing the toolchain (never in the push procedure)")
    v = sub.add_parser("coverage")
    v.add_argument("--json", type=Path)
    s = sub.add_parser("spans")
    s.add_argument("module")
    args = ap.parse_args()
    try:
        if args.command == "status":
            target = load_target_manifest()
            tool = load_toolchain_manifest()
            led = load_ledger(required=False)
            print(f"target     {target['target_digest']}  ({len(target['modules'])} modules, {target['scope_bytes']:,} scope "
                  f"bytes, {target['inventory_functions']:,} functions / {target['inventory_bytes']:,} bytes)")
            print(f"toolchain  {tool['core_digest']}")
            if led is None:
                print("ledger     none")
                return 0
            last = led.journal[-1]
            print(f"ledger     {led.header['count']:,} proofs, {sum(led.header['bytes'].values()):,} bytes; journal "
                  f"{len(led.journal)} events, last: {last['event']} {last['utc']}")
            for m, n in led.header["bytes"].items():
                print(f"  {m:32} {n:>10,} B")
            return 0
        if args.command == "check":
            problems, warnings = full_check(ROOT, args.base, args.staged, args.quick)
            for w in warnings:
                print(f"warning: {w}")
            for p in problems:
                print(f"PROBLEM: {p}")
            print("ledger check: " + ("OK" if not problems else f"FAILED ({len(problems)} problem(s))"))
            return 1 if problems else 0
        if args.command == "coverage":
            cov = accepted_coverage()
            for name, r in [*cov["modules"].items(), ("ALL", cov["total"])]:
                print(f"{name:32} {r['accepted_functions']:6,}/{r['inventory_functions']:<6,} fns  "
                      f"{r['accepted_function_bytes']:10,}/{r['inventory_bytes']:<10,} B  {r['percent_of_inventory']:7.3f} % "
                      f"of the inventory, {r['percent_of_scope']:7.3f} % of the {r['scope_bytes']:,} scope bytes")
            print("(counted from the accepted ledger only: proofs from verification runs; each byte once)")
            if args.json:
                args.json.write_text(json.dumps(cov, indent=1) + "\n", encoding="utf-8")
            return 0
        if args.command == "spans":
            for s_, e_ in accepted_spans(args.module):
                print(f"{hx(s_)} {hx(e_)}")
            return 0
    except ProofError as e:
        print(f"REFUSED: {e}", file=sys.stderr)
        return 1
    return 2


if __name__ == "__main__":
    raise SystemExit(main())
