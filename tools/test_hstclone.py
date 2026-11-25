#!/usr/bin/env python3
"""Tests for clone code propagation and retargeting (hstclone.py)."""
from __future__ import annotations

import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hstclone as C


class WordsTests(unittest.TestCase):
    def test_words_converts_little_endian_bytes(self) -> None:
        data = b"\x01\x02\x03\x04\x05\x06\x07\x08"
        self.assertEqual(C.words(data), [0x04030201, 0x08070605])

    def test_words_truncates_unaligned_trailing_bytes(self) -> None:
        data = b"\x01\x02\x03\x04\x05\x06\x07"
        self.assertEqual(C.words(data), [0x04030201])

    def test_words_empty_input(self) -> None:
        self.assertEqual(C.words(b""), [])


class AnalyseTests(unittest.TestCase):
    def test_jal_instruction(self) -> None:
        # jal target 0x00400000 -> imm = 0x00100000, opcode = 3 -> w = (3 << 26) | 0x100000
        w = (3 << 26) | 0x00100000
        sig, refs = C.analyse([w])
        self.assertEqual(sig, (3 << 26,))
        self.assertEqual(refs, [("call", 0, 0x00400000)])

    def test_lui_and_lw_data_reference(self) -> None:
        # lui $v0, 0x0880 ($v0 is reg 2) -> op=0x0F, rt=2, imm=0x0880
        w_lui = (0x0F << 26) | (0 << 21) | (2 << 16) | 0x0880
        # lw $v1, 0x10($v0) -> op=0x23, rs=2, rt=3, imm=0x0010
        w_lw = (0x23 << 26) | (2 << 21) | (3 << 16) | 0x0010
        sig, refs = C.analyse([w_lui, w_lw])
        # Masked signature masks upper 16 bits (retaining op, rs, rt)
        self.assertEqual(sig, ((0x0F << 26) | (2 << 16), (0x23 << 26) | (2 << 21) | (3 << 16)))
        self.assertEqual(refs, [("hi", 0, 0x08800000), ("data", 1, 0x08800010)])

    def test_lui_and_addiu_negative_offset(self) -> None:
        # lui $v0, 0x0880
        w_lui = (0x0F << 26) | (0 << 21) | (2 << 16) | 0x0880
        # addiu $v0, $v0, -0x1000 (0xF000) -> op=0x09, rs=2, rt=2, imm=0xF000
        w_addiu = (0x09 << 26) | (2 << 21) | (2 << 16) | 0xF000
        sig, refs = C.analyse([w_lui, w_addiu])
        self.assertEqual(refs, [("hi", 0, 0x08800000), ("data", 1, 0x087FF000)])

    def test_register_clearing_erases_hi_state(self) -> None:
        # lui $v0, 0x0880 (reg 2)
        w_lui = (0x0F << 26) | (0 << 21) | (2 << 16) | 0x0880
        # addu $v0, $zero, $zero -> op=0, rs=0, rt=0, rd=2, funct=0x21
        w_cleared = (0x00 << 26) | (0 << 21) | (0 << 16) | (2 << 11) | 0x21
        # lw $v1, 0x10($v0) -> rs=2 has been overwritten, so not tracked as data ref
        w_lw = (0x23 << 26) | (2 << 21) | (3 << 16) | 0x0010
        sig, refs = C.analyse([w_lui, w_cleared, w_lw])
        self.assertEqual(refs, [("hi", 0, 0x08800000)])

    def test_andi_and_xori_zero_extend_immediates(self) -> None:
        # LUI $a0, 0x8001 (hi[4] = 0x80010000)
        lui_a0 = (0x0F << 26) | (4 << 16) | 0x8001
        # ANDI $a1, $a0, 0x8000 (op=0x0C, rs=4, rt=5, imm=0x8000)
        andi_inst = (0x0C << 26) | (4 << 21) | (5 << 16) | 0x8000
        # XORI $a2, $a0, 0x8000 (op=0x0E, rs=4, rt=6, imm=0x8000)
        xori_inst = (0x0E << 26) | (4 << 21) | (6 << 16) | 0x8000

        sig, refs = C.analyse([lui_a0, andi_inst, xori_inst])
        self.assertEqual(refs, [
            ("hi", 0, 0x80010000),
            ("data", 1, 0x80018000),
            ("data", 2, 0x80018000)
        ])

    def test_register_invalidation_clears_hi_state(self) -> None:
        # 1. R-type overwrites rd
        # LUI $a0 ($4), 0x8001
        lui_a0 = (0x0F << 26) | (4 << 16) | 0x8001
        # ADDU $a0 ($4), $t0 ($8), $t1 ($9) -> op=0, rd=4
        addu_a0 = (0x00 << 26) | (8 << 21) | (9 << 16) | (4 << 11) | 0x21
        # LW $v0 ($2), 0($a0) (op=0x23, rs=4, rt=2, imm=0) -> rs=4 was cleared!
        lw_a0 = (0x23 << 26) | (4 << 21) | (2 << 16) | 0x0000

        sig, refs = C.analyse([lui_a0, addu_a0, lw_a0])
        self.assertEqual(refs, [("hi", 0, 0x80010000)])  # no data ref for lw_a0!
        self.assertEqual(sig[2], lw_a0)  # full word retained since rs=4 not in hi

        # 2. GPR write RT (op in GPR_WRITE_RT, e.g. LW)
        # LUI $a1 ($5), 0x8001
        lui_a1 = (0x0F << 26) | (5 << 16) | 0x8001
        # LW $a1 ($5), 0($sp) (op=0x23, rs=29, rt=5)
        lw_into_a1 = (0x23 << 26) | (29 << 21) | (5 << 16) | 0x0000
        # ADDIU $v0 ($2), $a1 ($5), 0x0010 (op=0x09, rs=5, rt=2)
        addiu_after_lw = (0x09 << 26) | (5 << 21) | (2 << 16) | 0x0010

        sig2, refs2 = C.analyse([lui_a1, lw_into_a1, addiu_after_lw])
        self.assertEqual(refs2, [("hi", 0, 0x80010000)])
        self.assertEqual(sig2[2], addiu_after_lw)

        # 3. COP1 mfc1 (op=0x11, rs=0) writes rt
        # LUI $a2 ($6), 0x8001
        lui_a2 = (0x0F << 26) | (6 << 16) | 0x8001
        # MFC1 $a2 ($6), $f0 ($0) (op=0x11, rs=0, rt=6)
        mfc1_a2 = (0x11 << 26) | (0 << 21) | (6 << 16)
        # LW $v0 ($2), 0($a2) (op=0x23, rs=6, rt=2)
        lw_after_mfc1 = (0x23 << 26) | (6 << 21) | (2 << 16) | 0x0000

        sig3, refs3 = C.analyse([lui_a2, mfc1_a2, lw_after_mfc1])
        self.assertEqual(refs3, [("hi", 0, 0x80010000)])
        self.assertEqual(sig3[2], lw_after_mfc1)


