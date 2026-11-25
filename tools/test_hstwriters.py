#!/usr/bin/env python3
"""Tests for what the source writers (hstgen.py, hsttmpl.py, hstclone.py) leave alone: a starter function, a function
inside a source that spans several functions, and a function another source places with helpers=. Synthetic trees and
instruction words only."""
from __future__ import annotations

import io
import json
import sys
import tempfile
import unittest
from contextlib import redirect_stdout
from pathlib import Path
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hstattempts as A
import hstclone as C
import hstgen as G
import hsttmpl as T

HEADER = "/* HSTMATCH module=EBOOT.BIN start=0x{start:08x} end=0x{end:08x} symbol=FUN_{start:08x} lang=c99{more} */\n"


def tree(root: Path, functions: list[int], size: int, *, run: tuple[int, int], helper: tuple[int, int],
         starter: int) -> None:
    """A ledger of `functions`, a source over the run [run) and a source elsewhere that places `helper` as a helper,
    and `starter` held by a good first issue."""
    for folder in ("src/eboot", "wip", "data", "ledger"):
        (root / folder).mkdir(parents=True)
    (root / "ledger" / "campaign.json").write_text(json.dumps({"modules": [{
        "name": "EBOOT.BIN", "load_segments": [{"memory_end": 0x100000}],
        "functions": [{"start": s, "end": s + size, "boundary_status": "reviewed"} for s in functions]}]}),
        encoding="utf-8")
    (root / "src" / "eboot" / "run.c").write_text(HEADER.format(start=run[0], end=run[1], more="") + "int x;\n",
                                                   encoding="utf-8")
    lo, hi = helper
    (root / "src" / "eboot" / "placer.c").write_text(
        HEADER.format(start=0xF0000, end=0xF0040, more=f" helpers=FUN_{lo:08x}@0x{lo:08x}-0x{hi:08x}") + "int y;\n",
        encoding="utf-8")
    (root / "data" / "starter-issues.json").write_text(json.dumps({"schema": A.STARTER_SCHEMA, "entries": [
        {"module": "EBOOT.BIN", "start": f"0x{starter:08x}", "issue": 9000}]}), encoding="utf-8")


class HstgenTargetTests(unittest.TestCase):
    def test_hstgen_skips_starters_and_covered_functions(self) -> None:
        free, in_run, helper, starter = 0x90000, 0x90140, 0x90200, 0x90300
        starts = [free, 0x90100, in_run, helper, starter]
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            tree(root, starts, 0x40, run=(0x90100, 0x90180), helper=(helper, helper + 0x40), starter=starter)
            pseudo = root / "pseudo"
            pseudo.mkdir()
            rows = []
            for s in starts:
                (pseudo / f"FUN_{s:08x}.c").write_text(f"void FUN_{s:08x}(void)\n{{\n}}\n", encoding="utf-8")
                rows.append(json.dumps({"address": f"0x{s:08x}", "name": f"FUN_{s:08x}", "body_bytes": 0x40,
                                        "pseudocode_status": "decompiled", "pseudocode_file": f"FUN_{s:08x}.c"}))
            (pseudo / "functions.jsonl").write_text("\n".join(rows) + "\n", encoding="utf-8")
            seen: list[str] = []

            def translate(name, *_rest):
                seen.append(name)
                return "void f(void) {}\n"

            argv = ["hstgen.py", "--tag", "synthetic", "--dry-run", "--pseudo", str(pseudo)]
            with patch.object(sys, "argv", argv), patch.object(G.M, "ROOT", root), \
                    patch.dict(G.M.__dict__, {"CAMPAIGN": root}), patch.object(G, "translate", side_effect=translate), \
                    redirect_stdout(io.StringIO()):
                self.assertEqual(G.main(), 0)
            self.assertEqual(seen, [f"FUN_{free:08x}"])


class HsttmplCandidateTests(unittest.TestCase):
    def test_hsttmpl_skips_starters_and_covered_functions(self) -> None:
        size = 0xD0                                     # the growth-policy template's size range (200..240 bytes)
        free, in_run, helper, starter = 0x90400, 0x90100, 0x90200, 0x90300
        starts = [0x90000, in_run, helper, starter, free]
        # lui t0,0x0fff; ori t0,t0,0xffff (max_size of a 16-byte element); jal 0x1000; then nops
        body = [0x3C080FFF, 0x3508FFFF, (3 << 26) | (0x1000 >> 2)] + [0] * (size // 4 - 3)
        data = bytearray(0x100000)
        for s in starts:
            data[s:s + size] = b"".join(w.to_bytes(4, "little") for w in body)
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            tree(root, starts, size, run=(0x90000, 0x90200), helper=(helper, helper + size), starter=starter)
            with patch.object(T.M, "ROOT", root), patch.dict(T.M.__dict__, {"CAMPAIGN": root}), \
                    patch.object(T.M, "module_bytes", return_value=(bytes(data), {"start": 0, "file_offset": 0})), \
                    patch.dict(T._EXEMPLARS, clear=True):
                found = sorted({int(text.split("start=0x", 1)[1][:8], 16)
                                for _kind, _size, _symbols, text in T.candidates("EBOOT.BIN", "synthetic")})
        self.assertEqual(found, [free])


class HstcloneCoverTests(unittest.TestCase):
    def test_hstclone_does_not_write_a_function_placed_as_a_helper(self) -> None:
        exemplar, free, helper = 0x1000, 0x1040, 0x1080
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            tree(root, [exemplar, free, helper], 0x40, run=(exemplar, exemplar + 0x40), helper=(helper, helper + 0x40),
                 starter=0x5000)
            (root / "try").mkdir()
            data = b"\x01\x00\x02\x24" * 0x30            # three functions of one synthetic shape

            def run(cmd, **_kwargs):
                part = Path(cmd[cmd.index("--json") + 1])
                part.write_text(json.dumps([{"source": p, "status": "EXACT"} for p in cmd[cmd.index("--json") + 2:]]),
                                encoding="utf-8")

            argv = ["hstclone.py", "--tag", "synthetic"]
            with patch.object(sys, "argv", argv), patch.object(C.M, "ROOT", root), \
                    patch.dict(C.M.__dict__, {"CAMPAIGN": root}), \
                    patch.object(C.M, "module_bytes", return_value=(data, {"start": exemplar, "file_offset": 0})), \
                    patch.object(C.M, "load_symbols", return_value={}), \
                    patch.object(C.subprocess, "run", side_effect=run), redirect_stdout(io.StringIO()):
                self.assertEqual(C.main(), 0)
            written = sorted(p.name for p in (root / "src" / "eboot").iterdir())
        self.assertEqual(written, ["placer.c", "run.c", f"run_{free:08x}.c"])

    def test_covered_starts_reads_own_starts_runs_and_helpers(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            tree(root, [], 0x40, run=(0x2000, 0x2080), helper=(0x3000, 0x3040), starter=0x5000)
            with patch.object(C.M, "ROOT", root):
                got = C.covered_starts("EBOOT.BIN", [0x1ffc, 0x2000, 0x2040, 0x2080, 0x3000, 0x3040, 0xF0000])
                other = C.covered_starts("hstp_trans_main.prx", [0x2000, 0x3000])
        self.assertEqual(got, {0x2000, 0x2040, 0x3000, 0xF0000})
        self.assertEqual(other, set())


if __name__ == "__main__":
    unittest.main()
