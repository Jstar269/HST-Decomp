#!/usr/bin/env python3
"""Synthetic tests for the whole-module build (hstbuild.py, hstreloc.py): ELF rewriting, tiling, relocation tables
and their order, relocatable scaffolding, the shifted-build expectation, the census and the container."""
from __future__ import annotations

import argparse
import io
import shutil
import struct
import sys
import tempfile
import threading
import time
import unittest
from contextlib import redirect_stdout
from unittest import mock
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hstbuild as B
import hstmatch as M
import hstreloc as R


def make_object(texts: list[bytes], symbols: list[tuple[str, int, int, int, int]], relocs: dict[int, list[tuple[int, int, int]]],
                rodata: bytes = b"") -> bytes:
    """A relocatable object: `texts` are .text sections (section index 1..n), `symbols` are
    (name, value, size, info, shndx), `relocs[section index]` = [(offset, symbol index, type)]."""
    secs = [B.Sec("", 0, 0, 0, 0, 0, 0, 0, 0, 0, b"")]
    for t in texts:
        secs.append(B.Sec(".text", B.SHT_PROGBITS, 6, 0, 0, len(t), 0, 0, 4, 0, t))
    ro_idx = None
    if rodata:
        secs.append(B.Sec(".rodata", B.SHT_PROGBITS, 2, 0, 0, len(rodata), 0, 0, 8, 0, rodata))
        ro_idx = len(secs) - 1
    rel_idx = {}
    for sh, rows in relocs.items():
        data = b"".join(struct.pack("<II", off, (sym << 8) | typ) for off, sym, typ in rows)
        secs.append(B.Sec(".rel.text", B.SHT_REL, 0, 0, 0, len(data), 0, sh, 4, 8, data))
        rel_idx[sh] = len(secs) - 1
    strtab = bytearray(b"\0")
    symdata = bytearray(B.SYM.pack(0, 0, 0, 0, 0, 0))
    for name, value, size, info, shndx in symbols:
        off = len(strtab)
        strtab += name.encode() + b"\0"
        symdata += B.SYM.pack(off, value, size, info, 0, shndx)
    secs.append(B.Sec(".symtab", B.SHT_SYMTAB, 0, 0, 0, len(symdata), len(secs) + 1, 1, 4, 16, bytes(symdata)))
    symtab_idx = len(secs) - 1
    secs.append(B.Sec(".strtab", B.SHT_STRTAB, 0, 0, 0, len(strtab), 0, 0, 1, 0, bytes(strtab)))
    secs.append(B.Sec(".shstrtab", B.SHT_STRTAB, 0, 0, 0, 0, 0, 0, 1, 0, b""))
    for sh, ri in rel_idx.items():
        secs[ri].link = symtab_idx
    ehdr = (b"\x7fELF\x01\x01\x01" + b"\0" * 9, 1, 8, 1, 0, 0, 0, 0x10a23001, 52, 0, 0, 40, len(secs), len(secs) - 1)
    return B.build_elf(B.Elf(ehdr, secs))


GLOBAL_FUNC = (1 << 4) | 2
HEADER = {"module": "EBOOT.BIN", "start": 0x1000, "end": 0x1010, "symbol": "FUN_00001000"}


class RoundTripTests(unittest.TestCase):
    def test_build_then_parse_keeps_sections_and_data(self) -> None:
        obj = make_object([b"\x01" * 16], [("FUN_00001000", 0, 16, GLOBAL_FUNC, 1)], {})
        elf = B.parse_elf(obj)
        self.assertEqual([s.name for s in elf.secs], ["", ".text", ".symtab", ".strtab", ".shstrtab"])
        self.assertEqual(elf.by_name(".text").data, b"\x01" * 16)

    def test_aligned_layout_matches_how_ld_places_sections(self) -> None:
        self.assertEqual(B.aligned_layout([(6, 4), (4, 8), (1, 1), (4, 4)]), [0, 8, 12, 16])


