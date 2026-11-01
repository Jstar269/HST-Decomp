#!/usr/bin/env python3
"""Tests for the fake-match gate (hstgate.py) and for the places that must use it."""
from __future__ import annotations

import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hstgate as G


def kinds(text: str, header: dict | None = None, tree: str = "src") -> set[str]:
    return {f.kind for f in G.check_text(text, header, tree)}


def refused(text: str, header: dict | None = None, tree: str = "src") -> set[str]:
    return {f.kind for f in G.refusals(G.check_text(text, header, tree))}


class RefusalTests(unittest.TestCase):
    def test_plain_c_has_no_findings(self) -> None:
        self.assertEqual(kinds('#include <string.h>\n#pragma inline_depth(8)\nint f(int a) { return a + 1; }\n'), set())

    def test_asm_statements_are_refused_whatever_the_dialect(self) -> None:
        self.assertEqual(refused('void f(void) { __asm__ volatile ("addiu $2,$2,1"); }'), {"asm-emit"})
        self.assertEqual(refused('void f(void) { asm("nop"); }'), {"asm-emit"})
        self.assertEqual(refused('asm void f(void) {\n  addiu r3, r3, 1\n  blr\n}\n'), {"asm-emit"})
        self.assertEqual(refused('void f(void) { __asm { nop } }'), {"asm-emit"})
        self.assertEqual(refused('void f(void) { _asm { nop } }'), {"asm-emit"})

    def test_a_register_binding_and_an_empty_barrier_are_tagged_but_allowed(self) -> None:
        text = 'void f(const unsigned char *src) {\n    register unsigned char c asm("$2") = *src;\n}\n'
        self.assertEqual(kinds(text), {"register-asm-binding"})
        self.assertEqual(refused(text), set())
        self.assertEqual(kinds('void f(void) { __asm__ __volatile__("" ::: "memory"); }'), {"empty-asm-barrier"})
        self.assertEqual(refused('void f(void) { asm volatile("" : : : "memory"); }'), set())
        # a binding does not excuse real code in the same function
        self.assertEqual(refused('void f(void) { register int c asm("$2") = 1; asm("nop"); }'), {"asm-emit"})

    def test_data_directives_are_refused_but_member_access_and_designators_are_not(self) -> None:
        self.assertEqual(refused('void f(void) { __asm__(".word 0x27bdffe0"); }'), {"asm-emit", "data-directive"})
        self.assertEqual(refused('const char *s = ".byte 1, 2";'), {"data-directive"})
        self.assertEqual(refused('__asm__(".incbin \\"retail.bin\\"");'), {"asm-emit", "data-directive"})
        self.assertEqual(kinds('struct S { int word; } s = { .word = 1 };\nint g(struct S *p) { return p->word + s.word; }\n'), set())

    def test_pragmas(self) -> None:
        self.assertEqual(refused("#pragma asm\n"), {"pragma-code"})
        self.assertEqual(refused("#pragma push_asm\n"), {"pragma-code"})
        self.assertEqual(kinds("#pragma opt_strength_reduction off\n#pragma inline_depth(8)\n#pragma exceptions on\n"), set())
        findings = G.check_text("#pragma mystery on\n", None, "src")
        self.assertEqual([(f.kind, f.severity) for f in findings], [("pragma-unknown", G.WARN)])

    def test_text_section_tricks_and_binary_includes_are_refused(self) -> None:
        self.assertEqual(refused('__attribute__((section(".text"))) const unsigned code[] = { 0x27bdffe0 };'), {"text-section"})
        self.assertEqual(refused('#pragma section ".text"\n'), {"text-section"})
        self.assertEqual(refused('const unsigned char blob[] = {\n#include "retail.bin"\n};\n'), {"binary-include"})
        self.assertEqual(refused('#include "table.inc"\n'), {"binary-include"})
        self.assertEqual(refused('#include <stdio.h>\n#include "local.h"\n'), set())

    def test_extra_is_refused_only_in_src(self) -> None:
        self.assertEqual(refused("int x;", {"extra": "-opt nopeephole"}, "src"), {"extra-header"})
        self.assertEqual(refused("int x;", {"extra": "-opt nopeephole"}, "wip"), set())
        self.assertEqual(refused("int x;", {"module": "EBOOT.BIN"}, "src"), set())

    def test_comments_are_not_scanned_but_strings_are(self) -> None:
        self.assertEqual(kinds("/* asm(\".word 1\") */\n// __asm__ volatile (\"nop\");\nint x;\n"), set())
        self.assertEqual(refused('const char *s = "/* not a comment */ .word 7";'), {"data-directive"})

    def test_findings_carry_line_numbers(self) -> None:
        found = G.check_text("int a;\nint b;\nvoid f(void) { asm(\"nop\"); }\n")
        self.assertEqual([f.line for f in found], [3])


