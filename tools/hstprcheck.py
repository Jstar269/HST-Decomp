#!/usr/bin/env python3
"""Checks for a contribution: what a pull request adds must be source, never game data or private material.

    python tools/hstprcheck.py [--base origin/main] [--repo .]

Compares the commits of the branch (`git diff <base>...HEAD`) with the base and refuses, for each file the branch
adds or changes:

  binary          a binary file (git sees it as binary, or it holds a NUL byte), or a name the ignore rules keep
                  out for that reason (retail modules, objects, images of discs, archives, compilers, key files)
  place           a path that is never versioned: build/, try/, keys/, Ghidra projects and caches, hst-config.json
  size            a changed file over 1 MiB (sources are a few kilobytes; nothing a contribution needs is that big);
                  for the append-only compiler log (docs/MWCC-NOTES.md) the limit applies to what is appended, and the
                  generated objdiff report (docs/objdiff-report.json, one unit per function) has a 32 MiB limit
  report          that report when its units do not add up to its module figures (tools/hstreport.py unit_problems)
  log             a changed or deleted line in that log (append only). One exception, for redaction: a line may
                  change when the new line is the old one with local path roots (`C:\\...`, `/mnt/c/...`,
                  `/home/...`) replaced by the placeholder `<private tree>` and nothing else changed (REDACTION)
  private         an added line with a personal or secret value: a home directory path (C:\\Users\\..., /home/...,
                  /mnt/c/Users/...), a private key block, a GitHub, OpenAI or AWS token
  dump            an added file with more than 16 lines that look like a disassembly listing (an address, an
                  instruction word, a mnemonic): disassembly of the game is never published
  symbols         a deleted or changed line in symbols/ (the symbol maps are append-only); deleting, renaming or
                  replacing (a symbolic link) a file of symbols/ or the log deletes all its lines and is refused too
  header          a source in src/ or wip/ whose first line is not an HSTMATCH header with module, start, end and
                  symbol, or that repeats the header further down; a source in src/ without one of the provenance
                  notes CONTRIBUTING.md lists right after the header (tools/hstlint.py provenance_note: one-line
                  annotations such as a docs/DATA-TABLES.md pointer may come first). A near miss in wip/ needs the
                  header only (CONTRIBUTING.md, "Near misses")
  gate            a source the fake-match gate refuses (tools/hstgate.py: asm that emits instructions, data
                  directives, code-emitting pragmas, extra= under src/, cc=gcc404x-* outside the SDK units), with the
                  SDK units of the base (data/sdk-gcc4-units.json), so a pull request cannot widen them for itself

It needs no game, compiler or configuration, so it runs in CI on every pull request and before you open one. It is a
first filter: the maintainer still reviews every change and re-verifies every source with tools/hstmatch.py before
it is accepted. Exit status 1 when anything is refused.
"""
from __future__ import annotations

import argparse
import re
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hstgate
import hstlint

MATCH = "HSTMATCH"
CONFIG = "hst-config.json"
MAX_BYTES = 1 << 20
# Append-only logs (CONTRIBUTING.md, "Compiler notes"): the size limit applies to what a change appends, never to the
# whole log, which grows with every lesson and is read with tail and grep from fixed line numbers, so it stays one
# file. Changing or deleting an existing line is refused, as for symbols/.
APPEND_ONLY_LOGS = ("docs/MWCC-NOTES.md",)
# The one change an append-only line may take: a local path root (a drive letter, /mnt/<drive>/ or /home/ and the path
# characters after it) replaced by this placeholder, every other byte of the line kept (is_redaction).
REDACTION = "<private tree>"
LOCAL_ROOT = r"(?:[A-Za-z]:[\\/]|/mnt/[a-z]/|/home/)[^\s`'\"<>|()\[\]]*?"
LOCAL_PATH = re.compile(r"[A-Za-z]:[\\/]+\w|/mnt/[a-z]/\w|/home/\w")
# The objdiff progress report (tools/hstreport.py) lists every reviewed function of the two game modules, one unit per
# line, so it is several MiB. It has its own limit, and it must still add up: its units, to the module figures.
OBJDIFF_REPORT = "docs/objdiff-report.json"
REPORT_MAX_BYTES = 32 << 20
BINARY_SUFFIXES = {".o", ".obj", ".elf", ".bin", ".prx", ".pbp", ".iso", ".cso", ".exe", ".dll", ".so", ".a", ".zip",
                   ".7z", ".gz", ".tar", ".gzf", ".rep", ".img", ".dat", ".sfo", ".at3", ".pmf"}
