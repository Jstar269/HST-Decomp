#!/usr/bin/env python3
"""Build the would-be public tree: the tracked files minus everything that must not leave the private repo.

    python tools/hstpublish.py --out <empty or new directory> [--list]

Copies every file git tracks (`git ls-files`) into --out, byte for byte, except:

  * sources whose origin class is in EXCLUDED_CLASSES (see tools/hstorigin.py; the classes are computed fresh
    from the sources, not read from ORIGINS.json);
  * files that git tracks although .gitignore says they are ignored (private material that was added by force).

It refuses to write anything (exit status 1) when a file it would copy holds private content (PRIVATE_CONTENT: a
maintainer's local path, a personal home directory, a private key or an access token): the export copies bytes,
so such a line would otherwise be published unchanged. Replace the path with `<private tree>` on the private branch
and run it again.

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
import re
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
# scePsmfP, scePsmf), `sony-middleware-sgx` is the SGX sound library, Sony middleware treated like the SDK (owner
# decision 2026-11-01: never published as source, counted as the SDK is), and `gcc-unattributed` is psp-gcc-built
# SDK code of EBOOT.BIN and the Sharing module that is not newlib and has not been attributed to anything else yet.
# A public build gets these functions from the user's own copy of the game. To release a class, remove it from this
# tuple (and re-run hstorigin.py).
# Classes: game, newlib, mw-runtime, sony-prx, sony-middleware-sgx, gcc-unattributed.
EXCLUDED_CLASSES = ("sony-prx", "sony-middleware-sgx", "gcc-unattributed")
# Maintainer-only files that stay in the maintainers' tree: working research notes, and the scripts that run
# external agent lanes (they depend on the maintainers' own tool setup). A trailing "/" excludes a whole directory.
PRIVATE_PATHS = ("docs/research/", "docs/LANE-TASK.md", "docs/AGENT-BRIEF.md", "tools/hstauto.sh", "tools/hstsupervise.sh",
                 "tools/hstlane.py", "tools/hstrestart.sh", "tools/hstpushprep.sh",
                 "tools/hstexec.py", "tools/test_hstexec.py", "tools/test_hstlane.py",
                 # the accepted ledger names every proven source, the Sony SDK ones included (tools/hstproof.py)
                 "data/accepted-ledger.jsonl", "data/accepted-journal.jsonl")
# ---------------------------------------------------------------------------------------------------------------

MANIFEST = "ORIGINS.json"
# Content that never leaves the private repository, whatever file holds it (public AGENTS.md section 5: nothing that
# shows a local path). The patterns of tools/hstprcheck.py PRIVATE and the push preparation scan (hstpushprep.sh),
# widened to the maintainer's workspace root on any drive and in its WSL and MSYS forms. Placeholders (`<you>`,
# `/home/user`) and the well-known non-personal Windows profiles (Default, Public) stay allowed.
# PRIVATE_PATH runs on the ASCII-lower-cased text and starts with the path separator, so the regex engine skips ahead
# to the next slash or backslash (a scan of the whole tree takes seconds, not minutes); the drive letter, /mnt/<drive>,
# MSYS and /home forms are checked behind it. Its forms: <drive>:\Users\<name>, the workspace root `nk` directly under
# a drive (Windows, /mnt/<drive>/ and the MSYS /<drive>/ form), /mnt/<drive>/Users/<name> and /home/<name>, with / or
# \ (repeated, as in escaped JSON) as separator. PRIVATE_TOKEN runs only on text that holds one of its literal
# prefixes (_TOKEN_PREFIXES), which a substring test finds quickly.
_NOT_PERSONAL = r"(?!<|(?:default|public|all users|test|user|you)\b)"
PRIVATE_PATH = re.compile(
    r"[\\/](?:(?<=[a-z]:[\\/])[\\/]*(?:users[\\/]+" + _NOT_PERSONAL + r"\w|nk\b)|"
    r"(?<=/)mnt/[a-z]/(?:nk\b|users/" + _NOT_PERSONAL + r"\w)|(?<![\w.~-]/)[a-z]/nk\b|(?<=/)home/(?!<|user\b)\w)")
PRIVATE_TOKEN = re.compile(r"-----BEGIN [A-Z ]*PRIVATE KEY-----|\bghp_[A-Za-z0-9]{20}|\bgithub_pat_\w{20}|"
                           r"\bsk-[A-Za-z0-9]{20}|\bAKIA[0-9A-Z]{16}\b")
_TOKEN_PREFIXES = ("PRIVATE KEY-----", "ghp_", "github_pat_", "sk-", "AKIA")
_ASCII_LOWER = str.maketrans("ABCDEFGHIJKLMNOPQRSTUVWXYZ", "abcdefghijklmnopqrstuvwxyz")  # keeps offsets


def private_content(files) -> list[str]:
    """`<path>:<line>` for each (path, bytes) that holds a PRIVATE_PATH or PRIVATE_TOKEN (the first hit per file);
    the matched text is not repeated, so a token is not printed again."""
    hits = []
    for path, data in files:
        text = data.decode("utf-8", "replace")
        token = PRIVATE_TOKEN.search(text) if any(k in text for k in _TOKEN_PREFIXES) else None
        found = [m.start() for m in (PRIVATE_PATH.search(text.translate(_ASCII_LOWER)), token) if m]
        if found:
            hits.append(f"{path}:{text.count(chr(10), 0, min(found)) + 1}")
    return hits


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

    hits = private_content((p, (ROOT / p).read_bytes()) for p in keep if p != MANIFEST)
    if hits:
        for h in hits:
            print(f"REFUSED private content (a local path or a secret): {h}", file=sys.stderr)
        print(f"REFUSED: {len(hits)} file(s) hold private content; nothing was written", file=sys.stderr)
        return 1

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