class PrepareTests(unittest.TestCase):
    def test_a_helper_outside_the_header_span_is_dropped_and_its_symbol_is_defined_by_the_layout(self) -> None:
        # section 1 = the target at 0x1000, section 2 = a weak copy that lands at 0x1010 (outside the span)
        obj = make_object([b"\xaa" * 16, b"\xbb" * 8],
                          [("FUN_00001000", 0, 16, GLOBAL_FUNC, 1), ("helper", 0, 8, GLOBAL_FUNC, 2), ("ext", 0, 0, 1 << 4, 0)],
                          {1: [(4, 2, 4), (8, 3, 4)]})
        new, prep = B.prepare_object(Path("src/eboot/x.c"), HEADER, obj, "o00001")
        elf = B.parse_elf(new)
        self.assertEqual([s.name for s in elf.secs if s.name.startswith(".hst")], [".hst.text"])
        self.assertEqual(elf.by_name(".hst.text").size, 16)
        _idx, syms = B.parse_symbols(elf)
        strtab = elf.by_name(".strtab").data
        by_name = {B.strtab_get(strtab, s[0]): s for s in syms[1:]}
        name, helper = next((k, v) for k, v in by_name.items() if k.startswith("__hst_dropped_o00001"))
        self.assertEqual((helper[1], helper[5]), (0, B.SHN_UNDEF))               # the link defines it ...
        self.assertEqual(prep.dropped[name], (0x1010, None))                     # ... at its isolated address
        self.assertEqual(prep.globals_defined, {"FUN_00001000"})
        self.assertEqual(prep.undefined, {"ext"})
        self.assertEqual((prep.text_addr, prep.text_size), (0x1000, 16))
        self.assertEqual(prep.relocs, [(0x1004, 4), (0x1008, 4)])

    def test_a_function_inside_a_shared_text_section_is_sliced_with_its_relocations(self) -> None:
        # one .text with two functions (a library file compiled whole): the target is the second, at offset 16
        header = dict(HEADER, start=0x2000, end=0x2010, symbol="FUN_00002000")
        obj = make_object([b"\x11" * 16 + b"\x22" * 16],
                          [("first", 0, 16, GLOBAL_FUNC, 1), ("FUN_00002000", 16, 16, GLOBAL_FUNC, 1), ("sect", 0, 0, 3, 1)],
                          {1: [(4, 3, 4), (20, 3, 4), (24, 2, 4)]})
        new, prep = B.prepare_object(Path("src/eboot/lib.c"), header, obj, "o00002")
        elf = B.parse_elf(new)
        text = elf.by_name(".hst.text")
        self.assertEqual(text.data, b"\x22" * 16)
        rel = next(s for s in elf.secs if s.type == B.SHT_REL)
        rows = [struct.unpack_from("<II", rel.data, o) for o in range(0, len(rel.data), 8)]
        self.assertEqual([r[0] for r in rows], [4, 8])                            # offsets 20 and 24, rebased by 16
        _idx, syms = B.parse_symbols(elf)
        strtab = elf.by_name(".strtab").data
        named = {B.strtab_get(strtab, s[0]): s for s in syms[1:]}
        target = named["FUN_00002000"]
        self.assertEqual((target[1], target[5]), (0, elf.secs.index(text)))        # rebased to the slice
        section_symbol = syms[3]                                                    # the original .text section symbol
        self.assertEqual((section_symbol[1], section_symbol[5]), (0, B.SHN_UNDEF))
        sym_name = B.strtab_get(strtab, section_symbol[0])
        self.assertEqual(prep.dropped[sym_name], (0x2000 - 16, 0x2000))            # section start, anchored to this tile
        self.assertEqual(prep.records["text"], [(0x2004, 4, "sect"), (0x2008, 4, "FUN_00002000")])
        self.assertEqual((prep.text_addr, prep.text_size), (0x2000, 16))
        self.assertEqual(prep.relocs, [(0x2004, 4), (0x2008, 4)])

    def test_placed_rodata_is_kept_and_other_data_is_not(self) -> None:
        header = dict(HEADER, rodata="0x2000")
        obj = make_object([b"\xaa" * 16], [("FUN_00001000", 0, 16, GLOBAL_FUNC, 1)], {1: [(0, 1, 5)]}, rodata=b"\x01" * 12)
        new, prep = B.prepare_object(Path("src/eboot/d.c"), header, obj, "o00003")
        elf = B.parse_elf(new)
        self.assertEqual(elf.by_name(".hst.rodata").size, 12)
        self.assertEqual(prep.data, [("rodata", 0x2000, 12)])
        no_header = B.prepare_object(Path("src/eboot/d.c"), HEADER, obj, "o00004")[0]
        self.assertIsNone(B.parse_elf(no_header).by_name(".hst.rodata"))

    def test_a_target_outside_its_span_is_refused(self) -> None:
        obj = make_object([b"\xaa" * 16], [("OTHER", 0, 16, GLOBAL_FUNC, 1)], {})
        with self.assertRaises(B.Excluded):
            B.prepare_object(Path("src/eboot/x.c"), HEADER, obj, "o00005")


class WordsTests(unittest.TestCase):
    def test_words_asm_is_little_endian_and_complete(self) -> None:
        text = B.words_asm(bytes.fromhex("e0ffbd27") + bytes.fromhex("1c00bfaf"))
        self.assertEqual(text.strip(), ".word 0x27bdffe0,0xafbf001c")
        self.assertEqual(B.words_asm(b"\0" * 64).count(".word"), 2)             # 32 bytes per line


class IsolationTests(unittest.TestCase):
    def test_paths_are_split_into_calls_under_the_budget_in_order(self) -> None:
        files = [f"src/eboot/FUN_{i:08x}.c" for i in range(6000)]
        parts = B.chunks(files)
        self.assertGreater(len(parts), 1)
        self.assertEqual([f for p in parts for f in p], files)
        self.assertTrue(all(sum(len(f) + 1 for f in p) <= B.ARG_BUDGET for p in parts))
        self.assertEqual(B.chunks(["x" * (B.ARG_BUDGET + 5)]), [["x" * (B.ARG_BUDGET + 5)]])   # one long path still runs
        self.assertEqual(B.chunks([]), [])


