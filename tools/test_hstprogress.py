#!/usr/bin/env python3
"""Synthetic tests for the tier ladder and the public progress files (hstprogress.py)."""
from __future__ import annotations

import json
import os
import sys
import tempfile
import unittest
import xml.etree.ElementTree as ET
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hstprogress as P


def figures() -> dict:
    return {"total": {"functions": 100, "matched_functions": 10, "code_bytes": 4000, "matched_code_bytes": 400,
                      "percent_code_bytes": 10.0, "blocked_functions": 2, "blocked_code_bytes": 64},
            "near_misses": 3, "sources": 10}


class PublishableScopeTests(unittest.TestCase):
    """known_private: only what is known private leaves the publishable denominator (owner decision 2026-11-10)."""
    SGX = (0x1000, 0x2000)
    SPANS = ([0x3000], [0x3100])      # one private multi-function source covering 0x3000..0x3100

    def private(self, start: int, end: int, hit: bool = False, starts: set[int] | None = None) -> bool:
        return P.known_private(start, end, hit, self.SGX, starts or set(), self.SPANS)

    def test_sgx_block_and_private_sources_are_private(self) -> None:
        self.assertTrue(self.private(0x1800, 0x1810))
        self.assertTrue(self.private(0x3040, 0x3080))
        self.assertTrue(self.private(0x5000, 0x5010, starts={0x5000}))

    def test_a_public_source_wins_and_unattributed_code_stays_publishable(self) -> None:
        self.assertFalse(self.private(0x1800, 0x1810, hit=True))     # e.g. a newlib source inside the block
        self.assertFalse(self.private(0x4000, 0x4040))               # nothing covers it yet: counted publishable
        self.assertFalse(self.private(0x2000, 0x2010))               # the block's end is exclusive
        self.assertFalse(P.known_private(0x1800, 0x1810, False, None, set(), ([], [])))


