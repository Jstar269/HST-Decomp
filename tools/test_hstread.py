#!/usr/bin/env python3
"""Tests for the readability tool (hstread.py)."""
from __future__ import annotations

import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hstread as R

HEAD = "/* HSTMATCH module=EBOOT.BIN start=0x00010000 end=0x00010020 symbol=FUN_00010000 lang=c99 */\n"
NOTE = "/* Mechanical translation of Ghidra pseudocode (hstgen.py tag t1); exact match verified. */\n"
BLOCK = ("typedef unsigned char undefined;\ntypedef unsigned char bool;\ntypedef unsigned int uint;\n"
         "typedef unsigned int undefined4;\ntypedef short undefined2;\ntypedef int code();\n#define true 1\n"
         "#define false 0\n#define ABS(x) ((x) < 0 ? -(x) : (x))\n")


class MeasureTests(unittest.TestCase):
    def test_counts_each_kind(self) -> None:
        text = HEAD + NOTE + BLOCK + ("void FUN_00010000(int param_1)\n{\n  uint uVar1;\n"
                                      "  uVar1 = *(uint *)(param_1 + 0x14);\n  FUN_00020000(uVar1, DAT_00030000);\n}\n")
        m = R.measure(text)
        self.assertEqual(m["addr"], 3)
        self.assertEqual(m["ghidra"], 5)       # param_1 twice, uVar1 three times
        self.assertEqual(m["offset"], 1)
        self.assertEqual(m["boiler"], 9)
        self.assertEqual(m["score"], 3 + 5 + 2 + 2)

    def test_comments_do_not_count(self) -> None:
        self.assertEqual(R.measure(HEAD + NOTE + "/* FUN_00010000 param_1 */\nint f(void) { return 0; }\n")["score"], 0)


class TidyTests(unittest.TestCase):
    def test_block_becomes_include_and_types_are_renamed(self) -> None:
        text = HEAD + NOTE + BLOCK + "\nvoid FUN_00010000(uint a, undefined2 b)\n{\n  if (a) {\n    return;\n  }\n  return;\n}\n"
        new, why = R.tidy_text(text)
        self.assertEqual(why, "tidied")
        self.assertEqual(new, HEAD + NOTE + '#include "hst/types.h"\n\nvoid FUN_00010000(u32 a, s16 b)\n{\n'
                                            "  if (a) {\n    return;\n  }\n}\n")

    def test_used_code_and_abs_lines_stay(self) -> None:
        text = HEAD + NOTE + BLOCK + "\nint FUN_00010000(int a)\n{\n  return ABS((*(code *)a)());\n}\n"
        new, _ = R.tidy_text(text)
        self.assertIn("typedef int code();\n", new)
        self.assertIn("#define ABS(x)", new)
        self.assertNotIn("#include", new)      # nothing from the header is used

    def test_cpp_keeps_used_true_define(self) -> None:
        head = HEAD.replace("lang=c99", "lang=c++")
        text = head + NOTE + "typedef unsigned int uint;\n#define true 1\n#define false 0\n\nint f(uint a) { return true; }\n"
        new, _ = R.tidy_text(text)
        self.assertIn("#define true 1\n", new)
        self.assertNotIn("#define false", new)
        self.assertIn("int f(u32 a)", new)

    def test_skips(self) -> None:
        self.assertEqual(R.tidy_text(HEAD.replace(" lang", " cc=gcc171-O2 lang") + NOTE + BLOCK)[1], "cc= profile")
        self.assertEqual(R.tidy_text(HEAD + NOTE + BLOCK + "int u8;\n")[1], "uses a new name")
        self.assertEqual(R.tidy_text(HEAD + NOTE + "typedef float undefined4;\n")[1], "undefined4 is float")
        self.assertEqual(R.tidy_text(HEAD + NOTE + "int f(void) { return 0; }\n")[1], "no typedef block")


    def test_crlf_source_keeps_crlf_and_is_tidied_like_lf(self) -> None:
        text = HEAD + NOTE + BLOCK + "\n\n\nvoid FUN_00010000(uint a)\n{\n  if (a) {\n    a = 0;\n  }\n  return;\n}\n"
        lf, _ = R.tidy_text(text)
        crlf, why = R.tidy_text(text.replace("\n", "\r\n"))
        self.assertEqual(why, "tidied")
        self.assertEqual(crlf, lf.replace("\n", "\r\n"))
        self.assertNotIn("return;", crlf)

    def test_tidy_command_writes_crlf_back(self) -> None:
        import argparse
        import contextlib
        import io
        import tempfile
        text = (HEAD + NOTE + BLOCK + "\nvoid FUN_00010000(uint a)\n{\n  return;\n}\n").replace("\n", "\r\n")
        with tempfile.TemporaryDirectory() as d:
            path = Path(d) / "FUN_00010000.c"
            path.write_bytes(text.encode("utf-8"))
            with contextlib.redirect_stderr(io.StringIO()):
                R.cmd_tidy(argparse.Namespace(paths=[path], write=True, list=False))
            data = path.read_bytes()
        self.assertIn(b"u32 a", data)
        self.assertEqual(data.count(b"\n"), data.count(b"\r\n"))