# --------------------------------------------------------------------------------------------------------------
# relocation tables, ordering, scaffolding, the shifted build and the container (tools/hstreloc.py and hstbuild.py)

def lui(rt: int, imm: int) -> int:
    return 0x3C000000 | (rt << 16) | (imm & 0xFFFF)


def addiu(rt: int, rs: int, imm: int) -> int:
    return 0x24000000 | (rs << 21) | (rt << 16) | (imm & 0xFFFF)


def lw(rt: int, rs: int, imm: int) -> int:
    return 0x8C000000 | (rs << 21) | (rt << 16) | (imm & 0xFFFF)


def jal(target: int) -> int:
    return 0x0C000000 | ((target >> 2) & 0x03FFFFFF)


def words(*ws: int) -> bytes:
    return b"".join(struct.pack("<I", w) for w in ws)


class RelocationFormatTests(unittest.TestCase):
    def test_format_a_round_trip_and_the_info_layout(self) -> None:
        entries = [R.Entry(0x1234, R.R_MIPS_HI16), R.Entry(0x1238, R.R_MIPS_LO16, 0, 1), R.Entry(0x40, R.R_MIPS_32, 1, 0)]
        blob = R.encode_a(entries)
        self.assertEqual(len(blob), 24)
        self.assertEqual(struct.unpack_from("<II", blob, 8), (0x1238, 0x00010006))      # type | oseg << 8 | aseg << 16
        self.assertEqual(struct.unpack_from("<II", blob, 16), (0x40, 0x00000102))
        self.assertEqual(R.decode_a(blob, nseg=2), entries)

    def test_a_table_that_breaks_the_layout_is_refused(self) -> None:
        with self.assertRaises(ValueError):
            R.decode_a(struct.pack("<II", 0, 0x01000002))                                # bits 24-31 set
        with self.assertRaises(ValueError):
            R.decode_a(b"\0" * 12)                                                         # not a multiple of 8
        with self.assertRaises(ValueError):
            R.decode_a(struct.pack("<II", 0, 0x00000106), nseg=1)                         # offset segment 1 of 1
        with self.assertRaises(ValueError):
            R.decode_a(struct.pack("<II", 0, 0x00000003))                                 # R_MIPS_REL32: unknown here

    def test_hi16_partners_and_a_run_without_partner(self) -> None:
        t = [R.Entry(0, 5), R.Entry(4, 5), R.Entry(8, 6), R.Entry(12, 4), R.Entry(16, 5), R.Entry(20, 6)]
        self.assertEqual(R.partner_index(t), [2, 2, None, None, 5, None])
        with self.assertRaises(ValueError):
            R.partner_index([R.Entry(0, 6), R.Entry(4, 5)])

    def test_format_b_commands_decode_to_records(self) -> None:
        f, t = 2, 3                      # flag-index bits, kind-index bits; 2 segments: 1 segment bit; 10 displacement bits
        flags = bytes([3, 0x00, 0x01])  # [count, set offset, relocate with a displacement and no addend]
        kinds = bytes([3, 2, 7])        # [count, kind 2 (R_MIPS_32), kind 7 (jal)]
        cmds = [1 | (0 << 2) | (0x40 << 3),                       # offset segment 0, offset 0x40
                2 | (1 << 2) | (1 << 3) | (0 << 6),                # relocate +0 there: R_MIPS_32 into segment 1
                2 | (0 << 2) | (2 << 3) | (8 << 6)]                # relocate +8: jal into segment 0
        blob = bytes([0, 0, f, t]) + flags + kinds + b"".join(struct.pack("<H", c) for c in cmds)
        self.assertEqual(R.decode_b(blob, nseg=2), [R.EntryB(0, 0x40, 2, 1, 0), R.EntryB(0, 0x48, 7, 0, 0)])
        self.assertEqual([e.type for e in R.decode_b(blob, nseg=2)], [R.R_MIPS_32, R.R_MIPS_26])