class WiringTests(unittest.TestCase):
    """The gate must be called from the matcher, the harvester and the linter."""

    def source(self, name: str) -> str:
        return (Path(__file__).resolve().parent / name).read_text(encoding="utf-8")

    def test_hstmatch_reports_a_distinct_status(self) -> None:
        text = self.source("hstmatch.py")
        self.assertIn("import hstgate", text)
        self.assertIn('status, detail = "EXACT_FAILS_GATE"', text)

    def test_hstlane_harvest_refuses_and_hstlint_reports(self) -> None:
        if (Path(__file__).resolve().parent / "hstlane.py").is_file():   # maintainer-only, not published
            self.assertIn("hstgate.refusals(hstgate.check_file(p, src_root))", self.source("hstlane.py"))
        self.assertIn('flags["gate"]', self.source("hstlint.py"))

    def test_check_file_uses_the_tree_of_the_path(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / "src").mkdir()
            (root / "wip").mkdir()
            text = '/* HSTMATCH module=EBOOT.BIN start=0x0 end=0x4 symbol=f lang=c99 extra="-opt x" */\nint f;\n'
            (root / "src" / "a.c").write_text(text, encoding="utf-8")
            (root / "wip" / "a.c").write_text(text, encoding="utf-8")
            self.assertEqual({f.kind for f in G.refusals(G.check_file(root / "src" / "a.c", root))}, {"extra-header"})
            self.assertEqual(G.refusals(G.check_file(root / "wip" / "a.c", root)), [])

    def test_the_tree_has_no_refused_source_and_only_the_known_idioms(self) -> None:
        root = G.ROOT
        refused_files, idioms, vfpu = [], [], []
        for p in sorted((root / "src").rglob("*")):
            if p.suffix in (".c", ".cpp"):
                found = G.check_file(p, root)
                if G.refusals(found):
                    refused_files.append(p.name)
                if any(f.kind == "vfpu-inline-asm" for f in found):
                    vfpu.append(p.name)       # the VFPU rule's sources (owner decision 2026-10-27)
                    self.assertIn(" asm=vfpu", p.read_text(encoding="utf-8").split("\n", 1)[0], p.name)
                elif found:
                    idioms.append(p.name)
        self.assertEqual(refused_files, [])
        # the two known idiom sources are Sony SDK code, which the public tree does not carry (hstpublish.py)
        known = ["str_copy_n_0006f644.c", "str_copy_n_00070e04.c"]
        self.assertEqual(sorted(idioms), [n for n in known if any((root / "src").rglob(n))])


