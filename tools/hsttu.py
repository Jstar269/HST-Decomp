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
              left out, since an inline function from a header can sit in any file). Two different names in one
              function would be a contradiction: such a function stays unassigned (none today).
  twin        EBOOT.BIN only: the function is the twin of a Sharing function that has a unit. Twins are the functions
              whose masked signatures (tools/hstclone.py) are unique in both modules, kept only when they are in the
              same order in both (the longest run in order), so a coincidence cannot pair code from two places.
  link-order  the function lies between two functions of the same unit, with no function of another unit between
              them: a linker places each object file's code in one piece.
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
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

ROOT = Path(__file__).resolve().parent.parent
TU_PATH = ROOT / "data" / "translation-units.json"
SHARING, EBOOT = "hstp_trans_main.prx", "EBOOT.BIN"
FILE_NAME = re.compile(rb"(?<=\x00)([A-Za-z0-9_][A-Za-z0-9_.+-]{0,120}\.(?:cpp|cc|c))\x00")


def file_names(data: bytes, segments: list[dict]) -> dict[int, str]:
    """{address: name} of the NUL-terminated source-file names in a module image."""
    out = {}
    for m in FILE_NAME.finditer(data):
        off = m.start(1)
        for s in segments:
            if s["file_offset"] <= off < s["file_end"]:
                out[s["start"] + off - s["file_offset"]] = m.group(1).decode("ascii")
    return out


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
    "signatures": {start: sig}}} (see load) and the SDK spans (tools/hstreport.py excluded_spans)."""
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
        out[name] = link_order([f[0] for f in mod["functions"]], ev, spans,
                               frozenset(s for s, u in found.items() if u is None))
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
        out[name] = {"functions": functions, "names": file_names(data, mod["load_segments"]), "signatures": signatures}
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
