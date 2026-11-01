#!/usr/bin/env python3
"""Tests for the source linting tool (hstlint.py)."""
from __future__ import annotations

import io
import json
import sys
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hstlint as L


class NormTests(unittest.TestCase):
    def test_norm_removes_comments_and_storage_specifiers(self) -> None:
        raw = "extern static const int /* comment */ DAT_00010000;"
        self.assertEqual(L.norm(raw), "int DAT_00010000;")

    def test_norm_normalizes_whitespace_and_punctuation(self) -> None:
        raw = " int  *  FUN_00010000 ( int   a ,  char * b ) "
        self.assertEqual(L.norm(raw), "int*FUN_00010000(int,char *)")

    def test_norm_drops_parameter_names_keeping_types(self) -> None:
        self.assertEqual(L.norm("void FUN_00010000(int x, void *p, struct S *s, const char *msg)"),
                         "void FUN_00010000(int,void *,struct S *,char *)")
        self.assertEqual(L.norm("int FUN_00010000(void)"), "int FUN_00010000(void)")
        self.assertEqual(L.norm("int FUN_00010000(unsigned int count)"), "int FUN_00010000(unsigned int)")

    def test_norm_array_declarations(self) -> None:
        self.assertEqual(L.norm("extern char DAT_00010000 [ 100 ] ;"), "char DAT_00010000[100];")


class PatternTests(unittest.TestCase):
    def test_note_kinds_match_expected_provenance_comments(self) -> None:
        samples = {
            "clone": "/* Clone of src/foo/bar.c (hstclone.py tag 1.0) */",
            "corrected": "/* Ghidra pseudocode (hstgen.py tag v1) corrected by hand */",
            "generated": "/* Mechanical translation of Ghidra pseudocode (hstgen.py tag v1) */",
            "library": "/* Library source: newlib */",
            "template": "/* (template instance, hsttmpl.py tag 1.0) */",
            "hand": "/* Hand-written reconstruction */",
        }
        for kind, text in samples.items():
            matched_kind = next((k for k, rx in L.NOTE_KINDS if rx.search(text)), None)
            self.assertEqual(matched_kind, kind, f"Failed for kind {kind}")

    def test_dupcond_regex(self) -> None:
        self.assertTrue(L.DUPCOND.search("if (x > 0) {\n  if (x > 0) {\n    foo();\n  }\n}"))
        self.assertIsNone(L.DUPCOND.search("if (x > 0) {\n  if (y > 0) {\n    foo();\n  }\n}"))

    def test_self_assign_regex(self) -> None:
        self.assertTrue(L.SELF.search("x = x;"))
        self.assertTrue(L.SELF.search("  val = val ;"))
        self.assertIsNone(L.SELF.search("x = y;"))

    def test_decl_regex(self) -> None:
        code = (
            "extern int FUN_00010000(int a);\n"
            "void FUN_00010004(void);\n"
            "extern int DAT_00020000;\n"
            "extern char *PTR_00020004[10];\n"
            "return FUN_00010000;\n"
        )
        matches = [m.group(2) for m in L.DECL.finditer(code) if m.group(1).strip() not in ("return", "else", "goto")]
        self.assertEqual(matches, ["FUN_00010000", "FUN_00010004", "DAT_00020000", "PTR_00020004"])


class ProvenanceNoteTests(unittest.TestCase):
    HEADER = "/* HSTMATCH module=EBOOT.BIN start=0x00010000 end=0x00010008 symbol=FUN_00010000 lang=c99 */\n"
    HAND = "/* Hand-written reconstruction from the disassembly; exact match verified. */"

    def test_the_note_follows_the_header_or_its_one_line_annotations(self) -> None:
        self.assertEqual(L.provenance_note(self.HEADER + self.HAND + "\nint x;\n"), ("hand", self.HAND))
        pointer = "/* CSV table reader: row count (this+8); see docs/DATA-TABLES.md */\n"
        self.assertEqual(L.provenance_note(self.HEADER + pointer + self.HAND + "\n")[0], "hand")
        self.assertEqual(L.provenance_note(self.HEADER + pointer * 3 + self.HAND + "\n"),
                         (None, pointer.strip()))                   # at most two annotations come first
        self.assertEqual(L.provenance_note(self.HEADER + "/* a\n   longer comment */\n" + self.HAND + "\n")[0], None)
        self.assertEqual(L.provenance_note(self.HEADER + "int x;\n" + self.HAND + "\n"), (None, ""))   # code first
        self.assertEqual(L.provenance_note(self.HEADER.strip()), (None, ""))


