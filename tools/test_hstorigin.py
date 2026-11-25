#!/usr/bin/env python3
"""Tests for the origin classes (hstorigin.py): synthetic sources, plus the SGX block against the committed units; and
for the publish policy built on them (hstpublish.py, hstpublic.py check), on throwaway git repositories."""
from __future__ import annotations

import io
import json
import subprocess
import sys
import tempfile
import unittest
from contextlib import redirect_stderr, redirect_stdout
from pathlib import Path
from unittest import mock

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hstorigin as O

EBOOT = "EBOOT.BIN"


def source(root: Path, rel: str, start: int, cc: str | None, symbol: str | None = None) -> Path:
    path = root / rel
    path.parent.mkdir(parents=True, exist_ok=True)
    extra = f" cc={cc}" if cc else ""
    path.write_text(f"/* HSTMATCH module={EBOOT} start=0x{start:08x} end=0x{start + 8:08x} "
                    f"symbol={symbol or f'FUN_{start:08x}'} lang=c99{extra} */\nint f(void) {{ return 0; }}\n",
                    encoding="utf-8")
    return path


class ClassTests(unittest.TestCase):
    def test_the_sgx_block_is_sony_middleware_whatever_the_compiler(self) -> None:
        lo, hi = O.SGX_BLOCKS[EBOOT]
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            cases = {
                source(root, "src/eboot/a.c", lo, "gcc171-O2"): O.SGX,
                source(root, "src/eboot/b.c", hi - 8, None): O.SGX,              # mwccpsp matches a few tiny ones
                source(root, "src/eboot/c.c", hi, "gcc171-O2"): "gcc-unattributed",
                source(root, "src/eboot/d.c", lo - 8, "gcc171-O2"): "gcc-unattributed",
                source(root, "src/eboot/e.c", hi, None): "game",
            }
            for path, want in cases.items():
                self.assertEqual(O.own_class(path, root), want, path.name)

    def test_a_newlib_note_still_comes_first(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            path = source(root, "src/eboot/n.c", O.SGX_BLOCKS[EBOOT][0], "gcc171-O2")
            path.write_text(path.read_text(encoding="utf-8") + "/* Library source: newlib 1.15.0 */\n", encoding="utf-8")
            self.assertEqual(O.own_class(path, root), "newlib")

    def test_sgx_is_a_class_of_place_for_clones(self) -> None:
        lo, hi = O.SGX_BLOCKS[EBOOT]
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            source(root, "src/eboot/sdk.c", hi + 0x100, "gcc171-O2")
            source(root, "src/eboot/sgx.c", lo + 0x10, "gcc171-O2")
            for rel, start, exemplar in (("src/eboot/in.c", lo + 0x20, "src/eboot/sdk.c"),
                                         ("src/eboot/out.c", hi + 0x200, "src/eboot/sgx.c")):
                path = source(root, rel, start, "gcc171-O2")
                head, body = path.read_text(encoding="utf-8").split("\n", 1)
                path.write_text(f"{head}\n/* Clone of {exemplar} (hstclone.py tag t1) */\n{body}", encoding="utf-8")
            classes, warnings = O.classify(root)
        self.assertEqual(warnings, [])
        self.assertEqual((classes["src/eboot/in.c"], classes["src/eboot/out.c"]), (O.SGX, "gcc-unattributed"))

    def test_a_clone_with_its_own_newlib_note_is_newlib(self) -> None:
        # the note comes first for a clone too (as in own_class and in lbporigin.py): a clone whose own note says
        # newlib is newlib, whatever its exemplar's class or its place
        lo, hi = O.SGX_BLOCKS[EBOOT]
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            source(root, "src/eboot/sdk.c", hi + 0x100, "gcc171-O2")
            for rel, start in (("src/eboot/n1.c", hi + 0x200), ("src/eboot/n2.c", lo + 0x20)):
                path = source(root, rel, start, "gcc171-O2")
                head, body = path.read_text(encoding="utf-8").split("\n", 1)
                path.write_text(f"{head}\n/* Clone of src/eboot/sdk.c (hstclone.py tag t1) */\n"
                                "/* Library source: newlib 1.14.0 libc_string/strcpy.c function strcpy */\n" + body,
                                encoding="utf-8")
            classes, _warnings = O.classify(root)
        self.assertEqual((classes["src/eboot/n1.c"], classes["src/eboot/n2.c"]), ("newlib", "newlib"))

    def test_the_class_is_excluded_like_the_sdk(self) -> None:
        import hstpublish
        self.assertIn(O.SGX, O.CLASSES)
        self.assertIn(O.SGX, hstpublish.EXCLUDED_CLASSES)

    def test_sgx_blocks_span_exactly_the_sgx_units(self) -> None:
        tu_path = O.ROOT / "data" / "translation-units.json"
        if not tu_path.exists():
            self.skipTest("no data/translation-units.json")
        doc = json.loads(tu_path.read_text(encoding="utf-8"))
        for module, (lo, hi) in O.SGX_BLOCKS.items():
            ends = {}
            for line in (O.ROOT / "data" / f"functions-{module}.txt").read_text(encoding="utf-8").splitlines():
                if line.strip() and not line.startswith("#"):
                    s, e = line.split()[:2]
                    ends[int(s, 16)] = int(e, 16)
            sgx = sorted(int(a, 16) for u in doc["modules"][module] if u["name"].lower().startswith("sgx")
                         for a, _k in u["functions"])
            self.assertEqual((sgx[0], ends[sgx[-1]]), (lo, hi), module)
            self.assertEqual(sum(1 for s in ends if lo <= s < hi), 592, module)


# Private forms built at run time, so this file itself holds none (the export scans every published file).
DRIVE_ROOT = "D:" + "/" + "nk" + "/private/notes.md"
BACKSLASH_HOME = "D:" + "\\" + "Users" + "\\" + "alice\\x"
ESCAPED_ROOT = "D:" + "\\\\" + "nk" + "\\\\x"
WSL_ROOT = "/mnt/d/" + "nk" + "/x"
MSYS_ROOT = " /d/" + "nk" + "/x"
HOME = "/" + "home/alice/x"
TOKEN = "ghp" + "_" + "A" * 24


class PublishScanTests(unittest.TestCase):
    """The export (hstpublish.py) and the pre-push check (hstpublic.py check) refuse private content in any file."""

    def repo(self, files: dict[str, str], message: str = "base") -> Path:
        tmp = tempfile.TemporaryDirectory()
        self.addCleanup(tmp.cleanup)
        root = Path(tmp.name) / "repo"
        root.mkdir()
        for rel, text in files.items():
            (root / rel).parent.mkdir(parents=True, exist_ok=True)
            (root / rel).write_text(text, encoding="utf-8", newline="\n")
        for args in (("init", "-q", "-b", "full"), ("config", "user.email", "test@example.invalid"),
                     ("config", "user.name", "test"), ("config", "core.autocrlf", "false"), ("add", "-A"),
                     ("commit", "-q", "-m", message)):
            subprocess.run(["git", *args], cwd=root, check=True, capture_output=True)
        return root

    def test_the_patterns_find_local_roots_homes_and_tokens_but_not_placeholders(self) -> None:
        import hstpublish
        for text in (DRIVE_ROOT, BACKSLASH_HOME, ESCAPED_ROOT, WSL_ROOT, MSYS_ROOT, HOME, TOKEN, DRIVE_ROOT.lower()):
            self.assertEqual(hstpublish.private_content([("f", f"a\nsee {text}\n".encode())]), ["f:2"], text)
        for text in ("C:" + "\\Users\\<you>\\x", "C:" + "\\Users\\Default\\AppData\\Local", "C:" + "/Users/test/f.c",
                     "/home/user/x", "/mnt/c/work/x", "https://example.invalid/d/" + "nk", "%LOCALAPPDATA%\\Android",
                     "C:" + "\\\\Users\\\\..."):
            self.assertEqual(hstpublish.private_content([("f", text.encode())]), [], text)

    def test_the_export_refuses_a_file_with_a_local_path_and_writes_nothing(self) -> None:
        import hstpublish
        root = self.repo({"ORIGINS.json": "{}\n", "README.md": "public\n", "docs/notes.md": f"x\nsee {DRIVE_ROOT}\n"})
        out = root.parent / "out"
        err = io.StringIO()
        with mock.patch.object(hstpublish, "ROOT", root), \
                mock.patch.object(hstpublish.hstorigin, "classify", return_value=({}, [])), \
                mock.patch.object(sys, "argv", ["hstpublish.py", "--out", str(out)]), redirect_stderr(err):
            self.assertEqual(hstpublish.main(), 1)
        self.assertFalse(out.exists())
        self.assertIn("docs/notes.md:2", err.getvalue())
        (root / "docs/notes.md").write_text("x\nsee <private tree>/notes.md\n", encoding="utf-8", newline="\n")
        with mock.patch.object(hstpublish, "ROOT", root), \
                mock.patch.object(hstpublish.hstorigin, "classify", return_value=({}, [])), \
                mock.patch.object(sys, "argv", ["hstpublish.py", "--out", str(out)]), redirect_stdout(io.StringIO()):
            self.assertEqual(hstpublish.main(), 0)
        self.assertTrue((out / "docs/notes.md").is_file())

    def test_the_push_check_refuses_private_content_in_a_file_or_the_message(self) -> None:
        import hstpublic
        # (files, commit message, check of the commit, check of the work tree: files only)
        cases = (({"ORIGINS.json": "{}\n", "data/x.json": f'{{"p": "{ESCAPED_ROOT}"}}\n'}, "base", 1, 1),
                 ({"ORIGINS.json": "{}\n", "README.md": "public\n"}, f"notes from {WSL_ROOT}", 1, 0),
                 ({"ORIGINS.json": "{}\n", "README.md": "public\n"}, "clean", 0, 0))
        for files, message, want_commit, want_tree in cases:
            root = self.repo(files, message)
            with mock.patch.object(hstpublic, "ROOT", root), redirect_stderr(io.StringIO()):
                self.assertEqual(hstpublic.check_tree(None, "HEAD"), want_commit, (files, message))
                self.assertEqual(hstpublic.check_tree(root, None), want_tree, (files, message))

    def test_sync_refuses_a_full_checkout_with_uncommitted_changes(self) -> None:
        import hstpublic
        root = self.repo({"ORIGINS.json": "{}\n", "README.md": "public\n"})
        (root / "README.md").write_text("edited, not committed\n", encoding="utf-8")
        with mock.patch.object(hstpublic, "ROOT", root), self.assertRaises(SystemExit) as raised:
            hstpublic.sync(root.parent / "public", "m")
        self.assertIn("uncommitted changes", str(raised.exception))


if __name__ == "__main__":
    unittest.main()
