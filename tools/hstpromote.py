#!/usr/bin/env python3
"""The promotion gate: the only writer of the accepted ledger and the two locks (formats: tools/hstproof.py).

    python tools/hstpromote.py lock --reason TEXT        create the target and toolchain locks (refuses when they exist)
    python tools/hstpromote.py seed [--baseline JSON] [--report FILE]
                                                         the first ledger, from a full verification run of src/
    python tools/hstpromote.py promote [PATH ...] [--paths-from FILE] [--all] [--strict]
                                       [--promoted-out FILE] [--json FILE]
    python tools/hstpromote.py verify [--since REV | --sample N] [--json FILE]
                                                         re-run accepted proofs and compare (never writes)
    python tools/hstpromote.py migrate-toolchain --reason TEXT
    python tools/hstpromote.py retarget --reason TEXT
    python tools/hstpromote.py retract MODULE START --reason TEXT
  common: --parallel N (concurrent compiles, default 4), --keep-run, --lock-wait SECONDS

VERIFICATION RUN. Every proof comes from one, in a fresh directory build/proof/<run id>/ (never reused, so no artifact
of an earlier build can be read):
  1. pre-checks: a regular .c/.cpp file under src/; a header the matcher parses; no `extra=`; a compiler profile the
     toolchain lock records; a module the target lock records; a range that starts on a function start and ends on a
     function end of the locked inventory (no partial-function claim); the hstgate check passes.
  2. tools/hstmatch.py on the candidates (HSTMATCH_BUILD_ROOT=<run>/match; its EXACT semantics are unchanged).
  3. this tool's own compile of every candidate, from the profile the toolchain lock records: the object must be
     byte-identical to the matcher's (both compilers are deterministic), so the matcher cannot substitute an object.
  4. on the matcher's artifacts, with tools/hstproof.py's own ELF reader: each was written during this run; the function
     is linked at its retail address; the linked code is the object's code with only relocation fields filled in; the
     claimed range lies inside the object's code; the bytes equal the retail bytes, read through the module's own
     program headers from the image the target lock pins; the same for each placed .rodata/.data/.sdata; the link
     script defines exactly the object's undefined names, each at the address the symbol map (or its address name)
     gives, with placeholders only for names the compared code does not reference. A source with same-file helpers
     (header `helpers=`, tools/hstmatch.py) is checked function by function: every function of the object is the
     target or a listed helper (each on function boundaries of the locked inventory), alone in its own .text section,
     linked once at its own retail address, the object's code with only relocation fields filled in, and equal to the
     retail bytes there; a helper that differs refuses the source. The proof records each helper in `compared` as
     `.text:<name>`, but claims only the header's [start, end): a helper's bytes are owned (counted) by its own
     one-function source, so each byte still has one owner.
  5. the source is unchanged since the run started.
A source that passes all of it gets a proof; any failure is a refusal that names the reason.

PROMOTE. Preflight (any failure refuses everything): the target lock matches the live target, the toolchain lock the
live toolchain, the ledger and journal load and validate, every proof is bound to the locked digests. Candidates:
  * every accepted proof that no longer holds for the tree (its source changed or vanished, a header it includes
    changed, a name it links resolves differently): always reverified, whatever the arguments;
  * the named sources (default: every source in src/ without a proof); --all reverifies every proof as well.
V_new = the proofs that still hold + the new ones. A new proof that overlaps another owner's bytes is refused (each
byte has one owner, so a clone or a multi-function source counts once). The ledger is written only when V_new covers
every byte V_accepted covers, compared as byte ranges per module, not as totals; otherwise the promotion is REFUSED, the
ledger is left untouched and the lost ranges are named. --strict also refuses when any candidate is refused.
Exit status: 0 written or nothing to do; 3 written (or unchanged) while some candidates were refused; 1 refused;
2 usage error. --promoted-out lists the named sources that hold a valid proof afterwards (the push procedure stages
exactly those).

SEED. Only when no ledger exists: a verification run of every source in src/. Where proofs overlap, the larger range
owns the bytes and a contained source is reported as subsumed (not a proof); a partial overlap is refused. --baseline
compares the run with an earlier `hstmatch.py --all --json` file and reports every difference.

VERIFY re-runs accepted proofs (--since REV: those that differ from the ledger at REV, as the push procedure does;
--sample N: a random sample; default all) and compares the object, the compared bytes and the dependencies with the
proof. Exit 1 on any difference. It never writes.

MIGRATE-TOOLCHAIN (the qualified migration): when the live toolchain differs from the lock (a compiler, kit, binutils,
include/, profile or comparison-code change), every accepted proof is reverified with the live toolchain; only if all
of them still hold are the lock (with the reason and the changes in its history) and the rebound ledger written.
RETARGET: when the live target differs from the lock (a reviewed boundary changed, ...), every proof is checked against
the new target (inventory alignment, the retail bytes of every compared range); only if all hold is the lock rewritten
with the reason. RETRACT removes one proof explicitly, with a reason in the journal; nothing else can remove accepted
bytes, and `hstproof.py check --base` reports any other loss.
"""
from __future__ import annotations

import argparse
import bisect
import json
import os
import random
import shutil
import subprocess
import sys
import time
from collections import Counter
from dataclasses import dataclass
from pathlib import Path

TOOLS = Path(__file__).resolve().parent
sys.path.insert(0, str(TOOLS))
import hstgate  # noqa: E402
import hstmatch as M  # noqa: E402
import hstproof as P  # noqa: E402
from hstproof import ProofError, hx, unhx  # noqa: E402

ROOT = P.ROOT
CHUNK = 300             # sources per hstmatch.py call (keeps the command line short)
PLACED = ("rodata", "data", "sdata")
PLACEHOLDER = "/* not referenced by the compared function */"


def say(msg: str) -> None:
    print(msg, flush=True)


# --------------------------------------------------------------------------------------------------------------
# lock and preflight

class PromoteLock:
    """try/promote.lock (mkdir): one writer at a time. Separate from try/auto.lock, which the push procedure holds while
    it calls this tool."""

    def __init__(self, root: Path, wait: float):
        self.path = root / "try" / "promote.lock"
        self.wait = wait

    def __enter__(self):
        self.path.parent.mkdir(parents=True, exist_ok=True)
        deadline = time.time() + self.wait
        told = False
        while True:
            try:
                self.path.mkdir()
                break
            except FileExistsError:
                if time.time() >= deadline:
                    info = (self.path / "owner.json").read_text(encoding="utf-8") if (self.path / "owner.json").exists() else "?"
                    raise ProofError("lock", f"another promotion holds {self.path} ({info.strip()}); remove the directory "
                                             "only when no hstpromote.py is running") from None
                if not told:
                    say(f"waiting for {self.path} ...")
                    told = True
                time.sleep(5)
        (self.path / "owner.json").write_text(json.dumps({"pid": os.getpid(), "utc": P.utc_now(), "argv": sys.argv[1:4]}),
                                              encoding="utf-8")
        return self

    def __exit__(self, *exc):
        shutil.rmtree(self.path, ignore_errors=True)


