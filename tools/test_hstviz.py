#!/usr/bin/env python3
"""Synthetic tests for hstviz.py (progress visuals drawing, SVG generation, gather, write, main)."""
from __future__ import annotations

import datetime
import json
import sys
import tempfile
import unittest
import xml.etree.ElementTree as ET
from pathlib import Path
from unittest import mock

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hstviz as V


class HelperTests(unittest.TestCase):
    def test_esc(self) -> None:
        self.assertEqual(V.esc("a & b < c > d"), "a &amp; b &lt; c &gt; d")
        self.assertEqual(V.esc("no special chars"), "no special chars")

    def test_fill(self) -> None:
        self.assertEqual(V.fill("kept"), 'fill="url(#hatch)"')
        self.assertEqual(V.fill("matched"), 'class="s-matched"')
        self.assertEqual(V.fill("todo"), 'class="s-todo"')

    def test_mb(self) -> None:
        self.assertEqual(V.mb(1_000_000), "1.00 MB")
        self.assertEqual(V.mb(2_500_000), "2.50 MB")
        self.assertEqual(V.mb(500_000), "0.50 MB")

    def test_svg_open(self) -> None:
        out = V.svg_open(920, 600, "Test <Label> & Title")
        self.assertIn('viewBox="0 0 920 600"', out)
        self.assertIn('aria-label="Test &lt;Label&gt; &amp; Title"', out)
        self.assertIn('<rect class="bg" width="920" height="600" rx="8"/>', out)

    def test_stacked_bar(self) -> None:
        parts = [("matched", 300), ("todo", 700)]
        res = V.stacked_bar(10, 20, 100, 15, parts, 1000, "Module Bar")
        self.assertIn('class="s-matched"', res)
        self.assertIn('class="s-todo"', res)
        self.assertIn('<title>Module Bar: Matched (tier 1) 300 B (30.00%)</title>', res)

    def test_cell_states(self) -> None:
        # Range 0x0 to 0x100 (256 bytes), 4 cells (64 bytes each):
        # Cell 0 [0, 64): covered by matched [0, 64) -> matched
        # Cell 1 [64, 128): covered by blocked [64, 96) (32 B) and data (32 B). Tied! Tied goes to function state.
        # Cell 2 [128, 192): covered by todo [128, 192) -> todo
        # Cell 3 [192, 256): no function -> all data -> data
        funcs = [(0x0, 0x40, "matched"), (0x40, 0x60, "blocked"), (0x80, 0xc0, "todo")]
        states = V.cell_states(funcs, 0, 0x100, 4)
        self.assertEqual(states, ["matched", "blocked", "todo", "data"])


