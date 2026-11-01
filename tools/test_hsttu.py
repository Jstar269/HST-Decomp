#!/usr/bin/env python3
"""Tests for the translation-unit recovery (hsttu.py): synthetic modules only, no game needed."""
from __future__ import annotations

import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hsttu as T

SEGMENTS = [{"file_offset": 0x10, "file_end": 0x100, "start": 0x1000}]


def module(functions: list[tuple[int, list[int]]], names: dict[int, str], sigs: dict[int, tuple] | None = None) -> dict:
    """functions: (start, [addresses it loads]); every function is 8 bytes."""
    return {"functions": [(s, s + 8, [("data", 0, a) for a in loads]) for s, loads in functions],
            "names": names, "signatures": sigs or {}}


class FileNameTests(unittest.TestCase):
    def test_file_names_are_found_at_their_addresses(self) -> None:
        data = bytearray(0x100)
        data[0x20:0x20 + 16] = b"chr_manager.cpp\0"
        data[0x40:0x40 + 9] = b"sgxsys.c\0"
        data[0x50:0x50 + 8] = b"Thing.h\0"                 # headers are not units
        data[0x60:0x60 + 10] = b"%s/%s.cpp\0"              # a format string is not a file name
        self.assertEqual(T.file_names(bytes(data), SEGMENTS), {0x1010: "chr_manager.cpp", 0x1030: "sgxsys.c"})


class RecoverTests(unittest.TestCase):
    def test_strings_then_link_order(self) -> None:
        names = {0x9000: "a.cpp", 0x9010: "b.cpp"}
        sharing = module([(0x00, [0x9000]), (0x08, []), (0x10, [0x9000]), (0x18, []), (0x20, [0x9010]), (0x28, [])],
                         names)
        eboot = module([(0x0, [])], {})
        got = T.recover({T.SHARING: sharing, T.EBOOT: eboot}, {})
        self.assertEqual(got[T.SHARING], {0x00: ("a.cpp", "string"), 0x08: ("a.cpp", "link-order"),
                                          0x10: ("a.cpp", "string"), 0x20: ("b.cpp", "string")})
        # 0x18 lies between a.cpp and b.cpp: no evidence; 0x28 after b.cpp's last evidence: none either

    def test_two_names_in_one_function_and_sdk_functions_stay_unassigned(self) -> None:
        names = {0x9000: "a.cpp", 0x9010: "b.cpp"}
        sharing = module([(0x00, [0x9000]), (0x08, []), (0x10, [0x9000, 0x9010]), (0x18, [0x9000])], names)
        got = T.recover({T.SHARING: sharing, T.EBOOT: module([], {})}, {T.SHARING: [(0x08, 0x10)]})
        self.assertEqual(got[T.SHARING], {0x00: ("a.cpp", "string"), 0x18: ("a.cpp", "string")})
        # 0x08 is SDK library code (counted elsewhere), 0x10 names two files, and nothing fills around them but a.cpp

    def test_twins_carry_units_to_eboot_in_order_only(self) -> None:
        sharing = module([(0x00, [0x9000]), (0x08, []), (0x10, [0x9000])], {0x9000: "a.cpp"},
                         {0x00: ("x",), 0x08: ("y",), 0x10: ("z",)})
        # EBOOT: twins of 0x00 and 0x10 in the same order, plus an out-of-order lookalike that must not count
        eboot = module([(0x100, []), (0x108, []), (0x110, []), (0x118, [])], {},
                       {0x100: ("x",), 0x108: ("q",), 0x110: ("z",), 0x118: ("y",)})
        got = T.recover({T.SHARING: sharing, T.EBOOT: eboot}, {})
        self.assertEqual(got[T.EBOOT], {0x100: ("a.cpp", "twin"), 0x108: ("a.cpp", "link-order"),
                                        0x110: ("a.cpp", "twin")})

    def test_ordered_pairs_keep_the_longest_run_in_order(self) -> None:
        a = {1: ("p",), 2: ("q",), 3: ("r",), 4: ("s",), 5: ("dup",), 6: ("dup",)}
        b = {10: ("p",), 20: ("r",), 30: ("q",), 40: ("s",), 50: ("dup",)}
        pairs = T.ordered_pairs(a, b)
        self.assertEqual(len(pairs), 3)                     # p, then q or r, then s; "dup" is not unique in a
        self.assertEqual([pairs[0], pairs[-1]], [(1, 10), (4, 40)])

    def test_pieces_count_a_split_by_another_unit(self) -> None:
        units = {1: ("a.cpp", "string"), 2: ("b.cpp", "string"), 3: ("a.cpp", "string"), 5: ("b.cpp", "string")}
        self.assertEqual(T.pieces([1, 2, 3, 4, 5], units), {"a.cpp": 2, "b.cpp": 2})
        self.assertEqual(T.pieces([1, 4, 3], {1: ("a.cpp", "string"), 3: ("a.cpp", "string")}), {"a.cpp": 1})


class FileTests(unittest.TestCase):
    def test_render_and_read_round_trip(self) -> None:
        units = {T.SHARING: {0x10: ("a.cpp", "string"), 0x18: ("a.cpp", "link-order"), 0x40: ("b.c", "string")},
                 T.EBOOT: {0x100: ("a.cpp", "twin")}}
        text = T.render(T.document(units))
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "tu.json"
            path.write_text(text, encoding="utf-8")
            self.assertEqual(T.read(path), {T.EBOOT: {0x100: "a.cpp"},
                                            T.SHARING: {0x10: "a.cpp", 0x18: "a.cpp", 0x40: "b.c"}})
            self.assertEqual(T.read(Path(tmp) / "missing.json"), {})

    def test_committed_file_is_well_formed(self) -> None:
        if not T.TU_PATH.exists():
            self.skipTest("no data/translation-units.json")
        units = T.read()
        for name, starts in units.items():
            self.assertIn(name, (T.SHARING, T.EBOOT))
            self.assertTrue(all(u.endswith((".c", ".cpp", ".cc")) and "/" not in u for u in starts.values()))
            self.assertEqual(T.pieces(sorted(starts), {s: (u, "") for s, u in starts.items()}),
                             {u: 1 for u in set(starts.values())})


if __name__ == "__main__":
    unittest.main()
