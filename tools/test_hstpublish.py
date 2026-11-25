#!/usr/bin/env python3
"""Tests for the public-tree export (hstpublish.py), on a throwaway git repository. No game data."""
from __future__ import annotations

import contextlib
import io
import os
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path
from unittest import mock

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hstorigin  # noqa: E402
import hstprcheck  # noqa: E402
import hstpublish as P  # noqa: E402


class ExportTests(unittest.TestCase):
    def setUp(self) -> None:
        self.tmp = tempfile.TemporaryDirectory()
        self.repo = Path(self.tmp.name) / "repo"
        self.out = Path(self.tmp.name) / "out"
        self.repo.mkdir()
        subprocess.run(["git", "init", "-q"], cwd=self.repo, check=True)

    def tearDown(self) -> None:
        self.tmp.cleanup()

    def write(self, rel: str, text: str) -> None:
        (self.repo / rel).parent.mkdir(parents=True, exist_ok=True)
        (self.repo / rel).write_text(text, encoding="utf-8")

    def export(self) -> int:
        subprocess.run(["git", "add", "-A"], cwd=self.repo, check=True)
        with mock.patch.object(P, "ROOT", self.repo), \
                mock.patch.object(hstorigin, "classify", lambda *a, **k: ({}, [])), \
                mock.patch.object(sys, "argv", ["hstpublish.py", "--out", str(self.out)]), \
                contextlib.redirect_stdout(io.StringIO()), contextlib.redirect_stderr(io.StringIO()):
            return P.main()

    def test_private_paths_stay_out(self) -> None:
        self.write("data/accepted-ledger.jsonl", '{"source": "src/x/sdk.c"}\n')
        self.write("docs/LANE-TASK.md", "lane brief\n")
        self.write("README.md", "public\n")
        self.assertEqual(self.export(), 0)
        self.assertEqual(sorted(p.relative_to(self.out).as_posix() for p in self.out.rglob("*") if p.is_file()),
                         ["README.md"])

    @unittest.skipUnless(hasattr(os, "symlink") and os.name != "nt", "needs POSIX symbolic links")
    def test_a_symbolic_link_does_not_publish_its_target(self) -> None:
        # a tracked link to a private file (or to anything outside the tree) would be copied with its target's bytes
        self.write("data/accepted-ledger.jsonl", '{"source": "src/x/sdk.c"}\n')
        self.write("docs/LANE-TASK.md", "lane brief\n")
        self.write("README.md", "public\n")
        os.symlink("../data/accepted-ledger.jsonl", self.repo / "docs" / "ledger.jsonl")
        os.symlink("LANE-TASK.md", self.repo / "docs" / "brief.md")
        self.assertEqual(self.export(), 0)
        published = {p.relative_to(self.out).as_posix(): p.read_text(encoding="utf-8")
                     for p in self.out.rglob("*") if p.is_file()}
        self.assertEqual(published, {"README.md": "public\n"})


class SecretFormatTests(unittest.TestCase):
    """Current GitHub, OpenAI and AWS credential formats and PGP key blocks (each fixture is assembled at run time,
    so this file holds none of them)."""
    SAMPLES = ("gh" + "o_" + "a1" * 18,                   # GitHub OAuth token
               "gh" + "s_" + "b2" * 18,                   # GitHub Actions / app installation token
               "gh" + "u_" + "c3" * 18, "gh" + "r_" + "d4" * 18,
               "sk-" + "proj-" + "Ab1_-x" * 8,           # OpenAI project key
               "sk-" + "svcacct-" + "Zz9-_y" * 8,        # OpenAI service-account key
               "AS" + "IA" + "Q7" * 8,                    # AWS temporary access key id
               "-----BEGIN PGP " + "PRIVATE KEY BLOCK-----")

    def test_prcheck_refuses_them(self) -> None:
        for sample in self.SAMPLES:
            with self.subTest(sample=sample[:8]):
                self.assertIsNotNone(hstprcheck.PRIVATE.search(f"token = {sample}"))

    def test_publish_refuses_them(self) -> None:
        for sample in self.SAMPLES:
            with self.subTest(sample=sample[:8]):
                self.assertEqual(P.private_content([("f.txt", f"x\nkey {sample}\n".encode())]), ["f.txt:2"])

    def test_ordinary_words_pass(self) -> None:
        for text in ("task-list-of-things-to-do-here", "the disk-based sk-learn style", "ASIAN_FONT_TABLE"):
            with self.subTest(text=text):
                self.assertIsNone(hstprcheck.PRIVATE.search(text))
                self.assertEqual(P.private_content([("f.txt", text.encode())]), [])


class HomeDirectoryFormTests(unittest.TestCase):
    """macOS home directories and Git for Windows' (MSYS) form of a Windows profile name the user account too
    (CONTRIBUTING.md: no paths that name your user account). Fixtures are assembled at run time."""
    PERSONAL = ("see /" + "Users/alice/hst-decomp/try", "file:///" + "Users/alice/x.c",
                "cd /c/" + "Users/alice/hst-decomp", "cd /d/" + "Users/bob/work")
    FINE = ("/" + "Users/Shared/hst", "/" + "Users/<you>/hst-decomp", "src/" + "Users/list.c",
            "https://example.invalid/" + "users/alice", "the users/alice map")

    def test_prcheck(self) -> None:
        for text in self.PERSONAL:
            with self.subTest(text=text):
                self.assertIsNotNone(hstprcheck.PRIVATE.search(text))
        for text in self.FINE:
            with self.subTest(text=text):
                self.assertIsNone(hstprcheck.PRIVATE.search(text))

    def test_publish(self) -> None:
        for text in self.PERSONAL:
            with self.subTest(text=text):
                self.assertEqual(P.private_content([("f.md", text.encode())]), ["f.md:1"])
        for text in self.FINE:
            with self.subTest(text=text):
                self.assertEqual(P.private_content([("f.md", text.encode())]), [])


class LetterCaseTests(unittest.TestCase):
    def test_prcheck_refuses_home_directories_in_any_letter_case(self) -> None:
        # Windows paths are case-insensitive, and os.path.normcase and many tools write them lower-cased
        for text in ("see c:" + "\\users\\someone\\game", "see /mnt/c/" + "users/someone/game",
                     "see C:/" + "USERS/someone/game"):
            with self.subTest(text=text):
                self.assertIsNotNone(hstprcheck.PRIVATE.search(text))


if __name__ == "__main__":
    unittest.main()