NEVER = (re.compile(r"^(build|try|keys)/"), re.compile(r"^ghidra/(projects|out|cache|settings)/"),
         re.compile(r"(^|/)[^/]*keyfile[^/]*$", re.I), re.compile(rf"^{re.escape(CONFIG)}$"))
PRIVATE = re.compile(r"[A-Za-z]:[\\/]+Users[\\/]+(?!<)\w|/home/(?!<|user\b)\w|/mnt/[a-z]/Users/(?!<)\w|"
                     r"-----BEGIN [A-Z ]*PRIVATE KEY-----|\bghp_[A-Za-z0-9]{20}|\bgithub_pat_\w{20}|"
                     r"\bsk-[A-Za-z0-9]{20}|\bAKIA[0-9A-Z]{16}\b")
DUMP_LINE = re.compile(r"^\W*(0x)?[0-9a-fA-F]{6,8}:?\s+(0x)?[0-9a-fA-F]{8}\s+[a-z][a-z0-9.]{1,9}\b")
DUMP_LIMIT = 16
HEADER = re.compile(rf"^/\* {MATCH} (?=.*\bmodule=\S)(?=.*\bstart=0x[0-9a-fA-F]+)(?=.*\bend=0x[0-9a-fA-F]+)"
                    r"(?=.*\bsymbol=\S).*\*/\s*$")


def git(repo: Path, *args: str) -> str:
    return subprocess.run(["git", *args], cwd=repo, check=True, capture_output=True, text=True,
                          encoding="utf-8", errors="replace").stdout


def git_z(repo: Path, *args: str) -> list[str]:
    """The NUL-separated fields of a `git ... -z` listing. With -z git writes each path as it is, never C-quoted (a
    name with a non-ASCII byte, a quote, a backslash or a tab is "quoted\\303\\251" otherwise, which names no file);
    the bytes are decoded with surrogateescape so that the path still names the file on disk."""
    out = subprocess.run(["git", *args], cwd=repo, check=True, capture_output=True).stdout
    return out.decode("utf-8", "surrogateescape").split("\0")[:-1]


def changed_files(repo: Path, base: str) -> list[tuple[str, str]]:
    """(status, path) of every file the branch adds or modifies relative to the merge base (deletions are fine)."""
    fields = git_z(repo, "diff", "--name-status", "-z", "--no-renames", "--diff-filter=AM", f"{base}...HEAD")
    return list(zip(fields[0::2], fields[1::2]))


def removed_files(repo: Path, base: str) -> list[tuple[str, str]]:
    """(status, path) of every file the branch deletes (D) or turns into another kind of file (T: a symbolic link,
    a submodule). A rename is a deletion and an addition (--no-renames)."""
    fields = git_z(repo, "diff", "--name-status", "-z", "--no-renames", "--diff-filter=DT", f"{base}...HEAD")
    return list(zip(fields[0::2], fields[1::2]))


def append_only(path: str) -> bool:
    return path.startswith("symbols/") or path in APPEND_ONLY_LOGS


def binary_paths(repo: Path, base: str) -> set[str]:
    fields = git_z(repo, "diff", "--numstat", "-z", "--no-renames", "--diff-filter=AM", f"{base}...HEAD")
    return {f.split("\t", 2)[2] for f in fields if f.startswith("-\t-\t")}