class VfpuRuleTests(unittest.TestCase):
    """Project rule (owner decision 2026-10-27): VFPU-only inline asm is allowed, as the idiom vfpu-inline-asm, when
    the header says asm=vfpu and a line starting 'Original inline VFPU asm:' gives the reason. Anything else is
    asm-emit."""
    HEADER = {"module": "EBOOT.BIN", "start": "0x0", "end": "0x4", "symbol": "f", "asm": "vfpu"}
    OK = ('/* HSTMATCH module=EBOOT.BIN start=0x0 end=0x4 symbol=f cc=gcc171-O2 asm=vfpu */\n'
          '/* Original inline VFPU asm: the SDK vector helper loads, dots and stores through the VFPU. */\n'
          'void f(float *v, const float *a) {\n'
          '    __asm__ volatile ("lv.q C000, 0(%1)\\n"\n'
          '                      "vdot.q S010, C000, C000\\n"\n'
          '                      "vi2f.q C100, C200, 1/2^31\\n"\n'
          '                      "sv.s S010, 16(%0)" : : "r"(v), "r"(a) : "memory");\n'
          '}\n')

    def found(self, text: str, header: dict | None) -> set[tuple[str, str]]:
        return {(f.kind, f.severity) for f in G.check_text(text, header, "src")}

    def test_vfpu_only_asm_with_header_and_reason_passes(self) -> None:
        self.assertEqual(self.found(self.OK, self.HEADER), {("vfpu-inline-asm", G.IDIOM)})
        self.assertEqual(G.refusals(G.check_text(self.OK, self.HEADER, "src")), [])
        # a mtv/mfv move and a reason given as a // comment pass too
        text = self.OK.replace("/* Original inline VFPU asm: the SDK", "// Original inline VFPU asm: the SDK").replace(
            "through the VFPU. */", "through the VFPU.").replace('"vdot.q', '"mtv $4, S000\\n" "vdot.q')
        self.assertEqual(self.found(text, self.HEADER), {("vfpu-inline-asm", G.IDIOM)})
        # the SDK's .set push / noreorder / pop wrapper emits nothing and is allowed
        text = self.OK.replace('"lv.q C000, 0(%1)\\n"', '".set push\\n" ".set noreorder\\n" "lv.q C000, 0(%1)\\n"').replace(
            '"sv.s S010, 16(%0)"', '"sv.s S010, 16(%0)\\n" ".set pop\\n"')
        self.assertIn(".set noreorder", text)
        self.assertEqual(self.found(text, self.HEADER), {("vfpu-inline-asm", G.IDIOM)})
        # but a template of directives alone is not VFPU code
        self.assertFalse(G.vfpu_only(".set push\n.set noreorder\n.set pop\n"))

    def test_a_memory_operand_must_have_an_m_constraint(self) -> None:
        # sv.s to a local through an "m" operand (the compiler addresses it as offset($sp)) passes
        text = ('/* HSTMATCH module=EBOOT.BIN start=0x0 end=0x4 symbol=f cc=gcc171-O2 asm=vfpu */'+chr(10)+
                '/* Original inline VFPU asm: the dot product is stored to the result with sv.s. */'+chr(10)+
                'float f(const float *a) { float r; __asm__ volatile ("lv.q C010, 0(%1)'+chr(92)+'n" "vdot.t S000, C010, C010'+chr(92)+'n" '
                '"sv.s S000, %0'+chr(92)+'n" : "=m"(r) : "r"(a)); return r; }'+chr(10))
        self.assertEqual(self.found(text, self.HEADER), {("vfpu-inline-asm", G.IDIOM)})
        self.assertEqual(self.found(text.replace('%0'+chr(92)+'n" : "=m"(r)', '%[out]'+chr(92)+'n" : [out] "=m"(r)'), self.HEADER),
                         {("vfpu-inline-asm", G.IDIOM)})
        # the same bare operand with a register constraint is refused
        self.assertEqual(self.found(text.replace('"=m"(r)', '"=r"(r)'), self.HEADER), {("asm-emit", G.REFUSE)})
        self.assertEqual(self.found(text.replace('S000, %0', 'S000, %2'), self.HEADER), {("asm-emit", G.REFUSE)})

    def test_a_non_vfpu_instruction_is_refused(self) -> None:
        for bad in ("addiu $2, $2, 1", "nop", "lw $2, 0($4)", "mfc1 $8, $f12", ".set mips2", ".set noat", "bvt 0, 1f",
                    "1:", ".word 0", ".align 4"):
            text = self.OK.replace("vdot.q S010, C000, C000", bad)
            self.assertIn(("asm-emit", G.REFUSE), self.found(text, self.HEADER), bad)
        # a VFPU load through a bare symbolic operand can expand to lui/addu: refused
        text = self.OK.replace("lv.q C000, 0(%1)", "lv.q C000, %1")
        self.assertEqual(self.found(text, self.HEADER), {("asm-emit", G.REFUSE)})
        # a VFPU instruction does not excuse a second, non-VFPU asm statement in the same source
        text = self.OK.replace("}\n", '    __asm__ volatile ("sync");\n}\n')
        self.assertEqual(self.found(text, self.HEADER), {("vfpu-inline-asm", G.IDIOM), ("asm-emit", G.REFUSE)})

    def test_a_missing_reason_line_is_refused(self) -> None:
        text = self.OK.replace("Original inline VFPU asm:", "VFPU:")
        self.assertEqual(self.found(text, self.HEADER), {("asm-emit", G.REFUSE)})
        # the reason must follow the prefix on the same line, and the prefix must start the line
        text = self.OK.replace("Original inline VFPU asm: the SDK vector helper loads, dots and stores through "
                               "the VFPU.", "Original inline VFPU asm:")
        self.assertEqual(self.found(text, self.HEADER), {("asm-emit", G.REFUSE)})
        text = self.OK.replace("/* Original inline VFPU asm:", "/* see: Original inline VFPU asm:")
        self.assertEqual(self.found(text, self.HEADER), {("asm-emit", G.REFUSE)})

    def test_a_missing_header_key_is_refused(self) -> None:
        header = {k: v for k, v in self.HEADER.items() if k != "asm"}
        self.assertEqual(self.found(self.OK, header), {("asm-emit", G.REFUSE)})
        self.assertEqual(self.found(self.OK, None), {("asm-emit", G.REFUSE)})
        self.assertEqual(self.found(self.OK, dict(self.HEADER, asm="any")), {("asm-emit", G.REFUSE)})
        # check_file reads the key from the HSTMATCH header itself
        with tempfile.TemporaryDirectory() as d:
            root = Path(d)
            (root / "src").mkdir()
            (root / "src" / "ok.c").write_text(self.OK, encoding="utf-8")
            (root / "src" / "nokey.c").write_text(self.OK.replace(" asm=vfpu", ""), encoding="utf-8")
            self.assertEqual(G.refusals(G.check_file(root / "src" / "ok.c", root)), [])
            self.assertEqual(G.summary(G.check_file(root / "src" / "nokey.c", root)), "asm-emit")

    def test_asm_in_a_define_is_checked(self) -> None:
        text = '#define BUMP() __asm__ volatile ("addiu $2, $2, 1")\nvoid f(void) { BUMP(); }\n'
        self.assertEqual(refused(text), {"asm-emit"})


if __name__ == "__main__":
    unittest.main()