class OperandDecodingTests(unittest.TestCase):
    """Only instructions whose rs is a GPR operand complete a lui; every GPR write ends the lui's value."""
    LUI_S0 = (0x0F << 26) | (16 << 16) | 0x0809          # lui s0, 0x0809
    LUI_T0 = (0x0F << 26) | (8 << 16) | 0x0809           # lui t0, 0x0809
    LW_A0_4_T0 = (0x23 << 26) | (8 << 21) | (4 << 16) | 4  # lw a0, 4(t0)

    @staticmethod
    def fpu(funct: int) -> int:                         # <op>.s $f0, $f0, $f12: rs field = fmt S = 16 (s0's number)
        return (0x11 << 26) | (16 << 21) | (12 << 16) | funct

    @staticmethod
    def ext(rt: int, rs: int, pos: int, size: int) -> int:
        return (0x1F << 26) | (rs << 21) | (rt << 16) | ((size - 1) << 11) | (pos << 6)

    def test_fpu_arithmetic_is_not_a_low_half(self) -> None:
        add_sig, add_refs = C.analyse([self.LUI_S0, self.fpu(0x00), 0x03E00008, 0])   # add.s
        mul_sig, _ = C.analyse([self.LUI_S0, self.fpu(0x02), 0x03E00008, 0])          # mul.s
        self.assertNotEqual(add_sig, mul_sig)
        self.assertEqual(add_refs, [("hi", 0, 0x08090000)])

    def test_ext_field_is_not_a_low_half(self) -> None:
        sig0, refs = C.analyse([self.LUI_T0, self.ext(9, 8, 0, 8)])
        sig8, _ = C.analyse([self.LUI_T0, self.ext(9, 8, 8, 8)])
        self.assertNotEqual(sig0, sig8)
        self.assertEqual(refs, [("hi", 0, 0x08090000)])

    def test_branch_likely_offset_is_not_a_low_half(self) -> None:
        beql = lambda off: (0x14 << 26) | (8 << 21) | off  # noqa: E731  beql t0, zero, off
        self.assertNotEqual(C.analyse([self.LUI_T0, beql(2)])[0], C.analyse([self.LUI_T0, beql(3)])[0])

    def test_fpu_load_is_still_a_low_half(self) -> None:
        lwc1 = (0x31 << 26) | (16 << 21) | 0x10           # lwc1 $f0, 0x10(s0)
        self.assertEqual(C.analyse([self.LUI_S0, lwc1])[1], [("hi", 0, 0x08090000), ("data", 1, 0x08090010)])

    def test_writes_through_ext_ll_sc_and_cfc1_end_the_high_half(self) -> None:
        writers = {"ext": self.ext(8, 9, 0, 8), "ll": (0x30 << 26) | (9 << 21) | (8 << 16),
                   "sc": (0x38 << 26) | (9 << 21) | (8 << 16), "cfc1": (0x11 << 26) | (2 << 21) | (8 << 16) | (31 << 11)}
        for name, w in writers.items():
            with self.subTest(name):
                sig, refs = C.analyse([self.LUI_T0, w, self.LW_A0_4_T0])
                self.assertEqual(refs, [("hi", 0, 0x08090000)])
                self.assertEqual(sig[2], self.LW_A0_4_T0)

    def test_seb_writes_rd_and_keeps_the_source(self) -> None:
        seb_t0_t1 = (0x1F << 26) | (9 << 16) | (8 << 11) | (0x10 << 6) | 0x20   # seb t0, t1
        _sig, refs = C.analyse([self.LUI_T0, seb_t0_t1, self.LW_A0_4_T0])
        self.assertEqual(refs, [("hi", 0, 0x08090000)])

    def test_jal_ends_a_high_half_in_ra(self) -> None:
        lui_ra = (0x0F << 26) | (31 << 16) | 0x0809
        lw_ra = (0x23 << 26) | (31 << 21) | (4 << 16) | 4
        _sig, refs = C.analyse([lui_ra, (3 << 26) | 0x100000, 0, lw_ra])
        self.assertEqual([r for r in refs if r[0] == "data"], [])