class PortTests(unittest.TestCase):
    E_OLD = HEAD + NOTE + "/* calls FUN_00020000 */\nvoid FUN_00010000(int param_1)\n{\n  FUN_00020000(param_1 + 0x14, DAT_00030000);\n}\n"
    S_HEAD = "/* HSTMATCH module=hstp_trans_main.prx start=0x00011000 end=0x00011020 symbol=FUN_00011000 lang=c99 */\n"
    S_OLD = S_HEAD + NOTE + "/* calls FUN_00021000 */\nvoid FUN_00011000(int param_1)\n{\n  FUN_00021000(param_1 + 0x14, DAT_00031000);\n}\n"

    def test_address_map(self) -> None:
        self.assertEqual(R.address_map(self.E_OLD, self.S_OLD),
                         {"FUN_00020000": "FUN_00021000", "FUN_00010000": "FUN_00011000", "DAT_00030000": "DAT_00031000"})
        self.assertIsNone(R.address_map(self.E_OLD, self.S_OLD.replace("0x14", "0x18")))
        self.assertEqual(R.twin_key(self.E_OLD), R.twin_key(self.S_OLD))

    def test_port_keeps_twin_header_and_maps_addresses(self) -> None:
        note2 = "/* Ghidra pseudocode (hstgen.py tag t1) corrected by hand: names; exact match verified. */\n"
        e_new = HEAD + note2 + "/* calls FUN_00020000 */\nvoid FUN_00010000(int self)\n{\n  FUN_00020000(self + 0x14, DAT_00030000);\n}\n"
        out = R.port_text(self.E_OLD, e_new, self.S_OLD)
        self.assertEqual(out, self.S_HEAD + note2 + "/* calls FUN_00021000 */\nvoid FUN_00011000(int self)\n{\n"
                                                    "  FUN_00021000(self + 0x14, DAT_00031000);\n}\n")

    def test_port_follows_a_rename_and_keeps_clone_note(self) -> None:
        clone_note = "/* Clone of src/eboot/FUN_00010000.c (hstclone.py tag c1): addresses substituted. */\n"
        s_old = self.S_OLD.replace(NOTE, clone_note)
        e_new = HEAD.replace("symbol=FUN_00010000", "symbol=reset") + NOTE.replace("t1", "t2") + \
            "/* calls FUN_00020000 */\nvoid reset(int param_1)\n{\n  FUN_00020000(param_1 + 0x14, DAT_00030000);\n}\n"
        out = R.port_text(self.E_OLD, e_new, s_old)
        self.assertIn("symbol=reset ", out)
        self.assertIn(clone_note, out)
        self.assertIn("void reset(int param_1)", out)


    def test_port_keeps_twin_note_when_only_the_exemplar_line_endings_differ(self) -> None:
        # the exemplar at the base commit is CRLF; the edited copy may read back with other line endings
        twin_note = "/* Sharing twin corrected by hand; exact match verified. */\n"
        s_old = self.S_OLD.replace(NOTE, twin_note)
        e_new = self.E_OLD.replace("param_1", "self")
        out = R.port_text(self.E_OLD.replace("\n", "\r\n"), e_new, s_old)
        self.assertEqual(out.split("\n")[1], twin_note.rstrip("\n"))

    def test_port_keeps_the_twin_line_endings(self) -> None:
        e_new = self.E_OLD.replace("param_1", "self")
        lf = R.port_text(self.E_OLD, e_new, self.S_OLD)
        self.assertEqual(R.port_text(self.E_OLD, e_new, self.S_OLD.replace("\n", "\r\n")), lf.replace("\n", "\r\n"))
        crlf_exemplar = R.port_text(self.E_OLD.replace("\n", "\r\n"), e_new.replace("\n", "\r\n"), self.S_OLD)
        self.assertEqual(crlf_exemplar, lf)


class TrueTwinTests(unittest.TestCase):
    def test_family_members_are_not_twins(self) -> None:
        import tempfile
        with tempfile.TemporaryDirectory() as d:
            lane = Path(d)
            plain = "/* h */\n/* note */\n"
            files = {"src/eboot/a.c": plain, "src/eboot/b.c": plain,
                     "src/sharing_main/c.c": "/* h */\n/* Clone of src/eboot/a.c (hstclone.py tag c1) */\n",
                     "src/sharing_main/d.c": plain}
            for rel, text in files.items():
                (lane / rel).parent.mkdir(parents=True, exist_ok=True)
                (lane / rel).write_text(text, encoding="utf-8")
            group = sorted(files)
            self.assertEqual(R.true_twins("src/eboot/a.c", group, lane), ["src/sharing_main/c.c"])
            self.assertEqual(R.true_twins("src/eboot/b.c", group, lane), [])     # two eboot members: ambiguous
            self.assertEqual(R.true_twins("src/eboot/a.c", ["src/eboot/a.c", "src/sharing_main/d.c"], lane),
                             ["src/sharing_main/d.c"])


if __name__ == "__main__":
    unittest.main()
