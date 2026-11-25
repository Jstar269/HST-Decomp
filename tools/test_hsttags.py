#!/usr/bin/env python3
"""Tests for the --tag of the tools that write into src/ (hstclone.py, hstgen.py, hsttmpl.py): a tag that the
provenance note cannot carry, as tools/hstlint.py reads it, is refused before any work. Synthetic, game-free."""
from __future__ import annotations

import io
import sys
import tempfile
import unittest
from contextlib import redirect_stderr, redirect_stdout
from pathlib import Path
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hstclone as C
import hstgen as G
import hstlint as L
import hsttmpl as T

TOOLS = (("hstclone.py", C), ("hstgen.py", G), ("hsttmpl.py", T))


class TagTests(unittest.TestCase):
    def run_main(self, module, name: str, tag: str, root: Path):
        err = io.StringIO()
        with patch.object(sys, "argv", [name, "--tag", tag, "--dry-run"]), patch.object(module.M, "ROOT", root), \
                patch.dict(module.M.__dict__, {"CAMPAIGN": root}), redirect_stdout(io.StringIO()), \
                redirect_stderr(err):
            try:
                module.main()
            except SystemExit as stop:
                return stop.code, err.getvalue()
            except Exception as exc:                # main's tools go on past the tag and fail on the empty tree
                return type(exc).__name__, err.getvalue()
        return 0, err.getvalue()

    def test_a_tag_the_note_cannot_carry_is_refused(self) -> None:
        for name, module in TOOLS:
            for tag in ("c 1", "c1+c2", "r5:a", "a(b)", "..", "x/../../y"):
                with self.subTest(tool=name, tag=tag), tempfile.TemporaryDirectory() as tmp:
                    code, err = self.run_main(module, name, tag, Path(tmp))
                    self.assertEqual(code, 2)
                    self.assertIn("--tag", err)
                    self.assertEqual(list(Path(tmp).iterdir()), [])

    def test_every_accepted_tag_reads_back_from_the_notes(self) -> None:
        notes = {
            "clone": "Clone of src/eboot/a.c (hstclone.py tag {tag}): addresses substituted; exact match verified",
            "generated": "Mechanical translation of Ghidra pseudocode (hstgen.py tag {tag}); exact match verified",
            "template": "Container growth policy for a 4-byte element type (template instance, hsttmpl.py tag {tag}):",
        }
        for tag in ("c1", "r5-a", "t1.2", "gen_7"):
            self.assertEqual(C.tag_arg(tag), tag)
            for kind, note in notes.items():
                with self.subTest(tag=tag, kind=kind):
                    text = "/* HSTMATCH module=EBOOT.BIN start=0x1 end=0x2 symbol=f */\n/* " + note.format(tag=tag) + " */\n"
                    self.assertEqual(L.provenance_note(text)[0], kind)


if __name__ == "__main__":
    unittest.main()
