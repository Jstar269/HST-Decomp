#!/usr/bin/env python3
"""Recover the game's translation units (its original source files) from evidence in the program, and write them to
data/translation-units.json for the progress report (tools/hstreport.py).

    python tools/hsttu.py            write data/translation-units.json (needs your decrypted modules, like hstmatch.py)
    python tools/hsttu.py --check    exit 1 when the committed file differs from what the modules give now

WHY. A decomp's progress map groups functions by the object file they came from. This game's Sharing module
(hstp_trans_main.prx) is a debug build: its assertions name the source file they are in (`chr_manager.cpp`), and
EBOOT.BIN, built from the same code, keeps a few of those assertions too. That is enough to put a part of the
functions back into their files, with no guessing.

EVIDENCE, in this order, and nothing else:
  string      the function loads the address of a file-name string (a `*.c`, `*.cpp` or `*.cc` name; headers are
              left out, since an inline function from a header can sit in any file). The name need not follow a NUL:
              the compiler can place it right after other data, so every address inside a NUL-terminated name where a
              name could start counts, and only an address the code loads becomes evidence. Two different names in
              one function would be a contradiction: such a function stays unassigned (none today).
  twin        EBOOT.BIN only: the function is the twin of a Sharing function that has a unit. Twins are the functions
              whose masked signatures (tools/hstclone.py) are unique in both modules, kept only when they are in the
              same order in both (the longest run in order), so a coincidence cannot pair code from two places.
  link-order  the function lies between two functions of the same unit, with no function of another unit between
              them: a linker places each object file's code in one piece.
  class       the function belongs to a C++ class whose other known functions are all in one unit: MWCC emits a
              class's vtable with its out-of-line methods. A class is a vtable (a run of relocated words in .data that
              point at function starts, after MWCC's header of two zero words) with its functions: the virtuals no
              other vtable holds, and the constructors and destructors, which load the vtable's address (a HI16/LO16
              relocation pair to its header). A class whose known functions are in two units decides nothing. Guard:
              a function joins a unit only when no function of another unit lies between it and that unit's nearest
              function, so a class never makes a unit cross another (an inline virtual from a header can be emitted in
              any file that uses it). Class and link-order evidence repeat until nothing changes.
A function before a unit's first or after its last evidence stays unassigned, and so does every SDK library function
(the origin classes the public tree leaves out), even inside a unit. A unit split by another unit's function would
mean the evidence disagrees: the tool refuses to write that (none today).

The file lists unit names (file names as the program spells them, without directory) and function start addresses:
both are already in the public tree's lists. It holds no game bytes.
"""
from __future__ import annotations

import argparse
import bisect
import itertools
import json
import re
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

ROOT = Path(__file__).resolve().parent.parent
TU_PATH = ROOT / "data" / "translation-units.json"
SHARING, EBOOT = "hstp_trans_main.prx", "EBOOT.BIN"
FILE_NAME = re.compile(rb"([A-Za-z0-9_][A-Za-z0-9_.+-]{0,120}\.(?:cpp|cc|c))\x00")
R_MIPS_32, R_MIPS_HI16, R_MIPS_LO16 = 2, 5, 6


def file_names(data: bytes, segments: list[dict]) -> dict[int, str]:
    """{address: name} of every place inside a NUL-terminated source-file name where a name can start (a letter or
    `_`): a name can follow other data with no NUL between, and only an address the code loads is ever evidence."""
    out = {}
    for m in FILE_NAME.finditer(data):
        for off in range(m.start(1), m.end(1) - 2):
            if not (chr(data[off]).isalpha() or data[off] == 0x5F):
                continue
            for s in segments:
                if s["file_offset"] <= off < s["file_end"]:
                    out[s["start"] + off - s["file_offset"]] = data[off:m.end(1)].decode("ascii")
    return out


def elf_sections(data: bytes) -> dict[str, tuple[int, int, int]]:
    """{name: (address, file offset, size)} from an ELF image's section headers."""
    shoff = struct.unpack_from("<I", data, 0x20)[0]
    shentsize, shnum, shstrndx = struct.unpack_from("<HHH", data, 0x2E)
    hdr = [struct.unpack_from("<IIIIII", data, shoff + i * shentsize) for i in range(shnum)]
    names_at = hdr[shstrndx][4]
    out = {}
    for name_off, _type, _flags, addr, off, size in hdr:
        end = data.index(b"\0", names_at + name_off)
        out[data[names_at + name_off:end].decode("ascii", "replace")] = (addr, off, size)
    return out


