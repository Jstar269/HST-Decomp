#!/usr/bin/env python3
"""Checks of the source-mutation operators of tools/hstperm.py (no toolchain needed): each operator rewrites one
spot the documented way, leaves the text a balanced C function, and the whole set survives a fuzz run.

    python tools/test_hstperm_mutations.py [-v]
"""
from __future__ import annotations

import random
import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hstperm as P  # noqa: E402

FILL = """void FUN_00296b3c(unsigned char *param_1, unsigned char param_2, int param_3)
{
  for (; param_3 != 0; param_3 = param_3 + -1) {
    *param_1 = param_2;
    param_1 = param_1 + 1;
  }
}"""

MIXED = """int FUN_00000000(int *tab, struct S *a, struct S *b, int n)
{
  int i;
  int acc;
  int idx;

  acc = 0;
  i = 0;
  while (i < n) {
    idx = (i + 1) * 4;
    acc = acc + tab[i];
    acc = acc + (a->x + b->y);
    i = i + 1;
  }
  if (acc != 0) {
    acc = (int)acc | 1;
  }
  return acc;
}"""


def run(op, text: str, seed: int = 1):
    lines = text.split("\n")
    lo, _hi = P.body_bounds(lines)
    changed = op(lines, lo, len(lines), random.Random(seed))
    return changed, "\n".join(lines)


def balanced(text: str) -> bool:
    return (text.count("{") == text.count("}") and text.count("(") == text.count(")")
            and text.count("[") == text.count("]"))


class Operators(unittest.TestCase):
    def test_for_loop_becomes_a_while_loop(self) -> None:
        changed, out = run(P.m_loop_form, FILL)
        self.assertTrue(changed)
        self.assertEqual(out, FILL.replace("  for (; param_3 != 0; param_3 = param_3 + -1) {", "  while (param_3 != 0) {")
                         .replace("    param_1 = param_1 + 1;\n  }", "    param_1 = param_1 + 1;\n    param_3 = param_3 + -1;\n  }"))

    def test_while_loop_becomes_for_or_guarded_do_while(self) -> None:
        seen = set()
        for seed in range(40):
            changed, out = run(P.m_loop_form, MIXED, seed)
            self.assertTrue(changed)
            self.assertTrue(balanced(out), out)
            seen.add("for (; i < n; ) {" in out and "for" or "do {" in out and "do" or "?")
        self.assertEqual(seen, {"for", "do"})

    def test_a_loop_with_continue_is_left_alone(self) -> None:
        text = FILL.replace("*param_1 = param_2;", "if (param_2) continue;\n    *param_1 = param_2;")
        self.assertFalse(run(P.m_loop_form, text)[0])

    def test_compound_assignment_both_ways(self) -> None:
        changed, out = run(P.m_compound, "void f(int a)\n{\n  a = a + 3;\n}")
        self.assertTrue(changed)
        self.assertIn("a += 3;", out)
        changed, out = run(P.m_compound, "void f(int a, int b)\n{\n  a += b + 1;\n}")
        self.assertIn("a = a + (b + 1);", out)                       # the right side keeps its grouping
        changed, out = run(P.m_compound, FILL)                       # inside a for header
        self.assertTrue(changed)
        self.assertIn("param_3 -= 1" if "-= 1" in out else "param_3 += -1", out)

    def test_index_and_pointer_arithmetic(self) -> None:
        _c, out = run(P.m_index, "int f(int *a, int i)\n{\n  return a[i];\n}")
        self.assertIn("return *(a + i);", out)
        _c, out = run(P.m_index, "int f(int *a, int i)\n{\n  return *(a + i);\n}")
        self.assertIn("return a[i];", out)

    def test_temporary_is_introduced_for_a_pure_subexpression(self) -> None:
        text = "int f(int a, int b)\n{\n  int r;\n\n  r = (a + b) * 3;\n  return r;\n}"
        changed, out = run(P.m_temp, text)
        self.assertTrue(changed)
        self.assertIn("  int t0 = a + b;\n  r = t0 * 3;", out)

    def test_temporary_never_wraps_a_call_or_a_cast_or_a_condition(self) -> None:
        text = "int f(int a, int b)\n{\n  int r;\n\n  r = g(a + b);\n  r = (int)(a);\n  if ((a + b) != 0) {\n    r = 1;\n  }\n  return r;\n}"
        self.assertFalse(run(P.m_temp, text)[0])

    def test_casts_are_added_and_removed(self) -> None:
        _c, out = run(P.m_cast, "int f(int a)\n{\n  int r;\n\n  r = (int)a + 1;\n  return r;\n}")
        self.assertTrue(balanced(out))
        self.assertNotEqual(out, "int f(int a)\n{\n  int r;\n\n  r = (int)a + 1;\n  return r;\n}")
        added = set()
        for seed in range(20):
            _c, out = run(P.m_cast, "int f(int a, int b)\n{\n  int r;\n\n  r = a + b;\n  return r;\n}", seed)
            added.add(out.count("(int)") + out.count("(unsigned int)"))
        self.assertEqual(added, {1})

    def test_declaration_moves_to_its_first_use(self) -> None:
        text = "int f(int a)\n{\n  int x;\n  int y;\n\n  y = a;\n  y = y + 1;\n  x = y;\n  return x;\n}"
        outs = {run(P.m_decl_scope, text, seed)[1] for seed in range(30)}
        self.assertTrue(any("  int y;\n  y = y + 1;" in o or "  y = a;\n  int x;" in o or "  int y;\n  y = a;" in o for o in outs))
        for o in outs:
            self.assertEqual(sorted(o.split("\n")), sorted(text.split("\n")), "only the order of lines may change")

    def test_declaration_moves_into_the_block_of_all_its_uses(self) -> None:
        text = "int f(int a)\n{\n  int t;\n\n  if (a != 0) {\n    t = a;\n    a = t + 1;\n  }\n  return a;\n}"
        outs = {run(P.m_decl_scope, text, seed)[1] for seed in range(30)}
        want = "  if (a != 0) {\n    int t;\n    t = a;"
        self.assertTrue(any(want in o for o in outs), outs)

    def test_register_is_added_and_removed(self) -> None:
        _c, out = run(P.m_register, "int f(int a)\n{\n  int x;\n\n  x = a;\n  return x;\n}")
        self.assertIn("  register int x;", out)
        _c, out = run(P.m_register, "int f(int a)\n{\n  register int x;\n\n  x = a;\n  return x;\n}", 5)
        self.assertTrue(balanced(out))

    def test_struct_field_reads_are_reordered(self) -> None:
        text = "int f(struct S *a, struct S *b)\n{\n  return a->x + b->y;\n}"
        _c, out = run(P.m_field_commute, text)
        self.assertIn("return b->y + a->x;", out)


class Fuzz(unittest.TestCase):
    def test_the_whole_set_keeps_functions_balanced_and_never_raises(self) -> None:
        rng = random.Random(7)
        for text in (FILL, MIXED):
            for _ in range(400):
                out = P.mutate(text, rng)
                if out is not None:
                    self.assertTrue(balanced(out), out)

    def test_new_operators_are_in_the_search_set(self) -> None:
        names = {f.__name__ for _w, f in P.MUTATIONS if hasattr(f, "__name__")}
        for name in ("m_temp", "m_cast", "m_compound", "m_index", "m_decl_scope", "m_loop_form", "m_register",
                     "m_field_commute"):
            self.assertIn(name, names)


if __name__ == "__main__":
    unittest.main()
