#!/usr/bin/env python3
"""Checks of tools/hstperm.py's run loop on a synthetic tree (no toolchain, no game data): the compile-and-score step
is replaced by a stub, so only what main() does with the scores is tested.

    python tools/test_hstperm_run.py [-v]
"""
from __future__ import annotations

import json
import sys
import tempfile
import unittest
from pathlib import Path
from unittest import mock

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hstperm as P  # noqa: E402

SOURCE = """/* HSTMATCH module={module} start=0x00010000 end=0x00010010 symbol=FUN_00010000 lang=c99 */
int FUN_00010000(int x)
{
  int a;
  int b;
  a = x + 1;
  b = x + 2;
  return a * b;
}
"""


class RunTests(unittest.TestCase):
    def tree(self, module: str) -> Path:
        tmp = tempfile.TemporaryDirectory()
        self.addCleanup(tmp.cleanup)
        root = Path(tmp.name)
        (root / "src" / "eboot").mkdir(parents=True)
        gen = root / "try" / "gen-a1"
        (gen / "v0").mkdir(parents=True)
        (gen / "v0" / "FUN_00010000.c").write_text(SOURCE.replace("{module}", module), encoding="utf-8")
        row = {"source": "try/gen-a1/v0/FUN_00010000.c", "module": module, "start": "0x00010000",
               "status": "MISMATCH", "detail": "4/4 words; 1 differ"}
        (gen / "summary-0.json").write_text(json.dumps([row]), encoding="utf-8")
        return root

    def run_main(self, root: Path, seed_score: int, mutant_score: int, rounds: int = 1) -> int:
        def fake_scored(cmd, files, part_dir, ends, owner):
            return {str(p.resolve()): seed_score if p.parent.name == "seed" else mutant_score for p in files}

        argv = ["hstperm.py", "--tag", "t1", "--rounds", str(rounds), "--mutants", "4"]
        with mock.patch.object(P.M, "ROOT", root), mock.patch.object(P, "run_scored", fake_scored), \
                mock.patch.object(sys, "argv", argv), mock.patch("sys.stdout"):
            return P.main()

    def test_a_seed_that_is_already_exact_is_installed(self) -> None:
        # the generation summary said MISMATCH, but the recompiled seed is EXACT now (headers improved since)
        root = self.tree("EBOOT.BIN")
        self.assertEqual(self.run_main(root, seed_score=0, mutant_score=5), 0)
        installed = list((root / "src" / "eboot").glob("*.c"))
        self.assertEqual([p.name for p in installed], ["FUN_00010000.c"])
        self.assertIn("exact match verified", installed[0].read_text(encoding="utf-8"))
        report = json.loads((root / "try" / "perm-t1" / "report.json").read_text(encoding="utf-8"))
        self.assertEqual((report["accepted"], report["written"]), (1, 1))

    def test_an_exact_seed_is_installed_without_any_round(self) -> None:
        root = self.tree("EBOOT.BIN")
        self.run_main(root, seed_score=0, mutant_score=5, rounds=0)
        self.assertTrue((root / "src" / "eboot" / "FUN_00010000.c").exists())

    def test_the_first_match_of_a_module_creates_its_src_directory(self) -> None:
        root = self.tree("libfont.prx")
        self.assertFalse((root / "src" / "libfont").exists())
        self.assertEqual(self.run_main(root, seed_score=3, mutant_score=0), 0)
        self.assertTrue((root / "src" / "libfont" / "FUN_00010000.c").exists())

    def test_a_near_miss_is_not_installed(self) -> None:
        root = self.tree("EBOOT.BIN")
        self.run_main(root, seed_score=3, mutant_score=2)
        self.assertEqual(list((root / "src").rglob("*.c")), [])
        best = json.loads((root / "try" / "perm-t1" / "best.json").read_text(encoding="utf-8"))
        self.assertEqual([v["score"] for v in best.values()], [2])


if __name__ == "__main__":
    unittest.main()
