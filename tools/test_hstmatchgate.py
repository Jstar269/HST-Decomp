#!/usr/bin/env python3
"""Synthetic tests for the clean-checkout match gate (hstmatchgate.py): disposable Git repositories with stand-in
tools. No game, compiler or configuration: every tool the gate runs here is a short script that checks its arguments."""
from __future__ import annotations

import os
import shutil
import subprocess
import sys
import tempfile
import time
import unittest
from pathlib import Path
from unittest import mock

TOOLS = Path(__file__).resolve().parent
GATE = TOOLS / "hstmatchgate.py"
sys.path.insert(0, str(TOOLS))
import hstmatchgate  # noqa: E402

LEDGER = "data/accepted-ledger.jsonl"


def stub(expected_args: list[str], code: int = 0) -> str:
    """A stand-in tool: it fails (exit 9) when called with other arguments, otherwise exits with `code`."""
    return f"import sys\nif sys.argv[1:] != {expected_args!r}:\n    raise SystemExit(9)\nraise SystemExit({code})\n"


class MatchGateTests(unittest.TestCase):
    def setUp(self):
        workspace = tempfile.TemporaryDirectory()
        self.addCleanup(workspace.cleanup)
        self.root = Path(workspace.name)
        self.tools = self.root / "tools"
        self.tools.mkdir()
        shutil.copyfile(GATE, self.tools / GATE.name)
        self.write_tool("hstmatch.py", stub(["--all", "--parallel", "4"]))
        self.write_tool("hstprogress.py", stub(["--check-readme"]))
        (self.root / "data").mkdir()
        (self.root / "data/modules.json").write_text("{}\n", encoding="utf-8")
        # the gitignored places the real tools write to stay allowed
        (self.root / ".gitignore").write_text("/build/\n/try/\n/hst-config.json\n__pycache__/\n", encoding="utf-8")
        self.git("init", "-q")
        self.head = self.commit()

    def write_tool(self, name: str, text: str) -> None:
        (self.tools / name).write_text(text, encoding="utf-8")

    def git(self, *args: str) -> str:
        return subprocess.check_output(["git", *args], cwd=self.root, text=True).strip()

    def commit(self) -> str:
        self.git("add", "-A")
        self.git("-c", "user.name=Match Gate Fixture", "-c", "user.email=fixture@example.test",
                 "-c", "commit.gpgsign=false", "commit", "-qm", "Synthetic fixture")
        return self.git("rev-parse", "HEAD")

    def gate(self, *args: str) -> subprocess.CompletedProcess:
        return subprocess.run([sys.executable, str(self.tools / GATE.name), *args],
                              cwd=self.root, text=True, capture_output=True, timeout=30, check=False)

    def logs(self) -> list[Path]:
        return list(Path(self.git("rev-parse", "--absolute-git-dir")).glob("hstmatchgate-*.log"))

    def finished_log(self, head: str) -> str:
        result = self.gate(head)
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertIn(f"STARTED {head}", result.stdout)
        log = Path(result.stdout.split("Log: ", 1)[1].strip())
        deadline = time.monotonic() + 30
        while time.monotonic() < deadline:
            text = log.read_text(encoding="utf-8")
            if text.endswith(f" {head}\n") and text.splitlines()[-1].startswith("DONE "):
                return text
            time.sleep(0.05)
        self.fail("the detached worker did not finish")

    # --- refusals -------------------------------------------------------------------------------------------------

    def test_refuses_a_different_head(self):
        other = "0" * len(self.head)
        result = self.gate(other)
        self.assertEqual(result.returncode, 1)
        self.assertIn(f"REFUSED: HEAD {self.head} does not match the requested commit {other}", result.stderr)
        self.assertEqual(self.logs(), [], "a refused run starts no worker and writes no log")

    def test_refuses_an_older_commit_of_the_same_branch(self):
        old = self.head
        (self.root / "README.md").write_text("later commit\n", encoding="utf-8")
        new = self.commit()
        result = self.gate(old)
        self.assertEqual(result.returncode, 1)
        self.assertIn(f"HEAD {new} does not match the requested commit {old}", result.stderr)

    def test_refuses_a_dirty_checkout(self):
        for kind in ("untracked", "unstaged", "staged", "staged new file"):
            with self.subTest(kind=kind):
                path = self.tools / "hstmatch.py" if kind in ("unstaged", "staged") else self.root / "scratch.c"
                original = path.read_bytes() if path.exists() else None
                path.write_text("print('dirty')\n", encoding="utf-8")
                if kind.startswith("staged"):
                    self.git("add", str(path))
                result = self.gate(self.head)
                self.assertEqual(result.returncode, 1)
                self.assertIn("REFUSED: checkout has staged, unstaged or untracked changes", result.stderr)
                self.assertEqual(self.logs(), [])
                self.git("reset", "-q")
                if original is None:
                    path.unlink()
                else:
                    path.write_bytes(original)
                self.assertEqual(self.git("status", "--porcelain"), "")

    def test_refuses_an_abbreviated_or_malformed_sha(self):
        for value in (self.head[:12], self.head.upper(), "HEAD", self.head + "0"):
            with self.subTest(value=value):
                result = self.gate(value)
                self.assertEqual(result.returncode, 2)
                self.assertIn("full lowercase Git commit SHA", result.stderr)
        self.assertEqual(self.logs(), [])

    # --- accepted runs --------------------------------------------------------------------------------------------

    def test_clean_public_checkout_runs_the_matcher_and_the_progress_check(self):
        # gitignored local inputs (configuration, build products, scratch) do not make a checkout dirty
        (self.root / "hst-config.json").write_text("{}\n", encoding="utf-8")
        (self.root / "try").mkdir()
        (self.root / "try/notes.txt").write_text("scratch\n", encoding="utf-8")
        text = self.finished_log(self.head)
        self.assertTrue(text.startswith(f"COMMIT {self.head}\nTREE public\n"), text)
        self.assertIn("PASS hstmatch.py (exit 0)", text)
        self.assertIn("PASS hstprogress.py (exit 0)", text)
        self.assertNotIn("hstproof.py", text)
        self.assertTrue(text.endswith(f"DONE PASS {self.head}\n"), text)
        self.assertEqual(self.git("status", "--porcelain"), "")

    def test_private_checkout_checks_the_accepted_ledger_and_proof_coverage(self):
        (self.root / LEDGER).write_text('{"schema": "synthetic"}\n', encoding="utf-8")
        self.write_tool("hstproof.py", stub(["check"]))
        self.write_tool("hstcov.py", stub(["--proofs"]))
        head = self.commit()
        text = self.finished_log(head)
        self.assertIn("TREE private\n", text)
        self.assertIn("PASS hstmatch.py (exit 0)", text)
        self.assertIn("PASS hstproof.py (exit 0)", text)
        self.assertIn("PASS hstcov.py (exit 0)", text)
        self.assertNotIn("hstprogress.py", text)
        self.assertTrue(text.endswith(f"DONE PASS {head}\n"), text)

    @unittest.skipUnless(hasattr(os, "getsid"), "POSIX sessions")
    def test_the_worker_runs_outside_the_terminal_session(self):
        self.write_tool("hstmatch.py", "import os\nprint('SESSION', os.getsid(0))\n")
        head = self.commit()
        text = self.finished_log(head)
        session = int(text.split("SESSION ", 1)[1].split()[0])
        self.assertNotEqual(session, os.getsid(0), "a hang-up of the starting terminal's session would stop it")
        self.assertTrue(text.endswith(f"DONE PASS {head}\n"), text)

    def test_a_failed_match_still_runs_the_progress_check_and_fails_the_run(self):
        self.write_tool("hstmatch.py", stub(["--all", "--parallel", "4"], code=1))
        head = self.commit()
        text = self.finished_log(head)
        self.assertIn("FAIL hstmatch.py (exit 1)", text)
        self.assertIn("PASS hstprogress.py (exit 0)", text)
        self.assertTrue(text.endswith(f"DONE FAIL {head}\n"), text)

    def test_a_progress_difference_fails_the_run(self):
        self.write_tool("hstprogress.py", stub(["--check-readme"], code=1))
        head = self.commit()
        text = self.finished_log(head)
        self.assertIn("PASS hstmatch.py (exit 0)", text)
        self.assertIn("FAIL hstprogress.py (exit 1)", text)
        self.assertTrue(text.endswith(f"DONE FAIL {head}\n"), text)

    def test_a_missing_tool_fails_the_run(self):
        (self.tools / "hstprogress.py").unlink()
        head = self.commit()
        text = self.finished_log(head)
        self.assertIn("FAIL hstprogress.py (missing)", text)
        self.assertTrue(text.endswith(f"DONE FAIL {head}\n"), text)

    def test_a_checkout_changed_during_the_run_cannot_pass(self):
        self.write_tool("hstmatch.py", "open('scratch.c', 'w').close()\n")
        head = self.commit()
        text = self.finished_log(head)
        self.assertIn("PASS hstmatch.py (exit 0)", text)
        self.assertIn("FAIL checkout: checkout has staged, unstaged or untracked changes", text)
        self.assertTrue(text.endswith(f"DONE FAIL {head}\n"), text)

    def test_a_commit_made_during_the_run_cannot_pass(self):
        self.write_tool("hstmatch.py", "import subprocess\n"
                        "subprocess.run(['git', '-c', 'user.name=F', '-c', 'user.email=f@example.test', "
                        "'-c', 'commit.gpgsign=false', 'commit', '-q', '--allow-empty', '-m', 'moved'], check=True)\n")
        head = self.commit()
        text = self.finished_log(head)
        self.assertIn("FAIL checkout: HEAD ", text)
        self.assertIn(f"does not match the requested commit {head}", text)
        self.assertTrue(text.endswith(f"DONE FAIL {head}\n"), text)


