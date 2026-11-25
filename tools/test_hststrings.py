#!/usr/bin/env python3
"""Tests for the retail string-reference scanner (synthetic code only)."""
from __future__ import annotations

import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hststrings as S  # noqa: E402

T0, T1, T2, A0, RA = 8, 9, 10, 4, 31


def i_type(op: int, rs: int, rt: int, imm: int) -> int:
    return (op << 26) | (rs << 21) | (rt << 16) | (imm & 0xFFFF)


def r_type(rs: int, rt: int, rd: int, funct: int, sa: int = 0, op: int = 0) -> int:
    return (op << 26) | (rs << 21) | (rt << 16) | (rd << 11) | (sa << 6) | funct


def lui(rt: int, imm: int) -> int:
    return i_type(0x0F, 0, rt, imm)


def addiu(rt: int, rs: int, imm: int) -> int:
    return i_type(0x09, rs, rt, imm)


def scan(code: list[int]) -> list[tuple[int, str]]:
    """Code at 0x10; "FAKE" at address 0 (what a stale `lui t0, 0` would point at), "TEXT" at 0x40."""
    data = bytearray(0x80)
    data[0:5] = b"FAKE\0"
    data[0x40:0x45] = b"TEXT\0"
    for k, w in enumerate(code):
        data[0x10 + 4 * k:0x14 + 4 * k] = w.to_bytes(4, "little")
    return S.strings_in(bytes(data), {"start": 0, "file_offset": 0, "file_end": len(data)}, 0x10, 0x10 + 4 * len(code))


class StringReferenceTests(unittest.TestCase):
    def test_lui_addiu_pair(self) -> None:
        self.assertEqual(scan([lui(T0, 0), addiu(A0, T0, 0x40)]), [(0x14, "TEXT")])

    def test_store_keeps_the_high_half(self) -> None:
        sw = i_type(0x2B, T0, T1, 0x40)                 # sw t1, 0x40(t0) reads t0 and writes no register
        self.assertEqual(scan([lui(T0, 0), sw, addiu(A0, T0, 0x40)]), [(0x14, "TEXT"), (0x18, "TEXT")])

    def test_syscall_code_bits_keep_the_high_half(self) -> None:
        self.assertEqual(scan([lui(T0, 0), 0x0000080C, addiu(A0, T0, 0x40)]), [(0x18, "TEXT")])

    def test_lui_zero_is_not_tracked(self) -> None:
        self.assertEqual(scan([lui(0, 0), addiu(A0, 0, 0)]), [])

    def test_every_gpr_write_drops_the_stale_high_half(self) -> None:
        writers = {
            "addu": r_type(T1, T2, T0, 0x21),
            "sll": r_type(0, T1, T0, 0x00, 2),
            "clz": r_type(T1, 0, T0, 0x16),
            "max": r_type(T1, T2, T0, 0x2C),
            "addi": i_type(0x08, T1, T0, 1),
            "slti": i_type(0x0A, T1, T0, 1),
            "andi": i_type(0x0C, T1, T0, 1),
            "xori": i_type(0x0E, T1, T0, 1),
            "lb": i_type(0x20, T1, T0, 0),
            "lwl": i_type(0x22, T1, T0, 3),
            "lwr": i_type(0x26, T1, T0, 0),
            "ll": i_type(0x30, T1, T0, 0),
            "sc": i_type(0x38, T1, T0, 0),
            "mfc1": (0x11 << 26) | (T0 << 16) | (12 << 11),
            "cfc1": (0x11 << 26) | (2 << 21) | (T0 << 16) | (31 << 11),
            "seb": r_type(0, T1, T0, 0x20, 0x10, 0x1F),
            "wsbh": r_type(0, T1, T0, 0x20, 0x02, 0x1F),
            "ext": (0x1F << 26) | (T1 << 21) | (T0 << 16) | (7 << 11),
            "ins": (0x1F << 26) | (T1 << 21) | (T0 << 16) | (7 << 11) | 0x04,
        }
        for name, w in writers.items():
            with self.subTest(name):
                # lui t0, 0; <t0 overwritten>; addiu a0, t0, 0: no reference to "FAKE" at address 0
                self.assertEqual(scan([lui(T0, 0), w, addiu(A0, T0, 0)]), [])

    def test_calls_drop_a_high_half_in_the_link_register(self) -> None:
        jal = (0x03 << 26) | (0x1000 >> 2)
        bal = i_type(0x01, 0, 0x11, 4)
        for name, w in (("jal", jal), ("bal", bal)):
            with self.subTest(name):
                self.assertEqual(scan([lui(RA, 0), w, 0, addiu(A0, RA, 0)]), [])


if __name__ == "__main__":
    unittest.main()