class Context:
    """The locks, verified against the live target and toolchain, and the ledger, validated and bound."""

    def __init__(self, root: Path = ROOT, need_ledger: bool = True, check_toolchain: bool = True):
        self.root = root
        data = root / "data"
        self.target_path, self.toolchain_path = data / P.TARGET_PATH.name, data / P.TOOLCHAIN_PATH.name
        self.ledger_path, self.journal_path = data / P.LEDGER_PATH.name, data / P.JOURNAL_PATH.name
        self.target = P.load_target_manifest(self.target_path)
        self.live_target = P.verify_target(self.target, P.live_target(root))
        self.tool = P.load_toolchain_manifest(self.toolchain_path)
        if check_toolchain:
            P.verify_toolchain(self.tool, P.live_toolchain(root))
        self.ledger = P.load_ledger(self.ledger_path, self.journal_path, required=need_ledger)
        if self.ledger is not None:
            P.check_binding(self.ledger, self.tool["core_digest"], self.target["target_digest"])
        self.inventories = {m["name"]: P.inventory(m["name"], self.target) for m in self.target["modules"]}

    def write(self, proofs: list[dict], event: dict) -> dict:
        return P.write_ledger(proofs, event, self.tool["core_digest"], self.target["target_digest"], self.ledger_path,
                              self.journal_path, self.root)


# --------------------------------------------------------------------------------------------------------------
# the verification run

@dataclass
class Candidate:
    path: Path
    rel: str
    sha: str = ""
    header: dict | None = None


def candidates_for(rels, tree: dict[str, Path]) -> list[Candidate]:
    return [Candidate(tree[r], r) for r in sorted(rels) if r in tree]