class DetachTests(unittest.TestCase):
    """How the worker is started (the Windows half cannot run here, so Popen is a stand-in)."""

    def test_posix_starts_a_new_session(self):
        self.assertEqual(hstmatchgate.detach_options("posix"), [{"start_new_session": True}])

    def test_windows_hides_the_console_and_leaves_the_terminal_job_when_allowed(self):
        first, fallback = hstmatchgate.detach_options("nt")
        flags = first["creationflags"]
        self.assertTrue(flags & hstmatchgate.CREATE_NO_WINDOW, "wsl.exe children must not open console windows")
        self.assertTrue(flags & hstmatchgate.CREATE_NEW_PROCESS_GROUP)
        self.assertTrue(flags & hstmatchgate.CREATE_BREAKAWAY_FROM_JOB)
        self.assertFalse(flags & 0x00000008, "DETACHED_PROCESS would give every wsl.exe child a console window")
        self.assertEqual(fallback["creationflags"], flags & ~hstmatchgate.CREATE_BREAKAWAY_FROM_JOB)

    def test_windows_falls_back_when_the_job_forbids_breakaway(self):
        calls = []

        def popen(cmd, **kwargs):
            calls.append(kwargs["creationflags"])
            if kwargs["creationflags"] & hstmatchgate.CREATE_BREAKAWAY_FROM_JOB:
                raise PermissionError(5, "Access is denied")
            return mock.Mock(pid=1234)

        windows = hstmatchgate.detach_options("nt")
        with tempfile.TemporaryFile("w") as log, \
                mock.patch.object(hstmatchgate, "detach_options", return_value=windows), \
                mock.patch.object(hstmatchgate.subprocess, "Popen", side_effect=popen):
            worker = hstmatchgate.start_worker("a" * 40, log)
        self.assertEqual(worker.pid, 1234)
        self.assertEqual(len(calls), 2)
        self.assertFalse(calls[1] & hstmatchgate.CREATE_BREAKAWAY_FROM_JOB)


if __name__ == "__main__":
    unittest.main()
