#!/usr/bin/env python3
"""List and count every source that carries a steering pragma (tools/hstgate.py STEER_PRAGMAS).

    python tools/hststeer.py [--wip] [--json]

The steering class (owner decision 2026-11-09): an optimiser-pass switch set for the one function it makes
byte-identical, scoped by #pragma push/pop or reset and declared in the source's note, `steer=pragma:<name>`.
Each line is one source: its class per pragma (steer-pragma = the declared, scoped idiom; steer-pragma-undeclared
or steer-pragma-unscoped = a pragma accepted before the rule, not named), then the totals by pragma and class.
"""
from __future__ import annotations

import argparse
import json
import sys
from collections import Counter
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hstgate  # noqa: E402

ROOT = Path(__file__).resolve().parents[1]
KINDS = ("steer-pragma", "steer-pragma-undeclared", "steer-pragma-unscoped")


def scan(trees: list[str]) -> list[dict]:
    rows = []
    for t in trees:
        for p in sorted((ROOT / t).rglob("*")):
            if p.suffix not in (".c", ".cpp"):
                continue
            hits = [f for f in hstgate.check_file(p) if f.kind in KINDS]
            if not hits:
                continue
            pragmas = sorted({(hstgate.PRAGMA.match(f.text).group(1) if hstgate.PRAGMA.match(f.text) else "?", f.kind)
                              for f in hits})
            rows.append({"source": p.relative_to(ROOT).as_posix(), "pragmas": [{"name": n, "class": k} for n, k in pragmas]})
    return rows


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--wip", action="store_true", help="wip/ as well as src/")
    ap.add_argument("--json", action="store_true")
    args = ap.parse_args()
    rows = scan(["src"] + (["wip"] if args.wip else []))
    totals = Counter((x["name"], x["class"]) for r in rows for x in r["pragmas"])
    if args.json:
        print(json.dumps({"sources": rows, "totals": [{"name": n, "class": k, "sources": c}
                                                      for (n, k), c in sorted(totals.items())]}, indent=1))
        return 0
    for r in rows:
        print(f"{r['source']}: " + ", ".join(f"{x['name']} ({x['class']})" for x in r["pragmas"]))
    by_class = Counter(x["class"] for r in rows for x in r["pragmas"])
    print(f"{len(rows)} sources with a steering pragma; by class: {dict(sorted(by_class.items())) or 'none'}")
    for (n, k), c in sorted(totals.items()):
        print(f"  {n:24} {k:26} {c}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