class Run:
    def __init__(self, root: Path, profiles: dict, toolchain_digest: str, target: dict, inventories: dict,
                 parallel: int, keep: bool = False):
        self.root, self.profiles, self.toolchain_digest = root, profiles, toolchain_digest
        self.target, self.inventories, self.parallel, self.keep = target, inventories, parallel, keep
        self.id = time.strftime("%Y%m%dT%H%M%SZ", time.gmtime()) + "-" + os.urandom(3).hex()
        self.dir = root / "build" / "proof" / self.id
        self.dir.mkdir(parents=True)          # FileExistsError if it existed: a run directory is never reused
        self.started = time.time()
        self.modules = {m["name"] for m in target["modules"]}

    # -- 1. pre-checks
    def precheck(self, c: Candidate) -> None:
        p = c.path
        if p.is_symlink() or not p.is_file():
            raise ProofError("source", "not a regular file")
        r = P.rel(p, self.root)
        if not r.startswith("src/") or p.suffix not in P.SOURCE_SUFFIXES:
            raise ProofError("source", "only .c/.cpp files under src/ can be promoted")
        c.sha = P.sha256(p.read_bytes())
        try:
            h = M.parse_header(p)
        except ValueError as e:
            raise ProofError("header", str(e)) from None
        if "extra" in h:
            raise ProofError("extra-flags", "an `extra=` header is an experiment and never a match")
        cc = h.get("cc", P.DEFAULT_CC)
        if cc not in self.profiles:
            raise ProofError("profile", f"cc={cc} is not a compiler profile the toolchain lock records")
        if h["lang"] not in ("c99", "c++"):
            raise ProofError("header", f"lang={h['lang']} is not c99 or c++")
        if h["module"] not in self.modules:
            raise ProofError("header", f"module {h['module']} is not in the target lock")
        s, e = h["start"], h["end"]
        if not s < e or s % 4 or e % 4:
            raise ProofError("range", f"bad range [{hx(s)}, {hx(e)})")
        starts, ends = P.boundaries(self.inventories[h["module"]])
        if s not in starts or e not in ends:
            raise ProofError("range", f"[{hx(s)}, {hx(e)}) does not start and end on function boundaries of the locked "
                                      "inventory (a partial-function claim)")
        if h.get("helpers"):
            if self.profiles[cc].get("compiler") != "mwcc":
                raise ProofError("profile", "helpers= needs one .text section per function (an mwcc profile)")
            for name, hs, he in h["helpers"]:
                if hs not in starts or he not in ends:
                    raise ProofError("range", f"helper {name} [{hx(hs)}, {hx(he)}) is not a function of the locked "
                                              "inventory")
        gate = hstgate.refusals(hstgate.check_file(p, self.root))
        if gate:
            raise ProofError("gate", f"fails the fake-match gate: {hstgate.summary(gate)}")
        c.header = h

    def build_dir(self, c: Candidate) -> Path:
        return self.dir / "match" / c.header["module"] / M.build_key(c.path)

    # -- 2. the matcher
    def run_matcher(self, cands: list[Candidate]) -> dict[str, dict]:
        env = dict(os.environ)
        env["HSTMATCH_BUILD_ROOT"] = str(self.dir / "match")
        rows: dict[str, dict] = {}
        chunks = [cands[i:i + CHUNK] for i in range(0, len(cands), CHUNK)]
        for k, chunk in enumerate(chunks):
            out, log = self.dir / f"match-{k:03d}.json", self.dir / f"match-{k:03d}.log"
            cmd = [sys.executable, str(self.root / "tools" / "hstmatch.py"), "--quiet", "--parallel", str(self.parallel),
                   "--json", str(out)] + [str(c.path) for c in chunk]
            with log.open("wb") as fh:
                subprocess.run(cmd, cwd=self.root, env=env, stdout=fh, stderr=subprocess.STDOUT, check=False)
            if out.exists():
                for row in json.loads(out.read_text(encoding="utf-8")):
                    rows[row["source"]] = row
            if len(chunks) > 1:
                say(f"  matcher {k + 1}/{len(chunks)} ({sum(1 for r in rows.values() if r['status'] == 'EXACT')} EXACT so far)")
        return rows

    # -- 3. the promoter's own compile
    def run_own_compile(self, cands: list[Candidate]) -> dict[str, Path]:
        native, home = M.native_dir(), M.gcc_home()
        q = lambda s: "'" + s.replace("'", "'\\''") + "'"   # noqa: E731
        cmds, out = [], {}
        include = q(P.to_wsl(self.root / "include"))
        for i, c in enumerate(cands):
            d = self.dir / "own" / f"{i:05d}"
            d.mkdir(parents=True)
            obj, log = d / "obj.o", d / "compile.log"
            out[c.rel] = obj
            prof = self.profiles[c.header.get("cc", P.DEFAULT_CC)]
            if prof["compiler"] == "mwcc":
                cmds.append(f"cd {q(P.to_wsl(c.path.parent))} && MWCIncludes={include} {native}/wibo "
                            f"{native}/builds/{prof['build']}/mwccpsp.exe {prof['flags']} {prof['lang_option']} "
                            f"{c.header['lang']} -o {q(P.to_wsl(obj))} {q(c.path.name)} > {q(P.to_wsl(log))} 2>&1")
            elif prof["compiler"] == "psp-gcc":
                lang = "-x c++" if c.header["lang"] == "c++" else "-x c"
                kit = home
                if prof.get("kit"):
                    if prof["kit"] != M.GCC4_PROFILE_KIT or M.gcc4_home() is None:
                        raise ProofError("profile", f"kit {prof['kit']!r} of the toolchain lock is not configured")
                    kit = M.gcc4_home()
                bopt = f"-B {kit}/{prof['B']} " if prof.get("B") else ""
                cmds.append(f"t=$(mktemp -d /tmp/hstproof.XXXXXX) && cp {q(P.to_wsl(c.path))} $t/ && cd $t && "
                            f"{kit}/{prof['driver']} {bopt}-c {prof['flags']} {lang} -o obj.o {q(c.path.name)} > log 2>&1; "
                            f"cp log {q(P.to_wsl(log))}; [ -f obj.o ] && cp obj.o {q(P.to_wsl(obj))}; cd / && rm -rf $t")
            else:
                raise ProofError("profile", f"unknown compiler kind {prof['compiler']!r} in the toolchain lock")
        if cmds:
            jobs = self.dir / "own.jobs"
            jobs.write_text("\n".join(cmds) + "\n", encoding="utf-8", newline="\n")
            P.run_wsl_script(f"tr '\\n' '\\0' < {q(P.to_wsl(jobs))} | xargs -0 -P {self.parallel} -n 1 sh -c",
                             self.dir, "own-compile")
        return out

    # -- 4. independent checks of the artifacts
    def compare(self, c: Candidate, obj: bytes, linked: bytes, syms_ld: str, symmap: dict) -> tuple[dict, list]:
        h = c.header
        if h.get("helpers"):
            return self.compare_placed(c, obj, linked, syms_ld, symmap)
        start, end = h["start"], h["end"]
        span = end - start
        o, ln = P.Elf(obj, "object"), P.Elf(linked, "linked image")
        texts = o.named(".text", P.SHT_PROGBITS)
        if not texts:
            raise ProofError("object", "the object has no .text")
        fsym = [s for s in o.symbols() if s.name == h["symbol"] and 0 < s.shndx < len(o.sections)
                and o.sections[s.shndx].name == ".text"]
        if len(fsym) != 1 or fsym[0].shndx != texts[0].index:
            raise ProofError("symbol", f"{h['symbol']} is not defined once in the object's first .text section "
                                       "(the one the matcher places)")
        text_off, fsize = fsym[0].value, fsym[0].size
        if fsize > span:
            raise ProofError("range", f"{h['symbol']} is {fsize} bytes, longer than the claimed {span}")
        # MWCC emits one .text per function: ld lays them out in file order, each aligned
        place = o.placements(".text", start - text_off)
        if place[0][1] != start - text_off:
            raise ProofError("placement", "the first .text section cannot start where the matcher places it")
        code_lo, code_hi = place[0][1], place[-1][1] + place[-1][0].size
        if end > code_hi:
            raise ProofError("range", "the claimed range extends past the object's code")
        lsec = next((s for s in ln.sections if s.flags & P.SHF_ALLOC and s.type == P.SHT_PROGBITS
                     and s.addr <= code_lo and code_hi <= s.addr + s.size), None)
        if lsec is None:
            raise ProofError("placement", "no linked section holds the object's code at its address")
        image = ln.contents(lsec)
        at = lambda a, n: image[a - lsec.addr:a - lsec.addr + n]   # noqa: E731
        linked_defs: dict[str, list[int]] = {}
        for s in ln.symbols():
            if s.shndx != 0 and s.name:
                linked_defs.setdefault(s.name, []).append(s.value)
        if linked_defs.get(h["symbol"]) != [start]:
            raise ProofError("placement", f"{h['symbol']} is not linked (once) at its retail address {hx(start)}")
        where = {sec.index: addr for sec, addr in place}
        for s in o.symbols():   # every named code symbol of the object sits where the layout puts it
            if s.shndx in where and s.name and s.info & 0xF not in (P.STT_SECTION, P.STT_FILE) \
                    and s.name in linked_defs and where[s.shndx] + s.value not in linked_defs[s.name]:
                raise ProofError("placement", f"{s.name} is linked at {hx(linked_defs[s.name][0])}, the object's layout "
                                              f"puts it at {hx(where[s.shndx] + s.value)}")
        prev = code_lo
        for sec, addr in place:   # the linked code is the object's code with only relocation fields filled in
            if any(at(prev, addr - prev)):
                raise ProofError("relocation", f"the linker fill before {hx(addr)} is not zero")
            bad = P.masked_difference(o.contents(sec), at(addr, sec.size), o.relocations(sec))
            if bad is not None:
                raise ProofError("relocation", f"the linked code differs from the object outside relocation fields "
                                               f"at {hx(addr + bad)}")
            prev = addr + sec.size
        ours = at(start, span)
        retail = P.image_range(h["module"], start, end, self.target, executable=True)
        if ours != retail:
            n = sum(1 for i in range(0, span, 4) if ours[i:i + 4] != retail[i:i + 4])
            raise ProofError("mismatch", f"independent comparison: {n} of {span // 4} words differ from retail")
        compared = {".text": {"addr": hx(start), "size": span, "sha256": P.sha256(ours)}}
        self.compare_data(h, o, ln, compared)
        refs = [(addr + r.offset, r.sym) for sec, addr in place for r in o.relocations(sec)]
        return compared, self.check_symbols(o, syms_ld, symmap, refs, [(start, start + max(fsize, span))])

    def compare_placed(self, c: Candidate, obj: bytes, linked: bytes, syms_ld: str, symmap: dict) -> tuple[dict, list]:
        """A helpers= source (tools/hstmatch.py): every function of the object is the header symbol or a listed helper,
        alone at the start of its own .text section, linked once at its own retail address; the linked code there is the
        OBJECT's section (not the renamed copy the matcher linked) with only relocation fields filled in; and, followed
        by the linker's zero fill up to the end of its range, it equals the retail bytes. Each helper is recorded in
        `compared` as `.text:<name>`; the claimed range stays the target's."""
        h = c.header
        o, ln = P.Elf(obj, "object"), P.Elf(linked, "linked image")
        texts = {s.index: s for s in o.named(".text", P.SHT_PROGBITS)}
        syms = o.symbols()
        linked_defs: dict[str, list[int]] = {}
        for s in ln.symbols():
            if s.shndx != 0 and s.name:
                linked_defs.setdefault(s.name, []).append(s.value)
        compared: dict = {}
        used: dict[int, str] = {}
        refs, ranges = [], []
        for name, start, end in M.placed_functions(h):
            span = end - start
            hits = [s for s in syms if s.name == name and s.shndx in texts]
            if len(hits) != 1:
                raise ProofError("symbol", f"{name} is not defined once in a .text section of the object")
            sec = texts[hits[0].shndx]
            if hits[0].value != 0 or sec.index in used:
                raise ProofError("placement", f"{name} is not alone at the start of its own .text section")
            used[sec.index] = name
            if sec.size > span:
                raise ProofError("range", f"{name} is {sec.size} bytes, longer than its range of {span}")
            if linked_defs.get(name) != [start]:
                raise ProofError("placement", f"{name} is not linked (once) at its retail address {hx(start)}")
            lsec = next((s for s in ln.sections if s.flags & P.SHF_ALLOC and s.type == P.SHT_PROGBITS
                         and s.addr <= start and start + sec.size <= s.addr + s.size), None)
            if lsec is None:
                raise ProofError("placement", f"no linked section holds {name} at {hx(start)}")
            code = ln.contents(lsec)[start - lsec.addr:start - lsec.addr + sec.size]
            bad = P.masked_difference(o.contents(sec), code, o.relocations(sec))
            if bad is not None:
                raise ProofError("relocation", f"the linked code differs from the object outside relocation fields "
                                               f"at {hx(start + bad)}")
            for s in syms:   # every named symbol of the section sits where the placement puts it
                if s.shndx == sec.index and s.name and s.info & 0xF not in (P.STT_SECTION, P.STT_FILE) \
                        and s.name in linked_defs and start + s.value not in linked_defs[s.name]:
                    raise ProofError("placement", f"{s.name} is linked at {hx(linked_defs[s.name][0])}, its function's "
                                                  f"placement puts it at {hx(start + s.value)}")
            ours = code + bytes(span - sec.size)
            retail = P.image_range(h["module"], start, end, self.target, executable=True)
            if ours != retail:
                n = sum(1 for i in range(0, span, 4) if ours[i:i + 4] != retail[i:i + 4])
                what = "" if name == h["symbol"] else f" (helper {name})"
                raise ProofError("mismatch", f"independent comparison{what}: {n} of {span // 4} words differ from retail")
            compared[".text" if name == h["symbol"] else f".text:{name}"] = {"addr": hx(start), "size": span,
                                                                             "sha256": P.sha256(ours)}
            refs += [(start + r.offset, r.sym) for r in o.relocations(sec)]
            ranges.append((start, end))
        rest = [s for i, s in texts.items() if s.size and i not in used]
        if rest:
            raise ProofError("placement", f"the object has {len(rest)} function section(s) that are neither the target "
                                          "nor a listed helper")
        self.compare_data(h, o, ln, compared)
        return compared, self.check_symbols(o, syms_ld, symmap, refs, ranges)

    def compare_data(self, h: dict, o: P.Elf, ln: P.Elf, compared: dict) -> None:
        """Each placed .rodata/.data/.sdata: linked at its header address, the object's bytes with only relocation fields
        filled in, equal to retail; recorded in `compared`."""
        for key in PLACED:
            inputs = [s for s in o.named("." + key, P.SHT_PROGBITS) if s.size]
            if not inputs or key not in h:
                continue       # absent, empty, or left unplaced (then it is not compared and not claimed)
            addr = int(h[key], 16)
            outs = ln.named("." + key)
            if len(outs) != 1 or outs[0].addr != addr:
                raise ProofError("placement", f".{key} is not linked at {hx(addr)}")
            lb = ln.contents(outs[0])
            for sec, a in o.placements("." + key, addr):
                bad = P.masked_difference(o.contents(sec), lb[a - addr:a - addr + sec.size], o.relocations(sec))
                if bad is not None:
                    raise ProofError("relocation", f"the linked .{key} differs from the object outside relocation fields")
            if lb != P.image_range(h["module"], addr, addr + len(lb), self.target, executable=False):
                raise ProofError("section-mismatch", f".{key} ({len(lb)} bytes at {hx(addr)}) differs from retail")
            compared["." + key] = {"addr": hx(addr), "size": len(lb), "sha256": P.sha256(lb)}

    def check_symbols(self, o: P.Elf, syms_ld: str, symmap: dict, refs: list, ranges: list[tuple[int, int]]) -> list:
        """`ranges`: the compared code ([lo, hi) linked addresses); a placeholder is allowed only for a name no
        relocation inside them uses."""
        syms = o.symbols()
        undefined = sorted({s.name for s in syms if s.shndx == 0 and s.name})
        defs: dict[str, tuple[int, bool]] = {}
        for line in syms_ld.splitlines():
            if not line.strip():
                continue
            name, sep, rest = line.partition(" = ")
            value, _, tail = rest.partition(";")
            if not sep or not P.HEX8.fullmatch(value) or tail.strip() not in ("", PLACEHOLDER) or name in defs:
                raise ProofError("symbols", f"unexpected line in the link script: {line[:80]}")
            defs[name] = (int(value, 16), tail.strip() == PLACEHOLDER)
        if sorted(defs) != undefined:
            raise ProofError("symbols", "the link script defines other names than the object's undefined ones")
        referenced = {syms[sym].name for addr, sym in refs if any(lo <= addr < hi for lo, hi in ranges)
                      and sym < len(syms) and syms[sym].shndx == 0}
        out = []
        for name in undefined:
            value, placeholder = defs[name]
            want = P.resolve(name, symmap)
            if placeholder:
                if value != 0 or want is not None or name in referenced:
                    raise ProofError("symbols", f"{name} was linked as a placeholder although "
                                                + ("it has an address" if want is not None else "the compared code uses it"))
                out.append([name, None])
            else:
                if want is None or want != value:
                    raise ProofError("symbols", f"{name} was linked at {hx(value)}, the symbol map gives "
                                                f"{hx(want) if want is not None else 'nothing'}")
                out.append([name, hx(value)])
        return out

    def make_proof(self, c: Candidate, row: dict | None, own_obj: Path, symmaps: dict) -> dict:
        h = c.header
        if row is None:
            raise ProofError("matcher", "tools/hstmatch.py produced no result for this source (see the run's match log)")
        if row.get("status") != "EXACT":
            raise ProofError("mismatch", f"tools/hstmatch.py: {row.get('status')} {row.get('detail', '')}".strip())
        if row.get("module") != h["module"] or row.get("start") != hx(h["start"]):
            raise ProofError("matcher", "the matcher's result names another function")
        art = self.build_dir(c)
        blobs = {}
        for name in ("obj.o", "linked.elf", "syms.ld"):
            f = art / name
            if not f.is_file():
                raise ProofError("artifact", f"the matcher left no {name}")
            if f.stat().st_mtime < self.started - 2:
                raise ProofError("stale-artifact", f"{name} predates this verification run")
            blobs[name] = f.read_bytes()
        if not own_obj.is_file():
            log = own_obj.with_name("compile.log")
            tail = log.read_text(errors="replace")[-300:] if log.exists() else ""
            raise ProofError("own-compile", f"the recorded profile produced no object: {tail.strip()}")
        if P.sha256(blobs["obj.o"]) != P.file_sha256(own_obj):
            raise ProofError("object", "the matcher's object differs from the one the recorded profile produces")
        symmap = symmaps.setdefault(h["module"], P.symbol_map(h["module"], self.root))
        compared, symbols = self.compare(c, blobs["obj.o"], blobs["linked.elf"], blobs["syms.ld"].decode("utf-8"), symmap)
        closure = P.include_closure(c.path, self.root)
        deps = P.deps_record(c.sha, closure, symbols)
        findings = hstgate.check_file(c.path, self.root)
        if hstgate.refusals(findings):
            raise ProofError("gate", f"fails the fake-match gate: {hstgate.summary(findings)}")
        if P.file_sha256(c.path) != c.sha:
            raise ProofError("source-changed", "the source changed during the verification run")
        proof = {"schema": P.PROOF_SCHEMA, "module": h["module"], "start": hx(h["start"]), "end": hx(h["end"]),
                 "symbol": h["symbol"], "source": c.rel, "source_sha256": c.sha, "cc": h.get("cc", P.DEFAULT_CC),
                 "lang": h["lang"], "deps": deps, "deps_sha256": P.digest(deps), "toolchain": self.toolchain_digest,
                 "target": self.target["target_digest"], "compared": compared,
                 "object_sha256": P.sha256(blobs["obj.o"]), "status": "EXACT",
                 "gate": {"result": "pass", "idioms": sorted({f.kind for f in findings if f.severity == hstgate.IDIOM})},
                 "run": self.id, "verified_utc": P.utc_now()}
        proof["proof_sha256"] = P.proof_digest(proof)
        return proof

    def verify(self, cands: list[Candidate]) -> tuple[dict[str, dict], dict[str, str]]:
        proofs: dict[str, dict] = {}
        refused: dict[str, str] = {}
        ok: list[Candidate] = []
        for c in cands:
            try:
                self.precheck(c)
                ok.append(c)
            except ProofError as e:
                refused[c.rel] = str(e)
        dirs = Counter((c.header["module"], self.build_dir(c)) for c in ok)
        for c in [c for c in ok if dirs[(c.header["module"], self.build_dir(c))] > 1]:
            refused[c.rel] = "artifact: another candidate shares its build directory name (same directory and stem)"
        ok = [c for c in ok if c.rel not in refused]
        if not ok:
            return proofs, refused
        say(f"verification run {self.id}: {len(ok)} source(s) ({len(refused)} refused before compiling)")
        rows = self.run_matcher(ok)
        own = self.run_own_compile(ok)
        symmaps: dict = {}
        for c in ok:
            try:
                proofs[c.rel] = self.make_proof(c, rows.get(c.rel), own[c.rel], symmaps)
            except ProofError as e:
                refused[c.rel] = str(e)
        return proofs, refused

    def finish(self, success: bool) -> None:
        if success and not self.keep:
            shutil.rmtree(self.dir, ignore_errors=True)
        else:
            say(f"run directory kept: {self.dir}")


