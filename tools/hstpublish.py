#!/usr/bin/env python3
"""Build the would-be public tree: the tracked files minus everything that must not leave the private repo.

    python tools/hstpublish.py --out <empty or new directory> [--list]

Copies every file git tracks (`git ls-files`) into --out, byte for byte, except:

  * sources whose origin class is in EXCLUDED_CLASSES (see tools/hstorigin.py; the classes are computed fresh
    from the sources, not read from ORIGINS.json);
  * files that git tracks although .gitignore says they are ignored (private material that was added by force).

ORIGINS.json is copied filtered: it lists only the files that were copied, so it does not name excluded sources.

What was excluded is printed (counts and bytes per reason; --list prints every excluded path).

This tool never touches a git remote and never uses the network: the only git command it runs is the local,
read-only `git ls-files`. It writes only inside --out.

Not filtered (review these before a real release): the symbol maps in symbols/ and the function boundary lists in
ghidra/ name functions of every module, and docs/ may discuss excluded code.
"""
from __future__ import annotations

import argparse
import json
import os
import shutil
import subprocess
import sys
from collections import defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hstorigin

ROOT = hstorigin.ROOT

# ---------------------------------------------------------------------------------------------------------------
# PUBLISH POLICY - the single setting that decides which origin classes stay private.
#
# Sony SDK excluded at publish time for now: `sony-prx` is code of Sony system-library modules (libfont,
# scePsmfP, scePsmf) and `gcc-unattributed` is psp-gcc-built SDK code of EBOOT.BIN and the Sharing module that is
# not newlib and has not been attributed to anything else yet. A public build gets these functions from the
# user's own copy of the game. To release a class, remove it from this tuple (and re-run hstorigin.py).
# Classes: game, newlib, mw-runtime, sony-prx, gcc-unattributed.
EXCLUDED_CLASSES = ("sony-prx", "gcc-unattributed")
# Maintainer-only files that stay in the maintainers' tree: working research notes, and the scripts that run
# external agent lanes (they depend on the maintainers' own tool setup). A trailing "/" excludes a whole directory.
PRIVATE_PATHS = ("docs/research/", "docs/LANE-TASK.md", "docs/AGENT-BRIEF.md", "tools/hstauto.sh", "tools/hstsupervise.sh",
                 "tools/hstlane.py", "tools/hstrestart.sh", "tools/hstpushprep.sh",
                 "tools/hstexec.py", "tools/test_hstexec.py", "tools/test_hstlane.py",
                 # the accepted ledger names every proven source, the Sony SDK ones included (tools/hstproof.py)
                 "data/accepted-ledger.jsonl", "data/accepted-journal.jsonl")
# ---------------------------------------------------------------------------------------------------------------

MANIFEST = "ORIGINS.json"


def git_lines(*args: str) -> list[str]:
    """Run a local read-only `git ls-files` variant in the repository root; the NUL-separated paths."""
    assert args and args[0] == "ls-files", "hstpublish runs git ls-files only"
    out = subprocess.run(["git", *args, "-z"], cwd=ROOT, check=True, capture_output=True,
                         env={**os.environ, "GIT_TERMINAL_PROMPT": "0"}).stdout
    return [p.decode("utf-8") for p in out.split(b"\0") if p]


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--out", type=Path, required=True, help="destination directory (new or empty)")
    ap.add_argument("--list", action="store_true", help="print every excluded path")
    args = ap.parse_args()
    out = args.out.resolve()
    if out == ROOT or ROOT in out.parents and ".git" in out.relative_to(ROOT).parts or out in ROOT.parents:
        raise SystemExit(f"refusing --out {out}: it must be a directory of its own, outside .git")
    if out.exists() and (not out.is_dir() or any(out.iterdir())):
        raise SystemExit(f"refusing --out {out}: it exists and is not empty")
    unknown = [c for c in EXCLUDED_CLASSES if c not in hstorigin.CLASSES]
    if unknown:
        raise SystemExit(f"EXCLUDED_CLASSES names unknown class(es): {', '.join(unknown)}")

    tracked = git_lines("ls-files")
    tracked_ignored = set(git_lines("ls-files", "-c", "-i", "--exclude-standard"))
    classes, warnings = hstorigin.classify()
    for w in warnings:
        print(f"warning: {w}", file=sys.stderr)
    if (ROOT / MANIFEST).exists() and hstorigin.load_manifest(ROOT / MANIFEST) != classes:
        print(f"warning: {MANIFEST} is stale (run tools/hstorigin.py); the fresh classification is used",
              file=sys.stderr)

    keep: list[str] = []
    excluded: dict[str, list[str]] = defaultdict(list)
    for path in tracked:
        if path in tracked_ignored:
            excluded["gitignored (tracked by force)"].append(path)
        elif any(path == q or q.endswith("/") and path.startswith(q) for q in PRIVATE_PATHS):
            excluded["maintainer-only path (PRIVATE_PATHS)"].append(path)
        elif classes.get(path) in EXCLUDED_CLASSES:
            excluded[f"origin class {classes[path]}"].append(path)
        elif not (ROOT / path).is_file():
            excluded["missing in the work tree"].append(path)
        else:
            keep.append(path)

    out.mkdir(parents=True, exist_ok=True)
    copied_bytes = 0
    for path in keep:
        dest = out / path
        dest.parent.mkdir(parents=True, exist_ok=True)
        if path == MANIFEST:
            kept_paths = set(keep)
            kept = {p: c for p, c in classes.items() if p in kept_paths}
            dest.write_text(json.dumps(kept, indent=0, sort_keys=True) + "\n", encoding="utf-8", newline="\n")
        else:
            shutil.copyfile(ROOT / path, dest)
        copied_bytes += dest.stat().st_size

    print(f"public tree: {out}")
    print(f"tracked files: {len(tracked)}; copied: {len(keep)} ({copied_bytes} bytes); "
          f"excluded: {sum(len(v) for v in excluded.values())}")
    for reason, paths in sorted(excluded.items()):
        size = sum((ROOT / p).stat().st_size for p in paths if (ROOT / p).is_file())
        print(f"  excluded {len(paths):5d} files, {size:9d} bytes: {reason}")
        if args.list:
            for p in paths:
                print(f"    {p}")
    kept_classes = defaultdict(int)
    for path in keep:
        if path in classes:
            kept_classes[classes[path]] += 1
    print("  sources kept by class: " + ", ".join(f"{c} {kept_classes[c]}" for c in hstorigin.CLASSES))
    print(f"  excluded origin classes (EXCLUDED_CLASSES): {', '.join(EXCLUDED_CLASSES)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
