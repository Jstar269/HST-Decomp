#!/usr/bin/env python3
"""Whole-module build: compile every source, scaffold the rest from the retail bytes, link in retail order, compare.

    python tools/hstbuild.py EBOOT.BIN [--parallel 6] [--private] [--src-dir DIR ...] [--reuse-isolation hstmatch.json]
                             [--shift N] [--out-dir DIR --report-dir DIR] [--mutate KIND --mutate-index K
                              --mutate-scope scaffold|compiled]

This is the step from "each function matches on its own" (tools/hstmatch.py) toward a module that is rebuilt from
source (docs/COMPLETION.md, tiers 2 and 3). For one module it

  1. compiles every source of the module exactly as hstmatch.py does (the same profile and flags: the objects are
     hstmatch's own, build/<module>/<dir>__<stem>/obj.o; hstmatch is run on the sources unless --reuse-isolation names
     a hstmatch --json result) and keeps those that are EXACT in isolation;
  2. reduces each object to what the isolated link compared (the function's text sections, the data sections its
     header places with rodata=/data=/...), renames them, and turns the symbols of everything it drops into undefined
     symbols the link defines from the layout;
  3. writes SCAFFOLDING for every retail byte that no source covers: one assembly file with a `.word` section per
     unmatched function (tiled by the ledger's function boundaries) and `.incbin` blobs of the retail section dump
     for everything else (data, tables, stubs, module info). Every scaffolding word that a retail relocation covers
     has its field cleared and a `.reloc` of the same type against the symbol of its target, defined as tile symbol
     + offset, so the link writes it and it moves with the layout. Scaffolding is tier 0: it never counts as progress;
  4. writes a linker script that places every tile at its retail address in retail order (defining `__hst_at_<addr>`
     at each), in the retail output sections, and links with `psp-ld -q` (the link's relocations are kept, to check
     the generated tables against);
  5. compares the linked sections with the retail sections and each source's ranges; GENERATES the relocation tables
     (tools/hstreloc.py: one block per tile, a compiled source's block from its object's records in their order, the
     others from the scaffolding's records) and the container (ELF header, program header, section header table,
     .shstrtab, laid out by the module's container profile), assembles the whole file and compares its SHA-256 with
     the retail module's. Only .comment (the original toolchain's name) is copied from the retail file;
  6. with --shift N, links the same objects again with N bytes of padding ahead of the code and N more ahead of the
     data, and compares the result byte for byte with an independent expectation computed from the retail image and
     its relocation tables (gate 5, static half; the runtime half is not run here: `shifted_runtime`). The census
     lists what makes a source name an address as a number; T3 counts the sources clean in both halves of it.

The Sony library modules have no section headers and format-B relocations: their scaffolding is relocatable too, but
their tables are not regenerated and their container is copied (counted).

Outputs (build/whole/<module>/, or --out-dir): scaffold-text.s, scaffold-data.bin, link.ld, syms.ld, linked.elf,
image.bin (link-shift.ld, linked-shift.elf, image-shift.bin) and try/build/<module>-report.json (or --report-dir; read by
tools/hstprogress.py and tools/hstmilestones.py: `t2_source_bytes` and `t3_source_bytes` map each source path to the
bytes it contributes that the build reproduces). The report names every source by its repository path.
"""
from __future__ import annotations

import argparse
import bisect
import contextlib
import dataclasses
import hashlib
import os
import json
import shlex
import struct
import subprocess
import sys
import time
from dataclasses import dataclass, field
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hstmatch as M
import hstorigin
import hstpublish
import hstreloc as R

ROOT = M.ROOT
_CLASSES: dict[str, str] = {}
SHN_UNDEF, SHN_ABS, SHN_COMMON = 0, 0xFFF1, 0xFFF2
SHT_PROGBITS, SHT_SYMTAB, SHT_STRTAB, SHT_REL, SHT_NOBITS = 1, 2, 3, 9, 8
PLACED_KEYS = ("rodata", "data", "sdata", "sbss", "bss")
CONTAINER_NOTE = ("ELF header, program header, section header table, relocation tables (.rel.*), .shstrtab and "
                  ".comment are taken from the retail file")


# --------------------------------------------------------------------------------------------------------------
# a small ELF32 little-endian reader and writer (relocatable objects and the linked module)

@dataclass
class Sec:
    name: str
    type: int
    flags: int
    addr: int
    off: int
    size: int
    link: int
    info: int
    align: int
    entsize: int
    data: bytes | None = None        # None for NOBITS


@dataclass
class Elf:
    ehdr: tuple
    secs: list[Sec] = field(default_factory=list)

    def by_name(self, name: str) -> Sec | None:
        return next((s for s in self.secs if s.name == name), None)


EH = struct.Struct("<16sHHIIIIIHHHHHH")
SH = struct.Struct("<IIIIIIIIII")


def parse_elf(b: bytes) -> Elf:
    ehdr = EH.unpack_from(b, 0)
    e_shoff, e_shentsize, e_shnum, e_shstrndx = ehdr[6], ehdr[11], ehdr[12], ehdr[13]
    raw = [SH.unpack_from(b, e_shoff + i * e_shentsize) for i in range(e_shnum)]
    stroff = raw[e_shstrndx][4] if raw else 0
    secs = []
    for n, t, fl, addr, off, size, link, info, align, ent in raw:
        name = b[stroff + n:b.index(b"\0", stroff + n)].decode() if e_shnum else ""
        data = None if t == SHT_NOBITS else bytes(b[off:off + size])
        secs.append(Sec(name, t, fl, addr, off, size, link, info, align, ent, data))
    return Elf(ehdr, secs)


def build_elf(elf: Elf) -> bytes:
    """Serialize: the sections keep their order and indices; offsets, shstrtab and the section header table are rebuilt."""
    names = bytearray(b"\0")
    name_off = []
    for s in elf.secs:
        name_off.append(len(names) if s.name else 0)
        if s.name:
            names += s.name.encode() + b"\0"
    shstrndx = elf.ehdr[13]
    elf.secs[shstrndx].data = bytes(names)
    elf.secs[shstrndx].size = len(names)
    out = bytearray(EH.size)
    offs = [0] * len(elf.secs)
    for i, s in enumerate(elf.secs):
        if i == 0:
            continue
        if s.type == SHT_NOBITS:
            offs[i] = len(out)
            continue
        align = max(s.align, 1)
        out += b"\0" * (-len(out) % align)
        offs[i] = len(out)
        out += s.data or b""
    out += b"\0" * (-len(out) % 4)
    shoff = len(out)
    for i, s in enumerate(elf.secs):
        out += SH.pack(name_off[i], s.type, s.flags, s.addr, offs[i], s.size, s.link, s.info, s.align, s.entsize)
    e = list(elf.ehdr)
    e[5] = 0          # e_phoff: relocatable objects have no program headers
    e[6] = shoff
    e[10] = 0
    out[:EH.size] = EH.pack(*e)
    return bytes(out)


SYM = struct.Struct("<IIIBBH")


def parse_symbols(elf: Elf) -> tuple[int, list[list]]:
    """(symtab section index, [[name_offset, value, size, info, other, shndx], ...])."""
    idx = next(i for i, s in enumerate(elf.secs) if s.type == SHT_SYMTAB)
    data = elf.secs[idx].data
    return idx, [list(SYM.unpack_from(data, o)) for o in range(0, len(data), SYM.size)]


def strtab_get(strtab: bytes, off: int) -> str:
    return strtab[off:strtab.index(b"\0", off)].decode()


def aligned_layout(sizes_aligns: list[tuple[int, int]]) -> list[int]:
    """Offsets of input sections placed one after another with their alignments (what ld does inside one output section)."""
    offs, pos = [], 0
    for size, align in sizes_aligns:
        align = max(align, 1)
        pos += -pos % align
        offs.append(pos)
        pos += size
    return offs


# --------------------------------------------------------------------------------------------------------------
# preparing one compiled source for the whole-module link

@dataclass
class Prepared:
    path: str                        # repository path of the source
    module: str
    uid: str                         # file stem of the rewritten object (o000123)
    start: int
    end: int
    text_addr: int                   # address of the first kept text section
    text_size: int                   # bytes the kept text sections occupy
    data: list[tuple[str, int, int]] = field(default_factory=list)   # (key, address, size) of placed data sections
    globals_defined: set[str] = field(default_factory=set)
    undefined: set[str] = field(default_factory=set)
    relocs: list[tuple[int, int]] = field(default_factory=list)       # (module address, relocation type) the object records
    # the object's relocation records per claim ("text", or a placed data key), in the object's own order:
    # [(module address, relocation type, symbol name)]
    records: dict[str, list[tuple[int, int, str]]] = field(default_factory=dict)
    # symbols of removed parts, now undefined in the object and defined by the link from the layout:
    # name -> (address in the isolated link, address of the tile to anchor it to, or None for the tile containing it)
    dropped: dict[str, tuple[int, int | None]] = field(default_factory=dict)


class Excluded(Exception):
    pass