def new_run(ctx: Context, args, profiles: dict | None = None, toolchain_digest: str | None = None) -> Run:
    return Run(ctx.root, profiles or ctx.tool["core"]["profiles"], toolchain_digest or ctx.tool["core_digest"],
               ctx.target, ctx.inventories, args.parallel, args.keep_run)


# --------------------------------------------------------------------------------------------------------------
# helpers

def overlaps(a: dict, b: dict) -> bool:
    am, as_, ae = P.proof_range(a)
    bm, bs, be = P.proof_range(b)
    return am == bm and as_ < be and bs < ae


class Owners:
    """Disjoint owned ranges per module, sorted, with an overlap lookup by bisection (owners never overlap, so their
    ends rise with their starts)."""

    def __init__(self, proofs=()):
        self.starts: dict[str, list[int]] = {}
        self.items: dict[str, list[tuple[int, int, dict]]] = {}
        for p in proofs:
            self.add(p)

    def clashes(self, p: dict) -> list[dict]:
        m, s, e = P.proof_range(p)
        st, it = self.starts.get(m, []), self.items.get(m, [])
        j = bisect.bisect_left(st, e) - 1
        out = []
        while j >= 0 and it[j][1] > s:
            out.append(it[j][2])
            j -= 1
        return out

    def add(self, p: dict) -> None:
        m, s, e = P.proof_range(p)
        st, it = self.starts.setdefault(m, []), self.items.setdefault(m, [])
        i = bisect.bisect_left(st, s)
        st.insert(i, s)
        it.insert(i, (s, e, p))

    def all(self) -> list[dict]:
        return [p for m in sorted(self.items) for _s, _e, p in self.items[m]]


