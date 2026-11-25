#!/usr/bin/env python3
"""Tests for the contribution checks (hstprcheck.py), on a throwaway git repository."""
from __future__ import annotations

import os
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

    def test_unusual_file_names_are_checked_like_any_other(self) -> None:
        leaked = "token ghp_" + "a" * 30 + "\n"
        self.write("docs/caf\u00e9.md", leaked)                                   # C-quoted by git without -z
        self.write('docs/say "hi".md', leaked)
        self.write("docs/big\u00e9.txt", ("y" * 99 + "\n") * 12000)   # over 1 MiB
        self.write("src/eboot/caf\u00e9.c", "int f(void) { return 0; }\n")       # no header
        self.write(":(exclude)*", leaked)                                         # pathspec magic as a name
        self.write("docs/[ab].md", leaked)                                        # a glob as a name
        (self.repo / os.fsdecode(b"docs/raw\xff.md")).write_bytes(leaked.encode())   # not UTF-8
        self.commit("names")
        got = sorted(r.split()[0] for r in self.refusals())
        self.assertEqual(got, ["header", "name", "private", "private", "private", "private", "size"])
        for rel in (os.fsdecode(b"docs/raw\xff.md"), "docs/big\u00e9.txt", "src/eboot/caf\u00e9.c"):
            (self.repo / rel).unlink()
        self.write("docs/caf\u00e9.md", "a clean note\n")
        self.write('docs/say "hi".md', "a clean note\n")
        self.write("docs/[ab].md", "a clean note\n")
        self.write(":(exclude)*", "a clean note\n")
        self.commit("clean")
        self.assertEqual(self.refusals(), [])                                     # a clean file with such a name passes

    def test_symbols_are_append_only(self) -> None:
        self.write("symbols/EBOOT.BIN.txt", "a = 0x00010004;\n")
        self.commit("edit")
        self.assertEqual([r.split()[0] for r in self.refusals()], ["symbols"])

    def test_lines_that_look_like_diff_headers_or_hold_other_separators_are_checked(self) -> None:
        listing = [f"{0x10000 + 4 * i:08x}:  27bdfff0  addiu sp,sp,-16" for i in range(20)]
        self.write("docs/plus.txt", "".join(f"++ {line}\n" for line in listing))      # "+++ ..." in the diff
        self.write("docs/cr.txt", "".join(f"{line}\r" for line in listing))           # one line to git
        self.write("docs/ff.md", "page\x0ctoken ghp_" + "a" * 30 + "\n")              # after a form feed
        self.commit("hidden")
        got = sorted(r.split()[0] + " " + r.split()[1] for r in self.refusals())
        self.assertEqual(got, ["dump docs/cr.txt:", "dump docs/plus.txt:", "private docs/ff.md:"])

    def test_a_log_or_symbols_line_that_starts_with_two_dashes_is_append_only_too(self) -> None:
        self.git("checkout", "-q", "base")
        self.write("docs/MWCC-NOTES.md", "- 2026-10-01: a lesson\n-- a continuation\n---\n")
        self.write("symbols/EBOOT.BIN.txt", "a = 0x00010000;\n-- b = 0x00010004;\n")
        self.commit("log")
        self.git("checkout", "-q", "topic")
        self.git("merge", "-q", "base")
        self.write("docs/MWCC-NOTES.md", "- 2026-10-01: a lesson\n- 2026-11-01: appended\n")   # "---" lines deleted
        self.write("symbols/EBOOT.BIN.txt", "a = 0x00010000;\n-- b = 0x00099999;\n")
        self.commit("rewrite")
        self.assertEqual(sorted(r.split()[0] for r in self.refusals()), ["log", "symbols"])

    def test_append_only_files_cannot_be_deleted_renamed_or_replaced(self) -> None:
        self.git("checkout", "-q", "base")
        self.write("docs/MWCC-NOTES.md", "- 2026-10-01: a lesson\n")
        self.write("symbols/hstp_trans_main.prx.txt", "c = 0x00000010;\n")
        self.commit("log")
        self.git("checkout", "-q", "topic")
        self.git("merge", "-q", "base")
        self.git("mv", "symbols/EBOOT.BIN.txt", "symbols/EBOOT.txt")                 # a rename that edits a line
        self.write("symbols/EBOOT.txt", "a = 0x00099999;\n")
        (self.repo / "symbols/hstp_trans_main.prx.txt").unlink()                     # a deletion
        (self.repo / "docs/MWCC-NOTES.md").unlink()                                  # the log, replaced by a link
        (self.repo / "docs/MWCC-NOTES.md").symlink_to("../README.md")
        self.commit("rewrite")
        got = sorted(r.split()[0] + " " + r.split()[1] for r in self.refusals())
        self.assertEqual(got, ["log docs/MWCC-NOTES.md:", "symbols symbols/EBOOT.BIN.txt:",
                               "symbols symbols/hstp_trans_main.prx.txt:"])
        self.git("checkout", "-q", "base", "--", "symbols", "docs")
        self.git("rm", "-q", "--cached", "symbols/EBOOT.txt")
        (self.repo / "symbols/EBOOT.txt").unlink()
        self.write("symbols/new_module.prx.txt", "d = 0x00000020;\n")                # a new map is an append
        (self.repo / "src/eboot/FUN_00010000.c").unlink()                            # other deletions are fine
        self.commit("restore")
        self.assertEqual(self.refusals(), [])

    def test_header_note_and_gate(self) -> None:
        self.write("src/eboot/no_header.c", "int f(void) { return 0; }\n")
        self.write("src/eboot/no_note.c", HEADER + "/* written somehow */\nint FUN_00010000(void) { return 0; }\n")
        self.write("src/eboot/asm.c", HEADER + NOTE + 'void FUN_00010000(void) { asm("addiu $sp,$sp,-16"); }\n')
        self.commit("bad")
        got = sorted(r.split()[0] + " " + r.split()[1] for r in self.refusals())
        self.assertEqual(got, ["gate src/eboot/asm.c:", "header src/eboot/no_header.c:", "header src/eboot/no_note.c:"])

    def test_the_sdk_units_are_the_bases_not_the_pull_requests(self) -> None:
        units = lambda *ranges: ('{"modules": {"EBOOT.BIN": [' + ", ".join(
            f'{{"start": "0x{lo:08x}", "end": "0x{hi:08x}"}}' for lo, hi in ranges) + "]}}\n")
        self.git("checkout", "-q", "base")
        self.write("data/sdk-gcc4-units.json", units((0x4000, 0x5000)))
        self.commit("units")
        self.git("checkout", "-q", "topic")
        self.git("merge", "-q", "base")
        gcc = lambda addr: GOOD.replace("lang=c99", "cc=gcc404x-O2").replace("00010000", f"{addr:08x}") \
            .replace("0x00010008", f"0x{addr + 8:08x}")
        self.write("src/eboot/FUN_00004100.c", gcc(0x4100))                          # inside the base's units
        self.write("src/eboot/FUN_00010010.c", gcc(0x10010))                         # outside them ...
        self.write("data/sdk-gcc4-units.json", units((0x4000, 0x5000), (0x10010, 0x10018)))   # ... widened here
        self.commit("widen")
        got = [r.split()[0] + " " + r.split()[1] for r in self.refusals()]
        self.assertEqual(got, ["gate src/eboot/FUN_00010010.c:"])

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
               "- 2026-10-02: measure only from `C:/work/tree/project/try/gen-aN/` from now on\n")
        self.git("checkout", "-q", "base")
        self.write("docs/MWCC-NOTES.md", log)
        self.commit("log with a local path")
        self.git("checkout", "-q", "topic")
        self.git("merge", "-q", "base")
        self.write("docs/MWCC-NOTES.md", log.replace("C:/work/tree", "<private tree>") + "- 2026-11-02: appended\n")
        self.commit("redact and append")
        self.assertEqual(self.refusals(), [])                           # a redaction is the one allowed change
        self.write("docs/MWCC-NOTES.md", log.replace("C:/work/tree/project/try", "<private tree>/x"))
        self.commit("redact and rewrite")
        self.assertEqual([r.split()[0] for r in self.refusals()], ["log"])  # the rest of the line must stay

    def test_a_redaction_replaces_only_local_path_roots(self) -> None:
        R = P.is_redaction
        self.assertTrue(R(r"image `C:\work\tree\a\b.elf` here", r"image `<private tree>\a\b.elf` here"))
        self.assertTrue(R("from `/mnt/c/work/x/try/` and `/home/user/y`", "from `<private tree>/try/` and `<private tree>/y`"))
        self.assertFalse(R("from `C:/work/x/try/`", "from `<private tree>/try/` (edited)"))
        self.assertFalse(R("value 0x10 here", "value <private tree> here"))              # nothing local was there
        self.assertFalse(R("a `C:/work/x` and `C:/work/y`", "a `<private tree>` and `C:/work/y`"))  # a path is left
        self.assertFalse(R("a lesson", "a different lesson"))
        self.assertEqual(P.unredacted(["a C:/work/x b", "c"], ["a <private tree> b"]), ["c"])

if __name__ == "__main__":
    unittest.main()