def prepare_object(path: Path, h: dict, obj_bytes: bytes, uid: str) -> tuple[bytes, Prepared]:
    """Reduce one hstmatch object to what the isolated link compared: the text of the function span (a section that
    only partly lies in the span is sliced, relocations and symbols with it) and the data sections the header places.
    Everything else is removed; the symbols of removed parts become unique undefined symbols, recorded in
    Prepared.dropped with the address they had in the isolated link; the link defines each as the tile containing that
    address + offset, so a reference still resolves to the same value and moves with the layout."""
    elf = parse_elf(obj_bytes)
    symtab_idx, syms = parse_symbols(elf)
    strtab = bytearray(elf.secs[elf.secs[symtab_idx].link].data)
    names = [strtab_get(bytes(strtab), s[0]) for s in syms]
    text = [i for i, s in enumerate(elf.secs) if s.name == ".text" and s.type == SHT_PROGBITS]
    if not text:
        raise Excluded("object has no .text section")
    offs = dict(zip(text, aligned_layout([(elf.secs[i].size, elf.secs[i].align) for i in text])))
    target = next((s for s, n in zip(syms, names) if n == h["symbol"] and s[5] in offs), None)
    if target is None:
        raise Excluded(f"symbol {h['symbol']} is not defined in a .text section")
    base = h["start"] - (offs[target[5]] + target[1])
    abs0 = {i: base + offs[i] for i in text}
    for name, hstart, _hend in h.get("helpers") or []:
        # a same-file helper (helpers=) sits at its own retail address, not next to the target: its section is dropped
        # here like any code outside the span, and the references to it resolve to that address (the tile there)
        hit = next((s for s, n in zip(syms, names) if n == name and s[5] in offs), None)
        if hit is not None:
            abs0[hit[5]] = hstart - hit[1]
    span = {i: (max(abs0[i], h["start"]), min(abs0[i] + elf.secs[i].size, h["end"])) for i in text}
    kept = [i for i in text if span[i][0] < span[i][1]]
    if target[5] not in kept:
        raise Excluded("the target function is outside its own header span")
    if kept != text[text.index(kept[0]):text.index(kept[0]) + len(kept)]:
        raise Excluded("the kept text sections are not contiguous in the object")
    placed: list[tuple[str, int, int, list[int]]] = []
    for key in PLACED_KEYS:
        if key in h:
            idxs = [i for i, s in enumerate(elf.secs) if s.name == f".{key}" and s.size]
            if idxs:
                lay = aligned_layout([(elf.secs[i].size, elf.secs[i].align) for i in idxs])
                placed.append((key, int(h[key], 16), lay[-1] + elf.secs[idxs[-1]].size, idxs))
    data_keep = {i: f".hst.{key}" for key, _a, _n, idxs in placed for i in idxs}
    # --- slice the partly covered text sections
    sliced: dict[int, tuple[int, int]] = {}       # section -> (lo, hi) byte range kept
    for i in kept:
        lo, hi = span[i][0] - abs0[i], span[i][1] - abs0[i]
        if (lo, hi) != (0, elf.secs[i].size):
            sliced[i] = (lo, hi)
    # --- new section list: kept text, placed data, their relocation sections, symtab/strtab/shstrtab
    keep_old = sorted(set(kept) | set(data_keep) | {symtab_idx, elf.secs[symtab_idx].link, elf.ehdr[13]} |
                      {i for i, s in enumerate(elf.secs) if s.type == SHT_REL and s.info in (set(kept) | set(data_keep))})
    keep_old = [0] + [i for i in keep_old if i != 0]
    new_of = {old: new for new, old in enumerate(keep_old)}
    new_secs = []
    for old in keep_old:
        sec = elf.secs[old]
        c = Sec(sec.name, sec.type, sec.flags, sec.addr, sec.off, sec.size, sec.link, sec.info, sec.align, sec.entsize, sec.data)
        if old in kept:
            c.name = ".hst.text"
            if old in sliced:
                lo, hi = sliced[old]
                c.data, c.size, c.align = sec.data[lo:hi], hi - lo, 1
        elif old in data_keep:
            c.name = data_keep[old]
        elif sec.type == SHT_REL:
            tgt = sec.info
            c.link, c.info = new_of[sec.link], new_of[tgt]
            if tgt in sliced:
                lo, hi = sliced[tgt]
                rows = [struct.unpack_from("<II", sec.data, o) for o in range(0, len(sec.data), 8)]
                c.data = b"".join(struct.pack("<II", off - lo, info) for off, info in rows if lo <= off < hi)
                c.size = len(c.data)
        elif old == symtab_idx:
            c.link = new_of[sec.link]
        new_secs.append(c)
    # --- symbols
    defined, undefined = set(), set()
    dropped: dict[str, tuple[int, int | None]] = {}
    out_syms = []
    for k, sy in enumerate(syms):
        sy = list(sy)
        shndx, bind, typ = sy[5], sy[3] >> 4, sy[3] & 15
        if k == 0 or shndx == SHN_UNDEF:
            if shndx == SHN_UNDEF and k > 0 and names[k]:
                undefined.add(names[k])
        elif shndx == SHN_COMMON:
            raise Excluded(f"common symbol {names[k]}")
        elif shndx >= 0xFF00:
            pass                                        # SHN_ABS and friends
        elif shndx in new_of and shndx not in sliced:
            sy[5] = new_of[shndx]
            if bind == 1 and names[k] and shndx in kept:
                defined.add(names[k])
            elif bind == 1 and names[k] and shndx in data_keep:
                defined.add(names[k])
        elif shndx in sliced and sy[1] < sliced[shndx][1] and (sy[1] >= sliced[shndx][0] and typ != 3):
            sy[1] -= sliced[shndx][0]                   # a symbol inside the kept slice, rebased
            sy[5] = new_of[shndx]
            if bind == 1 and names[k]:
                defined.add(names[k])
        else:
            # a symbol of a removed part, or the section symbol of a sliced/removed section, under a unique name. A
            # removed text part becomes an undefined symbol that the link defines from the layout (so it moves with
            # its tile under --shift); the section symbol of a sliced section is anchored to this source's own tile
            # (the section starts before the slice). A removed data section that the header does not place has no
            # module address: it stays absolute at 0, as before.
            name = f"__hst_dropped_{uid}_{k}"
            sy[0] = len(strtab)
            strtab += name.encode() + b"\0"
            if shndx in abs0:
                value = abs0[shndx] + (0 if typ == 3 else sy[1])
                dropped[name] = (value, h["start"] if (typ == 3 and shndx in sliced) else None)
                sy[1], sy[2], sy[5], sy[3] = 0, 0, SHN_UNDEF, (1 << 4) | (typ if typ != 3 else 0)
            else:
                sy[1], sy[5], sy[3] = 0, SHN_ABS, (1 << 4) | (typ if typ != 3 else 0)
        out_syms.append(sy)
    symdata = b"".join(SYM.pack(*sy) for sy in out_syms)
    new_secs[new_of[symtab_idx]].data = symdata
    new_secs[new_of[symtab_idx]].size = len(symdata)
    strsec = new_secs[new_of[elf.secs[symtab_idx].link]]
    strsec.data, strsec.size = bytes(strtab), len(strtab)
    ehdr = list(elf.ehdr)
    ehdr[13] = new_of[elf.ehdr[13]]
    ehdr[12] = len(new_secs)
    text_addr, text_end = span[kept[0]][0], span[kept[-1]][1]
    rel_of: dict[int, list[list[tuple[int, int]]]] = {}
    for sec in elf.secs:
        if sec.type == SHT_REL:
            rel_of.setdefault(sec.info, []).append([struct.unpack_from("<II", sec.data, o) for o in range(0, len(sec.data), 8)])
    records: dict[str, list[tuple[int, int, str]]] = {"text": []}
    for i in kept:                                   # kept text sections in object order, each one's rows in order
        lo = sliced[i][0] if i in sliced else 0
        hi = sliced[i][1] if i in sliced else elf.secs[i].size
        for rows in rel_of.get(i, []):
            records["text"] += [(abs0[i] + off, info & 0xFF, names[info >> 8]) for off, info in rows if lo <= off < hi]
    for key, addr, _size, idxs in placed:
        lay = aligned_layout([(elf.secs[i].size, elf.secs[i].align) for i in idxs])
        records[key] = [(addr + lay[n] + off, info & 0xFF, names[info >> 8])
                        for n, i in enumerate(idxs) for rows in rel_of.get(i, []) for off, info in rows]
    relocs = [(a, t) for recs in records.values() for a, t, _n in recs]
    prep = Prepared(path=path.as_posix(), module=h["module"], uid=uid, start=h["start"], end=h["end"],
                    text_addr=text_addr, text_size=text_end - text_addr,
                    data=[(k, a, sz) for k, a, sz, _ in placed], globals_defined=defined, undefined=undefined,
                    relocs=sorted(relocs), records=records, dropped=dropped)
    return build_elf(Elf(tuple(ehdr), new_secs)), prep


# --------------------------------------------------------------------------------------------------------------
# the retail module and the tiling

@dataclass
class Retail:
    data: bytes
    elf: Elf
    loaded: list[Sec]                # parts of the loadable image, in address order: the sections, or one pseudo-section
                                     # per PT_LOAD segment (.seg0, .seg1) when the module has no section header table
    bss: list[tuple[int, int]]       # address ranges that are zero-initialised and have no file content (memsz beyond filesz)
    segments: list[tuple]            # (offset, vaddr, filesz, memsz, flags) of every PT_LOAD


def load_retail(module: str) -> tuple[Retail, dict]:
    cache: dict = {}
    data, seg = M.module_bytes(cache, module)
    elf = parse_elf(data)
    phoff, phentsize, phnum = elf.ehdr[5], elf.ehdr[9], elf.ehdr[10]
    segments = []
    for i in range(phnum):
        p_type, p_off, p_vaddr, _paddr, p_filesz, p_memsz, p_flags, _align = struct.unpack_from("<IIIIIIII", data, phoff + i * phentsize)
        if p_type == 1:
            segments.append((p_off, p_vaddr, p_filesz, p_memsz, p_flags))
    if len(elf.secs) > 1:
        loaded = [s for s in elf.secs if s.flags & 2 and s.type == SHT_PROGBITS and s.name and s.size]
    else:
        loaded = [Sec(f".seg{i}", SHT_PROGBITS, 2 | (4 if fl & 1 else 0) | (1 if fl & 2 else 0), va, off, fsz, 0, 0, 16, 0, None)
                  for i, (off, va, fsz, msz, fl) in enumerate(segments)]
    loaded.sort(key=lambda s: s.addr)
    bss = [(va + fsz, va + msz) for _off, va, fsz, msz, _fl in segments if msz > fsz]
    return Retail(data, elf, loaded, bss, segments), cache


def ledger_functions(module: str) -> list[tuple[int, int]]:
    led = json.loads((M.CAMPAIGN / "ledger" / "campaign.json").read_text(encoding="utf-8-sig"))
    mod = next(m for m in led["modules"] if m["name"] == module)
    return sorted((f["start"], f["end"]) for f in mod["functions"])


def words_asm(data: bytes) -> str:
    out = []
    for i in range(0, len(data), 32):
        chunk = data[i:i + 32]
        out.append("    .word " + ",".join(f"0x{int.from_bytes(chunk[j:j + 4], 'little'):08x}" for j in range(0, len(chunk), 4)))
    return "\n".join(out)


# --------------------------------------------------------------------------------------------------------------

def to_wsl_dir(p: Path) -> str:
    return M.to_wsl(p)


ARG_BUDGET = 24000     # characters of paths per hstmatch call: Windows refuses command lines over 32,767


def chunks(files: list[str], budget: int = ARG_BUDGET) -> list[list[str]]:
    out: list[list[str]] = [[]]
    size = 0
    for f in files:
        if out[-1] and size + len(f) + 1 > budget:
            out.append([])
            size = 0
        out[-1].append(f)
        size += len(f) + 1
    return [c for c in out if c]