def lost_ranges(old: list[dict], new: list[dict]) -> list[tuple[str, int, int]]:
    out = []
    for module in sorted({p["module"] for p in old}):
        was = [P.proof_range(p)[1:] for p in old if p["module"] == module]
        now = [P.proof_range(p)[1:] for p in new if p["module"] == module]
        out += [(module, s, e) for s, e in P.subtract(was, now)]
    return out


def read_paths(args, root: Path) -> list[str] | None:
    """The named sources; None when nothing was named (then: every unproven source). An empty --paths-from file names
    nothing, so only the accepted proofs that no longer hold are reverified."""
    names = list(args.paths or [])
    if args.paths_from:
        names += [ln.strip() for ln in Path(args.paths_from).read_text(encoding="utf-8").splitlines() if ln.strip()]
    elif not names:
        return None
    out = []
    for n in names:
        p = Path(n)
        p = p if p.is_absolute() else root / p
        try:
            out.append(P.rel(p, root))
        except ValueError:
            out.append(n)
    return out


def emit(args, doc: dict) -> None:
    if getattr(args, "json", None):
        Path(args.json).write_text(json.dumps(doc, indent=1, sort_keys=True) + "\n", encoding="utf-8")


def refuse(args, reason: str, **extra) -> int:
    say(f"REFUSED: {reason}")
    say("the accepted ledger is unchanged")
    emit(args, {"command": args.command, "status": "refused", "reason": reason, **extra})
    return 1


def print_refusals(refused: dict[str, str], limit: int = 60) -> None:
    for i, (r, why) in enumerate(sorted(refused.items())):
        if i == limit:
            say(f"  ... and {len(refused) - limit} more")
            break
        say(f"  not promoted: {r}: {why}")


# --------------------------------------------------------------------------------------------------------------
# commands

def cmd_lock(args) -> int:
    root = ROOT
    data = root / "data"
    with PromoteLock(root, args.lock_wait):
        tpath, cpath = data / P.TARGET_PATH.name, data / P.TOOLCHAIN_PATH.name
        if tpath.exists() or cpath.exists():
            return refuse(args, "the locks exist; change them with `retarget` or `migrate-toolchain`")
        target = P.live_target(root)
        tdig = P.digest(target)
        core = P.live_toolchain(root)
        cdig = P.digest(core)
        now = P.utc_now()
        P.write_json(tpath, {**target, "target_digest": tdig,
                             "history": [{"utc": now, "event": "lock", "reason": args.reason, "digest": tdig}]})
        P.write_json(cpath, {"schema": P.TOOLCHAIN_SCHEMA, "core": core, "core_digest": cdig,
                             "symbol_maps": "bound per proof: each proof records the address every undefined name it "
                                            "links resolved to; a proof whose resolutions change is reverified",
                             "history": [{"utc": now, "event": "lock", "reason": args.reason, "digest": cdig}]})
        say(f"target lock    {tdig}\ntoolchain lock {cdig}")
        return 0


def ownership(proofs: dict[str, dict]) -> tuple[list[dict], dict[str, str], dict[str, str]]:
    """Seed ownership: larger ranges first; a range inside an owner is subsumed; a partial overlap is refused."""
    owners = Owners()
    subsumed, refused = {}, {}
    for r, p in sorted(proofs.items(), key=lambda kv: (kv[1]["module"], -(unhx(kv[1]["end"]) - unhx(kv[1]["start"])),
                                                      unhx(kv[1]["start"]), kv[0])):
        clash = owners.clashes(p)
        if not clash:
            owners.add(p)
            continue
        _m, s, e = P.proof_range(p)
        holder = next((o for o in clash if unhx(o["start"]) <= s and e <= unhx(o["end"])), None)
        if holder is not None:
            subsumed[r] = holder["source"]
        else:
            refused[r] = "overlap: partially overlaps " + ", ".join(o["source"] for o in clash)
    return owners.all(), subsumed, refused


