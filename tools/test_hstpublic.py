#!/usr/bin/env python3
"""Tests for the public-branch check (hstpublic.py check_tree), on a throwaway git repository. No game data."""
from __future__ import annotations

import contextlib
import io
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path
from unittest import mock

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hstpublic as P  # noqa: E402


class CheckTreeTests(unittest.TestCase):
    def setUp(self) -> None:
        self.tmp = tempfile.TemporaryDirectory()
        self.repo = Path(self.tmp.name)
        for args in (("init", "-q"), ("config", "user.email", "t@example.invalid"), ("config", "user.name", "t")):
            self.git(*args)
        self.write("ORIGINS.json", "{}\n")
        self.write("README.md", "public\n")

    def tearDown(self) -> None:
        self.tmp.cleanup()

    def git(self, *args: str) -> str:
        return subprocess.run(["git", *args], cwd=self.repo, check=True, capture_output=True, text=True).stdout

    def write(self, rel: str, text: str) -> None:
        (self.repo / rel).parent.mkdir(parents=True, exist_ok=True)
        (self.repo / rel).write_text(text, encoding="utf-8")

    def check(self) -> tuple[int, int]:
        """(worktree check, commit check) exit statuses after committing everything."""
        self.git("add", "-A")
        self.git("commit", "-q", "-m", "publish")
        with contextlib.redirect_stderr(io.StringIO()), mock.patch.object(P, "ROOT", self.repo):
            return P.check_tree(self.repo, None), P.check_tree(None, "HEAD")

    def test_a_public_tree_passes(self) -> None:
        self.assertEqual(self.check(), (0, 0))

    def test_maintainer_only_files_are_refused(self) -> None:
        # `sync` stages the whole public worktree (`git add -A`), so an untracked maintainer-only file there (the
        # accepted ledger, a lane brief) would be committed; the check is the last gate before the commit and the push
        for rel in ("data/accepted-ledger.jsonl", "docs/LANE-TASK.md", "docs/research/notes.md", "tools/hstlane.py"):
            with self.subTest(rel=rel):
                self.write(rel, "maintainer only\n")
                self.assertEqual(self.check(), (1, 1))
                self.git("rm", "-q", rel)
                self.git("commit", "-q", "-m", "drop")


if __name__ == "__main__":
    unittest.main()
