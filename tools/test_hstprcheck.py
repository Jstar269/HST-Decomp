#!/usr/bin/env python3
"""Tests for the contribution checks (hstprcheck.py), on a throwaway git repository."""
from __future__ import annotations

import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hstprcheck as P

HEADER = "/* HSTMATCH module=EBOOT.BIN start=0x00010000 end=0x00010008 symbol=FUN_00010000 lang=c99 */\n"
NOTE = "/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */\n"
GOOD = HEADER + NOTE + "int FUN_00010000(void) { return 0; }\n"


class PrCheckTests(unittest.TestCase):
    def setUp(self) -> None:
        self.tmp = tempfile.TemporaryDirectory()
        self.repo = Path(self.tmp.name)
        self.git("init", "-q", "-b", "main")
        self.git("config", "user.email", "test@example.invalid")
        self.git("config", "user.name", "test")
        self.git("config", "core.autocrlf", "false")
        self.write("symbols/EBOOT.BIN.txt", "a = 0x00010000;\n")
        self.write("src/eboot/FUN_00010000.c", GOOD)
        self.commit("base")
        self.git("branch", "base")
        self.git("checkout", "-q", "-b", "topic")

    def tearDown(self) -> None:
        self.tmp.cleanup()

    def git(self, *args: str) -> None:
        subprocess.run(["git", *args], cwd=self.repo, check=True, capture_output=True)

    def write(self, rel: str, data: str | bytes) -> None:
        path = self.repo / rel
        path.parent.mkdir(parents=True, exist_ok=True)
        (path.write_bytes if isinstance(data, bytes) else lambda d: path.write_text(d, encoding="utf-8", newline="\n"))(data)

    def commit(self, message: str) -> None:
        self.git("add", "-A")
        self.git("commit", "-q", "-m", message)

    def refusals(self) -> list[str]:
        return P.check(self.repo, "base")

    def test_a_clean_source_passes(self) -> None:
        self.write("src/eboot/FUN_00010010.c", GOOD.replace("00010000", "00010010").replace("0x00010008", "0x00010018"))
        self.write("symbols/EBOOT.BIN.txt", "a = 0x00010000;\nb = 0x00010010;   # appended\n")
        self.commit("match")
        self.assertEqual(self.refusals(), [])

    def test_binaries_and_never_versioned_places(self) -> None:
        self.write("inputs/EBOOT.BIN", b"\x7fELF\0\0")
        self.write("docs/notes.txt", b"text\0more")
        self.write("try/me/scratch.c", "int x;\n")
        self.write("hst-config.json", "{}\n")
        self.commit("bad")
        kinds = sorted(r.split()[0] for r in self.refusals())
        self.assertEqual(kinds, ["binary", "binary", "place", "place"])

    def test_private_paths_secrets_and_dumps(self) -> None:
        self.write("docs/a.md", "see C:" + "\\Users\\someone\\game\n")   # split: this file is checked too
        self.write("docs/b.md", "token ghp_" + "a" * 30 + "\n")
        self.write("docs/c.md", "placeholders are fine: C:\\Users\\<you>\\x and /home/<user>/x\n")
        self.write("wip/x.txt", "".join(f"{0x10000 + 4 * i:08x}:  27bdfff0  addiu sp,sp,-16\n" for i in range(20)))
        self.commit("bad")
        got = self.refusals()
        self.assertEqual(sorted(r.split()[0] + " " + r.split()[1] for r in got),
                         ["dump wip/x.txt:", "private docs/a.md:", "private docs/b.md:"])

    def test_symbols_are_append_only(self) -> None:
        self.write("symbols/EBOOT.BIN.txt", "a = 0x00010004;\n")
        self.commit("edit")
        self.assertEqual([r.split()[0] for r in self.refusals()], ["symbols"])

    def test_header_note_and_gate(self) -> None:
        self.write("src/eboot/no_header.c", "int f(void) { return 0; }\n")
        self.write("src/eboot/no_note.c", HEADER + "/* written somehow */\nint FUN_00010000(void) { return 0; }\n")
        self.write("src/eboot/asm.c", HEADER + NOTE + 'void FUN_00010000(void) { asm("addiu $sp,$sp,-16"); }\n')
        self.commit("bad")
        got = sorted(r.split()[0] + " " + r.split()[1] for r in self.refusals())
        self.assertEqual(got, ["gate src/eboot/asm.c:", "header src/eboot/no_header.c:", "header src/eboot/no_note.c:"])

    def test_note_rules_agree_with_published_sources(self) -> None:
        body = "int FUN_00010000(void) { return 0; }\n"
        # a docs/DATA-TABLES.md pointer between the header and the note, as 44 published sources carry
        self.write("src/eboot/annotated.c", HEADER + "/* Table reader; see docs/DATA-TABLES.md */\n" + NOTE + body)
        # a near miss needs the header only, with trailing comments (CONTRIBUTING.md, "Near misses"), as harvests write
        self.write("wip/near_00010000.c", HEADER + body + "// near miss from lane x (unreviewed)\n")
        self.commit("ok")
        self.assertEqual(self.refusals(), [])
        self.write("wip/twice_00010000.c", HEADER + HEADER + body)
        self.write("src/eboot/twice.c", HEADER + NOTE + HEADER + body)
        self.write("src/eboot/late_note.c", HEADER + "/* a */\n/* b */\n/* c */\n" + NOTE + body)   # too far down
        self.commit("bad")
        got = sorted(r.split()[0] + " " + r.split()[1] for r in self.refusals())
        self.assertEqual(got, ["header src/eboot/late_note.c:", "header src/eboot/twice.c:",
                               "header wip/twice_00010000.c:"])

    def test_the_compiler_log_is_judged_by_what_is_appended(self) -> None:
        big = "".join(f"- 2026-10-{i % 28 + 1:02d} 0x{i:08x}: a lesson that is long enough to count {'x' * 60}\n"
                      for i in range(12000))                            # over 1 MiB, like the real log
        self.git("checkout", "-q", "base")                                # the big log is already on the base
        self.write("docs/MWCC-NOTES.md", big)
        self.commit("log")
        self.git("checkout", "-q", "topic")
        self.git("merge", "-q", "base")
        self.write("docs/MWCC-NOTES.md", big + "- 2026-11-01 0x00010000: one new lesson\n")
        self.commit("append")
        self.assertEqual(self.refusals(), [])                             # a one-line append to a big log is fine
        self.write("docs/MWCC-NOTES.md", big.replace("0x00000005", "0x00000006", 1) + "- one new lesson\n")
        self.commit("edit")
        self.assertEqual([r.split()[0] for r in self.refusals()], ["log"])
        self.write("docs/README.md", big)
        self.commit("big doc")
        self.assertIn("size", [r.split()[0] for r in self.refusals()])   # other files keep the whole-file limit


    def test_a_local_path_in_the_compiler_log_may_be_redacted(self) -> None:
        log = ("- 2026-10-01 0x00010000: a lesson\n"
               "- 2026-10-02: measure only from `C:/nk/private/hst-decomp/try/gen-aN/` from now on\n")
        self.git("checkout", "-q", "base")
        self.write("docs/MWCC-NOTES.md", log)
        self.commit("log with a local path")
        self.git("checkout", "-q", "topic")
        self.git("merge", "-q", "base")
        self.write("docs/MWCC-NOTES.md", log.replace("C:/nk/private", "<private tree>") + "- 2026-11-02: appended\n")
        self.commit("redact and append")
        self.assertEqual(self.refusals(), [])                           # a redaction is the one allowed change
        self.write("docs/MWCC-NOTES.md", log.replace("C:/nk/private/hst-decomp/try", "<private tree>/x"))
        self.commit("redact and rewrite")
        self.assertEqual([r.split()[0] for r in self.refusals()], ["log"])  # the rest of the line must stay

    def test_a_redaction_replaces_only_local_path_roots(self) -> None:
        R = P.is_redaction
        self.assertTrue(R(r"image `C:\nk\private\a\b.elf` here", r"image `<private tree>\a\b.elf` here"))
        self.assertTrue(R("from `/mnt/c/nk/x/try/` and `/home/user/y`", "from `<private tree>/try/` and `<private tree>/y`"))
        self.assertFalse(R("from `C:/nk/x/try/`", "from `<private tree>/try/` (edited)"))
        self.assertFalse(R("value 0x10 here", "value <private tree> here"))              # nothing local was there
        self.assertFalse(R("a `C:/nk/x` and `C:/nk/y`", "a `<private tree>` and `C:/nk/y`"))  # a path is left
        self.assertFalse(R("a lesson", "a different lesson"))
        self.assertEqual(P.unredacted(["a C:/nk/x b", "c"], ["a <private tree> b"]), ["c"])

if __name__ == "__main__":
    unittest.main()