def cmd_seed(args) -> int:
    root = ROOT
    with PromoteLock(root, args.lock_wait):
        try:
            ctx = Context(root, need_ledger=False)
        except ProofError as e:
            return refuse(args, str(e))
        if ctx.ledger is not None or ctx.journal_path.exists():
            return refuse(args, "a ledger exists; the seed runs only once (use promote)")
        tree = P.tree_sources(root)
        baseline, base_doc = None, None
        if args.baseline:
            raw = Path(args.baseline).read_bytes()
            baseline = {row["source"]: row["status"] for row in json.loads(raw)}
            base_doc = {"file": Path(args.baseline).name, "sha256": P.sha256(raw), "rows": len(baseline),
                        "exact": sum(1 for s in baseline.values() if s == "EXACT")}
        run = new_run(ctx, args)
        proofs, refused = run.verify(candidates_for(tree, tree))
        owners, subsumed, overlap = ownership(proofs)
        refused.update(overlap)
        differences = []
        if baseline is not None:
            for r in sorted(set(tree) | set(baseline)):
                b, ok = baseline.get(r), r in proofs
                if (b == "EXACT") != ok:
                    differences.append({"source": r, "baseline": b, "run": "EXACT" if ok else refused.get(r, "absent")})
        event = {"event": "seed", "run": run.id, "sources": len(tree), "proved": len(proofs), "owners": len(owners),
                 "refused": len(refused), "subsumed": dict(sorted(subsumed.items())), "baseline": base_doc,
                 "baseline_differences": len(differences)}
        ev = ctx.write(owners, event)
        report = {"command": "seed", "status": "seeded", "run": run.id, "journal_seq": ev["seq"], "owners": len(owners),
                  "refused": refused, "subsumed": subsumed, "baseline": base_doc, "baseline_differences": differences}
        if args.report:
            Path(args.report).write_text(json.dumps(report, indent=1, sort_keys=True) + "\n", encoding="utf-8")
        emit(args, report)
        say(f"seeded: {len(owners):,} proofs from {len(tree):,} sources; {len(subsumed)} subsumed, {len(refused)} refused; "
            f"baseline differences: {len(differences) if baseline is not None else 'no baseline given'}")
        for r, holder in sorted(subsumed.items()):
            say(f"  subsumed: {r} (inside {holder})")
        print_refusals(refused)
        for d in differences[:40]:
            say(f"  baseline difference: {d['source']}: baseline {d['baseline']}, run {d['run']}")
        run.finish(True)
        return 0 if not refused else 3


def cmd_promote(args) -> int:
    root = ROOT
    with PromoteLock(root, args.lock_wait):
        try:
            ctx = Context(root)
        except ProofError as e:
            return refuse(args, str(e))
        led = ctx.ledger
        tree = P.tree_sources(root)
        symmaps: dict = {}
        intact, stale, why = [], [], {}
        for p in led.proofs:
            problems = P.proof_problems(p, root, symmaps, ctx.inventories)
            if problems or args.all:
                stale.append(p)
                why[p["source"]] = "; ".join(problems) or "--all"
            else:
                intact.append(p)
        named = read_paths(args, root)
        proven = led.by_source()
        if named is None:
            requested = {r for r in tree if r not in proven}
        else:
            requested = set(named)
            for r in sorted(requested - set(tree)):
                say(f"  not a source in src/ (skipped): {r}")
        intact_paths = {p["source"] for p in intact}
        stale_paths = {p["source"] for p in stale}
        cand = (stale_paths & set(tree)) | ((requested & set(tree)) - intact_paths)
        say(f"accepted {len(led.proofs):,}: {len(intact):,} still hold, {len(stale)} to reverify "
            f"({sum(1 for r in stale_paths if r not in tree)} with the source gone); {len(cand - stale_paths)} new candidate(s)")
        for r in sorted(stale_paths)[:20]:
            say(f"  reverify {r}: {why[r]}")
        new, refused = {}, {}
        run = None
        if cand:
            run = new_run(ctx, args)
            new, refused = run.verify(candidates_for(cand, tree))
        owned = Owners(intact)
        added, replaced = [], []
        old_by_path = {p["source"]: p for p in stale}
        for r, proof in sorted(new.items(), key=lambda kv: (kv[0] not in stale_paths, kv[1]["module"], unhx(kv[1]["start"]))):
            clash = owned.clashes(proof)
            if clash:
                refused[r] = "overlap: its range overlaps the accepted range of " + ", ".join(o["source"] for o in clash) \
                             + " (a byte has one owner)"
                continue
            owned.add(proof)
            (replaced if r in old_by_path else added).append(proof)
        owners = owned.all()
        lost = lost_ranges(led.proofs, owners)
        if lost:
            detail = []
            for module, s, e in lost:
                srcs = [p["source"] for p in led.proofs if p["module"] == module and unhx(p["start"]) < e and s < unhx(p["end"])]
                reasons = [f"{x}: {refused.get(x) or why.get(x) or 'not reverified'}" for x in srcs]
                detail.append({"module": module, "start": hx(s), "end": hx(e), "sources": srcs, "reasons": reasons})
                say(f"  LOST {module} [{hx(s)}, {hx(e)}) {e - s} bytes: " + " | ".join(reasons))
            if run:
                run.finish(False)
            return refuse(args, f"V_new does not cover V_accepted: {sum(e - s for _m, s, e in lost)} accepted bytes in "
                                f"{len(lost)} range(s) would be lost", lost=detail, refused=refused)
        if args.strict and refused:
            print_refusals(refused)
            if run:
                run.finish(False)
            return refuse(args, f"--strict: {len(refused)} candidate(s) refused", refused=refused)
        changed = bool(added or replaced) or len(owners) != len(led.proofs)
        ev = None
        if changed:
            dropped = sorted(p["source"] for p in stale if p["source"] not in {q["source"] for q in replaced})
            entry = lambda p: {"module": p["module"], "start": p["start"], "end": p["end"], "source": p["source"]}  # noqa: E731
            ev = ctx.write(owners, {"event": "promote", "run": run.id if run else None,
                                    "added": [entry(p) for p in added], "replaced": [entry(p) for p in replaced],
                                    "dropped": dropped, "refused": len(refused)})
        final = {p["source"] for p in owners}
        promoted = sorted(r for r in (requested | stale_paths) if r in final)
        if args.promoted_out:
            Path(args.promoted_out).write_text("".join(r + "\n" for r in promoted), encoding="utf-8", newline="\n")
        print_refusals(refused)
        status = "promoted" if changed else "unchanged"
        say(f"{status}: {len(added)} added, {len(replaced)} re-proved, {len(refused)} refused; ledger "
            f"{len(owners):,} proofs" + (f" (journal event {ev['seq']})" if ev else ""))
        emit(args, {"command": "promote", "status": status, "run": run.id if run else None, "added": [p["source"] for p in added],
                    "replaced": [p["source"] for p in replaced], "promoted": promoted, "refused": refused,
                    "journal_seq": ev["seq"] if ev else led.header["journal_seq"]})
        if run:
            run.finish(True)
        return 3 if refused else 0