def hunk_lines(repo: Path, base: str, path: str, sign: str) -> list[str]:
    """The lines of the file's hunks that start with sign ("+" added, "-" removed), without it. Only lines inside a
    hunk count: a file line that starts with "++" or "--" is "+++ ..." or "--- ..." in the diff, and only the file
    headers before the first "@@" are skipped. The diff is split at newlines only (what git counts as a line), never
    at the CR, form feed or other separators str.splitlines() also splits at, so no part of a line is dropped."""
    # bytes, decoded here: text mode turns a CR into \n. --literal-pathspecs: the path names this one file, never
    # pathspec magic (":(exclude)*") or a glob ("[ab].c")
    out = subprocess.run(["git", "--literal-pathspecs", "diff", "--no-renames", "-U0", f"{base}...HEAD", "--", path],
                         cwd=repo, check=True, capture_output=True).stdout.decode("utf-8", "replace")
    lines, in_hunk = [], False
    for line in out.split("\n"):
        if line.startswith("diff --git "):
            in_hunk = False
        elif line.startswith("@@"):
            in_hunk = True
        elif in_hunk and line.startswith(sign):
            lines.append(line[1:])
    return lines


def added_lines(repo: Path, base: str, path: str) -> list[str]:
    return hunk_lines(repo, base, path, "+")


def removed_lines(repo: Path, base: str, path: str) -> list[str]:
    return hunk_lines(repo, base, path, "-")


def is_redaction(old: str, new: str) -> bool:
    """True when new is old with one or more local path roots replaced by REDACTION and no other byte changed, and
    new names no local path any more."""
    if REDACTION not in new or LOCAL_PATH.search(new):
        return False
    pattern = LOCAL_ROOT.join(re.escape(part) for part in new.split(REDACTION))
    return re.fullmatch(pattern, old) is not None


def unredacted(removed: list[str], added: list[str]) -> list[str]:
    """The removed lines that no added line redacts (each added line redacts at most one removed line)."""
    free = list(added)
    left = []
    for old in removed:
        hit = next((i for i, new in enumerate(free) if is_redaction(old, new)), None)
        if hit is None:
            left.append(old)
        else:
            free.pop(hit)
    return left


def report_problems(data: bytes) -> list[str]:
    """Why an objdiff report is not one tools/hstreport.py wrote ([] when its units add up to its modules)."""
    import json

    import hstreport
    try:
        return hstreport.unit_problems(json.loads(data.decode("utf-8")))
    except (ValueError, KeyError, TypeError, AttributeError) as exc:
        return [f"not an objdiff report tools/hstreport.py writes ({type(exc).__name__})"]


def base_sdk_units(repo: Path, base: str, into: Path) -> Path:
    """A root that holds the merge base's data/sdk-gcc4-units.json (or none, when the base has none), for the gate's
    gcc404x-outside-sdk rule: a pull request that widens the SDK units must not pass its own sources with them."""
    rel = hstgate.SDK_UNITS_FILE.as_posix()
    merge_base = git(repo, "merge-base", base, "HEAD").strip()
    shown = subprocess.run(["git", "show", f"{merge_base}:{rel}"], cwd=repo, capture_output=True)
    if shown.returncode == 0:
        (into / rel).parent.mkdir(parents=True, exist_ok=True)
        (into / rel).write_bytes(shown.stdout)
    return into


def gate_findings(file: Path, repo: Path, units_root: Path) -> list:
    """hstgate.check_file, with the SDK units read under units_root (the base's) instead of the pull request's."""
    text = file.read_text(encoding="utf-8", errors="replace")
    header = None
    m = re.search(r"/\*\s*HSTMATCH\s+(.*?)\*/", text[:2000], re.S)
    if m:
        header = {k: v.strip('"') for k, v in re.findall(r'(\w+)=("[^"]*"|\S+)', m.group(1))}
    return hstgate.check_text(text, header, hstgate.tree_of(file, repo), units_root)


def check(repo: Path, base: str) -> list[str]:
    with tempfile.TemporaryDirectory(prefix="hstprcheck-") as tmp:
        return _check(repo, base, base_sdk_units(repo, base, Path(tmp)))