class LadderTests(unittest.TestCase):
    def test_t2_comes_only_from_build_reports_and_only_for_the_chosen_sources(self) -> None:
        reports = {"EBOOT.BIN": {"module": "EBOOT.BIN", "t2_source_bytes": {"src/a.c": 100, "src/b.c": 50}}}
        every = P.tier_ladder(1000, 300, reports, None)
        self.assertEqual((every["t1_bytes"], every["t2_bytes"], every["t3_bytes"], every["t4_bytes"]), (300, 150, 0, 0))
        public = P.tier_ladder(1000, 300, reports, {"src/a.c"})
        self.assertEqual(public["t2_bytes"], 100)
        self.assertEqual(public["t1_percent"], 30.0)
        self.assertEqual(public["strict_percent"], 0.0)            # nothing is estimated: the strict tier is 0 until T3 exists
        scratch = {"EBOOT.BIN": {"module": "EBOOT.BIN", "t2_source_bytes": {"src/a.c": 100, "try/f1sample/b.c": 70, "wip/c.c": 5}}}
        self.assertEqual(P.tier_ladder(1000, 300, scratch, None)["t2_bytes"], 100)      # only accepted sources under src/
        none = P.tier_ladder(1000, 300, {}, None)
        self.assertEqual(none["t2_bytes"], 0)

    def test_t3_comes_only_from_a_shifted_build_that_passed(self) -> None:
        rep = {"module": "EBOOT.BIN", "t2_source_bytes": {"src/a.c": 100, "src/b.c": 50},
               "t3_source_bytes": {"src/a.c": 100, "try/x.c": 9}, "shifted_build_passes": True}
        self.assertEqual(P.tier_ladder(1000, 300, {"EBOOT.BIN": rep}, None)["t3_bytes"], 100)
        self.assertEqual(P.tier_ladder(1000, 300, {"EBOOT.BIN": rep}, {"src/b.c"})["t3_bytes"], 0)
        failed = dict(rep, shifted_build_passes=False)
        self.assertEqual(P.tier_ladder(1000, 300, {"EBOOT.BIN": failed}, None)["t3_bytes"], 0)

    def test_build_reports_ignores_files_that_are_not_reports(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            d = Path(tmp)
            (d / "EBOOT.BIN-report.json").write_text(json.dumps({"module": "EBOOT.BIN", "build_mode": "public", "t2_source_bytes": {"x": 4}}), encoding="utf-8")
            (d / "EBOOT.BIN-private-report.json").write_text(json.dumps({"module": "EBOOT.BIN", "build_mode": "private", "t2_source_bytes": {"x": 9}}), encoding="utf-8")
            (d / "bad-report.json").write_text("{not json", encoding="utf-8")
            (d / "other-report.json").write_text(json.dumps({"module": "m"}), encoding="utf-8")
            self.assertEqual(list(P.build_reports(d)), ["EBOOT.BIN"])
            self.assertEqual(P.build_reports(d)["EBOOT.BIN"]["t2_source_bytes"], {"x": 4})                      # public by default
            self.assertEqual(P.build_reports(d, "private")["EBOOT.BIN"]["t2_source_bytes"], {"x": 9})
            os.utime(d / "EBOOT.BIN-report.json", (946857600, 946857600))                  # 2000-01-03 UTC
            self.assertEqual(P.build_reports(d)["EBOOT.BIN"]["built"], "2000-01-03")       # no date: the file's
            (d / "EBOOT.BIN-report.json").write_text(json.dumps({"module": "EBOOT.BIN", "build_mode": "public", "t2_source_bytes": {"x": 4}, "built": "1999-12-31"}), encoding="utf-8")
            self.assertEqual(P.build_reports(d)["EBOOT.BIN"]["built"], "1999-12-31")       # the report's own date
        self.assertEqual(P.build_reports(Path(tmp) / "missing"), {})


class OutputTests(unittest.TestCase):
    def test_badge_is_a_shields_endpoint_with_the_strict_figure_first(self) -> None:
        b = P.badge(P.tier_ladder(1000, 150, {}, None))
        self.assertEqual(b["schemaVersion"], 1)
        self.assertEqual(b["color"], "red")
        self.assertTrue(b["message"].startswith("0.0%"))
        self.assertIn("tier 1: 15.0%", b["message"])

    def test_visuals_are_well_formed_and_account_for_every_byte(self) -> None:
        import hstviz as V
        rows = [{"date": "2026-10-20", "source": "readme:x", "percent_code_bytes": "24.055"},
                {"date": "2026-10-21", "source": "readme:y", "percent_code_bytes": "24.489"},
                {"date": "2026-10-21", "source": "hstprogress", "percent_code_bytes": "24.569", "strict_percent": "0.000"}]
        funcs = [(0x0, 0x40, "matched"), (0x40, 0x60, "blocked"), (0x80, 0xc0, "todo")]
        by_state = dict.fromkeys(V.STATE_KEYS, 0)
        for a, b, st in funcs:
            by_state[st] += b - a
        by_state["data"] = 0x100 - sum(b - a for a, b, _ in funcs)
        g = {"private": False, "ladder": P.tier_ladder(0x100, 0x40, {}, None), "date": "2026-10-21", "sources": 1,
             "scope": {"scope_bytes": 0x100}, "rows": rows,
             "modules": {"EBOOT.BIN": {"image": 0x100, "by_state": by_state, "funcs": funcs, "segments": [(0, 0x100)]}}}
        for text in (V.dashboard(g), V.address_map(g), V.history(g)):
            self.assertTrue(ET.fromstring(text).tag.endswith("svg"))
        self.assertIn("Blocked by the toolchain  <tspan class=\"t3 num\">32 B", V.dashboard(g))
        self.assertEqual(V.history(g).count("<circle"), 2)          # one point per day; the live row replaces the seed
        # each map cell takes the state with most bytes; bytes outside functions are data, and a tie goes to the function
        self.assertEqual(V.cell_states(funcs, 0, 0x100, 4), ["matched", "blocked", "todo", "data"])

    def test_old_rows_keep_their_columns_and_sort_seeds_before_live_rows(self) -> None:
        rows = [{"date": "2026-10-21", "source": "hstprogress"}, {"date": "2026-10-21", "source": "readme:2be034d"},
                {"date": "2026-10-20", "source": "readme:68bfc7a"}]
        self.assertEqual([r["source"] for r in sorted(rows, key=P.row_order)], ["readme:68bfc7a", "readme:2be034d", "hstprogress"])
        self.assertEqual(P.COLUMNS[:10], ["date", "source", "matched_functions", "total_functions", "matched_code_bytes",
                                          "total_code_bytes", "percent_code_bytes", "near_misses", "blocked_functions",
                                          "blocked_code_bytes"])


if __name__ == "__main__":
    unittest.main()