def compare_proofs(old: dict, new: dict | None, refusal: str | None) -> str | None:
    if new is None:
        return f"no longer verifies: {refusal}"
    for k in ("module", "start", "end", "symbol", "source_sha256", "object_sha256", "compared", "deps_sha256", "cc", "lang"):
        if old.get(k) != new.get(k):
            return f"{k} differs from the proof"
    return None


def cmd_verify(args) -> int:
    root = ROOT
    try:
        ctx = Context(root)
    except ProofError as e:
        return refuse(args, str(e))
    proofs = ctx.ledger.proofs
    if args.since:
        old = P.git_file(root, args.since, "data/accepted-ledger.jsonl")
        before = set()
        if old:
            before = {json.loads(x)["proof_sha256"] for x in old.split(b"\n")[1:] if x.strip()}
        proofs = [p for p in proofs if p["proof_sha256"] not in before]
    elif args.sample:
        rng = random.Random(args.seed)
        proofs = rng.sample(proofs, min(args.sample, len(proofs)))
        say(f"sample of {len(proofs)} (seed {args.seed})")
    tree = P.tree_sources(root)
    problems = {}
    symmaps: dict = {}
    for p in proofs:
        why = P.proof_problems(p, root, symmaps, ctx.inventories)
        if why:
            problems[p["source"]] = "the tree no longer holds the proof: " + "; ".join(why)
    todo = [p for p in proofs if p["source"] not in problems]
    if todo:
        run = new_run(ctx, args)
        new, refused = run.verify(candidates_for({p["source"] for p in todo}, tree))
        for p in todo:
            d = compare_proofs(p, new.get(p["source"]), refused.get(p["source"]))
            if d:
                problems[p["source"]] = d
        run.finish(not problems)
    for r, d in sorted(problems.items()):
        say(f"  DIFFERENT {r}: {d}")
    say(f"verify: {len(proofs)} proof(s) checked, {len(problems)} different")
    emit(args, {"command": "verify", "status": "ok" if not problems else "different", "checked": len(proofs),
                "different": problems})
    return 1 if problems else 0


GATE_FILES = {"tools/hstgate.py", "data/sdk-gcc4-units.json"}   # what the gate decides from


def migration_scope(old: dict, new: dict) -> tuple[str, set[str]]:
    """'gate' when only tools/hstgate.py or its SDK unit list (data/sdk-gcc4-units.json) changed; 'include' when only
    include/ changed (with or without the gate);
    'full' for anything else (a compiler, kit, binutils, environment, profile or other comparison-code change)."""
    keys = {k for k in set(old) | set(new) if old.get(k) != new.get(k)}
    a, b = old.get("comparison_code") or {}, new.get("comparison_code") or {}
    code = {f for f in set(a) | set(b) if a.get(f) != b.get(f)}
    oi, ni = old.get("include") or {}, new.get("include") or {}
    headers = {f for f in set(oi) | set(ni) if oi.get(f) != ni.get(f)}
    if keys - {"comparison_code", "include"} or code - GATE_FILES:
        return "full", headers
    return ("include" if headers else "gate"), headers


def cmd_migrate(args) -> int:
    root = ROOT
    with PromoteLock(root, args.lock_wait):
        try:
            ctx = Context(root, check_toolchain=False)   # bound to the lock; the live toolchain is what changed
        except ProofError as e:
            return refuse(args, str(e))
        live = P.live_toolchain(root)
        new_digest = P.digest(live)
        if new_digest == ctx.tool["core_digest"]:
            say("toolchain unchanged: nothing to migrate")
            return 0
        changes = P.describe_toolchain_diff(ctx.tool["core"], live)
        scope, changed_headers = migration_scope(ctx.tool["core"], live)
        say("toolchain changes: " + "; ".join(changes) + f" (migration scope: {scope})")
        led = ctx.ledger
        symmaps: dict = {}
        holding = {p["source"]: P.proof_problems(p, root, symmaps, ctx.inventories) for p in led.proofs}
        if scope == "include":   # a changed header is a change of every source that includes it (proof_problems says so)
            holding = {r: [w for w in ws if w != "included headers changed"] for r, ws in holding.items()}
        broken = {r: w for r, w in holding.items() if w}
        if broken:
            return refuse(args, f"{len(broken)} proof(s) do not hold for the tree; promote (or restore the tree) first: "
                                + "; ".join(f"{r}: {', '.join(w)}" for r, w in list(broken.items())[:5]))
        # what the change can affect: everything, except for a gate-only change (the gate verdict of every source is
        # re-decided) and an include/-only change (every source whose dependency record names a changed header, or
        # whose includes are undeterminable, is reverified)
        if scope == "full":
            todo = [p for p in led.proofs]
        elif scope == "gate":
            todo = []
        else:
            todo = [p for p in led.proofs if "include_tree_sha256" in p["deps"]
                    or any(h in changed_headers for h, _sha in p["deps"]["headers"])]
        run = new_run(ctx, args, live["profiles"], new_digest)
        tree = P.tree_sources(root)
        new, refused = run.verify(candidates_for({p["source"] for p in todo}, tree)) if todo else ({}, {})
        failures = {}
        out = []
        reverified = {p["source"] for p in todo}
        for p in led.proofs:
            if p["source"] in reverified:
                q = new.get(p["source"])
                if q is None:
                    failures[p["source"]] = refused.get(p["source"], "not reverified")
                elif P.proof_range(q) != P.proof_range(p):
                    failures[p["source"]] = "range differs"
                else:
                    out.append(q)
                continue
            findings = hstgate.check_file(root / p["source"], root)
            if hstgate.refusals(findings):
                failures[p["source"]] = f"fails the changed fake-match gate: {hstgate.summary(findings)}"
                continue
            q = {k: v for k, v in p.items() if k != "proof_sha256"}
            q["toolchain"] = new_digest
            q["gate"] = {"result": "pass", "idioms": sorted({f.kind for f in findings if f.severity == hstgate.IDIOM})}
            q["rebound"] = {"event": "migrate-toolchain", "scope": scope, "from": ctx.tool["core_digest"], "utc": P.utc_now()}
            q["proof_sha256"] = P.proof_digest(q)
            out.append(q)
        if failures:
            for r, w in sorted(failures.items())[:60]:
                say(f"  FAILS under the new toolchain: {r}: {w}")
            run.finish(False)
            return refuse(args, f"{len(failures)} accepted proof(s) do not hold under the new toolchain")
        ev = P.write_ledger(out, {"event": "migrate-toolchain", "reason": args.reason, "run": run.id,
                                  "from": ctx.tool["core_digest"], "to": new_digest, "changes": changes, "scope": scope,
                                  "reverified": len(todo), "rebound": len(out) - len(todo)},
                            new_digest, ctx.target["target_digest"], ctx.ledger_path, ctx.journal_path, root)
        doc = dict(ctx.tool)
        doc["core"], doc["core_digest"] = live, new_digest
        doc["history"] = ctx.tool.get("history", []) + [{"utc": P.utc_now(), "event": "migrate-toolchain",
                                                         "reason": args.reason, "from": ctx.tool["core_digest"],
                                                         "to": new_digest, "changes": changes, "scope": scope,
                                                         "run": run.id, "reverified": len(todo),
                                                         "journal_seq": ev["seq"]}]
        P.write_json(ctx.toolchain_path, doc)
        say(f"migrated ({scope}): {len(todo):,} proofs reverified, {len(out) - len(todo):,} rebound (gate re-run), "
            f"toolchain {new_digest}")
        run.finish(True)
        return 0


