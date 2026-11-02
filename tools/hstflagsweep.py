#!/usr/bin/env python3
"""Compiler-setting sweep for near-miss sources: does any mwccpsp build / option set turn a residual EXACT?

    python tools/hstflagsweep.py --tag f1 [--parallel 8] [--builds 219,210,...] wip/file.c [wip/file2.c ...]

A one- or two-word schedule swap that no spelling of the source changes often means the original translation unit
was built with other settings. Every source is compiled under each (build, option set) combination with
hstmatch.py's own compile/link/compare (`--flags`, so results are EXACT_EXTRA_FLAGS, never a profile match), and the
words that still differ are tabulated against the profile baseline (build 219, `-c -O4,p -sdatathreshold 0`).
A combination that reaches 0 is a finding about that translation unit's flags: the neighbours must be checked
before anything is claimed, and no per-function flag is installed by this tool.

The builds come from a native WSL copy of the decomp.me kits (one directory per build id next to a wibo); the default
is $HOME/hst-mwcc-sweep (created by copying C:/.../decompme-mwccpsp/builds and the wibo binary there).
Output: try/flagsweep-<tag>/results.json (every combination x file) and summary.txt.
"""
from __future__ import annotations

import argparse
import json
import os
import sys
from itertools import product
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hstmatch as M  # noqa: E402

BUILDS = ("219", "210", "205", "201", "192", "180", "151", "147", "139", "134", "121")
BASE = "-c {opt} -sdatathreshold 0 {more}"
OPTS = ("-O4,p", "-O4,s", "-O3,p", "-O3,s", "-O2,p")
INLINES = ("", "-inline off", "-inline auto", "-inline deferred", "-inline all")
EXTRAS = ("-opt nointrinsics", "-char unsigned", "-enum min", "-strict on", "-fp single", "-str nopool,noreuse",
          "-inline level=2", "-inline bottomup")


def combos(builds: list[str]) -> list[tuple[str, str, str]]:
    """(build, label, flags) in a stable order; the first one is the profile baseline."""
    out = [(b, f"{o} {i}".strip(), BASE.format(opt=o, more=i).strip()) for b, o, i in product(builds, OPTS, INLINES)]
    out += [(b, f"-O4,p {x}", BASE.format(opt="-O4,p", more=x).strip()) for b in builds for x in EXTRAS]
    out.sort(key=lambda c: (c[0], c[1]) != ("219", "-O4,p"))        # baseline first
    return out


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("files", nargs="+", type=Path)
    ap.add_argument("--tag", required=True)
    ap.add_argument("--parallel", type=int, default=8)
    ap.add_argument("--builds", default=",".join(BUILDS))
    ap.add_argument("--native", default="$HOME/hst-mwcc-sweep", help="WSL directory with wibo + builds/<id>/")
    args = ap.parse_args()
    work = M.ROOT / "try" / f"flagsweep-{args.tag}"
    if work.exists():
        raise SystemExit(f"{work} exists; use a new tag")
    work.mkdir(parents=True)
    os.environ["HST_MWCC_NATIVE"] = args.native
    files = [str(p.resolve()) for p in args.files]
    rows = []
    for n, (build, label, flags) in enumerate(combos(args.builds.split(","))):
        M.PROFILE["build"] = build
        out = work / f"c{n:03d}.json"
        os.environ["HSTMATCH_BUILD_ROOT"] = str(work / "build" / f"c{n:03d}")
        sys.argv = ["hstmatch.py", "--quiet", "--parallel", str(args.parallel), "--flags", flags, "--json", str(out)] + files
        try:
            M.main()
        except SystemExit as e:
            if e.code not in (0, 1, None):
                raise
        for r in json.loads(out.read_text(encoding="utf-8")):
            rows.append({"build": build, "options": label, "flags": flags, "source": r["source"], "status": r["status"],
                         "words_differ": r["words_differ"], "detail": r["detail"][:120]})
        print(f"{n} build={build} {label}: " + " ".join(str(r["words_differ"]) for r in rows[-len(files):]), flush=True)
    (work / "results.json").write_text(json.dumps(rows, indent=1) + "\n", encoding="utf-8")
    lines = []
    for f in sorted({r["source"] for r in rows}):
        mine = [r for r in rows if r["source"] == f and r["words_differ"] is not None]
        base = next((r for r in mine if r["build"] == "219" and r["options"] == "-O4,p"), None)
        b0 = base["words_differ"] if base else None
        best = sorted(mine, key=lambda r: r["words_differ"])
        lines.append(f"{f}: baseline {b0}; best {best[0]['words_differ'] if best else None}")
        for r in best[:5]:
            if b0 is None or r["words_differ"] < b0:
                lines.append(f"    {r['words_differ']:4d}  build {r['build']}  {r['flags']}  [{r['status']}]")
    (work / "summary.txt").write_text("\n".join(lines) + "\n", encoding="utf-8")
    print("\n".join(lines))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
