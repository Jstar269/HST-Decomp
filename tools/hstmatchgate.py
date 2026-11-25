#!/usr/bin/env python3
"""Run the whole match and progress check, detached, on a clean checkout at a pushed commit.

    python tools/hstmatchgate.py <full commit SHA>

Refuses (exit 1, nothing started) unless HEAD is exactly that commit and the checkout has no staged, unstaged or
untracked change; gitignored local files (hst-config.json, build/, try/) may stay. Then it starts a worker that no
closed terminal stops (a new session on POSIX; on Windows a new process group with a hidden console of its own, out
of the terminal's job where Windows allows it), prints the commit and the log path, and returns. The worker runs:

    every tree        tools/hstmatch.py --all --parallel 4     every source under src/ is EXACT
    public tree       tools/hstprogress.py --check-readme      README table, docs/objdiff-report.json and the
                                                               generated figure blocks agree with the tree
    private tree      tools/hstproof.py check                  accepted ledger, journal and locks against the tree
                      tools/hstcov.py --proofs                 proof-based coverage

The private tree is the one with the accepted ledger (data/accepted-ledger.jsonl; tools/hstpublish.py PRIVATE_PATHS
keeps it out of the public tree). The log (hstmatchgate-*.log in the checkout's Git directory) starts with
`COMMIT <sha>` and `TREE public|private`, has one `PASS <tool> (exit N)` or `FAIL <tool> (...)` line per tool, checks
the checkout again after the last tool, and ends with `DONE PASS <sha>` or `DONE FAIL <sha>`. A log without that last
line is incomplete. The log names local paths when a tool prints them: keep it private.
"""
from __future__ import annotations

import argparse
import os
import re
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
LEDGER = "data/accepted-ledger.jsonl"
MATCH = ("hstmatch.py", "--all", "--parallel", "4")  # AGENTS.md: --parallel 4 at most
PUBLIC_CHECKS = (("hstprogress.py", "--check-readme"),)
PRIVATE_CHECKS = (("hstproof.py", "check"), ("hstcov.py", "--proofs"))
SHA = re.compile(r"[0-9a-f]{40}|[0-9a-f]{64}")
# Windows process creation flags. DETACHED_PROCESS is not used: a worker without a console makes every console
# program it starts (wsl.exe, once per compile) open a console window of its own.
CREATE_NEW_PROCESS_GROUP = 0x00000200
CREATE_BREAKAWAY_FROM_JOB = 0x01000000
CREATE_NO_WINDOW = 0x08000000


def git(*args: str) -> str:
    return subprocess.check_output(["git", *args], cwd=ROOT, text=True).strip()


def verify_checkout(expected: str) -> None:
    head = git("rev-parse", "HEAD")
    if head != expected:
        raise ValueError(f"HEAD {head} does not match the requested commit {expected}")
    if git("status", "--porcelain=v1", "--untracked-files=all", "--ignore-submodules=none"):
        raise ValueError("checkout has staged, unstaged or untracked changes")


def checks() -> tuple[str, list[tuple[str, ...]]]:
    if (ROOT / LEDGER).is_file():
        return "private", [MATCH, *PRIVATE_CHECKS]
    return "public", [MATCH, *PUBLIC_CHECKS]


def run_checks(expected: str) -> int:
    print(f"COMMIT {expected}", flush=True)
    status = "FAIL"
    try:
        tree, steps = checks()
        print(f"TREE {tree}", flush=True)
        verify_checkout(expected)
        passed = True
        for tool, *args in steps:
            if not (ROOT / "tools" / tool).is_file():
                print(f"FAIL {tool} (missing)", flush=True)
                passed = False
                continue
            try:
                code = subprocess.run([sys.executable, "-u", f"tools/{tool}", *args], cwd=ROOT,
                                      stdin=subprocess.DEVNULL, check=False).returncode
            except OSError as error:
                print(f"FAIL {tool} ({error})", flush=True)
                passed = False
            else:
                print(f"{'PASS' if code == 0 else 'FAIL'} {tool} (exit {code})", flush=True)
                passed &= code == 0
        verify_checkout(expected)
        status = "PASS" if passed else "FAIL"
    except (ValueError, OSError, subprocess.CalledProcessError) as error:
        print(f"FAIL checkout: {error}", flush=True)
    finally:
        print(f"DONE {status} {expected}", flush=True)
    return 0 if status == "PASS" else 1


def detach_options(system: str = os.name) -> list[dict]:
    """Popen options to try in order, so that closing the terminal does not stop the worker."""
    if system != "nt":
        return [{"start_new_session": True}]
    flags = CREATE_NEW_PROCESS_GROUP | CREATE_NO_WINDOW
    # a terminal may run its programs in a job that is killed with it; leave it where the job allows breakaway
    return [{"creationflags": flags | CREATE_BREAKAWAY_FROM_JOB}, {"creationflags": flags}]


def start_worker(expected: str, log) -> subprocess.Popen:
    command = [sys.executable, "-u", str(Path(__file__).resolve()), expected, "--worker"]
    *preferred, last = detach_options()
    for options in preferred:
        try:
            return subprocess.Popen(command, cwd=ROOT, stdin=subprocess.DEVNULL, stdout=log,
                                    stderr=subprocess.STDOUT, **options)
        except OSError:
            pass  # e.g. the job forbids breakaway: try the next option
    return subprocess.Popen(command, cwd=ROOT, stdin=subprocess.DEVNULL, stdout=log, stderr=subprocess.STDOUT, **last)


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("expected_head", help="full SHA of the pushed commit (the pull request's head)")
    ap.add_argument("--worker", action="store_true", help=argparse.SUPPRESS)
    args = ap.parse_args()
    if not SHA.fullmatch(args.expected_head):
        ap.error("expected_head must be a full lowercase Git commit SHA")
    if args.worker:
        return run_checks(args.expected_head)

    log = None
    try:
        verify_checkout(args.expected_head)
        log_dir = Path(git("rev-parse", "--absolute-git-dir"))
        with tempfile.NamedTemporaryFile(mode="w", prefix="hstmatchgate-", suffix=".log", dir=log_dir,
                                         delete=False) as log:
            worker = start_worker(args.expected_head, log)
    except (ValueError, OSError, subprocess.CalledProcessError) as error:
        if log is not None:
            Path(log.name).unlink(missing_ok=True)  # no worker writes it
        print(f"REFUSED: {error}", file=sys.stderr)
        return 1

    print(f"STARTED {args.expected_head} (PID {worker.pid})")
    print(f"Log: {log.name}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
