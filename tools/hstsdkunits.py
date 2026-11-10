#!/usr/bin/env python3
"""The SDK units where the gcc404x-* profiles may be used: data/sdk-gcc4-units.json.

    python tools/hstsdkunits.py [--write]

Owner decision 2026-11-09: psp-gcc-4.0.4x (GCC 4.0.4 with the pspdev Allegrex patch and lbp-decomp's four measured
Allegrex changes, among them ins-from-zero; docs/GUIDE.md "SDK compiler profiles") is accepted for Sony SDK code only.
Game code keeps mwccpsp, so tools/hstgate.py refuses a cc=gcc404x-* source whose function is not inside one of the
units listed here (`gcc404x-outside-sdk`).

A unit is a maximal run of functions, in address order, none of which is game code, that contains at least one
function using Allegrex `ins rt,$zero,pos,size` (the later Sony compiler's signature, which no other profile emits).
A function is game code when an installed source compiles it with mwccpsp (no cc= or cc=mwcc*), or, when it has no
source, when tools/hstfamily.py predicts mwccpsp for it (try/family-predictions.json, probability at least 0.5) and it
has no `ins $zero`. The file is part of the toolchain lock (tools/hstproof.py COMPARISON_CODE): a change to it is a
gate-scope toolchain migration. Needs your decrypted modules. Without --write it prints the units and whether the
committed file still matches.
"""
from __future__ import annotations

import argparse
import json
import re
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hstmatch as M  # noqa: E402

ROOT = M.ROOT
OUT = ROOT / "data" / "sdk-gcc4-units.json"
SCHEMA = "hst-decomp.sdk-gcc4-units.v1"
MODULES = {"EBOOT.BIN": "eboot", "hstp_trans_main.prx": "sharing_main"}
HDR = re.compile(r"/\*\s*HSTMATCH\s+(.*?)\*/", re.S)


def ins_from_zero(word: int) -> bool:
    """Allegrex `ins rt,$zero,pos,size`: SPECIAL3 (0x1f), function 0x04, rs = $zero."""
    return word >> 26 == 0x1F and word & 0x3F == 0x04 and (word >> 21) & 0x1F == 0


def functions(module: str) -> list[tuple[int, int]]:
    rows = []
    for line in (ROOT / "data" / f"functions-{module}.txt").read_text(encoding="utf-8").splitlines():
        if line.strip() and not line.startswith("#"):
            a, b = line.split()[:2]
            rows.append((int(a, 16), int(b, 16)))
    return rows


def source_cc(module: str, funcs: list[tuple[int, int]]) -> dict[int, str]:
    starts = [s for s, _ in funcs]
    import bisect
    cover: dict[int, str] = {}
    for p in (ROOT / "src" / MODULES[module]).rglob("*"):
        if p.suffix not in (".c", ".cpp"):
            continue
        m = HDR.search(p.read_text(encoding="utf-8", errors="replace")[:2000])
        if not m:
            continue
        h = {k: v.strip('"') for k, v in re.findall(r'(\w+)=("[^"]*"|\S+)', m.group(1))}
        s, e = int(h["start"], 16), int(h["end"], 16)
        for i in range(bisect.bisect_left(starts, s), len(starts)):
            if starts[i] >= e:
                break
            cover[starts[i]] = h.get("cc", "mwcc219")
    return cover


def units(module: str, pred: dict) -> list[dict]:
    funcs = functions(module)
    cover = source_cc(module, funcs)
    data, seg = M.module_bytes({}, module)
    out: list[dict] = []
    cur = None
    for fs, fe in funcs:
        off = seg["file_offset"] + fs - seg["start"]
        raw = data[off:off + fe - fs]
        words = struct.unpack(f"<{len(raw) // 4}I", raw[:len(raw) // 4 * 4])
        ins0 = any(ins_from_zero(w) for w in words)
        cc = cover.get(fs)
        p = pred.get(f"{module}:0x{fs:08x}")
        game = (not cc.startswith("gcc")) if cc is not None else (not ins0 and p is not None and p >= 0.5)
        if game:
            cur = None
            continue
        if cur is None:
            cur = {"start": fs, "end": fe, "functions": 0, "ins_functions": []}
            out.append(cur)
        cur["end"] = fe
        cur["functions"] += 1
        if ins0:
            cur["ins_functions"].append(f"0x{fs:08x}")
    return [dict(u, start=f"0x{u['start']:08x}", end=f"0x{u['end']:08x}") for u in out if u["ins_functions"]]


def build() -> dict:
    pred = json.loads((ROOT / "try" / "family-predictions.json").read_text(encoding="utf-8"))["predictions"]
    return {"schema": SCHEMA,
            "meaning": "address ranges [start, end) of Sony SDK code where cc=gcc404x-* is allowed (tools/hstgate.py)",
            "method": "tools/hstsdkunits.py (see its docstring)",
            "modules": {m: units(m, pred) for m in MODULES}}


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--write", action="store_true")
    args = ap.parse_args()
    doc = build()
    for m, us in doc["modules"].items():
        print(f"{m}: {len(us)} units, {sum(u['functions'] for u in us)} functions, "
              f"{sum(len(u['ins_functions']) for u in us)} with ins $zero")
    text = json.dumps(doc, indent=1) + "\n"
    if args.write:
        OUT.write_text(text, encoding="utf-8", newline="\n")
        print(f"wrote {OUT.relative_to(ROOT).as_posix()}")
    elif OUT.exists():
        print("committed file " + ("matches" if OUT.read_text(encoding="utf-8") == text else "DIFFERS"))
    return 0


if __name__ == "__main__":
    sys.exit(main())