def relocations(data: bytes, secs: dict, name: str) -> list[tuple[int, int]]:
    """(address, type) of each relocation in section `name` (none when the image has no such section)."""
    if name not in secs:
        return []
    _a, off, size = secs[name]
    return [(o, info & 0xFF) for o, info in (struct.unpack_from("<II", data, off + k) for k in range(0, size, 8))]


def classes(data: bytes, seg: dict, functions: list[tuple[int, int, list]]) -> dict[int, set[int]]:
    """{vtable address: the class's functions} (see EVIDENCE, class) from a module image with relocations."""
    secs = elf_sections(data)
    if ".data" not in secs:
        return {}
    base = seg["file_offset"] - seg["start"]

    def word(a: int) -> int:
        return struct.unpack_from("<I", data, base + a)[0]

    starts = [f[0] for f in functions]
    ends = {f[0]: f[1] for f in functions}
    lo, hi = secs[".data"][0], secs[".data"][0] + secs[".data"][2]
    r32 = {o for o, t in relocations(data, secs, ".rel.data") if t == R_MIPS_32}
    known = set(starts)
    code_ptr = {o for o in r32 if lo <= o < hi and word(o) in known}
    vtables: dict[int, list[int]] = {}
    for o in sorted(code_ptr):
        if o - 4 in code_ptr or o - 8 in r32 or word(o - 8) != 0 or word(o - 4) != 0:
            continue
        run = [o]
        while run[-1] + 4 in code_ptr:
            run.append(run[-1] + 4)
        vtables[o] = [word(x) for x in run]
    holders: dict[int, set[int]] = {}
    for v, fs in vtables.items():
        for f in fs:
            holders.setdefault(f, set()).add(v)
    members: dict[int, set[int]] = {}
    for f, vs in holders.items():
        if len(vs) == 1:
            members.setdefault(next(iter(vs)), set()).add(f)
    header = {v - 8: v for v in vtables}
    his: dict[int, int] = {}
    for o, t in relocations(data, secs, ".rel.text"):
        x = word(o)
        if t == R_MIPS_HI16:
            his[(x >> 16) & 31] = (x & 0xFFFF) << 16
        elif t == R_MIPS_LO16 and (x >> 21) & 31 in his:
            imm = x & 0xFFFF
            target = (his[(x >> 21) & 31] + (imm - 0x10000 if imm & 0x8000 else imm)) & 0xFFFFFFFF
            i = bisect.bisect_right(starts, o) - 1
            if target in header and i >= 0 and o < ends[starts[i]]:
                members.setdefault(header[target], set()).add(starts[i])
    return members


def no_crossing(starts: list[int], units: dict[int, tuple[str, str]], f: int, unit: str) -> bool:
    """True when no function of another unit lies between f and `unit`'s nearest function."""
    mine = sorted(s for s, (u, _k) in units.items() if u == unit)
    if not mine:
        return False
    i = bisect.bisect_left(mine, f)
    g = min((mine[k] for k in (i - 1, i) if 0 <= k < len(mine)), key=lambda m: abs(m - f))
    span = starts[bisect.bisect_left(starts, min(g, f)):bisect.bisect_right(starts, max(g, f))]
    return all(units.get(s, (unit,))[0] == unit for s in span)


def anchors(functions: list[tuple[int, int, list]], names: dict[int, str]) -> dict[int, str | None]:
    """{start: unit} of the functions that load a file-name address (None: two different names)."""
    out: dict[int, str | None] = {}
    for start, _end, refs in functions:
        units = {names[a] for kind, _i, a in refs if kind == "data" and a in names}
        if units:
            out[start] = units.pop() if len(units) == 1 else None
    return out