def run_isolation(files: list[Path], parallel: int, out_json: Path) -> list[dict]:
    """hstmatch on every file, in calls short enough for the Windows command line; a call that writes no result is
    an error (its rows would otherwise be read from a previous run's file)."""
    out_json.parent.mkdir(parents=True, exist_ok=True)
    cmd = [sys.executable, str(Path(M.__file__)), "--quiet", "--parallel", str(parallel), "--json", str(out_json)]
    rows: list[dict] = []
    for part in chunks([str(f.relative_to(ROOT)) if f.is_relative_to(ROOT) else str(f) for f in files]):
        out_json.unlink(missing_ok=True)
        done = subprocess.run(cmd + part, cwd=ROOT, stdout=subprocess.DEVNULL, stderr=subprocess.PIPE, text=True)
        if not out_json.exists():
            raise SystemExit(f"hstmatch wrote no result (exit {done.returncode}): {done.stderr.strip()[-2000:]}")
        rows += json.loads(out_json.read_text(encoding="utf-8"))
    out_json.write_text(json.dumps(rows, indent=1) + "\n", encoding="utf-8")
    return rows


def collect_sources(module: str, src_dirs: list[Path]) -> list[Path]:
    out = []
    for d in src_dirs:
        for p in sorted(d.rglob("*")):
            if p.suffix in (".c", ".cpp"):
                try:
                    if M.parse_header(p)["module"] == module:
                        out.append(p)
                except ValueError:
                    pass
    return out


LOCK_POLL = 5.0       # seconds between attempts to take a module's build lock


@contextlib.contextmanager
def module_lock(module: str, root: Path = ROOT, poll: float = LOCK_POLL):
    """One hstbuild run per module at a time: build/whole/<module> is wiped at the start of a run, so a second run on
    the same module would destroy the first one's files. The lock is the directory try/build/<module>.lock (mkdir is
    atomic); a run waits for it and removes it when it ends, also on an error."""
    lock = root / "try" / "build" / f"{module}.lock"
    lock.parent.mkdir(parents=True, exist_ok=True)
    waited = False
    while True:
        try:
            lock.mkdir()
            break
        except FileExistsError:
            if not waited:
                print(f"{module}: waiting for {lock} (another hstbuild run on this module holds it)", flush=True)
                waited = True
            time.sleep(poll)
    try:
        stamp = time.strftime("%Y-%m-%d %H:%M:%S")
        (lock / "owner.txt").write_text(f"pid {os.getpid()} since {stamp}\n", encoding="utf-8")
        yield lock
    finally:
        for f in lock.glob("*"):
            f.unlink(missing_ok=True)
        lock.rmdir()


def build(module: str, args: argparse.Namespace) -> int:
    out = getattr(args, "out_dir", None)
    with module_lock(module if not out else f"{module}@{Path(out).name}"):
        return build_locked(module, args)


def build_locked(module: str, args: argparse.Namespace) -> int:
    t0 = time.time()
    out_dir = Path(args.out_dir) / module if getattr(args, "out_dir", None) else ROOT / "build" / "whole" / module
    obj_dir = out_dir / "obj"
    if out_dir.exists():
        for p in out_dir.rglob("*"):
            if p.is_file():
                p.unlink()
    obj_dir.mkdir(parents=True, exist_ok=True)
    src_dirs = [Path(d) if Path(d).is_absolute() else ROOT / d for d in (args.src_dir or ["src"])]
    sources = collect_sources(module, src_dirs)
    public = not args.private
    if public and not _CLASSES:
        _CLASSES.update(hstorigin.classify(ROOT)[0])
    print(f"{module}: {len(sources)} sources, {'public' if public else 'private'} build")
    # 1. isolation (the same compile and compare as hstmatch)
    if args.reuse_isolation:
        rows = json.loads(Path(args.reuse_isolation).read_text(encoding="utf-8"))
    else:
        rows = run_isolation(sources, args.parallel, ROOT / "try" / "build" / f"{module}-isolation.json")
    status = {Path(r["source"]).as_posix(): r for r in rows}
    if args.reuse_isolation:       # sources the reused run did not cover (added since) are matched now
        missing = [p for p in sources if p.resolve().relative_to(ROOT).as_posix() not in status]
        if missing:
            print(f"  {len(missing)} sources not in {Path(args.reuse_isolation).name}: running hstmatch on them")
            extra_dir = Path(args.report_dir) if getattr(args, "report_dir", None) else ROOT / "try" / "build"
            rows += run_isolation(missing, args.parallel, extra_dir / f"{module}-isolation-extra.json")
            status = {Path(r["source"]).as_posix(): r for r in rows}
    retail, cache = load_retail(module)
    report: dict = {"module": module, "schema": "hst-decomp.build-report.v1", "excluded": [], "differs_in_link": [],
                    "build_mode": "public" if public else "private", "owner_supplied_sources": []}
    prepared: list[Prepared] = []
    owner_supplied = 0
    for n, p in enumerate(sources):
        rel = p.resolve().relative_to(ROOT).as_posix()
        if public:
            cls = _CLASSES.get(rel) or hstorigin.own_class(p, ROOT)
            if cls in hstpublish.EXCLUDED_CLASSES:
                h0 = M.parse_header(p)
                report["owner_supplied_sources"].append({"path": rel, "class": cls, "bytes": h0["end"] - h0["start"]})
                owner_supplied += h0["end"] - h0["start"]
                continue
        row = status.get(rel) or status.get(p.as_posix())
        if row is None or row["status"] != "EXACT":
            report["excluded"].append({"path": rel, "reason": f"not EXACT in isolation ({row['status'] if row else 'no result'})"})
            continue
        h = M.parse_header(p)
        build_dir = ROOT / "build" / module / M.build_key(p)
        try:
            obj_bytes = (build_dir / "obj.o").read_bytes()
        except FileNotFoundError:
            report["excluded"].append({"path": rel, "reason": "no isolation object (build dir missing)"})
            continue
        uid = f"o{n:05d}"
        try:
            new_obj, prep = prepare_object(Path(rel), h, obj_bytes, uid)
        except Excluded as exc:
            report["excluded"].append({"path": rel, "reason": str(exc)})
            continue
        (obj_dir / f"{uid}.o").write_bytes(new_obj)
        prepared.append(prep)
    # overlaps between sources (a later source that overlaps an earlier one is left out)
    prepared.sort(key=lambda q: q.start)
    kept: list[Prepared] = []
    for q in prepared:
        if kept and q.start < kept[-1].end:
            report["excluded"].append({"path": q.path, "reason": f"overlaps {kept[-1].path}"})
            continue
        kept.append(q)
    prepared = kept
    # duplicate global definitions
    seen: dict[str, str] = {}
    for q in list(prepared):
        dup = [n for n in q.globals_defined if n in seen]
        if dup:
            report["excluded"].append({"path": q.path, "reason": f"global {dup[0]} also defined by {seen[dup[0]]}"})
            prepared.remove(q)
            (obj_dir / f"{q.uid}.o").unlink()
            continue
        for n in q.globals_defined:
            seen[n] = q.path
    print(f"  {len(prepared)} sources prepared, {len(report['excluded'])} excluded")
    link_and_compare(module, retail, prepared, out_dir, obj_dir, report, args)
    report["seconds"] = round(time.time() - t0, 1)
    report["built"] = time.strftime("%Y-%m-%d", time.gmtime())          # the date tools/hstprogress.py shows for T2
    rep_path = ROOT / "try" / "build" / (f"{module}-report.json" if public else f"{module}-private-report.json")
    if getattr(args, "report_dir", None):                # an experiment: its report must not replace the build's
        rep_path = Path(args.report_dir) / rep_path.name
    rep_path.parent.mkdir(parents=True, exist_ok=True)
    rep_path.write_text(json.dumps(report, indent=1) + "\n", encoding="utf-8")
    print(f"report: {rep_path.relative_to(ROOT).as_posix() if rep_path.is_relative_to(ROOT) else rep_path.as_posix()}")
    return 0


# --------------------------------------------------------------------------------------------------------------
# the retail relocation records

@dataclass
class RetailRel:
    table: str                       # ".rel.text", ... or "format B"
    index: int                       # position in its table
    site: int                        # module address of the relocated word
    type: int                        # MIPS relocation type (R.R_MIPS_*)
    target: int | None               # module address the word refers to (None: not recoverable)
    owner: int = -1                  # start of the tile whose block of the table the entry belongs to
    base: int = 0                    # format B: link address of the address segment; the word holds target - base


CONTAINER_PROFILE = {"EBOOT.BIN": "eboot", "hstp_trans_main.prx": "sharing"}
RELOC_NAMES = {R.R_MIPS_32: "R_MIPS_32", R.R_MIPS_26: "R_MIPS_26", R.R_MIPS_HI16: "R_MIPS_HI16", R.R_MIPS_LO16: "R_MIPS_LO16"}


def retail_word(retail: Retail, addr: int) -> int:
    s = next(s for s in retail.loaded if s.addr <= addr < s.addr + s.size)
    return struct.unpack_from("<I", retail.data, s.off + addr - s.addr)[0]


def function_lookup(funcs: list[tuple[int, int]]):
    starts = [a for a, _b in funcs]

    def function_of(addr: int) -> tuple[int, int]:
        k = bisect.bisect_right(starts, addr) - 1
        if k >= 0 and addr < funcs[k][1]:
            return funcs[k]
        return (addr, -1)                            # outside every function: its own group
    return function_of


def in_module(retail: Retail, addr: int) -> bool:
    return (any(s.addr <= addr < s.addr + s.size for s in retail.loaded)
            or any(lo <= addr < hi for lo, hi in retail.bss))


