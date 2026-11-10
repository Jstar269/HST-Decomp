#!/usr/bin/env python3
"""Synthetic tests for the pragma variants of tools/hstperm.py (function_span, unit_pragma, steer_pragma).

    python tools/test_hstperm_pragmas.py

No game data and no compiler: the tests check the text the functions write and that tools/hstgate.py classes it
(a scoped, declared steering pragma is the idiom `steer-pragma`; nothing is refused).
"""
from __future__ import annotations

import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hstgate
import hstperm as P

HEADER = "/* HSTMATCH module=EBOOT.BIN start=0x00001000 end=0x00001040 symbol=FUN_00001000 lang=c++ */"
SOURCE = "\n".join([
    HEADER,
    "/* Hand-written reconstruction from the disassembly; NEAR MISS (2 words). */",
    "struct Item { short key; };",
    'extern "C" void FUN_00002000(Item **a, int n);',
    "inline int Less(Item *a, Item *b)",
    "{",
    "    return a->key < b->key;",
    "}",
    "",
    'extern "C" void FUN_00001000(Item **first, Item **last)',
    "{",
    "    if (first != last) {",
    '        FUN_00002000(first, "}{");   /* a brace in a string and a } in a comment */',
    "        FUN_00001000(first + 1, last);",
    "    }",
    "}",
    "",
])


def header_of(text: str) -> dict:
    import re
    return dict(re.findall(r'(\w+)=("[^"]*"|\S+)', text.split("\n")[0]))


class FunctionSpan(unittest.TestCase):
    def test_finds_the_definition_not_the_calls_or_declarations(self):
        lines = SOURCE.split("\n")
        first, last = P.function_span(SOURCE, "FUN_00001000")
        self.assertEqual(lines[first], 'extern "C" void FUN_00001000(Item **first, Item **last)')
        self.assertEqual(lines[last], "}")
        self.assertEqual(last, len(lines) - 2)

    def test_declaration_only_is_not_a_definition(self):
        self.assertIsNone(P.function_span(SOURCE, "FUN_00002000"))

    def test_signature_on_two_lines(self):
        text = SOURCE.replace('extern "C" void FUN_00001000(', 'extern "C" void\nFUN_00001000(')
        first, _last = P.function_span(text, "FUN_00001000")
        self.assertEqual(text.split("\n")[first], 'extern "C" void')

    def test_brace_on_the_signature_line(self):
        text = HEADER + "\nint FUN_00001000(int a) {\n    return a;\n}\nint g;\n"
        self.assertEqual(P.function_span(text, "FUN_00001000"), (1, 3))


class UnitPragma(unittest.TestCase):
    def test_inserted_after_the_header(self):
        out = P.unit_pragma(SOURCE, "opt_strength_reduction", "off").split("\n")
        self.assertEqual(out[0], HEADER)
        self.assertEqual(out[1], "#pragma opt_strength_reduction off")
        self.assertEqual(P.unit_pragma(SOURCE, "inline_depth", "(8)").split("\n")[1], "#pragma inline_depth(8)")

    def test_not_twice(self):
        once = P.unit_pragma(SOURCE, "opt_unroll_loops", "off")
        self.assertIsNone(P.unit_pragma(once, "opt_unroll_loops", "off"))


class SteerPragma(unittest.TestCase):
    def test_scoped_to_the_function_and_declared(self):
        out = P.steer_pragma(SOURCE, "FUN_00001000", "opt_lifetimes", "off", "found by a test")
        lines = out.split("\n")
        self.assertEqual(lines[0], HEADER)
        self.assertIn("steer=pragma:opt_lifetimes", lines[1])
        d = lines.index('extern "C" void FUN_00001000(Item **first, Item **last)')
        self.assertEqual(lines[d - 2:d], ["#pragma push", "#pragma opt_lifetimes off"])
        self.assertEqual(lines[lines.index("#pragma pop") - 1], "}")
        self.assertLess(lines.index("}"), lines.index("#pragma push"))       # the inline helper stays outside
        # removing the three pragma lines and the declaration gives the seed back
        back = [l for l in lines if not l.startswith("#pragma") and "steer=pragma" not in l]
        self.assertEqual("\n".join(back), SOURCE)

    def test_the_gate_accepts_it_as_the_named_idiom(self):
        out = P.steer_pragma(SOURCE, "FUN_00001000", "opt_dead_assignments")
        findings = hstgate.check_text(out, header_of(out), "src")
        self.assertEqual(hstgate.refusals(findings), [])
        self.assertIn("steer-pragma", {f.kind for f in findings if f.severity == hstgate.IDIOM})

    def test_unlocated_function_wraps_the_code_after_the_leading_comments(self):
        out = P.steer_pragma(SOURCE, "__ct__1CFv", "opt_propagation").split("\n")
        self.assertIn("steer=pragma:opt_propagation", out[1])
        self.assertTrue(out[2].startswith("/* Hand-written"))
        self.assertEqual(out[3:5], ["#pragma push", "#pragma opt_propagation off"])
        self.assertEqual([l for l in out if l.strip()][-1], "#pragma pop")
        findings = hstgate.check_text("\n".join(out), header_of(SOURCE), "src")
        self.assertEqual(hstgate.refusals(findings), [])

    def test_refuses_a_source_that_already_has_it_or_a_scope(self):
        once = P.steer_pragma(SOURCE, "FUN_00001000", "opt_lifetimes")
        self.assertIsNone(P.steer_pragma(once, "FUN_00001000", "opt_lifetimes"))
        self.assertIsNone(P.steer_pragma(once, "FUN_00001000", "opt_propagation"))    # one scope per source
        self.assertIsNone(P.steer_pragma(SOURCE.replace(HEADER, HEADER + "\n#pragma opt_lifetimes off"),
                                         "FUN_00001000", "opt_lifetimes"))

    def test_every_steering_pragma_is_one_the_gate_knows(self):
        for name in P.STEER_PRAGMAS:
            self.assertIn(name, hstgate.STEER_PRAGMAS)


if __name__ == "__main__":
    unittest.main()