class TargetAndOrderTests(unittest.TestCase):
    # 0x00 lui v0,0x1 / 0x04 addiu a0,v0,-0x7ffc (-> 0x8004) / 0x08 jal 0x40 / 0x0c lw a1,0x10(v0) (a standalone LO16)
    CODE = words(lui(2, 1), addiu(4, 2, -0x7FFC), jal(0x40), lw(5, 2, 0x10))

    def word_at(self, a: int) -> int:
        return struct.unpack_from("<I", self.CODE, a)[0]

    def test_targets_follow_the_loader_and_a_standalone_lo16_takes_its_register_s_lui(self) -> None:
        table = [R.Entry(0, 5), R.Entry(4, 6), R.Entry(8, 4), R.Entry(12, 6)]
        got = R.recover_targets(table, self.word_at, lambda a: (0, 0x10))
        self.assertEqual(got, [0x8004, 0x8004, 0x40, 0x10010])

    def test_rel_text_is_one_block_per_tile_with_a_hi16_in_its_partner_s_block(self) -> None:
        # tile A at 0x00, tile B at 0x20. A HI16 run (two lui feeding one LO16: the 50 exceptions of the retail table)
        # stays in A's block; A's HI16 at 0x1c sits in front of its partner at 0x28, in B's block.
        table = [R.Entry(0x04, 4), R.Entry(0x08, 5), R.Entry(0x0C, 6), R.Entry(0x10, 5), R.Entry(0x14, 5),
                 R.Entry(0x18, 6), R.Entry(0x24, 4), R.Entry(0x1C, 5), R.Entry(0x28, 6)]
        owners = R.block_owner(table, lambda a: 0x20 if a >= 0x20 else 0x00)
        self.assertEqual(owners, [0, 0, 0, 0, 0, 0, 0x20, 0x20, 0x20])
        runs = R.runs_of(owners)
        self.assertEqual(runs, [(0, 0, 6), (0x20, 6, 3)])
        self.assertTrue(R.is_address_ordered(runs))
        # .rel.data is not: one object's data relocations interleave with another tile's
        self.assertFalse(R.is_address_ordered(R.runs_of([0x100, 0x200, 0x100])))
        self.assertFalse(R.is_address_ordered(R.runs_of([0x200, 0x100])))


def mini_retail(text: bytes, data: bytes, data_addr: int = 0x100) -> B.Retail:
    off_t, off_d = 0x54, 0x54 + data_addr
    blob = bytearray(off_d + len(data))
    blob[off_t:off_t + len(text)] = text
    blob[off_d:off_d + len(data)] = data
    secs = [B.Sec(".text", B.SHT_PROGBITS, 6, 0, off_t, len(text), 0, 0, 16, 1),
            B.Sec(".data", B.SHT_PROGBITS, 3, data_addr, off_d, len(data), 0, 0, 16, 1)]
    elf = B.Elf((b"\x7fELF" + bytes(12),) + (0,) * 13, [B.Sec("", 0, 0, 0, 0, 0, 0, 0, 0, 0)] + secs)
    return B.Retail(bytes(blob), elf, secs, [], [(off_t, 0, len(blob) - off_t, len(blob) - off_t, 7)])