def retail_records(retail: Retail, funcs: list[tuple[int, int]]) -> tuple[list[RetailRel], dict[str, list[R.Entry]]]:
    """Every relocation entry of the retail module with the address it refers to, and the format-A tables
    ({section name: entries}, empty for a module whose relocations are format B)."""
    function_of = function_lookup(funcs)
    word = lambda a: retail_word(retail, a)                 # noqa: E731
    inside = lambda a: in_module(retail, a)                 # noqa: E731
    out: list[RetailRel] = []
    tables: dict[str, list[R.Entry]] = {}
    for sec in retail.elf.secs:
        if sec.type == R.SHT_PSP_REL:
            entries = R.decode_a(sec.data or b"", len(retail.segments))
            tables[sec.name] = entries
            for k, (e, t) in enumerate(zip(entries, R.recover_targets(entries, word, function_of, inside))):
                out.append(RetailRel(sec.name, k, e.offset, e.type, t))
    phoff, phnum = retail.elf.ehdr[5], retail.elf.ehdr[10]
    bases = [va for _off, va, _fsz, _msz, _fl in retail.segments]
    for i in range(phnum):
        p_type, p_off, _va, _pa, p_filesz = struct.unpack_from("<IIIII", retail.data, phoff + 32 * i)
        if p_type != R.PT_PSP_REL_B:
            continue
        # format B words are relative to their address segment: the loader adds that segment's load address, so a
        # word holds target - (link address of the segment); a HI16 carries the low half it is paired with
        hi: dict[int, int] = {}                             # address segment -> immediate << 16 of the last HI16 into it
        for k, e in enumerate(R.decode_b(retail.data[p_off:p_off + p_filesz], len(retail.segments))):
            site = bases[e.oseg] + e.offset
            base = bases[e.aseg]
            w = word(site)
            if e.type == R.R_MIPS_32:
                t = w
            elif e.type == R.R_MIPS_26:
                t = (site & 0xF0000000) | ((w & 0x03FFFFFF) << 2)
                base = 0
            elif e.type == R.R_MIPS_HI16:
                hi[e.aseg] = (w & 0xFFFF) << 16
                t = hi[e.aseg] + e.addend
            else:
                t = hi.get(e.aseg, 0) + R.sext16(w)
            out.append(RetailRel("format B", k, site, e.type, (t + base) & 0xFFFFFFFF, base=base))
    return out, tables


# --------------------------------------------------------------------------------------------------------------
# tiles and anchors

@dataclass
class Tile:
    section: str
    start: int
    size: int
    kind: str                        # "asm", "blob" (scaffolding), "text" or a placed data key (a compiled source)
    ref: str                         # scaffold section name, or the object's uid

    @property
    def anchor(self) -> str:
        return f"__hst_at_{self.start:08x}"


class Anchors:
    """Which tile a module address belongs to, so every address can be written as tile symbol + offset: the link
    defines `__hst_at_<start>` at every tile it places, and a symbol defined from it moves with that tile."""

    def __init__(self, tiles: list[Tile], sections: list[Sec], bss: list[tuple[int, int]], bss_claims: list[Tile]):
        self.tiles = sorted(tiles, key=lambda t: t.start)
        self.starts = [t.start for t in self.tiles]
        self.ends = {s.addr + s.size: max((t for t in self.tiles if t.section == s.name), key=lambda t: t.start, default=None)
                     for s in sections}
        self.bss = bss
        self.bss_claims = sorted(bss_claims, key=lambda t: t.start)

    def tile_at(self, addr: int) -> Tile | None:
        k = bisect.bisect_right(self.starts, addr) - 1
        if k >= 0 and addr < self.tiles[k].start + self.tiles[k].size:
            return self.tiles[k]
        return None

    def of(self, addr: int, prefer: int | None = None) -> tuple[str, int] | None:
        if prefer is not None:
            t = self.tile_at(prefer)
            if t is not None:
                return t.anchor, addr - t.start
        t = self.tile_at(addr)
        if t is not None:
            return t.anchor, addr - t.start
        for c in self.bss_claims:
            if c.start <= addr < c.start + c.size:
                return c.anchor, addr - c.start
        t = self.ends.get(addr)
        if t is not None:
            return t.anchor, addr - t.start
        for n, (lo, hi) in enumerate(self.bss):
            if lo <= addr <= hi:
                return f"__hst_bss{n}", addr - lo
        return None

    def expr(self, addr: int, prefer: int | None = None) -> str | None:
        a = self.of(addr, prefer)
        if a is None:
            return None
        return f"{a[0]} + 0x{a[1]:x}" if a[1] >= 0 else f"{a[0]} - 0x{-a[1]:x}"


def target_symbol(r: RetailRel) -> str:
    return f"__hst_r_{r.target:08x}" if not r.base else f"__hst_r_{r.target:08x}_b{r.base:08x}"


def target_definition(r: RetailRel, anchors: "Anchors") -> str | None:
    """The link's definition of a record's target symbol: tile + offset, or for a segment-relative (format B) word the
    distance from the segment's start (a constant when the target lies outside the module, like a gp value)."""
    e = anchors.expr(r.target)
    if not r.base:
        return e
    b = anchors.expr(r.base)
    if e and b:
        return f"({e}) - ({b})"
    return f"0x{(r.target - r.base) & 0xFFFFFFFF:x}"


def masked(word: int, rtype: int) -> int:
    """The word with its relocated field cleared: the link writes the whole field from the target symbol."""
    if rtype == R.R_MIPS_32:
        return 0
    if rtype == R.R_MIPS_26:
        return word & 0xFC000000
    return word & 0xFFFF0000


MUTATIONS = ("flip", "drop", "swap", "addend", "instruction")


def mutate_records(recs: list[RetailRel], kind: str, index: int) -> tuple[list[RetailRel], dict]:
    """A deliberately wrong copy of the scaffolding's relocation records, for the mutation checks: `flip` changes the
    type of one record (R_MIPS_32 <-> R_MIPS_26), `drop` removes one, `swap` exchanges two neighbouring records of one
    block in their table order, `addend` points one record 4 bytes past its target. `index` picks the n-th eligible
    record."""
    recs = list(recs)
    if kind == "flip":
        cands = [k for k, r in enumerate(recs) if r.type in (R.R_MIPS_32, R.R_MIPS_26)]
        k = cands[index % len(cands)]
        r = recs[k]
        recs[k] = dataclasses.replace(r, type=R.R_MIPS_26 if r.type == R.R_MIPS_32 else R.R_MIPS_32)
    elif kind == "drop":
        k = index % len(recs)
        r = recs.pop(k)
    elif kind == "swap":
        cands = [k for k in range(len(recs) - 1) if recs[k].table == recs[k + 1].table and recs[k].owner == recs[k + 1].owner
                 and recs[k + 1].index == recs[k].index + 1 and R.R_MIPS_HI16 not in (recs[k].type, recs[k + 1].type)
                 and (recs[k].site, recs[k].type) != (recs[k + 1].site, recs[k + 1].type)]
        k = cands[index % len(cands)]
        a, b = recs[k], recs[k + 1]
        recs[k], recs[k + 1] = dataclasses.replace(b, index=a.index), dataclasses.replace(a, index=b.index)
        r = a
    elif kind == "addend":
        cands = [k for k, r in enumerate(recs) if r.type in (R.R_MIPS_32, R.R_MIPS_LO16) and r.target is not None]
        k = cands[index % len(cands)]
        r = recs[k]
        recs[k] = dataclasses.replace(r, target=r.target + 4)
    else:
        raise ValueError(kind)
    return recs, {"kind": kind, "index": index, "table": r.table, "entry": r.index, "site": f"0x{r.site:08x}",
                  "type": RELOC_NAMES.get(r.type, r.type)}


def mutate_compiled_object(prepared: list[Prepared], obj_dir: Path, kind: str, index: int) -> dict:
    """Change one compiled object's relocation-site addend or one non-relocated text word.

    These are test-only corruptions: the default link and shifted link must both notice them. The relocation-site
    variant is restricted to R_MIPS_32 and R_MIPS_LO16 records in compiled text so it changes the word represented
    by an entry in a compiled source's block. The instruction variant skips every relocation site so the linker
    cannot overwrite the changed word.
    """
    if kind not in ("addend", "instruction"):
        raise ValueError(f"compiled-object mutation does not support {kind!r}")
    objects = []
    for q in prepared:
        path = obj_dir / f"{q.uid}.o"
        elf = parse_elf(path.read_bytes())
        text_ids = [i for i, sec in enumerate(elf.secs) if sec.name == ".hst.text" and sec.data is not None]
        if not text_ids:
            continue
        bases = dict(zip(text_ids, aligned_layout([(elf.secs[i].size, elf.secs[i].align) for i in text_ids])))
        sites: set[int] = set()
        for relsec in elf.secs:
            if relsec.type != SHT_REL or relsec.info not in bases or not relsec.data:
                continue
            for off in range(0, len(relsec.data), 8):
                reloc_off, info = struct.unpack_from("<II", relsec.data, off)
                sites.add(q.text_addr + bases[relsec.info] + reloc_off)
                rtype = info & 0xFF
                if kind == "addend" and rtype in (R.R_MIPS_32, R.R_MIPS_LO16):
                    objects.append((q, path, elf, relsec.info, reloc_off, rtype,
                                    q.text_addr + bases[relsec.info] + reloc_off))
        if kind == "instruction":
            for secid in text_ids:
                sec = elf.secs[secid]
                for off in range(0, (len(sec.data) or 0) - 3, 4):
                    site = q.text_addr + bases[secid] + off
                    if site not in sites:
                        objects.append((q, path, elf, secid, off, 0, site))
    if not objects:
        raise ValueError(f"no compiled text candidates for {kind}")
    q, path, elf, secid, off, rtype, site = objects[index % len(objects)]
    sec = elf.secs[secid]
    data = bytearray(sec.data or b"")
    word, = struct.unpack_from("<I", data, off)
    if kind == "addend":
        if rtype == R.R_MIPS_32:
            changed = (word + 4) & 0xFFFFFFFF
        else:
            changed = (word & 0xFFFF0000) | ((word + 4) & 0xFFFF)
        type_name = RELOC_NAMES[rtype]
        table = ".rel.text"
    else:
        changed = word ^ 1
        type_name = "instruction word"
        table = None
    struct.pack_into("<I", data, off, changed)
    sec.data = bytes(data)
    path.write_bytes(build_elf(elf))
    return {"kind": kind, "scope": "compiled", "index": index, "path": q.path,
            "table": table, "site": f"0x{site:08x}", "type": type_name}


# --------------------------------------------------------------------------------------------------------------
# the link