class MainLintTests(unittest.TestCase):
    def setUp(self) -> None:
        self.tmp = tempfile.TemporaryDirectory()
        self.root = Path(self.tmp.name)
        self.src_dir = self.root / "src"
        self.try_dir = self.root / "try"
        self.src_dir.mkdir(parents=True)
        self.try_dir.mkdir(parents=True)

    def tearDown(self) -> None:
        self.tmp.cleanup()

    def create_source(self, rel_path: str, content: str) -> Path:
        p = self.src_dir / rel_path
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_text(content, encoding="utf-8")
        return p

    def test_main_runs_and_generates_json_report(self) -> None:
        self.create_source("mod/sample.c",
            "/* HSTMATCH module=A.BIN start=0x0 end=0x10 symbol=FUN_00010000 lang=c99 */\n"
            "/* Hand-written reconstruction */\n"
            "int FUN_00010000(int a) {\n"
            "    volatile int x = 1;\n"
            "    x = x;\n"
            "    if (a) {\n"
            "        if (a) {\n"
            "            goto done;\n"
            "        }\n"
            "    }\n"
            "done:\n"
            "    return a;\n"
            "}\n"
        )

        out_json = self.try_dir / "report.json"
        with patch.object(L.M, "ROOT", self.root), \
             patch.object(sys, "argv", ["hstlint.py", "--json", str(out_json)]):
            stdout_buf = io.StringIO()
            with patch("sys.stdout", stdout_buf):
                code = L.main()

            self.assertEqual(code, 0)
            self.assertTrue(out_json.exists())
            report = json.loads(out_json.read_text(encoding="utf-8"))
            self.assertEqual(report["sources"], 1)
            self.assertEqual(report["flags"]["volatile"], 1)
            self.assertEqual(report["flags"]["selfassign"], 1)
            self.assertEqual(report["flags"]["dupcond"], 1)
            self.assertEqual(report["flags"]["goto"], 1)

    def test_main_detects_header_and_lang_mismatch(self) -> None:
        # File with unknown note provenance comment (header finding)
        self.create_source("mod/bad_header.c",
            "/* HSTMATCH module=A.BIN start=0x0 end=0x10 symbol=FUN_00010001 lang=c99 */\n"
            "/* Unknown provenance note */\n"
            "int FUN_00010001(void) { return 0; }\n"
        )
        # .c file claiming lang=c++
        self.create_source("mod/bad_lang.c",
            "/* HSTMATCH module=A.BIN start=0x0 end=0x10 symbol=FUN_00010002 lang=c++ */\n"
            "/* Hand-written reconstruction */\n"
            "int FUN_00010002(void) { return 0; }\n"
        )

        out_json = self.try_dir / "report.json"
        with patch.object(L.M, "ROOT", self.root), \
             patch.object(sys, "argv", ["hstlint.py", "--json", str(out_json)]):
            stdout_buf = io.StringIO()
            with patch("sys.stdout", stdout_buf):
                L.main()

            report = json.loads(out_json.read_text(encoding="utf-8"))
            self.assertIn("src/mod/bad_header.c", report["files"]["header"])
            self.assertIn("src/mod/bad_lang.c", report["files"]["lang"])

    def test_main_detects_conflicting_prototypes(self) -> None:
        self.create_source("mod/f1.c",
            "/* HSTMATCH module=A.BIN start=0x0 end=0x10 symbol=FUN_00010000 lang=c99 */\n"
            "/* Hand-written reconstruction */\n"
            "extern int FUN_00020000(int a);\n"
            "int FUN_00010000(int a) { return FUN_00020000(a); }\n"
        )
        self.create_source("mod/f2.c",
            "/* HSTMATCH module=A.BIN start=0x10 end=0x20 symbol=FUN_00010010 lang=c99 */\n"
            "/* Hand-written reconstruction */\n"
            "extern int FUN_00020000(int a, int b);\n"
            "int FUN_00010010(int a) { return FUN_00020000(a, 0); }\n"
        )

        out_json = self.try_dir / "report.json"
        with patch.object(L.M, "ROOT", self.root), \
             patch.object(sys, "argv", ["hstlint.py", "--json", str(out_json)]):
            stdout_buf = io.StringIO()
            with patch("sys.stdout", stdout_buf):
                L.main()

            report = json.loads(out_json.read_text(encoding="utf-8"))
            self.assertEqual(report["conflicting_symbols"], 1)
            self.assertIn("A.BIN:FUN_00020000", report["conflicts"])

    def test_main_show_option_and_gate_failure(self) -> None:
        # Create file that fails fake-match gate
        self.create_source("mod/gate_fail.c",
            "/* HSTMATCH module=A.BIN start=0x0 end=0x10 symbol=FUN_00010000 lang=c99 */\n"
            "/* Hand-written reconstruction */\n"
            "void FUN_00010000(void) { __asm__ volatile (\"nop\"); }\n"
        )

        out_json = self.try_dir / "report.json"
        with patch.object(L.M, "ROOT", self.root), \
             patch.object(sys, "argv", ["hstlint.py", "--json", str(out_json), "--show", "gate"]):
            stdout_buf = io.StringIO()
            with patch("sys.stdout", stdout_buf):
                code = L.main()

            self.assertEqual(code, 1)
            output = stdout_buf.getvalue()
            self.assertIn("src/mod/gate_fail.c", output)
            self.assertIn("GATE: 1 source(s) fail the fake-match gate", output)


if __name__ == "__main__":
    unittest.main()