class DiagramTests(unittest.TestCase):
    def setUp(self) -> None:
        self.rows = [
            {"date": "2026-10-20", "source": "readme:x", "percent_code_bytes": "24.055", "strict_percent": "10.0"},
            {"date": "2026-10-21", "source": "hstprogress", "percent_code_bytes": "24.569", "strict_percent": "12.5"},
        ]
        funcs_eboot = [(0x0, 0x40, "matched"), (0x40, 0x60, "blocked"), (0x80, 0xc0, "todo")]
        by_state_eboot = dict.fromkeys(V.STATE_KEYS, 0)
        for a, b, st in funcs_eboot:
            by_state_eboot[st] += b - a
        by_state_eboot["data"] = 0x100 - sum(b - a for a, b, _ in funcs_eboot)

        by_state_other = dict.fromkeys(V.STATE_KEYS, 0)
        by_state_other["kept"] = 0x200

        self.g = {
            "private": False,
            "date": "2026-10-21",
            "sources": 5,
            "public_paths": {"src/a.c"},
            "ladder": {
                "t1_percent": 25.0, "t1_bytes": 0x40,
                "t2_percent": 20.0, "t2_bytes": 0x30,
                "t3_percent": 15.0, "t3_bytes": 0x20,
                "t4_percent": 10.0, "t4_bytes": 0x10,
                "strict_percent": 12.5,
            },
            "scope": {"scope_bytes": 0x300},
            "rows": self.rows,
            "modules": {
                "EBOOT.BIN": {
                    "image": 0x100,
                    "by_state": by_state_eboot,
                    "funcs": funcs_eboot,
                    "segments": [(0, 0x100)],
                },
                "libfont.prx": {
                    "image": 0x200,
                    "by_state": by_state_other,
                    "funcs": [],
                    "segments": [(0x100, 0x300)],
                },
            },
        }

    def test_dashboard_renders_valid_xml(self) -> None:
        svg = V.dashboard(self.g)
        self.assertTrue(ET.fromstring(svg).tag.endswith("svg"))
        self.assertIn("Hot Shots Tennis: Get a Grip, matching decompilation", svg)
        self.assertIn("12.5%", svg)   # strict hero
        self.assertIn("25.0%", svg)   # T1 hero
        self.assertIn("Sony modules", svg)

    def test_dashboard_tier_one_reads_100_only_when_complete(self) -> None:
        self.g["ladder"].update(t1_percent=99.999, t1_bytes=0x300 - 1)     # 99.97 %: one byte left
        self.assertNotIn(">100.0%<", V.dashboard(self.g))
        self.assertIn(">99.9%<", V.dashboard(self.g))
        self.g["ladder"].update(t1_percent=100.0, t1_bytes=0x300)
        self.assertIn(">100.0%<", V.dashboard(self.g))

    def test_dashboard_private_view(self) -> None:
        g_priv = dict(self.g, private=True)
        svg = V.dashboard(g_priv)
        self.assertTrue(ET.fromstring(svg).tag.endswith("svg"))
        self.assertIn("(PRIVATE VIEW)", svg)

    def test_address_map_renders_valid_xml(self) -> None:
        svg = V.address_map(self.g)
        self.assertTrue(ET.fromstring(svg).tag.endswith("svg"))
        self.assertIn("Address map of the two game modules", svg)
        self.assertIn("EBOOT.BIN", svg)

    def test_history_renders_valid_xml(self) -> None:
        svg = V.history(self.g)
        self.assertTrue(ET.fromstring(svg).tag.endswith("svg"))
        self.assertIn("Progress over time", svg)
        self.assertIn("path class=\"ln\"", svg)
        self.assertIn("path class=\"ln2\"", svg)

    def test_history_empty_rows(self) -> None:
        empty_g = dict(self.g, rows=[])
        svg = V.history(empty_g)
        self.assertTrue(ET.fromstring(svg).tag.endswith("svg"))
        self.assertIn("No history yet", svg)

    def test_history_single_strict_point(self) -> None:
        single_g = dict(self.g, rows=[{"date": "2026-10-21", "source": "hstprogress", "percent_code_bytes": "25.0", "strict_percent": "12.5"}])
        svg = V.history(single_g)
        self.assertTrue(ET.fromstring(svg).tag.endswith("svg"))
        self.assertIn("path class=\"ln2\"", svg)

    def test_milestones_chart_renders_valid_xml(self) -> None:
        rows = [
            {"title": "M1", "detail": "Detail 1", "criterion": "Crit 1", "progress": 100.0, "reached": "2026-10-01", "regressed": False},
            {"title": "M2", "detail": "Detail 2" * 20, "criterion": "Crit 2", "progress": 50.0, "reached": "2026-09-01", "regressed": True},
            {"title": "M3", "detail": "Detail 3", "criterion": "Crit 3", "progress": None, "reached": None, "regressed": False},
            {"title": "M4", "detail": "Detail 4", "criterion": "Crit 4", "progress": 0.0, "reached": None, "regressed": False},
        ]
        svg = V.milestones_chart(self.g, rows)
        self.assertTrue(ET.fromstring(svg).tag.endswith("svg"))
        self.assertIn("100%, reached 2026-10-01", svg)
        self.assertIn("REGRESSED (reached 2026-09-01): 50.00%", svg)
        self.assertIn("not measured yet", svg)


