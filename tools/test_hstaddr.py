#!/usr/bin/env python3
"""Synthetic-only tests for the AST numeric-address detector."""
from __future__ import annotations

import json
import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hstaddr as A
import hstport as P

NDK_CLANG = P.DEFAULT_NDK / P.NDK_CLANG_RELATIVE
CLANG = P.DEFAULT_HOST_CLANG if P.DEFAULT_HOST_CLANG.is_file() else NDK_CLANG


def make_root(tmp: str) -> Path:
    root = Path(tmp)
    (root / "include").mkdir()
    (root / "data").mkdir()
    (root / "data" / "modules.json").write_text(
        '{"modules":[{"name":"EBOOT.BIN","load_segments":[{"start":4096,"memory_end":8192}]}]}\n',
        encoding="utf-8")
    (root / "data" / "functions-EBOOT.BIN.txt").write_text("00001000 00002000\n", encoding="utf-8")
    return root


def write_source(root: Path, name: str, code: str, lang: str = "c99") -> Path:
    path = root / "src" / "eboot" / name
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(
        f"/* HSTMATCH module=EBOOT.BIN start=0x00001000 end=0x00002000 symbol=synthetic lang={lang} */\n"
        f"{code}\n",
        encoding="utf-8",
    )
    return path


@unittest.skipUnless(CLANG.is_file(), f"Clang not found: {CLANG}")
class NumericAddressTests(unittest.TestCase):
    def detect(self, root: Path, path: Path, lang: str = "c99") -> list[dict]:
        return A.findings(path, lang, root=root, overlay=P.DEFAULT_OVERLAY, compiler=CLANG)

    def test_literal_folded_and_enum_values_are_ast_findings(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = make_root(tmp)
            path = write_source(root, "constants.c",
                                "enum { MODULE_ADDRESS = 0x08800000 };\n"
                                "int *literal(void) { return (int *)0x08800000; }\n"
                                "int *folded(void) { return (int *)(0x08000000 + 0x10); }\n"
                                "int *enumerated(void) { return (int *)MODULE_ADDRESS; }\n"
                                "int *implicit(void) { return 0x08800000; }")
            result = self.detect(root, path)
            self.assertEqual([item["kind"] for item in result], [
                "integral-to-pointer", "integral-to-pointer", "integral-to-pointer", "integral-to-pointer",
            ])
            self.assertEqual([item["line"] for item in result], [3, 4, 5, 6])
            self.assertTrue(all(set(item) == {"kind", "line", "column"} for item in result))
            self.assertNotIn("0x08800000", json.dumps(result))
            self.assertNotIn("return", json.dumps(result))

    def test_null_pointer_constants_are_excluded(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = make_root(tmp)
            path = write_source(root, "nulls.c",
                                "#include <stddef.h>\n"
                                "int *zero(void) { return (int *)0; }\n"
                                "void *macro_null(void) { return NULL; }")
            self.assertEqual(self.detect(root, path), [])

    def test_pointer_sized_integer_variable_is_not_an_integer_constant_expression(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = make_root(tmp)
            path = write_source(root, "variable.c",
                                "#include <stdint.h>\n"
                                "void *from_uintptr(uintptr_t value) { return (void *)value; }")
            self.assertEqual(self.detect(root, path), [])

    def test_module_address_compared_with_pointer_integer_representation_is_reported(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = make_root(tmp)
            path = write_source(root, "compare.c",
                                "#include <stdint.h>\n"
                                "int matches(void *pointer) { return (uintptr_t)pointer == 0x1010; }")
            result = self.detect(root, path)
            self.assertEqual(len(result), 1)
            self.assertEqual(result[0]["kind"], "module-address-pointer-compare")
            self.assertEqual(result[0]["line"], 3)

    def test_module_address_constant_with_index_only_operand_is_reported(self) -> None:
        # A module-range constant plus a scaled index, with no address-carrying operand, is an absolute address.
        with tempfile.TemporaryDirectory() as tmp:
            root = make_root(tmp)
            path = write_source(root, "conversion.c",
                                "void *table(int index) { return (void *)(index * 4 + 0x1010); }")
            result = self.detect(root, path)
            self.assertEqual(len(result), 1)
            self.assertEqual(result[0]["kind"], "module-address-conversion")
            self.assertEqual(result[0]["line"], 2)

    # --- offsets from an address-carrying base are not module addresses -----------------------------------------

    def assert_clean(self, name: str, code: str, lang: str = "c99") -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = make_root(tmp)
            path = write_source(root, name, code, lang)
            self.assertEqual(self.detect(root, path, lang), [], name)

    def assert_flagged(self, name: str, code: str, lines: list[int], kind: str = "module-address-conversion",
                       lang: str = "c99") -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = make_root(tmp)
            path = write_source(root, name, code, lang)
            result = self.detect(root, path, lang)
            self.assertEqual([(item["kind"], item["line"]) for item in result], [(kind, line) for line in lines],
                             name)

    def test_offset_from_integer_parameter_base_is_not_reported(self) -> None:
        # Ghidra keeps a struct pointer in an int parameter: *(T *)(param_1 + offset).
        self.assert_clean("ghidra_param.c",
                          "int get(int param_1) { return *(int *)(param_1 + 0x1524); }\n"
                          "void put(int param_1, int v) { *(int *)(param_1 + 0x1010) = v; }\n"
                          "int below(int param_1) { return *(int *)(param_1 - 0x1010); }\n"
                          "int unsign(int param_1) { return *(int *)((unsigned int)param_1 + 0x1010); }")

    def test_offset_from_uintptr_base_is_not_reported(self) -> None:
        self.assert_clean("uintptr_base.c",
                          "#include <stdint.h>\n"
                          "void *from_base(uintptr_t base) { return (void *)(base + 0x1010); }")

    def test_offset_from_pointer_converted_to_integer_is_not_reported(self) -> None:
        self.assert_clean("pointer_base.c",
                          "#include <stdint.h>\n"
                          "typedef unsigned int u32;\n"
                          "int a(void *p) { return *(int *)((uintptr_t)p + 0x1010); }\n"
                          "int b(void *p) { return *(int *)((u32)(uintptr_t)p + 0x1010); }\n"
                          "int c(void *p, int off) { return *(int *)((uintptr_t)p + off + 0x1010); }\n"
                          "char *d(char *p) { return (char *)p + 0x1010; }\n"
                          "int e(void *p) { return *(int *)(0x1010 + (uintptr_t)p); }")

    def test_offset_from_loaded_value_is_not_reported(self) -> None:
        # The base is a member, an array element, a dereference, a local or a call result.
        self.assert_clean("loaded_base.c",
                          "struct S { int base; int *items; };\n"
                          "int f(void);\n"
                          "int member(struct S *s) { return *(int *)(s->base + 0x1010); }\n"
                          "int element(int *t) { return *(int *)(t[3] + 0x1010); }\n"
                          "int nested(int param_1) { return *(int *)(*(int *)(param_1 + 0x1010) + 0x1020); }\n"
                          "int local(void) { int iVar1 = f(); return *(int *)(iVar1 + 0x1010); }\n"
                          "int call(void) { return *(int *)(f() + 0x1010); }")

    def test_subscript_index_inside_a_loaded_value_is_not_reported(self) -> None:
        self.assert_clean("subscript.c",
                          "int *slot(int *param_1) { return (int *)param_1[0x1010]; }")

    def test_scaled_index_and_offset_from_base_are_not_reported(self) -> None:
        self.assert_clean("scaled.c",
                          "int get(int param_1, int param_2) { return *(int *)(param_1 + param_2 * 0x1010 + 0x1020); }\n"
                          "int shift(int param_1, int i) { return *(int *)(param_1 + (i << 4) + 0x1010); }")

    def test_conditional_of_based_operands_is_not_reported(self) -> None:
        self.assert_clean("conditional.c",
                          "int *pick(int c, int a, int b) { return (int *)(c ? a + 0x1010 : b + 0x1020); }")

    def test_pointer_reused_as_counter_increment_is_not_reported(self) -> None:
        self.assert_clean("counter.cpp",
                          "struct Self { int x; };\n"
                          "Self *step(Self *p) { return (Self *)((long long)p + 0x1001); }", "c++")

    def test_loaded_value_compared_with_module_range_constant_is_not_reported(self) -> None:
        self.assert_clean("loaded_compare.c",
                          "int is_tag(int param_1) { return *(int *)(param_1 + 0x10) == 0x1010; }")

    # --- absolute addresses stay reported -----------------------------------------------------------------------

    def test_constant_with_narrow_value_operand_is_reported(self) -> None:
        # An 8-bit value cannot hold an address, so the constant is the address.
        self.assert_flagged("narrow.c",
                            "unsigned char *byte_table(unsigned char i) { return (unsigned char *)(i + 0x1010); }",
                            [2])

    def test_constant_after_a_pointer_difference_is_reported(self) -> None:
        # (p - q) is a displacement, not an address, so the constant is the address.
        self.assert_flagged("difference.c",
                            "#include <stdint.h>\n"
                            "int *rel(char *p, char *q) { return (int *)((p - q) + 0x1010); }\n"
                            "int *neg(void *p) { return (int *)(0x1010 - (uintptr_t)p); }",
                            [3, 4])

    def test_folded_constant_part_in_module_range_is_reported(self) -> None:
        # Neither literal lies in the module range 0x1000-0x2000; their sum does.
        self.assert_flagged("folded_part.c",
                            "int *get(int i) { return (int *)(i * 4 + 0x3000 - 0x2000); }", [2])

    def test_bitwise_composition_with_module_range_constant_is_reported(self) -> None:
        self.assert_flagged("bitwise.c",
                            "int *compose(int i) { return (int *)((i << 2) | 0x1010); }", [2])

    def test_constant_branch_of_conditional_is_reported(self) -> None:
        self.assert_flagged("conditional_constant.c",
                            "int *pick(int c, int a) { return (int *)(c ? 0x1010 : a + 4); }", [2])

    def test_comma_and_low_bit_mask_of_based_values_are_not_reported(self) -> None:
        self.assert_clean("comma_mask.c",
                          "int *row(int base, int i) { int off; return (int *)(off = i * 0x1010, off + *(int *)(base + 0x1020)); }\n"
                          "unsigned *untag(unsigned *p) { return (unsigned *)(p[0x1010] & 0xfffffffe); }\n"
                          "char *align(int p) { return (char *)((p + 0x1007) & ~7); }")

    def test_comma_or_low_bit_mask_of_an_absolute_address_is_reported(self) -> None:
        self.assert_flagged("comma_mask_absolute.c",
                            "int *row(int i) { int off; return (int *)(off = i * 4, off * 2 + 0x1010); }\n"
                            "int *aligned(int i) { return (int *)((i * 4 + 0x1013) & ~3); }",
                            [2, 3])

    def test_truncated_value_with_module_range_constant_is_reported(self) -> None:
        self.assert_flagged("truncated.c",
                            "int *get(int param_1) { return (int *)(int)(short)(param_1 + 0x1010); }", [2])

    def test_pointer_compare_against_module_constant_still_reported(self) -> None:
        self.assert_flagged("compare_counter.c",
                            "#include <stdint.h>\n"
                            "int done(void *p) { return (uintptr_t)p < 0x1010; }", [3],
                            kind="module-address-pointer-compare")

    def test_cpp_reinterpret_cast_of_literal_is_reported_without_source_text(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = make_root(tmp)
            path = write_source(root, "cast.cpp",
                                "int *literal() { return reinterpret_cast<int *>(0x08800000); }", "c++")
            result = self.detect(root, path, "c++")
            self.assertEqual(len(result), 1)
            self.assertEqual(result[0]["kind"], "integral-to-pointer")
            self.assertEqual(set(result[0]), {"kind", "line", "column"})
            self.assertNotIn("reinterpret_cast", json.dumps(result))
            self.assertNotIn("0x08800000", json.dumps(result))

    def test_cpp_functional_pointer_cast_of_literal_is_reported(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = make_root(tmp)
            path = write_source(root, "functional.cpp",
                                "using address_pointer = int *;\n"
                                "address_pointer literal() { return address_pointer(0x08800000); }", "c++")
            result = self.detect(root, path, "c++")
            self.assertEqual(len(result), 1)
            self.assertEqual(result[0]["kind"], "integral-to-pointer")


    def test_implicit_enumerator_cast_is_reported(self) -> None:
        # FIRST is 1: an enumerator without an initialiser is an integer constant expression too
        self.assert_flagged("implicit_enum.c", "enum { NONE, FIRST };\n"
                            "int *first(void) { return (int *)FIRST; }", [3], kind="integral-to-pointer")

    def test_module_range_implicit_enumerator_with_index_is_reported(self) -> None:
        self.assert_flagged("implicit_enum_index.c", "enum { BEFORE = 0x100f, TABLE };\n"
                            "int *entry(int index) { return (int *)(TABLE + index * 4); }", [3])

    def test_cpp_constexpr_address_is_reported(self) -> None:
        self.assert_flagged("constexpr.cpp", "constexpr int ADDRESS = 0x1010;\n"
                            "int *plain() { return (int *)ADDRESS; }", [3], kind="integral-to-pointer", lang="c++")
        self.assert_flagged("constexpr_index.cpp", "constexpr int ADDRESS = 0x1010;\n"
                            "int *entry(int index) { return (int *)(ADDRESS + index * 4); }", [3], lang="c++")

    def test_cpp_constexpr_offset_from_base_is_not_reported(self) -> None:
        self.assert_clean("constexpr_offset.cpp", "constexpr int OFFSET = 0x1010;\n"
                          "int *field(int base) { return (int *)(base + OFFSET); }", lang="c++")


class NamedConstantTests(unittest.TestCase):
    """The values of enumerators and constexpr variables, from AST nodes shaped as Clang's JSON dump writes them."""

    @staticmethod
    def literal(value: int) -> dict:
        return {"kind": "ConstantExpr", "value": str(value),
                "inner": [{"kind": "IntegerLiteral", "value": str(value)}]}

    def test_enumerators_without_initialiser_count_on(self) -> None:
        enum = {"kind": "EnumDecl", "inner": [
            {"id": "0x1", "kind": "EnumConstantDecl", "name": "NONE"},
            {"id": "0x2", "kind": "EnumConstantDecl", "name": "FIRST"},
            {"id": "0x3", "kind": "EnumConstantDecl", "name": "BASE", "inner": [self.literal(0x1000)]},
            {"id": "0x4", "kind": "EnumConstantDecl", "name": "NEXT"},
        ]}
        enum_values, _ = A._named_constants([enum, *enum["inner"]])
        self.assertEqual(enum_values, {"0x1": 0, "0x2": 1, "0x3": 0x1000, "0x4": 0x1001})

    def test_reference_to_constexpr_variable_has_its_value(self) -> None:
        # Clang's referencedDecl names the variable's id and kind but does not repeat its constexpr flag
        reference = {"kind": "ImplicitCastExpr", "castKind": "LValueToRValue", "inner": [
            {"kind": "DeclRefExpr", "referencedDecl": {"id": "0x5", "kind": "VarDecl", "name": "ADDRESS"}}]}
        self.assertEqual(A._integer_value(reference, {}, {"0x5": 0x1010}), 0x1010)

    def test_only_constexpr_variables_have_values(self) -> None:
        plain = {"id": "0x6", "kind": "VarDecl", "name": "value", "inner": [self.literal(0x1010)]}
        constant = {"id": "0x7", "kind": "VarDecl", "name": "ADDRESS", "constexpr": True, "init": "c",
                    "inner": [self.literal(0x1010)]}
        _, constexpr_values = A._named_constants([plain, *plain["inner"], constant, *constant["inner"]])
        self.assertEqual(constexpr_values, {"0x7": 0x1010})


if __name__ == "__main__":
    unittest.main()
