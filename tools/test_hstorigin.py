#!/usr/bin/env python3
"""Tests for the origin classes (hstorigin.py): synthetic sources, plus the SGX block against the committed units."""
from __future__ import annotations

import json
import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hstorigin as O

EBOOT = "EBOOT.BIN"


def source(root: Path, rel: str, start: int, cc: str | None, symbol: str | None = None) -> Path:
    path = root / rel
    path.parent.mkdir(parents=True, exist_ok=True)
    extra = f" cc={cc}" if cc else ""
    path.write_text(f"/* HSTMATCH module={EBOOT} start=0x{start:08x} end=0x{start + 8:08x} "
                    f"symbol={symbol or f'FUN_{start:08x}'} lang=c99{extra} */\nint f(void) {{ return 0; }}\n",
                    encoding="utf-8")
    return path


class ClassTests(unittest.TestCase):
    def test_the_sgx_block_is_sony_middleware_whatever_the_compiler(self) -> None:
        lo, hi = O.SGX_BLOCKS[EBOOT]
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            cases = {
                source(root, "src/eboot/a.c", lo, "gcc171-O2"): O.SGX,
                source(root, "src/eboot/b.c", hi - 8, None): O.SGX,              # mwccpsp matches a few tiny ones
                source(root, "src/eboot/c.c", hi, "gcc171-O2"): "gcc-unattributed",
                source(root, "src/eboot/d.c", lo - 8, "gcc171-O2"): "gcc-unattributed",
                source(root, "src/eboot/e.c", hi, None): "game",
            }
            for path, want in cases.items():
                self.assertEqual(O.own_class(path, root), want, path.name)

    def test_a_newlib_note_still_comes_first(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            path = source(root, "src/eboot/n.c", O.SGX_BLOCKS[EBOOT][0], "gcc171-O2")
            path.write_text(path.read_text(encoding="utf-8") + "/* Library source: newlib 1.15.0 */\n", encoding="utf-8")
            self.assertEqual(O.own_class(path, root), "newlib")

    def test_sgx_is_a_class_of_place_for_clones(self) -> None:
        lo, hi = O.SGX_BLOCKS[EBOOT]
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            source(root, "src/eboot/sdk.c", hi + 0x100, "gcc171-O2")
            source(root, "src/eboot/sgx.c", lo + 0x10, "gcc171-O2")
            for rel, start, exemplar in (("src/eboot/in.c", lo + 0x20, "src/eboot/sdk.c"),
                                         ("src/eboot/out.c", hi + 0x200, "src/eboot/sgx.c")):
                path = source(root, rel, start, "gcc171-O2")
                head, body = path.read_text(encoding="utf-8").split("\n", 1)
                path.write_text(f"{head}\n/* Clone of {exemplar} (hstclone.py tag t1) */\n{body}", encoding="utf-8")
            classes, warnings = O.classify(root)
        self.assertEqual(warnings, [])
        self.assertEqual((classes["src/eboot/in.c"], classes["src/eboot/out.c"]), (O.SGX, "gcc-unattributed"))

    def test_the_class_is_excluded_like_the_sdk(self) -> None:
        import hstpublish
        self.assertIn(O.SGX, O.CLASSES)
        self.assertIn(O.SGX, hstpublish.EXCLUDED_CLASSES)

    def test_sgx_blocks_span_exactly_the_sgx_units(self) -> None:
        tu_path = O.ROOT / "data" / "translation-units.json"
        if not tu_path.exists():
            self.skipTest("no data/translation-units.json")
        doc = json.loads(tu_path.read_text(encoding="utf-8"))
        for module, (lo, hi) in O.SGX_BLOCKS.items():
            ends = {}
            for line in (O.ROOT / "data" / f"functions-{module}.txt").read_text(encoding="utf-8").splitlines():
                if line.strip() and not line.startswith("#"):
                    s, e = line.split()[:2]
                    ends[int(s, 16)] = int(e, 16)
            sgx = sorted(int(a, 16) for u in doc["modules"][module] if u["name"].lower().startswith("sgx")
                         for a, _k in u["functions"])
            self.assertEqual((sgx[0], ends[sgx[-1]]), (lo, hi), module)
            self.assertEqual(sum(1 for s in ends if lo <= s < hi), 592, module)


if __name__ == "__main__":
    unittest.main()