class GatherTests(unittest.TestCase):
    def test_gather_public_and_private(self) -> None:
        mock_ledger = {
            "modules": [
                {
                    "name": "EBOOT.BIN",
                    "load_segments": [{"start": 0x0, "memory_end": 0x100}],
                    "functions": [{"start": 0x0, "end": 0x40}, {"start": 0x40, "end": 0x80}],
                }
            ]
        }
        mock_classes = ({"src/a.c": "public"}, [])
        mock_scope = {"scope_bytes": 0x100, "modules": {"EBOOT.BIN": {"image_bytes": 0x100}}}
        mock_shown = {
            "sources": 1,
            "source_rows": {("EBOOT.BIN", 0x0): {"path": "src/a.c", "start": 0x0, "end": 0x40}},
            "total": {"matched_code_bytes": 0x40},
        }
        mock_reports = {}
        mock_ladder = {
            "strict_percent": 10.0,
            "t1_percent": 25.0, "t1_bytes": 0x40,
            "t2_percent": 20.0, "t2_bytes": 0x30,
            "t3_percent": 15.0, "t3_bytes": 0x20,
            "t4_percent": 10.0, "t4_bytes": 0x10,
        }
        mock_readme = {}
        mock_blocked = {("EBOOT.BIN", 0x40)}
        mock_attempts_json = {
            "functions": [
                {"module": "EBOOT.BIN", "start": "0x0", "status": "exact"},
                {"module": "EBOOT.BIN", "start": "0x40", "status": "blocked"},
            ]
        }

        with mock.patch.object(V.P, "ledger", return_value=mock_ledger), \
                mock.patch.object(V.hstorigin, "classify", return_value=mock_classes), \
                mock.patch.object(V.P, "scope", return_value=mock_scope), \
                mock.patch.object(V.P, "measure", return_value=mock_shown), \
                mock.patch.object(V.P, "build_reports", return_value=mock_reports), \
                mock.patch.object(V.P, "tier_ladder", return_value=mock_ladder), \
                mock.patch.object(V.A, "load_blocked", return_value=mock_blocked), \
                mock.patch.object(V.A, "collect", return_value=[]), \
                mock.patch.object(V.A, "to_json", return_value=mock_attempts_json), \
                mock.patch.object(V.P, "csv_rows", return_value=[]):

            g_pub = V.gather(private=False)
            self.assertFalse(g_pub["private"])
            self.assertIn("EBOOT.BIN", g_pub["modules"])
            self.assertEqual(g_pub["modules"]["EBOOT.BIN"]["funcs"], [(0x0, 0x40, "matched"), (0x40, 0x80, "blocked")])

            g_priv = V.gather(private=True)
            self.assertTrue(g_priv["private"])


class OutputAndMainTests(unittest.TestCase):
    def test_write_and_main(self) -> None:
        mock_g = {
            "private": False,
            "date": "2026-10-21",
            "sources": 1,
            "public_paths": set(),
            "ladder": {
                "strict_percent": 0.0,
                "t1_percent": 0.0, "t1_bytes": 0,
                "t2_percent": 0.0, "t2_bytes": 0,
                "t3_percent": 0.0, "t3_bytes": 0,
                "t4_percent": 0.0, "t4_bytes": 0,
            },
            "scope": {"scope_bytes": 100},
            "rows": [],
            "modules": {},
        }
        mock_milestone_rows = [{"id": "m1", "title": "M1", "progress": 100.0, "detail": "d", "criterion": "c"}]

        with tempfile.TemporaryDirectory() as tmp:
            tmp_path = Path(tmp)
            pub_dir = tmp_path / "docs"
            priv_dir = tmp_path / "try" / "progress-private"

            with mock.patch.object(V, "gather", return_value=mock_g), \
                    mock.patch.object(V, "PUBLIC_DIR", pub_dir), \
                    mock.patch.object(V, "PRIVATE_DIR", priv_dir), \
                    mock.patch("hstmilestones.compute", return_value=[]), \
                    mock.patch("hstmilestones.record", return_value=mock_milestone_rows):

                written = V.write(private=False)
                self.assertEqual(len(written), 4)
                for p in written:
                    self.assertTrue(p.exists())
                    self.assertTrue(p.stat().st_size > 0)

            # Test main() execution
            with mock.patch("argparse.ArgumentParser.parse_args", return_value=mock.Mock(private=False)), \
                    mock.patch.object(V, "write", return_value=[tmp_path / "docs" / "progress.svg"]) as mock_write, \
                    mock.patch.object(V, "ROOT", tmp_path):
                ret = V.main()
                self.assertEqual(ret, 0)
                mock_write.assert_called_once_with(False)


if __name__ == "__main__":
    unittest.main()
