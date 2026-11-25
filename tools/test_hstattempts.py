#!/usr/bin/env python3
"""Synthetic tests for the attempt ledger's blocked list (hstattempts.py) and how hstviz.py draws its reservations.
No game bytes: every address, header and record here is made up."""
from __future__ import annotations

import contextlib
import io
import json
import sys
import tempfile
import unittest
from pathlib import Path
from unittest import mock

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hstattempts as A
import hstviz as V

HEADER = "/* HSTMATCH module=EBOOT.BIN start=0x{start:08x} end=0x{end:08x} symbol=FUN_{start:08x} lang=c99 */\n"
SIZES = {"EBOOT.BIN": {0x1000: 0x1100, 0x1100: 0x1200, 0x2000: 0x2100, 0x3000: 0x3100, 0x4000: 0x4100,
                       0x5000: 0x5100}}


def tree(base: Path) -> Path:
    """A main tree whose one source spans the two functions 0x1000 and 0x1100, a blocked list naming 0x1100 and
    0x2000, two starter functions (0x3000 with a wip/ file) and a batch of an unfinished lane (0x5000)."""
    root = base / "root"
    for d in ("src", "wip", "data", "try/agent-queue"):
        (root / d).mkdir(parents=True)
    (root / "src" / "pair.c").write_text(HEADER.format(start=0x1000, end=0x1200) + "int x;\n", encoding="utf-8")
    (root / "wip" / "s.c").write_text(HEADER.format(start=0x3000, end=0x3100) + "int y;\n", encoding="utf-8")
    (base / "blocked.json").write_text(json.dumps({"schema": A.BLOCKED_SCHEMA, "entries": [
        {"module": "EBOOT.BIN", "start": f"0x{s:08x}", "reason": "unfolded-global-read"} for s in (0x1100, 0x2000)]}),
        encoding="utf-8")
    (root / A.STARTER_RELPATH).write_text(json.dumps({"schema": A.STARTER_SCHEMA, "entries": [
        {"module": "EBOOT.BIN", "start": f"0x{s:08x}", "issue": 7} for s in (0x3000, 0x4000)]}), encoding="utf-8")
    (root / "try" / "agent-queue" / "ds-a-00.json").write_text(json.dumps({"module": "EBOOT.BIN", "kind": "singles", "items": [
        {"address": "0x00005000", "end": "0x00005100", "size": 0x100, "family": []}]}), encoding="utf-8")
    (base / "ext" / "ds-a").mkdir(parents=True)
    (base / "ext" / "ds-a" / "LANE.json").write_text(json.dumps({"lane": "ds-a", "batch": "ds-a-00.json"}),
                                                     encoding="utf-8")
    return root


def collect(base: Path) -> A.Attempts:
    root = tree(base)
    return A.collect(("EBOOT.BIN",), queue_dir=root / "try" / "agent-queue", blocked_path=base / "blocked.json",
                     ext_root=base / "ext", measurements={}, root=root, sizes=SIZES)


class BlockedTests(unittest.TestCase):
    def test_a_blocked_function_a_multi_function_source_covers_is_exact(self) -> None:
        with tempfile.TemporaryDirectory() as d:
            att = collect(Path(d))
            self.assertEqual(att.status("EBOOT.BIN", 0x1100)["status"], "exact")
            self.assertEqual(att.status("EBOOT.BIN", 0x2000)["status"], "blocked")
            self.assertEqual(att.summary["EBOOT.BIN"]["blocked"], {"functions": 1, "code_bytes": 0x100})

    def test_the_blocked_check_names_an_entry_a_multi_function_source_covers(self) -> None:
        with tempfile.TemporaryDirectory() as d:
            root = tree(Path(d))
            out = io.StringIO()
            blocked, sources = A.load_blocked(Path(d) / "blocked.json"), A.scan_sources(root)
            with mock.patch.object(A, "load_blocked", lambda: blocked), \
                    mock.patch.object(A, "scan_sources", lambda: sources), \
                    mock.patch.object(A, "ledger_functions", lambda module: SIZES.get(module, {})), \
                    contextlib.redirect_stdout(out):
                rc = A.cmd_blocked()
            self.assertEqual(rc, 1, out.getvalue())
            self.assertIn("PROBLEM EBOOT.BIN 0x00001100", out.getvalue())
            self.assertNotIn("0x00002000", out.getvalue().split("blocked functions:")[0])


class VizReservationTests(unittest.TestCase):
    """A function held for a starter issue is not lane work: the public dashboard draws it by its own state."""

    def test_starter_functions_are_not_drawn_as_lane_work(self) -> None:
        with tempfile.TemporaryDirectory() as d:
            att = A.to_json(collect(Path(d)), "")
        led = {"modules": [{"name": "EBOOT.BIN", "load_segments": [{"start": 0x1000, "memory_end": 0x6000}],
                            "functions": [{"start": s, "end": e} for s, e in sorted(SIZES["EBOOT.BIN"].items())]}]}
        with mock.patch.object(V.P, "ledger", return_value=led), \
                mock.patch.object(V.hstorigin, "classify", return_value=({}, [])), \
                mock.patch.object(V.P, "scope", return_value={"scope_bytes": 0x5000,
                                                               "modules": {"EBOOT.BIN": {"image_bytes": 0x5000}}}), \
                mock.patch.object(V.P, "measure", return_value={"sources": 0, "source_rows": {},
                                                                "total": {"matched_code_bytes": 0}}), \
                mock.patch.object(V.P, "build_reports", return_value={}), \
                mock.patch.object(V.P, "tier_ladder", return_value={}), \
                mock.patch.object(V.A, "load_blocked", return_value={}), \
                mock.patch.object(V.A, "collect", return_value=None), \
                mock.patch.object(V.A, "to_json", return_value=att), \
                mock.patch.object(V.P, "csv_rows", return_value=[]):
            funcs = {a: st for a, _b, st in V.gather()["modules"]["EBOOT.BIN"]["funcs"]}
        self.assertEqual(funcs[0x3000], "near")          # a starter function with a wip/ file
        self.assertEqual(funcs[0x4000], "todo")          # a starter function nobody has touched
        self.assertEqual(funcs[0x5000], "lane")          # the batch of a lane that has not finished


if __name__ == "__main__":
    unittest.main()