def cmd_retarget(args) -> int:
    root = ROOT
    data = root / "data"
    with PromoteLock(root, args.lock_wait):
        try:
            old = P.load_target_manifest(data / P.TARGET_PATH.name)
            tool = P.load_toolchain_manifest(data / P.TOOLCHAIN_PATH.name)
            P.verify_toolchain(tool, P.live_toolchain(root))
            led = P.load_ledger(data / P.LEDGER_PATH.name, data / P.JOURNAL_PATH.name)
            P.check_binding(led, tool["core_digest"], old["target_digest"])
            live = P.live_target(root)
        except ProofError as e:
            return refuse(args, str(e))
        new_digest = P.digest(live)
        if new_digest == old["target_digest"]:
            say("target unchanged: nothing to retarget")
            return 0
        changes = P.describe_target_diff(P.target_identity(old), live)
        say("target changes: " + changes)
        new_manifest = {**live, "target_digest": new_digest}
        failures = {}
        rebound = []
        inv: dict = {}
        for p in led.proofs:
            module, s, e = P.proof_range(p)
            try:
                starts, ends = P.boundaries(inv.setdefault(module, P.live_inventory(module)))
                if s not in starts or e not in ends:
                    raise ProofError("range", "not aligned to the new inventory")
                for name, rec in p["compared"].items():
                    a = unhx(rec["addr"])
                    code = name.split(":")[0] == ".text"     # ".text:<helper>": a same-file helper (helpers=)
                    if code and name != ".text" and (a not in starts or a + rec["size"] not in ends):
                        raise ProofError("range", f"helper {name[6:]} is not aligned to the new inventory")
                    got = P.image_range(module, a, a + rec["size"], new_manifest, executable=code)
                    if P.sha256(got) != rec["sha256"]:
                        raise ProofError("mismatch", f"{name} bytes differ in the new target")
            except ProofError as err:
                failures[p["source"]] = str(err)
                continue
            q = {k: v for k, v in p.items() if k != "proof_sha256"}
            q["target"] = new_digest
            q["rebound"] = {"event": "retarget", "from": old["target_digest"], "utc": P.utc_now()}
            q["proof_sha256"] = P.proof_digest(q)
            rebound.append(q)
        if failures:
            for r, w in sorted(failures.items())[:60]:
                say(f"  does not hold for the new target: {r}: {w}")
            return refuse(args, f"{len(failures)} accepted proof(s) do not hold for the new target")
        ev = P.write_ledger(rebound, {"event": "retarget", "reason": args.reason, "from": old["target_digest"],
                                      "to": new_digest, "changes": changes},
                            tool["core_digest"], new_digest, data / P.LEDGER_PATH.name, data / P.JOURNAL_PATH.name, root)
        new_manifest["history"] = old.get("history", []) + [{"utc": P.utc_now(), "event": "retarget", "reason": args.reason,
                                                             "from": old["target_digest"], "to": new_digest,
                                                             "changes": changes, "journal_seq": ev["seq"]}]
        P.write_json(data / P.TARGET_PATH.name, new_manifest)
        say(f"retargeted: {len(rebound):,} proofs rebound to {new_digest}")
        return 0


def cmd_retract(args) -> int:
    root = ROOT
    with PromoteLock(root, args.lock_wait):
        try:
            ctx = Context(root)
        except ProofError as e:
            return refuse(args, str(e))
        start = int(args.start, 16)
        hit = [p for p in ctx.ledger.proofs if p["module"] == args.module and unhx(p["start"]) == start]
        if not hit:
            return refuse(args, f"no accepted proof starts at {hx(start)} in {args.module}")
        p = hit[0]
        _m, s, e = P.proof_range(p)
        say(f"RETRACTING {p['source']} [{p['start']}, {p['end']}): {e - s} accepted bytes leave the ledger; reason: {args.reason}")
        ev = ctx.write([q for q in ctx.ledger.proofs if q is not p],
                       {"event": "retract", "reason": args.reason,
                        "removed": [{"module": p["module"], "start": p["start"], "end": p["end"], "source": p["source"],
                                     "proof_sha256": p["proof_sha256"]}]})
        say(f"retracted (journal event {ev['seq']})")
        return 0


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="command", required=True)

    def common(sp):
        sp.add_argument("--parallel", type=int, default=4, help="concurrent compiles (default 4)")
        sp.add_argument("--keep-run", action="store_true", help="keep build/proof/<run>/ after a successful run")
        sp.add_argument("--lock-wait", type=float, default=3600, help="seconds to wait for try/promote.lock")
        sp.add_argument("--json", type=Path, help="write the result as JSON")

    lk = sub.add_parser("lock")
    lk.add_argument("--reason", required=True)
    common(lk)
    sd = sub.add_parser("seed")
    sd.add_argument("--baseline", type=Path)
    sd.add_argument("--report", type=Path)
    common(sd)
    pr = sub.add_parser("promote")
    pr.add_argument("paths", nargs="*")
    pr.add_argument("--paths-from", type=Path)
    pr.add_argument("--all", action="store_true", help="reverify every accepted proof as well")
    pr.add_argument("--strict", action="store_true", help="refuse when any candidate is refused")
    pr.add_argument("--promoted-out", type=Path)
    common(pr)
    vf = sub.add_parser("verify")
    vf.add_argument("--since", help="only proofs that differ from the ledger at this git revision")
    vf.add_argument("--sample", type=int)
    vf.add_argument("--seed", type=int, default=int(time.time()) % 100000)
    common(vf)
    mg = sub.add_parser("migrate-toolchain")
    mg.add_argument("--reason", required=True)
    common(mg)
    rt = sub.add_parser("retarget")
    rt.add_argument("--reason", required=True)
    common(rt)
    rr = sub.add_parser("retract")
    rr.add_argument("module")
    rr.add_argument("start")
    rr.add_argument("--reason", required=True)
    common(rr)
    args = ap.parse_args()
    if getattr(args, "reason", "x") is not None and not str(getattr(args, "reason", "x")).strip():
        ap.error("--reason must say why")
    commands = {"lock": cmd_lock, "seed": cmd_seed, "promote": cmd_promote, "verify": cmd_verify,
                "migrate-toolchain": cmd_migrate, "retarget": cmd_retarget, "retract": cmd_retract}
    try:
        return commands[args.command](args)
    except ProofError as e:
        return refuse(args, str(e))


if __name__ == "__main__":
    raise SystemExit(main())