def ordered_pairs(a: dict[int, tuple], b: dict[int, tuple]) -> list[tuple[int, int]]:
    """(start in a, start in b) of the functions whose signature is unique in both, longest run in the same order."""
    count_a, count_b = {}, {}
    for sig in a.values():
        count_a[sig] = count_a.get(sig, 0) + 1
    for sig in b.values():
        count_b[sig] = count_b.get(sig, 0) + 1
    in_b = {sig: s for s, sig in b.items() if count_b[sig] == 1}
    pairs = sorted((s, in_b[sig]) for s, sig in a.items() if count_a[sig] == 1 and sig in in_b)
    tails: list[int] = []
    tail_idx: list[int] = []
    prev: list[int | None] = [None] * len(pairs)
    for k, (_s, t) in enumerate(pairs):              # longest increasing subsequence of the b addresses
        i = bisect.bisect_left(tails, t)
        if i == len(tails):
            tails.append(t)
            tail_idx.append(k)
        else:
            tails[i] = t
            tail_idx[i] = k
        prev[k] = tail_idx[i - 1] if i else None
    out = []
    k = tail_idx[-1] if tail_idx else None
    while k is not None:
        out.append(pairs[k])
        k = prev[k]
    return out[::-1]


def link_order(starts: list[int], evidence: dict[int, tuple[str, str]], sdk: list[tuple[int, int]],
               contradicted: frozenset[int] = frozenset()) -> dict[int, tuple[str, str]]:
    """Add the functions between two consecutive evidenced functions of the same unit. An SDK library function in the
    gap stays out (it is counted in the SDK unit), the rest of the gap is still that unit's. A gap holding a function
    whose own evidence contradicts itself (`contradicted`) is not filled."""
    out = dict(evidence)
    keyed = sorted((s, u) for s, (u, _k) in evidence.items())
    for (a, ua), (b, ub) in itertools.pairwise(keyed):
        gap = starts[bisect.bisect_right(starts, a):bisect.bisect_left(starts, b)]
        if ua != ub or contradicted.intersection(gap):
            continue
        for g in gap:
            if not in_sdk(g, sdk):
                out.setdefault(g, (ua, "link-order"))
    return out


def in_sdk(start: int, sdk: list[tuple[int, int]]) -> bool:
    return any(x <= start < y for x, y in sdk)


def recover(modules: dict[str, dict], sdk: dict[str, list[tuple[int, int]]]) -> dict[str, dict[int, tuple[str, str]]]:
    """{module: {start: (unit, evidence)}} from `modules` {name: {"functions": [(start, end, refs)], "names": {...},
    "signatures": {start: sig}, "classes": {vtable: {start}}}} (see load; "classes" may be left out) and the SDK spans
    (tools/hstreport.py excluded_spans)."""
    out = {}
    for name in (SHARING, EBOOT):
        mod = modules[name]
        spans = sdk.get(name, [])
        found = anchors(mod["functions"], mod["names"])
        ev = {s: (u, "string") for s, u in found.items() if u is not None and not in_sdk(s, spans)}
        if name == EBOOT and SHARING in out:
            for s, e in ordered_pairs(modules[SHARING]["signatures"], mod["signatures"]):
                if s in out[SHARING] and e not in ev and not in_sdk(e, spans):
                    ev[e] = (out[SHARING][s][0], "twin")
        starts = [f[0] for f in mod["functions"]]
        contradicted = frozenset(s for s, u in found.items() if u is None)
        units = link_order(starts, ev, spans, contradicted)
        while mod.get("classes"):
            added = 0
            for _v, fs in sorted(mod["classes"].items()):
                known = {units[f][0] for f in fs if f in units}
                if len(known) != 1:
                    continue                    # no unit yet, or functions in two units: the class decides nothing
                unit = known.pop()
                for f in sorted(fs):
                    if f not in units and f not in contradicted and not in_sdk(f, spans) \
                            and no_crossing(starts, units, f, unit):
                        ev[f] = units[f] = (unit, "class")
                        added += 1
            if not added:
                break
            units = link_order(starts, ev, spans, contradicted)
        out[name] = units
    return out


def pieces(starts: list[int], units: dict[int, tuple[str, str]]) -> dict[str, int]:
    """{unit: number of pieces in address order, counting only a split by another unit} (1 each when the evidence
    agrees)."""
    out: dict[str, int] = {}
    prev = None
    for s in starts:
        u = units.get(s, (None,))[0]
        if u is None:
            continue
        if u != prev:
            out[u] = out.get(u, 0) + 1
        prev = u
    return out