class MappingTests(unittest.TestCase):
    def test_matching_references(self) -> None:
        e_refs = [("call", 0, 0x00400000), ("data", 1, 0x08801000)]
        t_refs = [("call", 0, 0x00500000), ("data", 1, 0x08901000)]
        res = C.mapping(e_refs, t_refs)
        self.assertIsNotNone(res)
        calls, data, his = res
        self.assertEqual(calls, {0x00400000: 0x00500000})
        self.assertEqual(data, {0x08801000: 0x08901000})
        self.assertEqual(his, {})

    def test_mismatched_length(self) -> None:
        e_refs = [("call", 0, 0x00400000)]
        t_refs = [("call", 0, 0x00500000), ("data", 1, 0x08901000)]
        self.assertIsNone(C.mapping(e_refs, t_refs))

    def test_mismatched_kind_or_index(self) -> None:
        e_refs = [("call", 0, 0x00400000)]
        t_refs = [("data", 0, 0x08901000)]
        self.assertIsNone(C.mapping(e_refs, t_refs))

        e_refs2 = [("call", 0, 0x00400000)]
        t_refs2 = [("call", 1, 0x00500000)]
        self.assertIsNone(C.mapping(e_refs2, t_refs2))

    def test_ambiguous_high_half_is_dropped_not_fatal(self) -> None:
        e_refs = [("hi", 0, 0x00300000), ("data", 1, 0x00301000), ("hi", 2, 0x00300000), ("data", 3, 0x00302000)]
        t_refs = [("hi", 0, 0x00310000), ("data", 1, 0x0031f000), ("hi", 2, 0x00320000), ("data", 3, 0x00320000)]
        res = C.mapping(e_refs, t_refs)
        self.assertIsNotNone(res)
        calls, data, his = res
        self.assertEqual(data, {0x00301000: 0x0031f000, 0x00302000: 0x00320000})
        self.assertEqual(his, {})

    def test_inconsistent_target_mapping(self) -> None:
        e_refs = [("call", 0, 0x00400000), ("call", 2, 0x00400000)]
        t_refs = [("call", 0, 0x00500000), ("call", 2, 0x00600000)]
        self.assertIsNone(C.mapping(e_refs, t_refs))