class ScaffoldingTests(unittest.TestCase):
    TEXT = words(lui(2, 0), addiu(2, 2, 0x104), jal(0x10), 0, 0x03E00008, 0, 0, 0)
    DATA = words(0x10, 0x108, 0, 0)

    def setup_module(self):
        retail = mini_retail(self.TEXT, self.DATA)
        report = {"excluded": []}
        sections, tiles, bss_claims, _claims = B.plan_tiles(retail, [], [(0, 0x10), (0x10, 0x20)], report, Path("."))
        anchors = B.Anchors(tiles, sections, [], bss_claims)
        table_t = [R.Entry(0, 5), R.Entry(4, 6), R.Entry(8, 4)]
        table_d = [R.Entry(0x100, 2), R.Entry(0x104, 2)]

        def word(a: int) -> int:
            return B.retail_word(retail, a)
        recs = [B.RetailRel(".rel.text", k, e.offset, e.type, t)
                for k, (e, t) in enumerate(zip(table_t, R.recover_targets(table_t, word)))]
        recs += [B.RetailRel(".rel.data", k, e.offset, e.type, t)
                 for k, (e, t) in enumerate(zip(table_d, R.recover_targets(table_d, word)))]
        return retail, sections, tiles, anchors, recs, {".rel.text": table_t, ".rel.data": table_d}

    def test_relocated_words_become_cleared_fields_with_a_reloc_against_a_layout_symbol(self) -> None:
        retail, sections, tiles, anchors, recs, _t = self.setup_module()
        self.assertEqual([(t.start, t.size, t.kind) for t in tiles],
                         [(0, 0x10, "asm"), (0x10, 0x10, "asm"), (0x100, 0x10, "blob")])
        with tempfile.TemporaryDirectory() as tmp:
            out = Path(tmp)
            counts = B.write_scaffold(out, retail, tiles, recs, anchors, {".text": 6, ".data": 3})
            asm = (out / "scaffold-text.s").read_text(encoding="utf-8")
            self.assertEqual(counts["relocated_words"], 5)
            self.assertIn(".word 0x3c020000,0x24420000,0x0c000000,0x00000000", asm)   # fields cleared
            self.assertIn(".reloc 0x0, R_MIPS_HI16, __hst_r_00000104", asm)
            self.assertIn(".reloc 0x8, R_MIPS_26, __hst_r_00000010", asm)
            self.assertIn(".reloc 0x4, R_MIPS_32, __hst_r_00000108", asm)
            self.assertEqual((out / "scaffold-data.bin").read_bytes()[:8], bytes(8))     # both data words cleared
            B.write_symbols(out, "NO-SUCH-MODULE", [], recs, anchors)
            syms = (out / "syms.ld").read_text(encoding="utf-8")
            self.assertIn("__hst_r_00000010 = __hst_at_00000010 + 0x0;", syms)            # tile + offset, so it moves
            self.assertIn("__hst_r_00000108 = __hst_at_00000100 + 0x8;", syms)
            B.write_link_script(out / "link.ld", out, sections, tiles, [], [], R.Shift(0x80, 0x100))
            ld = (out / "link.ld").read_text(encoding="utf-8")
            self.assertIn(".text 0x00000080 :", ld)
            self.assertIn(".data 0x00000200 :", ld)                                        # padding n ahead of each

    def test_anchors_cover_tiles_section_ends_and_the_zero_initialised_area(self) -> None:
        tiles = [B.Tile(".text", 0, 0x10, "asm", "a"), B.Tile(".text", 0x10, 0x10, "text", "o1")]
        secs = [B.Sec(".text", B.SHT_PROGBITS, 6, 0, 0, 0x20, 0, 0, 16, 1)]
        a = B.Anchors(tiles, secs, [(0x100, 0x180)], [B.Tile(".bss0", 0x140, 8, "bss", "o2")])
        self.assertEqual(a.of(0x14), ("__hst_at_00000010", 4))
        self.assertEqual(a.of(0x20), ("__hst_at_00000010", 0x10))                          # one past the end of .text
        self.assertEqual(a.of(0x144), ("__hst_at_00000140", 4))
        self.assertEqual(a.of(0x120), ("__hst_bss0", 0x20))
        self.assertIsNone(a.of(0x8000))
        self.assertEqual(a.expr(0x0C, prefer=0x10), "__hst_at_00000010 - 0x4")
        r = B.RetailRel("format B", 0, 0, R.R_MIPS_32, 0x14, base=0x10)
        self.assertEqual(B.target_definition(r, a), "(__hst_at_00000010 + 0x4) - (__hst_at_00000010 + 0x0)")
        far = B.RetailRel("format B", 0, 0, R.R_MIPS_32, 0x7FF0 + 0x10, base=0x10)
        self.assertEqual(B.target_definition(far, a), "0x7ff0")                             # gp: a segment-relative constant

    def test_a_reloc_scaffolding_tile_moves_under_shift(self) -> None:
        """Assembles and links the synthetic module with psp-as/psp-ld (skipped without the configured toolchain)."""
        try:
            M.pspdev_bin()
        except SystemExit as exc:  # hstconfig.ConfigError subclasses SystemExit
            self.skipTest(f"no PSPDEV toolchain configured: {exc}")
        if shutil.which("wsl.exe") is None:
            self.skipTest("no WSL")
        retail, sections, tiles, anchors, recs, tables = self.setup_module()
        (B.ROOT / "build").mkdir(exist_ok=True)
        with tempfile.TemporaryDirectory(dir=B.ROOT / "build") as tmp:
            out = Path(tmp)
            obj = out / "obj"
            obj.mkdir()
            B.write_scaffold(out, retail, tiles, recs, anchors, {".text": 6, ".data": 3})
            B.write_symbols(out, "NO-SUCH-MODULE", [], recs, anchors)
            (out / "objs.rsp").write_text("scaffold-text.o\n", encoding="utf-8", newline="\n")
            for n, name in ((0, "base.elf"), (0x80, "shift.elf")):
                B.write_link_script(out / f"{name}.ld", out, sections, tiles, [], [], R.Shift(n, 0x100))
                path = B.run_link(out, obj, f"{name}.ld", name, assemble=(n == 0))
                self.assertIsNotNone(path, B.link_log(out, ("as.log", f"{name}.log")))
                linked = B.parse_elf(path.read_bytes())
                want = R.expected_shift([(".text", 0, self.TEXT), (".data", 0x100, self.DATA)], list(tables.values()),
                                        lambda a: (0, 0x20), R.Shift(n, 0x100))
                self.assertEqual(linked.by_name(".text").data, want[".text"][1])
                self.assertEqual(linked.by_name(".data").data, want[".data"][1])
                self.assertEqual(linked.by_name(".data").addr, 0x100 + 2 * n)
            moved = B.parse_elf((out / "shift.elf").read_bytes()).by_name(".data").data
            self.assertEqual(struct.unpack_from("<II", moved, 0), (0x10 + 0x80, 0x108 + 0x100))  # code +n, data +2n


