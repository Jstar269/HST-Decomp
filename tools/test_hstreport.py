#!/usr/bin/env python3
"""Tests for the objdiff report written for bytematch.dev and recomp.fyi (hstreport.py)."""
from __future__ import annotations

import json
import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hstprcheck
import hstreport as R

MODULES = {"A.BIN": {"functions": 10, "matched_functions": 4, "code_bytes": 1000, "matched_code_bytes": 250},
           "B.prx": {"functions": 6, "matched_functions": 3, "code_bytes": 600, "matched_code_bytes": 300}}


def fig() -> dict:
    total = {k: sum(m[k] for m in MODULES.values()) for k in ("functions", "matched_functions", "code_bytes", "matched_code_bytes")}
    return {"modules": MODULES, "total": total}


def scope() -> dict:
    return {"modules": {"A.BIN": {"image_bytes": 1500}, "B.prx": {"image_bytes": 900}}}


# A ledger and sources for the per-unit report. A.BIN: x.c owns one function, y.c spans two, one function has no
# source, two are SDK library code (not published). B.prx: one source, one function without one.
FUNCS = {"A.BIN": [(0x0, 0x8), (0x8, 0x10), (0x10, 0x28), (0x28, 0x30), (0x30, 0x38), (0x38, 0x40)],
         "B.prx": [(0x0, 0x20), (0x20, 0x24)]}
SOURCES = {("A.BIN", 0x0): ("src/a/x.c", 0x0, 0x8), ("A.BIN", 0x10): ("src/a/y.c", 0x10, 0x30),
           ("B.prx", 0x20): ("src/b/z.cpp", 0x20, 0x24)}
SDK = {"A.BIN": [(0x30, 0x40)]}
SYMBOLS = {"src/a/x.c": "FUN_00000000", "src/a/y.c": "draw_thing", "src/b/z.cpp": "FUN_00000020"}


def unit_inputs() -> tuple[dict, dict, dict]:
    rows = {k: {"path": p, "start": s, "end": e, "module": k[0]} for k, (p, s, e) in SOURCES.items()}
    mods = {}
    for name, fs in FUNCS.items():
        hit = [f for f in fs if any(r["module"] == name and r["start"] <= f[0] and f[1] <= r["end"] for r in rows.values())]
        mods[name] = {"functions": len(fs), "matched_functions": len(hit), "code_bytes": sum(e - s for s, e in fs),
                      "matched_code_bytes": sum(e - s for s, e in hit)}
    led = {"modules": [{"name": n, "functions": [{"start": s, "end": e} for s, e in fs]} for n, fs in FUNCS.items()]}
    f = {"modules": mods, "total": {}, "source_rows": rows}
    return f, {"modules": {"A.BIN": {"image_bytes": 0x100}, "B.prx": {"image_bytes": 0x80}}}, led


def unit_report() -> dict:
    f, sc, led = unit_inputs()
    return R.build_report(f, sc, led, SDK, SYMBOLS)