def plan_tiles(retail: Retail, prepared: list[Prepared], funcs: list[tuple[int, int]], report: dict,
               obj_dir: Path) -> tuple[list[Sec], list[Tile], list[Tile], list[tuple]]:
    """Claims (the address ranges compiled sources cover) and the tiles of every loadable section: a compiled claim, or a
    scaffolding tile (an unmatched function, or the bytes between claims). A source whose ranges do not fit one
    loadable section (or the zero-initialised area) is left out."""
    sections = [s for s in retail.loaded if s.size]
    progbits = [x for x in sections if x.type == SHT_PROGBITS]
    inside = lambda a, n: any(x.addr <= a and a + n <= x.addr + x.size for x in progbits)       # noqa: E731
    in_bss = lambda a, n: any(lo <= a and a + n <= hi for lo, hi in retail.bss)                    # noqa: E731
    claims: list[tuple[int, int, str, str]] = []        # (start, end, kind, uid)
    for q in list(prepared):
        mine = [(q.text_addr, q.text_size, "text")] + [(a, n, k) for k, a, n in q.data]
        bad = next((c for c in mine if not (in_bss(c[0], c[1]) if c[2] in ("bss", "sbss") else inside(c[0], c[1]))), None)
        if bad is not None:
            report["excluded"].append({"path": q.path, "reason": f"{bad[2]} range 0x{bad[0]:08x}+{bad[1]:#x} is outside the loadable sections"})
            prepared.remove(q)
            (obj_dir / f"{q.uid}.o").unlink()
            continue
        claims += [(a, a + n, k, q.uid) for a, n, k in mine]
    claims.sort()
    for a, b in zip(claims, claims[1:]):
        if b[0] < a[1]:
            raise SystemExit(f"internal: claims overlap {a} / {b}")
    tiles: list[Tile] = []
    func_starts = sorted({f[0] for f in funcs})
    for s in sections:
        if s.type != SHT_PROGBITS:
            continue
        lo, hi = s.addr, s.addr + s.size
        in_sec = [c for c in claims if c[1] > lo and c[0] < hi]
        points = {lo, hi}
        for c in in_sec:
            points.update((max(c[0], lo), min(c[1], hi)))
        if s.flags & 4:                                   # executable section: tile by function
            points.update(f for f in func_starts if lo < f < hi)
        pts = sorted(points)
        for a, b in zip(pts, pts[1:]):
            owner = next((c for c in in_sec if c[0] <= a and b <= c[1]), None)
            if owner is not None:
                if a == owner[0]:
                    tiles.append(Tile(s.name, a, owner[1] - owner[0], owner[2], owner[3]))
                continue
            if s.flags & 4 and a % 4 == 0 and (b - a) % 4 == 0:
                tiles.append(Tile(s.name, a, b - a, "asm", f".hst.t_{a:08x}"))
            else:
                tiles.append(Tile(s.name, a, b - a, "blob", f".hst.d_{a:08x}"))
    bss_claims = [Tile(f".bss{n}", c[0], c[1] - c[0], c[2], c[3]) for c in claims if c[2] in ("bss", "sbss")
                  for n, (lo, hi) in enumerate(retail.bss) if lo <= c[0] < hi]
    return sections, tiles, bss_claims, claims


SECTION_FLAGS = {6: '"ax"', 2: '"a"', 3: '"aw"', 7: '"awx"'}


def write_scaffold(out_dir: Path, retail: Retail, tiles: list[Tile], recs: list[RetailRel], anchors: Anchors,
                   sec_flags: dict[str, int]) -> dict:
    """scaffold-text.s and scaffold-data.bin: every scaffolding tile as a section of retail bytes, with every word a
    retail relocation covers cleared and replaced by a `.reloc` against the symbol of its target, so the link writes
    it from the layout. Returns counts for the report."""
    by_site: dict[int, RetailRel] = {}
    for r in recs:
        by_site.setdefault(r.site, r)
    sites = sorted(by_site)
    image = retail.data
    sec_by_name = {s.name: s for s in retail.loaded}
    text_asm = [".set noreorder", ".set noat"]
    blob = bytearray()
    counts = {"code_asm": 0, "blob": 0, "relocated_words": 0, "unrelocatable_words": 0}
    unrelocatable: list[str] = []
    for t in tiles:
        if t.kind not in ("asm", "blob"):
            continue
        s = sec_by_name[t.section]
        chunk = bytearray(image[s.off + t.start - s.addr:s.off + t.start - s.addr + t.size])
        relocs = []
        k = bisect.bisect_left(sites, t.start)
        while k < len(sites) and sites[k] < t.start + t.size:
            r = by_site[sites[k]]
            k += 1
            off = r.site - t.start
            if r.target is None or target_definition(r, anchors) is None or off + 4 > t.size:
                counts["unrelocatable_words"] += 1
                if len(unrelocatable) < 20:
                    unrelocatable.append(f"0x{r.site:08x}")
                continue
            w, = struct.unpack_from("<I", chunk, off)
            struct.pack_into("<I", chunk, off, masked(w, r.type))
            relocs.append(f"    .reloc 0x{off:x}, {RELOC_NAMES[r.type]}, {target_symbol(r)}")
            counts["relocated_words"] += 1
        flags = SECTION_FLAGS.get(sec_flags.get(t.section, 3), '"aw"')
        text_asm.append(f".section {t.ref},{flags},%progbits")
        if t.kind == "asm":
            text_asm.append(words_asm(bytes(chunk)))
            counts["code_asm"] += t.size
        else:
            text_asm.append(f'    .incbin "{M.to_wsl(out_dir / "scaffold-data.bin")}", {len(blob)}, {t.size}')
            blob += chunk
            counts["blob"] += t.size
        text_asm += relocs
    (out_dir / "scaffold-text.s").write_text("\n".join(text_asm) + "\n", encoding="utf-8", newline="\n")
    (out_dir / "scaffold-data.bin").write_bytes(bytes(blob))
    counts["unrelocatable_examples"] = unrelocatable
    return counts


def write_symbols(out_dir: Path, module: str, prepared: list[Prepared], recs: list[RetailRel], anchors: Anchors) -> list[str]:
    """syms.ld: every name the compiled objects leave undefined, every dropped part of an object and every scaffolding
    relocation target, as tile symbol + offset (a name whose address lies outside the module stays absolute).
    Returns the names that have no address."""
    defined_all: set[str] = set()
    undefined_all: set[str] = set()
    for q in prepared:
        defined_all |= q.globals_defined
        undefined_all |= q.undefined
    sym_map = M.load_symbols(module)
    lines, unresolved = [], []
    for name in sorted(undefined_all - defined_all):
        addr = sym_map.get(name, M.default_address(name))
        if addr is None:
            unresolved.append(name)
            continue
        e = anchors.expr(addr)
        lines.append(f"{name} = {e};" if e else f"{name} = 0x{addr:08x};")
    for q in prepared:
        for name, (addr, prefer) in sorted(q.dropped.items()):
            e = anchors.expr(addr, prefer)
            lines.append(f"{name} = {e};" if e else f"{name} = 0x{addr:08x};")
    done: set[str] = set()
    for r in sorted((r for r in recs if r.target is not None), key=lambda r: (r.target, r.base)):
        name = target_symbol(r)
        e = target_definition(r, anchors)
        if e and name not in done:
            done.add(name)
            lines.append(f"{name} = {e};")
    (out_dir / "syms.ld").write_text("\n".join(lines) + "\n", encoding="utf-8", newline="\n")
    return unresolved


def write_link_script(path: Path, out_dir: Path, sections: list[Sec], tiles: list[Tile], bss: list[tuple[int, int]],
                      bss_claims: list[Tile], shift: R.Shift) -> None:
    ld = ['OUTPUT_FORMAT("elf32-littlemips")', f'INCLUDE "{M.to_wsl(out_dir / "syms.ld")}"', "SECTIONS", "{"]
    by_sec: dict[str, list[Tile]] = {}
    for t in tiles:
        by_sec.setdefault(t.section, []).append(t)
    for s in sections:
        ld.append(f"  {s.name} 0x{s.addr + shift.delta(s.addr):08x} :")
        ld.append("  {")
        for t in sorted(by_sec.get(s.name, []), key=lambda t: t.start):
            ld.append(f"    . = 0x{t.start - s.addr:08x}; {t.anchor} = .;      /* 0x{t.start:08x} */")
            if t.kind in ("asm", "blob"):
                ld.append(f"    scaffold-text.o({t.ref})")
            elif t.kind == "text":
                ld.append(f"    {t.ref}.o(.hst.text)")
            else:
                ld.append(f"    {t.ref}.o(.hst.{t.kind})")
        ld.append("  }")
    for n, (lo, hi) in enumerate(bss):
        ld.append(f"  .bss{n} 0x{lo + shift.delta(lo):08x} (NOLOAD) :")
        ld.append("  {")
        ld.append(f"    __hst_bss{n} = .;")
        for c in bss_claims:
            if lo <= c.start < hi:
                ld.append(f"    . = 0x{c.start - lo:08x}; {c.anchor} = .;      /* 0x{c.start:08x} */")
                ld.append(f"    {c.ref}.o(.hst.{c.kind})")
        ld.append("  }")
    ld += ["  /DISCARD/ : { *(*) }", "}"]
    path.write_text("\n".join(ld) + "\n", encoding="utf-8", newline="\n")


def run_link(out_dir: Path, obj_dir: Path, script: str, elf_name: str, assemble: bool) -> Path | None:
    pspdev = M.pspdev_bin()
    wo = shlex.quote(M.to_wsl(out_dir))
    wobj = shlex.quote(M.to_wsl(obj_dir))
    lines = []
    if assemble:
        lines += [f"cd {wo} && {pspdev}/psp-as -EL -o scaffold-text.o scaffold-text.s > as.log 2>&1 || exit 0",
                  f"cp {wo}/scaffold-text.o {wobj}/"]
    lines.append(f"cd {wobj} && {pspdev}/psp-ld -EL -e 0 -q --no-warn-mismatch -nostdlib "
                 f"-T {wo}/{script} -Map {wo}/{elf_name}.map -o {wo}/{elf_name} @{wo}/objs.rsp > {wo}/{elf_name}.log 2>&1")
    (out_dir / elf_name).unlink(missing_ok=True)
    M.run_wsl(lines, "wholebuild")
    path = out_dir / elf_name
    return path if path.exists() else None


def link_log(out_dir: Path, names: tuple[str, ...]) -> str:
    log = ""
    for name in names:
        f = out_dir / name
        if f.exists():
            log += f"--- {name}\n" + f.read_text(errors="replace")[-1500:]
    return log