class MapValueTests(unittest.TestCase):
    def test_start_address(self) -> None:
        self.assertEqual(C.map_value("FUN", 0x1000, 0x1000, 0x2000, {}, {}), 0x2000)

    def test_calls_and_data_lookup(self) -> None:
        calls = {0x00400000: 0x00500000}
        data = {0x08801000: 0x08901000}
        self.assertEqual(C.map_value("FUN", 0x00400000, 0x1000, 0x2000, calls, data), 0x00500000)
        self.assertEqual(C.map_value("DAT", 0x08801000, 0x1000, 0x2000, calls, data), 0x08901000)

    def test_address_kind_constant_shift(self) -> None:
        # DAT_08801008 is 8 bytes after DAT_08801000 (0 <= ev - value < MAX_DATA_SHIFT: 08801008 - 08801000 = 8)
        # ev=0x08801008, tv=0x08901008, ev-value=0
        data = {0x08801008: 0x08901008}
        self.assertEqual(C.map_value("DAT", 0x08801000, 0x1000, 0x2000, {}, data), 0x08901000)

    def test_unmapped_address_kind_raises_key_error(self) -> None:
        with self.assertRaises(KeyError):
            C.map_value("FUN", 0x00700000, 0x1000, 0x2000, {}, {})

    def test_non_address_kind_returns_none(self) -> None:
        self.assertIsNone(C.map_value("", 0x00700000, 0x1000, 0x2000, {}, {}))


class RetargetNamesTests(unittest.TestCase):
    def test_retarget_names(self) -> None:
        text = "void FUN_00400000(void) { DAT_08801000 = 1; unmapped_00700000 = 2; }"
        calls = {0x00400000: 0x00500000}
        data = {0x08801000: 0x08901000}
        res = C.retarget_names(text, 0x1000, 0x2000, calls, data)
        self.assertIn("FUN_00500000", res)
        self.assertIn("DAT_08901000", res)
        self.assertIn("unmapped_00700000", res)


class SymbolEntriesTests(unittest.TestCase):
    def test_symbol_entries_mapping(self) -> None:
        base = {"FUN_00400000": 0x00400000, "unmapped": 0x00700000}
        calls = {0x00400000: 0x00500000}
        res = C.symbol_entries(base, 0x1000, 0x2000, calls, {})
        self.assertEqual(res, {"FUN_00500000": 0x00500000})

    def test_symbol_entries_keep_names(self) -> None:
        base = {"__dl__FPv": 0x00400000}
        calls = {0x00400000: 0x00500000}
        res = C.symbol_entries(base, 0x1000, 0x2000, calls, {}, keep_names=True)
        self.assertEqual(res, {"__dl__FPv": 0x00500000})