def _check(repo: Path, base: str, units_root: Path) -> list[str]:
    refusals: list[str] = []
    binaries = binary_paths(repo, base)
    for status, path in removed_files(repo, base):
        if append_only(path):     # deleting, renaming or replacing the file deletes every line of it
            refusals.append(f"{'symbols' if path.startswith('symbols/') else 'log':<8} {path}: "
                            + ("deleted or renamed" if status == "D" else "replaced by another kind of file")
                            + " (append only)")
    for status, path in changed_files(repo, base):
        file = repo / path
        try:
            path.encode("utf-8")
        except UnicodeEncodeError:
            shown = path.encode("utf-8", "surrogateescape").decode("utf-8", "backslashreplace")
            refusals.append(f"name     {shown}: a file name must be UTF-8")
            continue
        if Path(path).suffix.lower() in BINARY_SUFFIXES or path in binaries:
            refusals.append(f"binary   {path}: binary files and game data are never added")
            continue
        if any(p.search(path) for p in NEVER):
            refusals.append(f"place    {path}: this path is never versioned")
            continue
        data = file.read_bytes() if file.is_file() else b""
        if path in APPEND_ONLY_LOGS and status == "M":
            if unredacted(removed_lines(repo, base, path), added_lines(repo, base, path)):
                refusals.append(f"log      {path}: existing lines changed or deleted (append only)")
            grown = sum(len(line.encode("utf-8")) + 1 for line in added_lines(repo, base, path))
            if grown > MAX_BYTES:
                refusals.append(f"size     {path}: {grown:,} bytes appended (limit {MAX_BYTES:,})")
                continue
        elif path == OBJDIFF_REPORT:
            if len(data) > REPORT_MAX_BYTES:
                refusals.append(f"size     {path}: {len(data):,} bytes (limit {REPORT_MAX_BYTES:,})")
                continue
            problems = report_problems(data)
            if problems:
                refusals.append(f"report   {path}: {problems[0]}")
        elif len(data) > MAX_BYTES:
            refusals.append(f"size     {path}: {len(data):,} bytes (limit {MAX_BYTES:,})")
            continue
        if b"\0" in data:
            refusals.append(f"binary   {path}: holds NUL bytes")
            continue
        added = added_lines(repo, base, path)
        for line in added:
            if PRIVATE.search(line):
                refusals.append(f"private  {path}: an added line holds a personal path or a secret: "
                                f"{line.strip()[:80]}")
                break
        # a listing with CR (or other non-newline) separators is one line to git: count each of its parts
        dump = sum(1 for line in added for part in line.splitlines() if DUMP_LINE.match(part))
        if dump > DUMP_LIMIT:
            refusals.append(f"dump     {path}: {dump} added lines look like a disassembly listing")
        if path.startswith("symbols/") and status == "M" and removed_lines(repo, base, path):
            refusals.append(f"symbols  {path}: existing lines changed or deleted (append only)")
        if re.match(r"^(src|wip)/.*\.(c|cpp)$", path):
            text = data.decode("utf-8", "replace")
            lines = text.split("\n")
            if not HEADER.match(lines[0]):
                refusals.append(f"header   {path}: line 1 is not a {MATCH} header with module, start, end, symbol")
            elif any(ln.lstrip().startswith(f"/* {MATCH} ") for ln in lines[1:]):
                refusals.append(f"header   {path}: a second {MATCH} header after line 1")
            elif path.startswith("src/") and hstlint.provenance_note(text)[0] is None:
                refusals.append(f"header   {path}: no provenance note CONTRIBUTING.md lists right after the header")
            found = hstgate.refusals(gate_findings(file, repo, units_root))
            if found:
                refusals.append(f"gate     {path}: " + "; ".join(str(f) for f in found)[:200])
    return refusals


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--base", default="origin/main", help="the branch the pull request goes into (default origin/main)")
    ap.add_argument("--repo", type=Path, default=Path(__file__).resolve().parents[1],
                    help="the checkout to check (default: this one)")
    args = ap.parse_args()
    repo = args.repo.resolve()
    try:
        git(repo, "rev-parse", "--verify", "--quiet", f"{args.base}^{{commit}}")
    except subprocess.CalledProcessError:
        raise SystemExit(f"no commit {args.base!r} to compare with: fetch it first (git fetch origin main), or pass "
                         "--base with the branch your pull request goes into")
    files = changed_files(repo, args.base)
    refusals = check(repo, args.base)
    for r in refusals:
        print("REFUSED " + r)
    print(f"{len(files)} added or changed file(s) against {args.base}: "
          + ("OK" if not refusals else f"{len(refusals)} refusal(s)"))
    return 1 if refusals else 0


if __name__ == "__main__":
    raise SystemExit(main())