def link_and_compare(module: str, retail: Retail, prepared: list[Prepared], out_dir: Path, obj_dir: Path,
                     report: dict, args: argparse.Namespace) -> None:
    funcs = ledger_functions(module)
    sections, tiles, bss_claims, claims = plan_tiles(retail, prepared, funcs, report, obj_dir)
    anchors = Anchors(tiles, sections, retail.bss, bss_claims)
    recs, tables = retail_records(retail, funcs)
    # each format-A entry's block owner: the tile of its site (a HI16: of its partner's site)
    for name, entries in tables.items():
        owners = R.block_owner(entries, lambda a: (anchors.tile_at(a) or Tile("", a, 0, "", "")).start)
        for r in recs:
            if r.table == name:
                r.owner = owners[r.index]
    def in_scaffold(addr: int) -> bool:
        t = anchors.tile_at(addr)
        return t is not None and t.kind in ("asm", "blob")
    # the scaffolding carries the records of the words it holds; the others belong to compiled objects
    scaffold_recs = [r for r in recs if in_scaffold(r.site)]
    fixed = [r for r in recs if not in_scaffold(r.site)]
    if getattr(args, "mutate", None):
        if getattr(args, "mutate_scope", "scaffold") == "compiled":
            mutation = mutate_compiled_object(prepared, obj_dir, args.mutate, args.mutate_index)
        else:
            scaffold_recs, mutation = mutate_records(scaffold_recs, args.mutate, args.mutate_index)
        report["mutation"] = mutation
        print(f"  MUTATION {mutation}")
    plan = sorted(scaffold_recs + fixed, key=lambda r: (r.table, r.index))
    sec_flags = {s.name: s.flags for s in retail.loaded}
    counts = write_scaffold(out_dir, retail, tiles, scaffold_recs, anchors, sec_flags)
    unresolved = write_symbols(out_dir, module, prepared, scaffold_recs, anchors)
    objs = ["scaffold-text.o"] + [f"{q.uid}.o" for q in prepared]
    (out_dir / "objs.rsp").write_text("\n".join(objs) + "\n", encoding="utf-8", newline="\n")
    report["scaffolding"] = {"assembled_code_bytes": counts["code_asm"], "blob_bytes": counts["blob"],
                             "owner_supplied_bytes": sum(o["bytes"] for o in report["owner_supplied_sources"]),
                             "note": "retail bytes, tier 0: never counted as progress; owner_supplied_bytes is the part of "
                                     "assembled_code_bytes that belongs to sources of an origin class the public tree leaves out "
                                     "(supplied from the owner's own executable at build time); relocation_entry_bytes are "
                                     "the relocation-table entries the build takes from the scaffolding's records "
                                     "(8 bytes each) rather than from a compiled object"}
    report["relocatable_scaffolding"] = {"relocated_words": counts["relocated_words"],
                                         "unrelocatable_words": counts["unrelocatable_words"],
                                         "unrelocatable_examples": counts["unrelocatable_examples"],
                                         "note": "every scaffolding word a retail relocation covers is cleared and written "
                                                 "by the link from a symbol defined as tile + offset"}
    if unresolved:
        report["unresolved_symbols"] = unresolved[:50]
        print(f"  {len(unresolved)} unresolved symbol names (first: {unresolved[:3]})")
    print(f"  scaffolding: {counts['relocated_words']:,} relocated words as .reloc, {counts['unrelocatable_words']} left absolute")
    data_start = next((s.addr for s in sections if s.name in (".data", ".seg1")), sections[-1].addr + sections[-1].size)
    write_link_script(out_dir / "link.ld", out_dir, sections, tiles, retail.bss, bss_claims, R.Shift(0, data_start))
    elf_path = run_link(out_dir, obj_dir, "link.ld", "linked.elf", assemble=True)
    if elf_path is None:
        report["link_failed"] = link_log(out_dir, ("as.log", "linked.elf.log"))
        print("  LINK FAILED\n" + report["link_failed"])
        return
    linked = parse_elf(elf_path.read_bytes())
    ctx = BuildContext(module, retail, prepared, tiles, anchors, plan, scaffold_recs, tables, sections, out_dir)
    compare(ctx, linked, report)
    shift_n = getattr(args, "shift", None)
    if shift_n is not None:
        shift_build(ctx, report, R.Shift(shift_n, data_start), obj_dir)


@dataclass
class BuildContext:
    module: str
    retail: Retail
    prepared: list[Prepared]
    tiles: list[Tile]
    anchors: Anchors
    recs: list[RetailRel]                # every relocation record of the build's plan (retail, or a mutated copy)
    scaffold_recs: list[RetailRel]       # the records of the words the scaffolding holds
    tables: dict[str, list[R.Entry]]     # retail format-A tables
    sections: list[Sec]
    out_dir: Path


# --------------------------------------------------------------------------------------------------------------
# the relocation tables the build generates

def generate_tables(ctx: BuildContext, shift: R.Shift) -> tuple[dict[str, list[R.Entry]], dict[str, int], dict]:
    """The format-A tables of the build, from the link's records: every entry of a compiled source from its object's
    relocation records (in the object's order), every other entry from the scaffolding's records; the sites at their
    linked (possibly shifted) addresses. The order is the tile blocks of the retail table: .rel.text is one block per
    tile in address order (the rule); a table whose blocks are not in address order (.rel.data) follows the
    scaffolding's run plan. A compiled block whose object order is not the retail block's is taken from the
    scaffolding (counted). Returns ({table: entries}, {source path: entries credited to it}, statistics)."""
    by_uid = {q.uid: q for q in ctx.prepared}
    tile_by_start = {t.start: t for t in ctx.tiles}
    out: dict[str, list[R.Entry]] = {}
    credit: dict[str, int] = {}
    stats: dict = {}
    for name, entries in ctx.tables.items():
        all_recs = sorted((r for r in ctx.recs if r.table == name), key=lambda r: r.index)
        owners = [r.owner for r in all_recs]
        runs = R.runs_of(owners)
        ordered = R.is_address_ordered(runs)
        retail_block: dict[int, list[RetailRel]] = {}
        for r in all_recs:
            retail_block.setdefault(r.owner, []).append(r)
        # compiled blocks: the object's records for this table, in object order
        compiled: dict[int, list[tuple[int, int]]] = {}
        sources_of: dict[int, str] = {}
        for owner, block in retail_block.items():
            t = tile_by_start.get(owner)
            if t is None or t.kind in ("asm", "blob"):
                continue
            q = by_uid[t.ref]
            recs = [(a, typ) for a, typ, _n in q.records.get(t.kind, []) if typ in R.KNOWN_TYPES]
            compiled[owner] = recs
            sources_of[owner] = q.path
        # HI16 entries of a compiled block that sit in another tile's block, and foreign HI16 in it, make the
        # object's order differ from the retail block: such a block is taken from the scaffolding.
        use_object = {o for o, recs in compiled.items() if recs == [(r.site, r.type) for r in retail_block[o]]}
        gen: list[R.Entry] = []
        pos: dict[int, int] = {}
        for owner, _first, count in runs:
            if owner in use_object:
                k = pos.get(owner, 0)
                part = compiled[owner][k:k + count]
                pos[owner] = k + count
                gen += [R.Entry(a + shift.delta(a), typ) for a, typ in part]
                if ordered:
                    credit[sources_of[owner]] = credit.get(sources_of[owner], 0) + len(part)
            else:                                    # a scaffolding block, or a compiled block not reproduced
                src = retail_block[owner]
                k = pos.get(owner, 0)
                part = src[k:k + count]
                pos[owner] = k + count
                gen += [R.Entry(r.site + shift.delta(r.site), r.type) for r in part]
        out[name] = gen
        stats[name] = {"entries": len(gen), "retail_entries": len(entries), "address_ordered": ordered,
                       "blocks": len(retail_block), "runs": len(runs),
                       "compiled_blocks": len(compiled), "compiled_blocks_from_object": len(use_object),
                       "compiled_blocks_from_scaffolding": [sources_of[o] for o in sorted(compiled) if o not in use_object][:20],
                       "entries_from_source": sum(len(compiled[o]) for o in use_object) if ordered else 0}
    return out, credit, stats


def emitted_relocations(linked: Elf, lo: int, hi: int) -> list[tuple[int, int]]:
    """(site, type) of every relocation `ld -q` kept in the linked module whose target lies inside [lo, hi)."""
    symtab = next((s for s in linked.secs if s.type == SHT_SYMTAB), None)
    values = [struct.unpack_from("<IIIBBH", symtab.data, o)[1] for o in range(0, len(symtab.data), 16)] if symtab else []
    out = []
    for sec in linked.secs:
        if sec.type != SHT_REL or not sec.data:
            continue
        for k in range(0, len(sec.data), 8):
            off, info = struct.unpack_from("<II", sec.data, k)
            sym, typ = info >> 8, info & 0xFF
            if typ == 0:
                continue
            v = values[sym] if sym < len(values) else 0
            if lo <= v <= hi:                        # a relocation against an address outside the module is not the PSP's
                out.append((off, typ))
    return sorted(out)


def table_rows(ctx: BuildContext, gen: dict[str, list[R.Entry]], want: dict[str, list[R.Entry]], report_key: str) -> tuple[list, bool]:
    rows, all_equal = [], True
    for name, entries in want.items():
        got = gen.get(name, [])
        g, w = R.encode_a(got), R.encode_a(entries)
        row = {"name": name, "entries": len(got), "retail_entries": len(entries), "equal": g == w}
        if g != w:
            all_equal = False
            d = R.first_difference(g, w)
            k = d // 8
            row["first_mismatch_entry"] = k
            if k < len(entries):
                row["retail_entry"] = f"0x{entries[k].offset:08x} {RELOC_NAMES.get(entries[k].type, entries[k].type)}"
            if k < len(got):
                row["built_entry"] = f"0x{got[k].offset:08x} {RELOC_NAMES.get(got[k].type, got[k].type)}"
        rows.append(row)
    return rows, all_equal


# --------------------------------------------------------------------------------------------------------------
# comparing the default build

def differing_offsets(a: bytes, b: bytes, cap: int = 200_000) -> list[int]:
    """Offsets where two byte strings differ (compared in 4 KB pieces, so equal stretches cost little), at most `cap`."""
    out: list[int] = []
    n = min(len(a), len(b))
    for base in range(0, n, 4096):
        x, y = a[base:base + 4096], b[base:base + 4096]
        if x != y:
            out += [base + i for i in range(len(x)) if x[i] != y[i]]
            if len(out) >= cap:
                return out[:cap]
    return out