class ReportTests(unittest.TestCase):
    def test_schema_shape_is_objdiff_report_version_2(self) -> None:
        rep = R.build_report(fig(), scope())
        self.assertEqual(set(rep), {"measures", "units", "version", "categories"})
        self.assertEqual(rep["version"], 2)
        fields = {"fuzzy_match_percent", "total_code", "matched_code", "matched_code_percent", "total_data", "matched_data",
                  "matched_data_percent", "total_functions", "matched_functions", "matched_functions_percent",
                  "complete_code", "complete_code_percent", "complete_data", "complete_data_percent", "total_units",
                  "complete_units"}
        for m in [rep["measures"]] + [u["measures"] for u in rep["units"]] + [c["measures"] for c in rep["categories"]]:
            self.assertEqual(set(m), fields)
            for k in ("total_code", "matched_code", "total_data", "matched_data", "complete_code", "complete_data"):
                self.assertIsInstance(m[k], str)            # pbjson writes 64-bit counters as decimal strings
                self.assertTrue(m[k].isdigit())
            for k in ("total_functions", "matched_functions", "total_units", "complete_units"):
                self.assertIsInstance(m[k], int)
        self.assertEqual([u["name"] for u in rep["units"]], ["A.BIN", "B.prx"])
        self.assertEqual([c["id"] for c in rep["categories"]], ["A.BIN", "B.prx"])
        self.assertEqual(rep["units"][0]["metadata"], {"module_name": "A.BIN", "progress_categories": ["A.BIN"]})
        self.assertEqual(rep["measures"]["total_units"], 2)

    def test_totals_equal_the_progress_figures(self) -> None:
        f = fig()
        rep = R.build_report(f, scope())
        m = rep["measures"]
        t = f["total"]
        self.assertEqual(int(m["total_code"]), t["code_bytes"])
        self.assertEqual(int(m["matched_code"]), t["matched_code_bytes"])
        self.assertEqual(m["total_functions"], t["functions"])
        self.assertEqual(m["matched_functions"], t["matched_functions"])
        self.assertEqual(m["matched_code_percent"], round(100 * 550 / 1600, 3))
        for key in ("total_code", "matched_code", "total_data"):
            self.assertEqual(sum(int(u["measures"][key]) for u in rep["units"]), int(m[key]))

    def test_data_is_never_claimed(self) -> None:
        rep = R.build_report(fig(), scope())
        self.assertEqual(int(rep["measures"]["total_data"]), (1500 - 1000) + (900 - 600))
        full = {k: dict(v, matched_functions=v["functions"], matched_code_bytes=v["code_bytes"]) for k, v in MODULES.items()}
        done = R.build_report({"modules": full, "total": {}}, scope())["measures"]
        self.assertEqual(done["matched_code_percent"], 100.0)           # all code matched ...
        self.assertEqual(done["matched_data"], "0")                      # ... and still no data and nothing linked
        self.assertGreater(int(done["total_data"]), 0)
        self.assertEqual((done["complete_code"], done["complete_units"]), ("0", 0))

    def test_functions_larger_than_the_image_are_refused(self) -> None:
        with self.assertRaises(ValueError):
            R.build_report(fig(), {"modules": {"A.BIN": {"image_bytes": 10}, "B.prx": {"image_bytes": 900}}})

    def test_module_report_has_no_private_path_strings(self) -> None:
        text = R.render(R.build_report(fig(), scope()))
        for needle in ("\\", "C:", "/nk/", "private", "try/", "hst-config", "wip/", "accepted-ledger", "sony", "Sony"):
            self.assertNotIn(needle, text)
        self.assertNotIn("sections", text)

    def test_committed_report_is_valid_and_public_safe(self) -> None:
        text = R.REPORT_PATH.read_text(encoding="utf-8")
        doc = json.loads(text)
        self.assertEqual(doc["version"], 2)
        for needle in ("\\", "C:", "/nk/", "private", "try/", "hst-config", "wip/"):
            self.assertNotIn(needle, text)
        m = doc["measures"]
        self.assertLessEqual(int(m["matched_code"]), int(m["total_code"]))
        self.assertEqual(R.unit_problems(doc), [])

    def test_differences_detect_a_stale_file(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "objdiff-report.json"
            self.assertTrue(R.differences(fig(), scope(), path=path))        # missing
            R.write(fig(), scope(), path=path)
            self.assertEqual(R.differences(fig(), scope(), path=path), [])
            newer = fig()
            newer["modules"] = {k: dict(v, matched_functions=v["matched_functions"] + 1,
                                        matched_code_bytes=v["matched_code_bytes"] + 8) for k, v in MODULES.items()}
            diffs = R.differences(newer, scope(), path=path)
            self.assertTrue(any("matched_code" in d for d in diffs))
            path.write_text("{not json", encoding="utf-8")
            self.assertTrue(any("not valid JSON" in d for d in R.differences(fig(), scope(), path=path)))


class UnitTests(unittest.TestCase):
    """The per-unit report: what bytematch.dev draws as its grid."""

    def test_module_figures_are_the_same_as_the_one_unit_report(self) -> None:
        f, sc, led = unit_inputs()
        per_unit = R.build_report(f, sc, led, SDK, SYMBOLS)
        per_module = R.build_report(f, sc)
        strip = lambda m: {k: v for k, v in m.items() if k != "total_units"}
        self.assertEqual(strip(per_unit["measures"]), strip(per_module["measures"]))
        for a, b in zip(per_unit["categories"], per_module["categories"]):
            self.assertEqual(strip(a["measures"]), strip(b["measures"]))
        self.assertEqual(per_unit["measures"]["total_units"], len(per_unit["units"]))

    def test_units_add_up_to_their_module(self) -> None:
        rep = unit_report()
        self.assertEqual(R.unit_problems(rep), [])
        for c in rep["categories"]:
            us = [u for u in rep["units"] if u["metadata"]["module_name"] == c["id"]]
            self.assertEqual(sum(int(u["measures"].get("total_code", "0")) for u in us), int(c["measures"]["total_code"]))
            self.assertEqual(sum(u["measures"].get("matched_functions", 0) for u in us), c["measures"]["matched_functions"])
            self.assertEqual(len(us), c["measures"]["total_units"])

    def test_unit_names_say_what_they_are_in_address_order(self) -> None:
        rep = unit_report()
        self.assertEqual([u["name"] for u in rep["units"]],
                         ["src/a/x.c", "A.BIN/not-yet-recovered/FUN_00000008", "src/a/y.c",
                          "A.BIN/sdk-library-not-published",
                          "B.prx/not-yet-recovered/FUN_00000000", "src/b/z.cpp"])
        y = rep["units"][2]
        self.assertEqual(y["metadata"], {"complete": False, "module_name": "A.BIN", "source_path": "src/a/y.c",
                                         "progress_categories": ["A.BIN"]})
        self.assertEqual(y["functions"], [
            {"name": "draw_thing", "size": "24", "fuzzy_match_percent": 100.0, "metadata": {"virtual_address": "16"},
             "address": "16"},
            {"name": "FUN_00000028", "size": "8", "fuzzy_match_percent": 100.0, "metadata": {"virtual_address": "40"},
             "address": "40"}])
        self.assertEqual(y["measures"], {"fuzzy_match_percent": 100.0, "total_code": "32", "matched_code": "32",
                                         "matched_code_percent": 100.0, "total_functions": 2, "matched_functions": 2,
                                         "matched_functions_percent": 100.0})
        gap = rep["units"][1]
        self.assertEqual(gap["measures"], {"total_code": "8", "total_functions": 1})
        self.assertTrue(gap["metadata"]["auto_generated"])
        self.assertNotIn("source_path", gap["metadata"])
        self.assertEqual(gap["functions"], [{"name": "FUN_00000008", "size": "8", "metadata": {"virtual_address": "8"},
                                             "address": "8"}])

    def test_sdk_library_code_is_counted_but_not_listed(self) -> None:
        sdk = unit_report()["units"][3]
        self.assertEqual(sdk["measures"], {"total_code": "16", "total_functions": 2})
        self.assertNotIn("functions", sdk)
        self.assertNotIn("source_path", sdk["metadata"])

    def test_render_is_one_unit_per_line_and_round_trips(self) -> None:
        rep = unit_report()
        text = R.render(rep)
        self.assertEqual(json.loads(text), rep)
        lines = text.split("\n")
        self.assertEqual(sum(1 for line in lines if line.startswith('{"name":')), len(rep["units"]))

    def test_unit_problems_find_a_report_that_does_not_add_up(self) -> None:
        rep = unit_report()
        rep["units"][1]["measures"]["total_code"] = "9"
        self.assertTrue(any("total_code" in p for p in R.unit_problems(rep)))
        rep = unit_report()
        del rep["units"][0]
        self.assertTrue(R.unit_problems(rep))
        rep = unit_report()
        rep["units"][2]["functions"].pop()
        self.assertTrue(any("do not add up to its measures" in p for p in R.unit_problems(rep)))

    def test_write_refuses_units_that_do_not_add_up(self) -> None:
        f, sc, led = unit_inputs()
        f["modules"]["A.BIN"]["functions"] += 1          # the figures and the ledger disagree
        with tempfile.TemporaryDirectory() as tmp, self.assertRaises(ValueError):
            R.write(f, sc, led, SDK, SYMBOLS, path=Path(tmp) / "r.json")

    def test_recovered_translation_units_come_first(self) -> None:
        f, sc, led = unit_inputs()
        tus = {"A.BIN": {0x8: "a.cpp", 0x10: "a.cpp"}, "B.prx": {0x20: "b.cpp"}}
        rep = R.build_report(f, sc, led, SDK, SYMBOLS, tus)
        self.assertEqual(R.unit_problems(rep), [])
        self.assertEqual([u["name"] for u in rep["units"]],
                         ["src/a/x.c", "A.BIN/a.cpp", "src/a/y.c", "A.BIN/sdk-library-not-published",
                          "B.prx/not-yet-recovered/FUN_00000000", "B.prx/b.cpp"])
        a_cpp = rep["units"][1]
        self.assertEqual(a_cpp["metadata"], {"complete": False, "module_name": "A.BIN", "progress_categories": ["A.BIN"]})
        self.assertEqual([(i["name"], i.get("fuzzy_match_percent")) for i in a_cpp["functions"]],
                         [("FUN_00000008", None), ("draw_thing", 100.0)])      # matched or not, all the unit's functions
        self.assertEqual(a_cpp["measures"]["total_functions"], 2)
        self.assertEqual(rep["units"][2]["measures"]["total_functions"], 1)  # y.c keeps only what is outside a.cpp
        b_cpp = rep["units"][5]
        self.assertEqual(b_cpp["metadata"], {"complete": True, "module_name": "B.prx", "progress_categories": ["B.prx"],
                                             "source_path": "src/b/z.cpp"})
        self.assertEqual((b_cpp["measures"]["complete_code"], b_cpp["measures"]["complete_units"]), ("4", 1))
        cat = rep["categories"][1]["measures"]
        self.assertEqual((cat["complete_code"], cat["complete_units"]), ("4", 1))
        self.assertEqual((rep["measures"]["complete_code"], rep["measures"]["complete_units"]), ("4", 1))
        plain = R.build_report(f, sc, led, SDK, SYMBOLS)
        for key in ("total_code", "matched_code", "total_functions", "matched_functions", "total_data"):
            self.assertEqual(rep["measures"][key], plain["measures"][key])

    def test_the_pr_checker_reads_the_report(self) -> None:
        self.assertEqual(hstprcheck.report_problems(R.render(unit_report()).encode()), [])
        bad = unit_report()
        bad["units"].pop()
        self.assertTrue(hstprcheck.report_problems(json.dumps(bad).encode()))
        self.assertTrue(hstprcheck.report_problems(b"[]"))


if __name__ == "__main__":
    unittest.main()