def load(led: dict) -> dict[str, dict]:
    import hstclone as C
    import hstmatch as M
    out = {}
    for name in (SHARING, EBOOT):
        mod = next(m for m in led["modules"] if m["name"] == name)
        data, seg = M.module_bytes({}, name)
        functions, signatures = [], {}
        for f in sorted(mod["functions"], key=lambda f: f["start"]):
            off = seg["file_offset"] + f["start"] - seg["start"]
            sig, refs = C.analyse(C.words(data[off:off + f["end"] - f["start"]]))
            functions.append((f["start"], f["end"], refs))
            if f.get("boundary_status") == "reviewed":
                signatures[f["start"]] = sig
        out[name] = {"functions": functions, "names": file_names(data, mod["load_segments"]), "signatures": signatures,
                     "classes": classes(data, seg, functions)}
    return out


def document(units: dict[str, dict[int, tuple[str, str]]]) -> dict:
    mods = {}
    for name in sorted(units):
        by_unit: dict[str, list] = {}
        for s, (u, kind) in sorted(units[name].items()):
            by_unit.setdefault(u, []).append([f"0x{s:08x}", kind])
        mods[name] = [{"name": u, "functions": fs} for u, fs in sorted(by_unit.items(), key=lambda kv: kv[1][0][0])]
    return {"about": "Translation units recovered by tools/hsttu.py: unit (file name in the program) -> function starts "
                     "with the evidence for each (string, twin, link-order). See that tool's docstring.",
            "modules": mods}


def render(doc: dict) -> str:
    lines = ["{", f' "about": {json.dumps(doc["about"])},', ' "modules": {']
    mods = list(doc["modules"].items())
    for i, (name, us) in enumerate(mods):
        lines.append(f"  {json.dumps(name)}: [")
        lines.extend(f"   {json.dumps(u, separators=(',', ':'))}" + ("," if j < len(us) - 1 else "")
                     for j, u in enumerate(us))
        lines.append("  ]" + ("," if i < len(mods) - 1 else ""))
    lines += [" }", "}"]
    return "\n".join(lines) + "\n"


def read(path: Path = TU_PATH) -> dict[str, dict[int, str]]:
    """{module: {start: unit}} from the committed file ({} when there is none)."""
    if not path.exists():
        return {}
    doc = json.loads(path.read_text(encoding="utf-8"))
    return {m: {int(s, 16): u["name"] for u in us for s, _k in u["functions"]} for m, us in doc["modules"].items()}


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--check", action="store_true")
    args = ap.parse_args()
    import hstorigin
    import hstprogress as P
    import hstreport
    led = P.ledger()
    classes, _warnings = hstorigin.classify(P.ROOT)
    mods = load(led)
    units = recover(mods, hstreport.excluded_spans(classes, P.ROOT))
    split = {m: [u for u, n in pieces([f[0] for f in mods[m]["functions"]], units[m]).items() if n > 1] for m in units}
    text = render(document(units))
    for m in (SHARING, EBOOT):
        kinds: dict[str, int] = {}
        for _u, k in units[m].values():
            kinds[k] = kinds.get(k, 0) + 1
        print(f"{m}: {len({u for u, _k in units[m].values()})} units, {len(units[m])} functions "
              f"({', '.join(f'{v} {k}' for k, v in sorted(kinds.items()))})"
              + (f"; IN PIECES: {', '.join(split[m])}" if split[m] else ""))
    if any(split.values()):
        print("refused: a unit in more than one piece means the evidence disagrees")
        return 1
    if args.check:
        same = TU_PATH.exists() and TU_PATH.read_text(encoding="utf-8") == text
        print(f"{TU_PATH.relative_to(ROOT).as_posix()}: " + ("agrees" if same else "DIFFERS (run tools/hsttu.py)"))
        return 0 if same else 1
    TU_PATH.write_text(text, encoding="utf-8", newline="\n")
    print(f"wrote {TU_PATH.relative_to(ROOT).as_posix()}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