def section_rows(retail: Retail, linked: Elf, sections: list[Sec], want: dict[str, tuple[int, bytes]]) -> list[dict]:
    rows = []
    for s in sections:
        if s.type != SHT_PROGBITS:
            continue
        got = linked.by_name(s.name)
        addr, data = want[s.name]
        row = {"name": s.name, "address": f"0x{addr:08x}", "size": len(data)}
        if got is None or got.data is None:
            row.update(equal=False, note="section missing from the link")
        else:
            row["linked_size"] = got.size
            if got.addr != addr:
                row["linked_address"] = f"0x{got.addr:08x}"
            equal = got.data == data and got.addr == addr
            row["equal"] = equal
            if not equal:
                diffs = differing_offsets(got.data, data)
                row["differing_bytes"] = len(diffs) + abs(len(got.data) - len(data))
                if diffs:
                    row["first_mismatch"] = f"0x{s.addr + diffs[0]:08x}"
                    row["mismatch_sites"] = [s.addr + i for i in diffs]
        rows.append(row)
    return rows


def assemble_image(ctx: BuildContext, linked: Elf, tables: dict[str, list[R.Entry]], shift: R.Shift,
                   linked_sections: dict[str, bytes] | None = None) -> tuple[bytes | None, dict, int]:
    """The whole module file from the build's parts: the linked sections, the generated relocation tables and a
    container laid out by the module's profile. Returns (file, parts, bytes copied from the retail file)."""
    retail = ctx.retail
    profile = CONTAINER_PROFILE.get(ctx.module)
    if profile is None or not ctx.tables:
        return None, {}, 0
    rel_for = {name[4:]: R.encode_a(entries) for name, entries in tables.items()}
    outs = []
    for s in retail.elf.secs:
        if not s.flags & 2:
            continue
        if s.type == SHT_NOBITS:
            outs.append(R.OutSection(s.name, s.addr + shift.delta(s.addr), None, s.flags, s.align, rel_for.get(s.name), 0))
            continue
        got = linked.by_name(s.name)
        data = (linked_sections or {}).get(s.name, got.data if got is not None and got.data is not None else b"")
        outs.append(R.OutSection(s.name, s.addr + shift.delta(s.addr), data, s.flags, 16, rel_for.get(s.name)))
    comment = retail.elf.by_name(".comment").data

    def word_at(a: int) -> int:
        o = next(o for o in outs if o.data and o.addr <= a < o.addr + len(o.data))
        return struct.unpack_from("<I", o.data, a - o.addr)[0]
    lib_ent = next(o for o in outs if o.name == ".lib.ent").data
    entry = R.module_start(lib_ent, word_at)
    end = max([hi for _lo, hi in retail.bss] + [s.addr + s.size for s in ctx.sections])
    memsz = end + shift.delta(end - 1)
    image, parts = R.write_module(R.PROFILES[profile], outs, entry if entry is not None else 0xFFFFFFFF, comment, memsz)
    return image, parts, len(comment)


def compare(ctx: BuildContext, linked: Elf, report: dict) -> None:
    retail, sections, prepared = ctx.retail, ctx.sections, ctx.prepared
    want = {s.name: (s.addr, retail.data[s.off:s.off + s.size]) for s in sections if s.type == SHT_PROGBITS}
    rows = section_rows(retail, linked, sections, want)
    for r in rows:
        r.pop("mismatch_sites", None)
    report["sections"] = rows
    shift0 = R.Shift(0, 0)
    tables, credit, stats = generate_tables(ctx, shift0)
    rel_rows, rel_equal = table_rows(ctx, tables, ctx.tables, "tables")
    emitted = emitted_relocations(linked, 0, max(hi for _lo, hi in retail.bss) if retail.bss else len(retail.data))
    generated = sorted((e.offset, e.type) for es in tables.values() for e in es)
    report["relocations"] = {"tables": rel_rows, "generation": stats,
                             "ld_emit_relocs_equal": emitted == generated if ctx.tables else None,
                             "ld_emit_relocs_entries": len(emitted)}
    if not ctx.tables:
        report["relocations"]["note"] = ("format B relocations (a module without section headers): the table is not "
                                         "regenerated, it stays in the container")
    if ctx.tables and emitted != generated:
        e, g = set(emitted), set(generated)
        report["relocations"]["ld_only"] = [f"0x{a:08x}/{t}" for a, t in sorted(e - g)[:10]]
        report["relocations"]["table_only"] = [f"0x{a:08x}/{t}" for a, t in sorted(g - e)[:10]]
    # the whole image
    image, parts, copied = assemble_image(ctx, linked, tables, shift0)
    if image is None:                                   # a module without section headers: container from retail
        image = bytearray(retail.data)
        for s in sections:
            got = linked.by_name(s.name)
            if s.type == SHT_PROGBITS and got is not None and got.data is not None and len(got.data) == s.size:
                image[s.off:s.off + s.size] = got.data
        image = bytes(image)
        copied = len(retail.data) - sum(s.size for s in sections if s.type == SHT_PROGBITS)
        container = {"note": CONTAINER_NOTE, "bytes_from_retail": copied, "generated": False}
    else:
        part_rows = {name: retail.data[o:o + n] == image[o:o + n] for name, (o, n) in parts.items()}
        container = {"note": CONTAINER_NOTE_GENERATED, "bytes_from_retail": copied, "generated": True,
                     "profile": CONTAINER_PROFILE[ctx.module], "parts_equal": part_rows,
                     "copied": {".comment": copied}}
    sha = hashlib.sha256(image).hexdigest()
    retail_sha = hashlib.sha256(retail.data).hexdigest()
    (ctx.out_dir / "image.bin").write_bytes(image)
    report.update(retail_sha256=retail_sha, image_sha256=sha, identical=(sha == retail_sha), container=container)
    # per source, in place
    t2: dict[str, int] = {}
    code_bytes = data_bytes = rel_bytes = 0
    for q in prepared:
        sec = next((s for s in retail.loaded if s.addr <= q.text_addr < s.addr + s.size), None)
        got = linked.by_name(sec.name) if sec else None
        ok = False
        if sec is not None and got is not None and got.data is not None:
            a = q.text_addr - sec.addr
            ok = got.data[a:a + q.text_size] == retail.data[sec.off + a:sec.off + a + q.text_size]
        data_ok = 0
        for key, addr, size in q.data:
            dsec = next((s for s in retail.loaded if s.addr <= addr < s.addr + s.size and s.type == SHT_PROGBITS), None)
            dgot = linked.by_name(dsec.name) if dsec else None
            if dsec is not None and dgot is not None and dgot.data is not None:
                a = addr - dsec.addr
                if dgot.data[a:a + size] == retail.data[dsec.off + a:dsec.off + a + size]:
                    data_ok += size
        if ok:
            rb = 8 * credit.get(q.path, 0) if rel_equal else 0
            t2[q.path] = q.text_size + data_ok + rb
            code_bytes += q.text_size
            data_bytes += data_ok
            rel_bytes += rb
        else:
            report["differs_in_link"].append({"path": q.path, "start": f"0x{q.start:08x}"})
    total_entries = sum(len(es) for es in tables.values())
    report["scaffolding"]["relocation_entry_bytes"] = 8 * total_entries - rel_bytes if ctx.tables else 0
    report["t2_source_bytes"] = t2
    report["t2_code_bytes"] = code_bytes
    report["t2_data_bytes"] = data_bytes
    report["t2_relocation_bytes"] = rel_bytes
    report["sources"] = {"linked": len(prepared), "in_place_exact": len(t2), "differs_in_link": len(report["differs_in_link"]),
                         "excluded": len(report["excluded"])}
    bad = [r for r in rows if not r.get("equal")]
    print(f"  link ok; {len(t2)}/{len(prepared)} sources reproduced in place ({code_bytes:,} code B, {data_bytes:,} data B, "
          f"{rel_bytes:,} relocation-table B from source)")
    ldq = report["relocations"]["ld_emit_relocs_equal"]
    print(f"  sections equal: {len(rows) - len(bad)}/{len(rows)}; relocation tables "
          f"{'not regenerated (format B)' if not ctx.tables else 'equal' if rel_equal else 'DIFFER'}; "
          f"ld -q records {'n/a' if ldq is None else 'agree' if ldq else 'DIFFER'}; whole image "
          f"{'IDENTICAL' if report['identical'] else 'differs'} (sha256 {sha[:16]} vs retail {retail_sha[:16]}); "
          f"container bytes from retail: {report['container']['bytes_from_retail']:,}")
    for r in bad[:6]:
        print(f"    differs: {r['name']} {r.get('first_mismatch', r.get('note', ''))} ({r.get('differing_bytes', '?')} bytes)")
    for r in rel_rows:
        if not r["equal"]:
            print(f"    table differs: {r['name']} entry {r['first_mismatch_entry']}: built {r.get('built_entry')} retail {r.get('retail_entry')}")
    for d in report["differs_in_link"][:6]:
        print(f"    source differs in the full link: {d['path']}")


CONTAINER_NOTE_GENERATED = ("ELF header, program header, section header table, relocation tables, .shstrtab are generated "
                            "from the build's layout (tools/hstreloc.py write_module, the module's container profile); "
                            ".comment, the original toolchain's identification string, is copied from the retail file")


# --------------------------------------------------------------------------------------------------------------
# the shifted build (gate 5, static half)

def source_scanner():
    """The shared source-level numeric-address detector, `tools/hstaddr.py` `findings(path, lang) -> [ {kind, line,
    column, ...} ]` (clang AST: integral-to-pointer casts of integer constants, module-range constants used as
    pointers). It is built on another branch; until it is merged into this tree the source-level half of the census is
    NOT RUN (reported as such) and no source counts in tier 3."""
    try:
        import hstaddr                                   # noqa: PLC0415 (optional, merged separately)
    except ImportError:
        return None
    return getattr(hstaddr, "findings", None)


def object_findings(q: Prepared, retail_sites: set[int], shift_sites: set[int]) -> list[dict]:
    """The object-level half of the census, from the build itself:
      unrelocated-address   a word the retail module relocates in the source's ranges has no relocation in the object
                            (the source names that address as a number, so the linker cannot move it)
      extra-relocation      the object relocates a word the retail module does not
      moves-wrongly         the shifted build differs from the expectation inside the source's ranges"""
    out = []
    ranges = [(q.text_addr, q.text_addr + q.text_size)] + [(a, a + n) for k, a, n in q.data if k not in ("bss", "sbss")]
    own = {a for recs in q.records.values() for a, typ, _n in recs if typ in R.KNOWN_TYPES}
    want = {s for s in retail_sites if any(a <= s < b for a, b in ranges)}
    if want - own:
        out.append({"kind": "unrelocated-address", "count": len(want - own), "first": f"0x{min(want - own):08x}"})
    extra = own - want
    if extra:
        out.append({"kind": "extra-relocation", "count": len(extra), "first": f"0x{min(extra):08x}"})
    moved = {s for s in shift_sites if any(a <= s < b for a, b in ranges)}
    if moved:
        out.append({"kind": "moves-wrongly", "count": len(moved), "first": f"0x{min(moved):08x}"})
    return out