class RetargetTests(unittest.TestCase):
    def test_retarget_full_source(self) -> None:
        text = (
            "/* HSTMATCH module=EBOOT.BIN start=0x00001000 end=0x00001020 rodata=0x08801000 */\n"
            "void FUN_00400000(void) {\n"
            "    int val = 0x08801000;\n"
            "}\n"
        )
        calls = {0x00400000: 0x00500000}
        data = {0x08801000: 0x08901000}
        out = C.retarget(text, 0x00001000, 0x00002000, 0x00002020, calls, data)
        self.assertIsNotNone(out)
        self.assertIn("start=0x00002000", out)
        self.assertIn("end=0x00002020", out)
        self.assertIn("rodata=0x08901000", out)
        self.assertIn("FUN_00500000", out)
        self.assertIn("0x8901000", out)

    def test_retarget_unmapped_address_returns_none(self) -> None:
        text = (
            "/* HSTMATCH module=EBOOT.BIN start=0x00001000 end=0x00001020 */\n"
            "void FUN_00700000(void) {}\n"
        )
        out = C.retarget(text, 0x00001000, 0x00002000, 0x00002020, {}, {})
        self.assertIsNone(out)

    def test_retarget_element_size(self) -> None:
        # int w[1] has max_size 0xffffffff // 4 = 0x3fffffff; the clone's max_size 0x1fffffff means 8-byte elements.
        text = (
            "/* HSTMATCH module=EBOOT.BIN start=0x00001000 end=0x00001020 */\n"
            "typedef struct { int w[1]; } elem;\n"
        )
        out = C.retarget(text, 0x00001000, 0x00002000, 0x00002020, {}, {0x3FFFFFFF: 0x1FFFFFFF})
        self.assertIsNotNone(out)
        self.assertIn("int w[2]", out)
        self.assertNotIn("int w[1]", out)

    def test_retarget_literal_from_a_lone_lui(self) -> None:
        # The high half 0x0880 is built by a lui whose low half is elsewhere: the literal keeps its low half and takes
        # the clone's high half. A small literal is left alone.
        text = (
            "/* HSTMATCH module=EBOOT.BIN start=0x00001000 end=0x00001020 */\n"
            "u32 f(void) { return 0x08801234 + 0x1234; }\n"
        )
        out = C.retarget(text, 0x00001000, 0x00002000, 0x00002020, {}, {}, his={0x08800000: 0x08900000})
        self.assertIsNotNone(out)
        self.assertIn("return 0x8901234 + 0x1234;", out)


class HelperRangeTests(unittest.TestCase):
    HEAD = ("/* HSTMATCH module=EBOOT.BIN start=0x00001000 end=0x00001020 symbol=FUN_00001000 "
            "helpers=FUN_00001020@0x00001020-0x0000104c */\n")

    def test_helper_range_follows_the_call_mapping(self) -> None:
        text = self.HEAD + "static void FUN_00001020(void) {}\nvoid FUN_00001000(void) { FUN_00001020(); }\n"
        out = C.retarget(text, 0x1000, 0x2000, 0x2020, {0x00001020: 0x00003000}, {})
        self.assertIn("helpers=FUN_00003000@0x00003000-0x0000302c", out)
        self.assertNotIn("0x00001020", out)

    def test_placed_data_keys_follow_the_data_mapping(self) -> None:
        text = ("/* HSTMATCH module=EBOOT.BIN start=0x00001000 end=0x00001020 data=0x002d8af8 rodata=0x002c0000 */\n"
                "void FUN_00001000(void) {}\n")
        out = C.retarget(text, 0x1000, 0x2000, 0x2020, {}, {0x002d8af8: 0x0037f8c4, 0x002c0000: 0x00360000})
        self.assertIn("data=0x0037f8c4", out)
        self.assertIn("rodata=0x00360000", out)

    def test_unmapped_helper_refuses_the_clone(self) -> None:
        text = self.HEAD + "void FUN_00001000(void) {}\n"
        self.assertIsNone(C.retarget(text, 0x1000, 0x2000, 0x2020, {}, {}))


if __name__ == "__main__":
    unittest.main()
