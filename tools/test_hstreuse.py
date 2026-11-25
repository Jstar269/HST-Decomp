#!/usr/bin/env python3
"""Tests for the newlib note parsing of the licensing tool (hstreuse.py)."""
from __future__ import annotations

import io
import subprocess
import sys
import tempfile
import unittest
from contextlib import redirect_stdout
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


GAME = "/* HSTMATCH module=EBOOT.BIN start=0x00100000 end=0x00100008 symbol=FUN_00100000 lang=c99 */\n"
SUN_NOTICE = ("/*\n * ====================================================\n"
              " * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.\n *\n"
              " * Developed at SunPro, a Sun Microsystems, Inc. business.\n"
              " * Permission to use, copy, modify, and distribute this\n"
              " * software is freely granted, provided that this notice\n * is preserved.\n */\n")
STRCPY = ("/* HSTMATCH module=EBOOT.BIN start=0x000149a8 end=0x00014a58 symbol=strcpy lang=c99 cc=gcc171-O2 */\n"
          "/* Library source: newlib 1.14.0 libc_string/strcpy.c function strcpy, preprocessed (comments kept). */\n"
          "char *strcpy(char *d, const char *s);\n")
BASE_TOML = """version = 1

[[annotations]]
path = ["src/**", "wip/**"]
SPDX-FileCopyrightText = "2026 Jstar269 and contributors"
SPDX-License-Identifier = "CC0-1.0"

[[annotations]]
path = ["REUSE.toml", ".gitignore", "third_party/newlib/README.md"]
SPDX-FileCopyrightText = "2026 Jstar269 and contributors"
SPDX-License-Identifier = "MIT"

{begin}
{end}
"""


class CheckCommandTests(unittest.TestCase):
    """`hstreuse.py check` on a throwaway git repository (no game data: synthetic sources only)."""

    def setUp(self) -> None:
        self.tmp = tempfile.TemporaryDirectory()
        self.root = Path(self.tmp.name)
        subprocess.run(["git", "init", "-q"], cwd=self.root, check=True)
        (self.root / "LICENSES").mkdir()
        for lid in ("CC0-1.0", "MIT", "LicenseRef-Newlib-Red-Hat", "LicenseRef-Newlib-Sun-fdlibm",
                    "LicenseRef-Newlib-COPYING-NEWLIB"):
            (self.root / "LICENSES" / f"{lid}.txt").write_text(lid + "\n", encoding="utf-8")
        (self.root / "third_party" / "newlib").mkdir(parents=True)
        (self.root / "third_party" / "newlib" / "README.md").write_text(f"{R.BEGIN_MD}\n{R.END_MD}\n", encoding="utf-8")
        (self.root / "REUSE.toml").write_text(BASE_TOML.format(begin=R.BEGIN_TOML, end=R.END_TOML), encoding="utf-8")
        (self.root / ".gitignore").write_text("", encoding="utf-8")
        classify = hstorigin.classify
        self.patches = [patch.object(R, "ROOT", self.root), patch.object(R, "REUSE_TOML", self.root / "REUSE.toml"),
                        patch.object(R, "LICENSES", self.root / "LICENSES"),
                        patch.object(R, "README", self.root / "third_party" / "newlib" / "README.md"),
                        patch.object(hstorigin, "classify", lambda root=self.root: classify(root)),
                        patch.object(hstorigin, "rel", lambda p, root=self.root: p.relative_to(root).as_posix())]
        for p in self.patches:
            p.start()

    def tearDown(self) -> None:
        for p in self.patches:
            p.stop()
        self.tmp.cleanup()

    def add(self, rel: str, text: str, track: bool = True) -> None:
        path = self.root / rel
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(text, encoding="utf-8")
        if track:
            subprocess.run(["git", "add", "--", rel], cwd=self.root, check=True)

    def check(self, regenerate: bool = True) -> tuple[int, str]:
        out = io.StringIO()
        with redirect_stdout(out):
            if regenerate:
                R.newlib_command(write=True)
            subprocess.run(["git", "add", "-A", "--", "REUSE.toml", "LICENSES", "third_party", ".gitignore"],
                           cwd=self.root, check=True)
            rc = R.check_command()
        return rc, out.getvalue()

    def test_fixture_is_compliant(self) -> None:
        self.add("src/eboot/FUN_00100000.c", GAME + "int f(void) { return 0; }\n")
        self.add("src/eboot/strcpy_000149a8.c", STRCPY)
        rc, out = self.check()
        self.assertEqual(rc, 0, out)

    def test_a_cc0_source_with_a_newlib_notice_is_a_problem(self) -> None:
        # upstream fdlibm text without the `Library source: newlib` note: classified game, so REUSE.toml dedicates it
        # under CC0-1.0, although the notice in the file says Sun's terms apply
        self.add("src/eboot/FUN_00100000.c", GAME + SUN_NOTICE + "float f(float x) { return x; }\n")
        rc, out = self.check()
        self.assertEqual(rc, 1, out)
        self.assertIn("src/eboot/FUN_00100000.c", out)

    def test_a_later_annotation_cannot_replace_the_generated_newlib_licence(self) -> None:
        self.add("src/eboot/strcpy_000149a8.c", STRCPY)
        self.check()
        for expression in ("LicenseRef-Newlib-COPYING-NEWLIB", "CC0-1.0 OR LicenseRef-Newlib-Red-Hat"):
            toml = (self.root / "REUSE.toml").read_text(encoding="utf-8").split(R.END_TOML)[0] + R.END_TOML + "\n"
            (self.root / "REUSE.toml").write_text(
                toml + f'\n[[annotations]]\npath = "src/eboot/strcpy_000149a8.c"\nprecedence = "override"\n'
                f'SPDX-FileCopyrightText = "x"\nSPDX-License-Identifier = "{expression}"\n', encoding="utf-8")
            rc, out = self.check(regenerate=False)
            self.assertEqual(rc, 1, f"{expression}: {out}")
            self.assertIn("src/eboot/strcpy_000149a8.c", out)

    def test_untracked_scratch_sources_are_left_out(self) -> None:
        # a scratch near miss under wip/ that git does not track must not change the generated block or the verdict
        # (lbpreuse.py has done this since its port: tracked_classes)
        self.add("src/eboot/strcpy_000149a8.c", STRCPY)
        self.check()
        self.add("wip/strcpy_scratch.c", STRCPY, track=False)
        rc, out = self.check(regenerate=False)
        self.assertEqual(rc, 0, out)
        with redirect_stdout(io.StringIO()):
            R.newlib_command(write=True)
        self.assertNotIn("wip/strcpy_scratch.c", (self.root / "REUSE.toml").read_text(encoding="utf-8"))

    def test_an_annotation_without_a_licence_is_reported_not_a_crash(self) -> None:
        self.add("src/eboot/FUN_00100000.c", GAME + "int f(void) { return 0; }\n")
        self.check()
        toml = (self.root / "REUSE.toml").read_text(encoding="utf-8")
        (self.root / "REUSE.toml").write_text(
            toml + '\n[[annotations]]\npath = "src/eboot/**"\nSPDX-FileCopyrightText = "x"\n'
            'SPDX-Licence-Identifier = "CC0-1.0"\n', encoding="utf-8")
        rc, out = self.check(regenerate=False)
        self.assertEqual(rc, 1, out)
        self.assertIn("lacks SPDX-License-Identifier", out)


if __name__ == "__main__":
    unittest.main()