class ShiftExpectationTests(unittest.TestCase):
    def test_every_kind_moves_with_its_target_s_region(self) -> None:
        text = words(lui(2, 0), addiu(4, 2, 0x104), jal(0x10), lw(5, 2, 0x108), 0, 0, 0, 0)
        data = words(0x10, 0x10C, 0, 0)
        table_t = [R.Entry(0, 5), R.Entry(4, 6), R.Entry(8, 4), R.Entry(12, 6)]        # the last is a standalone LO16
        table_d = [R.Entry(0x100, 2), R.Entry(0x104, 2)]
        secs = [(".text", 0, text), (".data", 0x100, data)]
        got = R.expected_shift(secs, [table_t, table_d], lambda a: (0, 0x20), R.Shift(0x80, 0x100))
        self.assertEqual(got[".text"][0], 0x80)
        self.assertEqual(got[".data"][0], 0x200)
        w = [struct.unpack_from("<I", got[".text"][1], 4 * i)[0] for i in range(4)]
        self.assertEqual(w, [lui(2, 0), addiu(4, 2, 0x204), jal(0x90), lw(5, 2, 0x208)])   # data targets +0x100, code +0x80
        self.assertEqual(struct.unpack_from("<II", got[".data"][1], 0), (0x90, 0x20C))
        same = R.expected_shift(secs, [table_t, table_d], lambda a: (0, 0x20), R.Shift(0, 0x100))
        self.assertEqual(same[".text"][1], text)

    def test_a_hi16_carry_and_the_failures_are_named(self) -> None:
        text = words(lui(2, 0), addiu(2, 2, 0x7FF0), 0, 0)                                  # target 0x7ff0, data
        got = R.expected_shift([(".text", 0, text), (".data", 0x100, bytes(0x7F00))], [[R.Entry(0, 5), R.Entry(4, 6)]],
                               lambda a: (0, 0x10), R.Shift(0x80, 0x100))
        self.assertEqual(struct.unpack_from("<II", got[".text"][1], 0), (lui(2, 1), addiu(2, 2, 0x80F0 - 0x10000)))
        with self.assertRaises(R.ShiftFailure) as cm:                                       # nothing tells code from data
            R.expected_shift([(".text", 0, words(lw(5, 9, 0x10)))], [[R.Entry(0, 6)]], lambda a: (0, 4),
                             R.Shift(0x80, 0x100))
        self.assertIn("0x00000000", str(cm.exception))
        with self.assertRaises(R.ShiftFailure):                                             # a jump leaving its 256 MB
            R.expected_shift([(".text", 0, words(jal(0x0FFFFFF0)))], [[R.Entry(0, 4)]], lambda a: (0, 4),
                             R.Shift(0x80, 0x10000000))

    def test_shifted_table_moves_every_site_and_keeps_the_order(self) -> None:
        t = [R.Entry(0x104, 2), R.Entry(0x10, 4)]
        self.assertEqual(R.shifted_table(t, R.Shift(0x80, 0x100)), [R.Entry(0x204, 2), R.Entry(0x90, 4)])


class GenerationTests(unittest.TestCase):
    def ctx(self, object_order: list[tuple[int, int, str]]) -> B.BuildContext:
        tiles = [B.Tile(".text", 0, 0x20, "asm", ".hst.t_00000000"), B.Tile(".text", 0x20, 0x20, "text", "o1")]
        table = [R.Entry(0x04, 4), R.Entry(0x24, 4), R.Entry(0x1C, 5), R.Entry(0x28, 6)]   # A's HI16 in B's block
        owners = R.block_owner(table, lambda a: 0x20 if a >= 0x20 else 0)
        recs = [B.RetailRel(".rel.text", k, e.offset, e.type, 0, owners[k]) for k, e in enumerate(table)]
        q = B.Prepared("src/b.c", "M", "o1", 0x20, 0x40, 0x20, 0x20, records={"text": object_order})
        secs = [B.Sec(".text", B.SHT_PROGBITS, 6, 0, 0, 0x40, 0, 0, 16, 1)]
        return B.BuildContext("M", None, [q], tiles, B.Anchors(tiles, secs, [], []), recs, [], {".rel.text": table},
                              secs, Path("."))

    def test_a_compiled_block_comes_from_its_object_only_when_the_object_order_is_the_table_s(self) -> None:
        same = self.ctx([(0x24, 4, "f"), (0x1C, 5, "x"), (0x28, 6, "x")])
        tables, credit, _stats = B.generate_tables(same, R.Shift(0, 0))
        self.assertEqual(tables[".rel.text"], [R.Entry(0x04, 4), R.Entry(0x24, 4), R.Entry(0x1C, 5), R.Entry(0x28, 6)])
        self.assertEqual(credit, {"src/b.c": 3})
        other = self.ctx([(0x28, 6, "x"), (0x24, 4, "f")])                                  # a different order
        tables, credit, stats = B.generate_tables(other, R.Shift(0x80, 0x1000))
        self.assertEqual(tables[".rel.text"], [R.Entry(0x84, 4), R.Entry(0xA4, 4), R.Entry(0x9C, 5), R.Entry(0xA8, 6)])
        self.assertEqual(credit, {})                                                         # taken from the scaffolding
        self.assertEqual(stats[".rel.text"]["compiled_blocks_from_scaffolding"], ["src/b.c"])