def source_findings(scan, path: str) -> list[dict]:
    """The source-level half: the shared detector's findings for one source, grouped by kind."""
    lang = M.parse_header(ROOT / path).get("lang", "c99")
    by_kind: dict[str, list[dict]] = {}
    for f in scan(ROOT / path, lang):
        by_kind.setdefault(str(f.get("kind", "numeric-address")), []).append(f)
    return [{"kind": k, "count": len(v), "first": f"line {v[0].get('line', '?')}"} for k, v in sorted(by_kind.items())]


def shift_build(ctx: BuildContext, report: dict, shift: R.Shift, obj_dir: Path) -> None:
    """Link the same objects with every section moved (padding of n bytes ahead of the code and n more ahead of the
    data) and compare the result byte for byte with an independent expectation computed from the retail image and the
    retail relocation tables alone (tools/hstreloc.py expected_shift)."""
    retail = ctx.retail
    res: dict = {"n": shift.n, "data_start": f"0x{shift.data_start:08x}"}
    report["shift"] = res
    report["shifted_runtime"] = "not run"
    report["shifted_build_passes"] = False
    align = max([s.align for s in retail.elf.secs if s.flags & 2] + [16])
    if shift.n <= 0 or shift.n % align:
        res["error"] = f"--shift must be a positive multiple of {align} (the largest section alignment of the module)"
        print(f"  SHIFT REFUSED: {res['error']}")
        return
    if not ctx.tables:
        res["error"] = "the module's relocations are format B, which the build does not regenerate yet"
        print(f"  SHIFT NOT RUN: {res['error']}")
        return
    write_link_script(ctx.out_dir / "link-shift.ld", ctx.out_dir, ctx.sections, ctx.tiles, retail.bss,
                      [t for t in ctx.anchors.bss_claims], shift)
    elf_path = run_link(ctx.out_dir, obj_dir, "link-shift.ld", "linked-shift.elf", assemble=False)
    if elf_path is None:
        res["error"] = "shifted link failed: " + link_log(ctx.out_dir, ("linked-shift.elf.log",))
        print("  SHIFTED LINK FAILED")
        return
    linked = parse_elf(elf_path.read_bytes())
    funcs = ledger_functions(ctx.module)
    t0 = time.time()
    try:
        expect = R.expected_shift([(s.name, s.addr, retail.data[s.off:s.off + s.size]) for s in ctx.sections
                                   if s.type == SHT_PROGBITS], list(ctx.tables.values()), function_lookup(funcs), shift)
    except R.ShiftFailure as exc:
        res["error"] = f"expectation: {exc}"
        print(f"  SHIFT EXPECTATION FAILED: {exc}")
        return
    rows = section_rows(retail, linked, ctx.sections, expect)
    mismatch_sites: set[int] = set()
    for r in rows:
        for a in r.pop("mismatch_sites", []):
            mismatch_sites.add(a & ~3)
    res["sections"] = rows
    res["sections_equal"] = all(r.get("equal") for r in rows)
    res["mismatched_words"] = len(mismatch_sites)
    res["first_mismatches"] = [describe_site(ctx, a) for a in sorted(mismatch_sites)[:10]]
    tables, _credit, _stats = generate_tables(ctx, shift)
    want_tables = {name: R.shifted_table(entries, shift) for name, entries in ctx.tables.items()}
    rel_rows, rel_equal = table_rows(ctx, tables, want_tables, "shift")
    res["tables"] = rel_rows
    res["tables_equal"] = rel_equal
    top = max(hi for _lo, hi in retail.bss) + 2 * shift.n if retail.bss else len(retail.data)
    emitted = emitted_relocations(linked, 0, top)
    generated = sorted((e.offset, e.type) for es in tables.values() for e in es)
    res["ld_emit_relocs_equal"] = emitted == generated
    image, parts, _copied = assemble_image(ctx, linked, tables, shift)
    want_image, want_parts, _c = assemble_image(ctx, linked, want_tables, shift,
                                                linked_sections={n: d for n, (_a, d) in expect.items()})
    res["image_equal"] = image == want_image
    if image != want_image:
        d = R.first_difference(image, want_image)
        res["image_first_mismatch"] = next((f"{n} +0x{d - o:x}" for n, (o, sz) in want_parts.items() if o <= d < o + max(sz, 1)),
                                           f"file offset 0x{d:x}")
    retail_entry = retail.elf.ehdr[4]
    res["entry"] = {"built": f"0x{struct.unpack_from('<I', image, 0x18)[0]:08x}",
                    "expected": f"0x{retail_entry + shift.delta(retail_entry):08x}"}
    entry_ok = struct.unpack_from("<I", image, 0x18)[0] == retail_entry + shift.delta(retail_entry)
    res["seconds"] = round(time.time() - t0, 1)
    (ctx.out_dir / "image-shift.bin").write_bytes(image)
    passes = res["sections_equal"] and rel_equal and res["ld_emit_relocs_equal"] and res["image_equal"] and entry_ok
    report["shifted_build_passes"] = bool(passes)
    # numeric-address census and tier 3: a source counts in T3 when the shifted build passes, the object-level
    # census has nothing on it, and the shared source-level detector ran and found nothing either
    retail_sites = {r.site for r in ctx.recs}
    scan = source_scanner()
    census: list[dict] = []
    t3: dict[str, int] = {}
    clean_objects: dict[str, int] = {}
    for q in ctx.prepared:
        if q.path not in report.get("t2_source_bytes", {}):
            continue
        found = object_findings(q, retail_sites, mismatch_sites)
        if not found:
            clean_objects[q.path] = report["t2_source_bytes"][q.path]
        if scan is not None:
            found += source_findings(scan, q.path)
        for f in found:
            census.append({"path": q.path, **f})
        if passes and scan is not None and not found:
            t3[q.path] = report["t2_source_bytes"][q.path]
    report["numeric_address_census"] = census
    report["numeric_address_scan"] = {
        "object_level": "run (relocation records of every compiled object against the retail table, and the shifted "
                        "build's byte comparison)",
        "source_level": "run (tools/hstaddr.py)" if scan is not None else
                        "NOT RUN: tools/hstaddr.py (the shared clang-AST detector) is not in this tree yet; until it is, "
                        "no source counts in tier 3",
        "object_clean_bytes": sum(clean_objects.values()) if passes else 0}
    kinds: dict[str, list[int]] = {}
    for f in census:
        k = kinds.setdefault(f["kind"], [0, 0])
        k[0] += 1
        k[1] += f["count"]
    report["numeric_address_kinds"] = {k: {"sources": v[0], "findings": v[1]} for k, v in sorted(kinds.items())}
    report["t3_source_bytes"] = t3
    print(f"  shifted build (n=0x{shift.n:x}): sections {'equal' if res['sections_equal'] else 'DIFFER'} "
          f"({res['mismatched_words']} words), tables {'equal' if rel_equal else 'DIFFER'}, ld -q "
          f"{'agrees' if res['ld_emit_relocs_equal'] else 'DIFFERS'}, image {'equal' if res['image_equal'] else 'DIFFERS'}"
          f" -> shifted_build_passes={report['shifted_build_passes']}; census: {len(census)} findings in "
          f"{len({f['path'] for f in census})} sources; T3 {sum(t3.values()):,} B"
          + ("" if scan is not None else " (source-level scan not run: tools/hstaddr.py absent)"))
    for m in res["first_mismatches"][:5]:
        print(f"    {m}")


def describe_site(ctx: BuildContext, addr: int) -> str:
    t = ctx.anchors.tile_at(addr)
    if t is None:
        return f"0x{addr:08x}"
    if t.kind in ("asm", "blob"):
        return f"0x{addr:08x} in scaffolding tile {t.ref}"
    q = next((q for q in ctx.prepared if q.uid == t.ref), None)
    return f"0x{addr:08x} in {q.path if q else t.ref}"


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("module", help="a module of the ledger (EBOOT.BIN, hstp_trans_main.prx, libfont.prx, ...), or all")
    ap.add_argument("--parallel", type=int, default=6)
    ap.add_argument("--private", action="store_true",
                    help="build with every source, the Sony SDK reconstructions too (default: the PUBLIC build, which leaves "
                         "the origin classes of hstpublish.EXCLUDED_CLASSES out and takes their bytes from the owner's executable)")
    ap.add_argument("--src-dir", action="append", help="directory of sources (default: src); repeatable")
    ap.add_argument("--reuse-isolation", help="a hstmatch --json result (and its build/ directories) instead of running hstmatch")
    ap.add_argument("--shift", type=lambda v: int(v, 0), metavar="N",
                    help="also link a shifted build (N bytes of padding ahead of the code and N more ahead of the data, a "
                         "multiple of the module's largest section alignment) and compare it byte for byte with the "
                         "expectation computed from the retail image and its relocation tables (gate 5, static half)")
    ap.add_argument("--mutate", choices=MUTATIONS,
                    help="testing only: inject one corruption; instruction requires --mutate-scope compiled")
    ap.add_argument("--mutate-index", type=int, default=0, help="which eligible record --mutate changes")
    ap.add_argument("--mutate-scope", choices=("scaffold", "compiled"), default="scaffold",
                    help="testing only: mutate scaffolding records (default), or a compiled object's addend/instruction")
    ap.add_argument("--out-dir", help="directory for the build products instead of build/whole (an experiment that must not "
                                      "disturb a running build of the same module)")
    ap.add_argument("--report-dir", help="directory for the report instead of try/build (read by hstprogress)")
    args = ap.parse_args()
    if args.mutate_scope == "compiled" and args.mutate not in ("addend", "instruction"):
        ap.error("--mutate-scope compiled supports only addend or instruction")
    if args.mutate == "instruction" and args.mutate_scope != "compiled":
        ap.error("--mutate instruction requires --mutate-scope compiled")
    if args.mutate_scope != "scaffold" and args.mutate is None:
        ap.error("--mutate-scope requires --mutate")
    if args.module == "all":
        led = json.loads((M.CAMPAIGN / "ledger" / "campaign.json").read_text(encoding="utf-8-sig"))
        return max(build(m["name"], args) for m in led["modules"])
    return build(args.module, args)


if __name__ == "__main__":
    raise SystemExit(main())
