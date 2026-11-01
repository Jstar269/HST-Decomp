#!/usr/bin/env python3
"""Tests for the newlib note parsing of the licensing tool (hstreuse.py)."""
from __future__ import annotations

import sys
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hstorigin
import hstreuse as R

HEADER = "/* HSTMATCH module=EBOOT.BIN start=0x0000f040 end=0x0000f13c symbol=fflush lang=c99 cc=gcc171-O2 */\n"


class AdaptedNoteTests(unittest.TestCase):
    def setUp(self) -> None:
        self.tmp = tempfile.TemporaryDirectory()
        self.root = Path(self.tmp.name)
        (self.root / "src" / "eboot").mkdir(parents=True)
        (self.root / "src" / "sharing_main").mkdir(parents=True)
        self.patches = [patch.object(R, "ROOT", self.root),
                        patch.object(hstorigin, "rel", lambda p, root=self.root: p.relative_to(root).as_posix())]
        for p in self.patches:
            p.start()

    def tearDown(self) -> None:
        for p in self.patches:
            p.stop()
        self.tmp.cleanup()

    def write(self, rel: str, text: str) -> Path:
        path = self.root / rel
        path.write_text(text, encoding="utf-8")
        return path

    def test_libc_note_without_release_takes_the_upstream_notice(self) -> None:
        # the form that stopped `hstreuse.py check` (2026-11-01): a two-word libc directory, no release, and no
        # copyright line in the source itself
        e = R.Entry(self.write("src/eboot/FUN_0000f040.c", HEADER + "/* Library source: newlib libc stdio fflush.c "
                               "function fflush (BSD-derived; Regents of the University of California, BSD licence), "
                               "adapted by hand from the newlib source; exact match verified. */\nint fflush(void);\n"))
        self.assertEqual((e.kind, e.ver, e.function, e.upstream), ("hand-adapted", None, "fflush", "libc/stdio/fflush.c"))
        self.assertEqual(e.expression, "LicenseRef-Newlib-UCB-Advertising-Notice")
        self.assertIn("Copyright (c) 1990, 2006 The Regents of the University of California", e.statements)

    def test_libm_note_and_unknown_file(self) -> None:
        e = R.Entry(self.write("src/eboot/FUN_0000b16c.c", HEADER + "/* Library source: newlib libm s_fabs.c function "
                               "fabs (fdlibm-derived), adapted by hand. */\n"))
        self.assertEqual((e.upstream, e.expression), ("libm/math/s_fabs.c", "LicenseRef-Newlib-Sun-fdlibm"))
        with self.assertRaisesRegex(SystemExit, "no ADAPTED_NOTICES entry"):
            R.Entry(self.write("src/eboot/FUN_00000010.c", HEADER + "/* Library source: newlib libc stdio fopen.c "
                               "function fopen, adapted by hand. */\n"))

    def test_clone_without_note_inherits_the_exemplar(self) -> None:
        self.write("src/eboot/FUN_000133f4.c", HEADER + "/* Library source: newlib libc stdlib rand.c function srand "
                   "(newlib, Red Hat / Cygnus licence), adapted by hand. */\n")
        e = R.Entry(self.write("src/sharing_main/FUN_000133f4_000133f4.c", HEADER + "/* Clone of "
                               "src/eboot/FUN_000133f4.c (hstclone.py tag a2c3s): addresses substituted. */\n"
                               "/* Hand-written reconstruction from the disassembly. */\n"))
        self.assertEqual((e.kind, e.function, e.upstream), ("clone", "srand", "libc/stdlib/rand.c"))
        self.assertEqual(e.expression, "LicenseRef-Newlib-Red-Hat")

    def test_unreadable_note_still_stops(self) -> None:
        with self.assertRaisesRegex(SystemExit, "cannot read the newlib note"):
            R.Entry(self.write("src/eboot/FUN_00000020.c", HEADER + "/* Library source: something else */\n"))


if __name__ == "__main__":
    unittest.main()