class CensusTests(unittest.TestCase):
    def test_a_source_naming_an_address_as_a_number_is_found(self) -> None:
        q = B.Prepared("src/x.c", "M", "o1", 0x100, 0x120, 0x100, 0x20,
                       records={"text": [(0x104, 5, "DAT_00300000"), (0x108, 6, "DAT_00300000")]})
        retail_sites = {0x104, 0x108, 0x110}                         # the word at 0x110 is relocated in retail only
        self.assertEqual(B.object_findings(q, retail_sites, set()),
                         [{"kind": "unrelocated-address", "count": 1, "first": "0x00000110"}])
        self.assertEqual(B.object_findings(q, {0x104, 0x108}, set()), [])
        self.assertEqual(B.object_findings(q, {0x104, 0x108}, {0x10C, 0x400}),
                         [{"kind": "moves-wrongly", "count": 1, "first": "0x0000010c"}])
        self.assertEqual(B.object_findings(q, {0x104}, set())[0]["kind"], "extra-relocation")


class MutationTests(unittest.TestCase):
    RECS = [B.RetailRel(".rel.text", 0, 0x10, R.R_MIPS_26, 0x40, 0), B.RetailRel(".rel.text", 1, 0x14, R.R_MIPS_32, 0x44, 0),
            B.RetailRel(".rel.text", 2, 0x18, R.R_MIPS_LO16, 0x48, 0)]

    def test_each_mutation_changes_one_thing(self) -> None:
        flip, _ = B.mutate_records(self.RECS, "flip", 0)
        self.assertEqual(flip[0].type, R.R_MIPS_32)
        drop, _ = B.mutate_records(self.RECS, "drop", 1)
        self.assertEqual([r.site for r in drop], [0x10, 0x18])
        swap, info = B.mutate_records(self.RECS, "swap", 0)
        self.assertEqual([(r.index, r.site) for r in sorted(swap, key=lambda r: r.index)], [(0, 0x14), (1, 0x10), (2, 0x18)])
        self.assertEqual(info["site"], "0x00000010")
        addend, _ = B.mutate_records(self.RECS, "addend", 0)
        self.assertEqual(addend[1].target, 0x48)

    def compiled_fixture(self, root: Path) -> tuple[B.Prepared, Path]:
        obj = make_object([words(0x24020001, 0x24420004, 0x03E00008, 0)],
                          [("FUN_00001000", 0, 16, GLOBAL_FUNC, 1), ("ext", 0, 0, 1 << 4, 0)],
                          {1: [(4, 2, R.R_MIPS_LO16)]})
        data, q = B.prepare_object(Path("src/eboot/test.c"), HEADER, obj, "o00006")
        obj_dir = root / "obj"
        obj_dir.mkdir()
        (obj_dir / f"{q.uid}.o").write_bytes(data)
        return q, obj_dir

    def test_compiled_addend_mutation_names_the_relocation_in_its_source_block(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            q, obj_dir = self.compiled_fixture(Path(tmp))
            path = obj_dir / f"{q.uid}.o"
            before = B.parse_elf(path.read_bytes()).by_name(".hst.text").data
            info = B.mutate_compiled_object([q], obj_dir, "addend", 0)
            after = B.parse_elf(path.read_bytes()).by_name(".hst.text").data
            self.assertEqual(info["path"], q.path)
            self.assertEqual(info["table"], ".rel.text")
            self.assertEqual(info["site"], "0x00001004")
            self.assertEqual(struct.unpack_from("<I", after, 4)[0], struct.unpack_from("<I", before, 4)[0] + 4)

    def test_compiled_instruction_mutation_skips_relocation_sites(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            q, obj_dir = self.compiled_fixture(Path(tmp))
            path = obj_dir / f"{q.uid}.o"
            before = B.parse_elf(path.read_bytes()).by_name(".hst.text").data
            info = B.mutate_compiled_object([q], obj_dir, "instruction", 0)
            after = B.parse_elf(path.read_bytes()).by_name(".hst.text").data
            self.assertEqual(info["path"], q.path)
            self.assertEqual(info["site"], "0x00001000")
            self.assertNotEqual(struct.unpack_from("<I", after, 0)[0], struct.unpack_from("<I", before, 0)[0])
            self.assertEqual(struct.unpack_from("<I", after, 4)[0], struct.unpack_from("<I", before, 4)[0])


class ContainerTests(unittest.TestCase):
    def sections(self) -> list:
        lib_ent = struct.pack("<IHHBBHI", 0, 0, 0x8000, 4, 0, 1, 0x60)
        resident = struct.pack("<II", R.MODULE_START_NID, 0x8)
        return [R.OutSection(".text", 0, words(0, 0, 0x03E00008, 0), 6, rel=R.encode_a([R.Entry(0, 4)])),
                R.OutSection(".lib.ent", 0x10, lib_ent, 2, rel=b""),
                R.OutSection(".rodata.sceModuleInfo", 0x20, bytes(0x34), 2),
                R.OutSection(".rodata.sceResident", 0x60, resident, 2),
                R.OutSection(".data", 0x70, words(0x10), 3, rel=R.encode_a([R.Entry(0x70, 2)])),
                R.OutSection(".bss", 0x80, None, 3, 128)]

    def test_the_module_file_is_laid_out_from_its_sections_by_either_profile(self) -> None:
        secs = self.sections()

        def word_at(a: int) -> int:
            s = next(s for s in secs if s.data and s.addr <= a < s.addr + len(s.data))
            return struct.unpack_from("<I", s.data, a - s.addr)[0]
        entry = R.module_start(secs[1].data, word_at)
        self.assertEqual(entry, 0x8)
        for name, link, bias, content in (("eboot", 3, 2, 0x54), ("sharing", 0, 0, 0x60)):
            image, _parts = R.write_module(R.PROFILES[name], secs, entry, b"TOOL\0", 0x100)
            elf = B.parse_elf(image)
            self.assertEqual([s.name for s in elf.secs],
                             ["", ".shstrtab", ".text", ".rel.text", ".lib.ent", ".rel.lib.ent", ".rodata.sceModuleInfo",
                              ".rodata.sceResident", ".data", ".rel.data", ".bss", ".comment"])
            self.assertEqual(elf.by_name(".text").off, content)
            self.assertEqual(elf.by_name(".data").off, content + 0x70)
            rel = elf.by_name(".rel.data")
            self.assertEqual((rel.link, rel.info, rel.entsize), (link, elf.secs.index(elf.by_name(".data")) + bias, 8))
            self.assertEqual(rel.data, R.encode_a([R.Entry(0x70, 2)]))
            self.assertEqual(elf.ehdr[4], 0x8)                                               # e_entry = module_start
            ph = struct.unpack_from("<IIIIIIII", image, 0x34)
            self.assertEqual(ph, (1, content, 0, content + 0x20, 0x74, 0x100, 7, 16))     # p_paddr: the module info
            self.assertEqual(elf.by_name(".comment").data, b"TOOL\0")
            self.assertEqual(len(image) % (4 if name == "eboot" else 1), 0)


class ExitStatusTests(unittest.TestCase):
    """A build that fails exits 1, as tools/lbpbuild.py does; the steps before the link are stand-ins."""

    def run_build(self, outcome: dict, shift: int | None = None) -> int:
        def link(_module, _retail, _prepared, _out_dir, _obj_dir, report, _args) -> None:
            report.update(outcome)
        with tempfile.TemporaryDirectory() as tmp:
            args = argparse.Namespace(out_dir=str(Path(tmp) / "out"), report_dir=str(Path(tmp) / "reports"),
                                      src_dir=None, private=True, reuse_isolation=None, parallel=1, shift=shift)
            with mock.patch.object(B, "collect_sources", return_value=[]), \
                    mock.patch.object(B, "run_isolation", return_value=[]), \
                    mock.patch.object(B, "load_retail", return_value=(None, None)), \
                    mock.patch.object(B, "link_and_compare", link), redirect_stdout(io.StringIO()):
                return B.build_locked("EBOOT.BIN", args)

    def test_an_identical_image_exits_0(self) -> None:
        self.assertEqual(self.run_build({"identical": True}), 0)

    def test_a_failed_link_exits_1(self) -> None:
        self.assertEqual(self.run_build({"link_failed": "psp-ld: undefined reference"}), 1)

    def test_an_image_that_differs_from_retail_exits_1(self) -> None:
        self.assertEqual(self.run_build({"identical": False}), 1)

    def test_a_shifted_build_that_fails_exits_1(self) -> None:
        self.assertEqual(self.run_build({"identical": True, "shifted_build_passes": False}, shift=0x1000), 1)
        self.assertEqual(self.run_build({"identical": True, "shifted_build_passes": True}, shift=0x1000), 0)


class LockTests(unittest.TestCase):
    def test_a_second_run_waits_for_the_module_lock_and_the_lock_is_removed_after_an_error(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            lock = root / "try" / "build" / "M.lock"
            lock.mkdir(parents=True)                                    # another run holds the lock
            threading.Timer(0.3, lock.rmdir).start()
            t0 = time.time()
            with B.module_lock("M", root=root, poll=0.05) as held:
                self.assertGreaterEqual(time.time() - t0, 0.25)       # it waited
                self.assertTrue(held.is_dir())
            self.assertFalse(lock.exists())
            with self.assertRaises(RuntimeError):
                with B.module_lock("M", root=root, poll=0.05):
                    raise RuntimeError("link failed")
            self.assertFalse(lock.exists())                             # released on the error too
            with B.module_lock("N", root=root, poll=0.05):              # another module is not blocked
                self.assertTrue((root / "try" / "build" / "N.lock").is_dir())


if __name__ == "__main__":
    unittest.main()
