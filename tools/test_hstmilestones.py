#!/usr/bin/env python3
"""Synthetic tests for the capability milestones (hstmilestones.py): reached dates, regressions, rebuild and platform rules."""
from __future__ import annotations

import json
import sys
import tempfile
import unittest
from pathlib import Path
from unittest import mock

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hstmilestones as H
import hstport


def module(image: int, kept: int = 0) -> dict:
    by_state = {"kept": kept}
    return {"image": image, "by_state": by_state, "funcs": []}


class RecordTests(unittest.TestCase):
    def test_first_reached_date_is_kept_and_a_drop_is_flagged(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "milestones.json"
            rows = H.record([{"id": "a", "title": "A", "progress": 100.0, "detail": ""},
                             {"id": "b", "title": "B", "progress": 50.0, "detail": ""}], path, today="2026-10-21")
            self.assertEqual([r["reached"] for r in rows], ["2026-10-21", None])
            again = H.record([{"id": "a", "title": "A", "progress": 100.0, "detail": ""},
                              {"id": "b", "title": "B", "progress": 100.0, "detail": ""}], path, today="2026-11-02")
            self.assertEqual([r["reached"] for r in again], ["2026-10-21", "2026-11-02"])   # the first date stays
            dropped = H.record([{"id": "a", "title": "A", "progress": 99.0, "detail": ""}], path, today="2026-11-03")
            self.assertEqual(dropped[0]["reached"], "2026-10-21")
            self.assertTrue(dropped[0]["regressed"])
            self.assertTrue(json.loads(path.read_text(encoding="utf-8"))["milestones"][0]["regressed"])


class BuildTests(unittest.TestCase):
    def test_public_rebuild_may_take_sony_code_from_the_owner_but_private_may_not(self) -> None:
        g_pub = {"public_paths": {"src/a.c"}, "modules": {"M": module(1000, kept=100)}}
        g_priv = {"public_paths": None, "modules": {"M": module(1000)}}
        # the owner-supplied Sony bytes are part of the scaffolding: the public view may discount them, the private may not
        rep = {"M": {"module": "M", "image_sha256_match": True, "scaffolding_bytes": 100, "owner_supplied_bytes": 100,
                     "t2_source_bytes": {"src/a.c": 900, "src/sdk.c": 0, "try/sample.c": 50}}}
        self.assertEqual(H.measure_build(["M"], g_pub, rep, shifted=False)["progress"], 100.0)
        self.assertLess(H.measure_build(["M"], g_priv, rep, shifted=False)["progress"], 100.0)
        self.assertEqual(H.measure_build(["M"], g_pub, rep, shifted=True)["progress"], 0.0)   # no relocated build yet

    def test_scaffolding_or_a_differing_image_is_never_complete(self) -> None:
        g = {"public_paths": None, "modules": {"M": module(1000)}}
        for rep in ({"image_sha256_match": False, "scaffolding_bytes": 0},
                    {"image_sha256_match": True, "scaffolding_bytes": 4}):
            rep.update(module="M", t2_source_bytes={"src/a.c": 1000})
            self.assertLess(H.measure_build(["M"], g, {"M": rep}, shifted=False)["progress"], 100.0)
        self.assertEqual(H.measure_build(["M"], g, {}, shifted=False)["progress"], 0.0)

    def test_hstbuild_report_schema_counts_container_and_scaffolding_and_only_src(self) -> None:
        g = {"public_paths": None, "modules": {"M": module(1000)}}
        rep = {"module": "M", "identical": True, "scaffolding": {"assembled_code_bytes": 0, "blob_bytes": 0, "note": "x"},
               "container": {"bytes_from_retail": 64}, "t2_source_bytes": {"src/a.c": 900, "try/f1sample/b.c": 36}}
        r = H.measure_build(["M"], g, {"M": rep}, shifted=False)
        self.assertLess(r["progress"], 100.0)                      # retail container bytes are still scaffolding
        self.assertAlmostEqual(r["progress"], 90.0)                 # the scratch sample does not count
        rep["container"]["bytes_from_retail"] = 0
        self.assertEqual(H.measure_build(["M"], g, {"M": rep}, shifted=False)["progress"], 100.0)


class ShiftedBuildTests(unittest.TestCase):
    def test_t3_counts_t3_source_bytes_and_completes_only_with_the_runtime_half(self) -> None:
        g = {"public_paths": None, "modules": {"M": module(1000)}}
        rep = {"module": "M", "identical": True, "scaffolding": {"assembled_code_bytes": 0, "note": "x"},
               "container": {"bytes_from_retail": 0}, "t2_source_bytes": {"src/a.c": 900, "src/b.c": 100},
               "t3_source_bytes": {"src/a.c": 900}, "shifted_build_passes": True, "shifted_runtime": "not run"}
        self.assertAlmostEqual(H.measure_build(["M"], g, {"M": rep}, shifted=True)["progress"], 90.0)
        self.assertEqual(H.measure_build(["M"], g, {"M": rep}, shifted=False)["progress"], 100.0)
        rep["t3_source_bytes"] = {"src/a.c": 900, "src/b.c": 100}
        self.assertLess(H.measure_build(["M"], g, {"M": rep}, shifted=True)["progress"], 100.0)   # runtime not run
        rep["shifted_runtime"] = "passed"
        self.assertEqual(H.measure_build(["M"], g, {"M": rep}, shifted=True)["progress"], 100.0)
        rep["shifted_build_passes"] = False
        self.assertEqual(H.measure_build(["M"], g, {"M": rep}, shifted=True)["progress"], 0.0)    # nothing without a pass


class PlatformTests(unittest.TestCase):
    def test_only_complete_dedicated_handlers_count(self) -> None:
        audit = {1: {"classification": "dedicated", "status": "complete"},
                 2: {"classification": "dedicated", "status": "partial"},
                 3: {"classification": "fake_success", "status": "stub"},
                 4: {"classification": "controlled_unsupported", "status": "controlled_unsupported"}}
        self.assertEqual(H.classify_imports([1, 2, 3, 4, 5], audit),
                         {"complete": 1, "partial or unreviewed": 1, "refused or fake": 2, "missing": 1})


class PortableTests(unittest.TestCase):
    def test_portable_reader_uses_code_bytes_and_selects_public_or_private_report(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            public_path = Path(tmp) / "port-report.json"
            private_path = Path(tmp) / "port-private-report.json"
            public_path.write_text(json.dumps({"targets": list(hstport.TARGETS),
                                               "sources": 8, "compiling_all_targets": 3,
                                               "code_bytes": 1000, "compiling_bytes": 375}), encoding="utf-8")
            private_path.write_text(json.dumps({"targets": ["x86_64-pc-windows-msvc", "i686-pc-windows-msvc",
                                                "aarch64-linux-android24", "armv7a-linux-androideabi24",
                                                "x86_64-linux-android24"],
                                               "sources": 10, "compiling_all_targets": 4,
                                               "code_bytes": 1200, "compiling_bytes": 600}), encoding="utf-8")
            with mock.patch.object(H, "PORT_REPORT", public_path), \
                    mock.patch.object(H, "PORT_PRIVATE_REPORT", private_path):
                public = H.measure_portable(private=False)
                private = H.measure_portable(private=True)
                self.assertEqual(public["progress"], 37.5)
                self.assertEqual(private["progress"], 50.0)
                self.assertEqual(public["detail"],
                                 "375 of 1,000 code bytes are source that compiles for all 5 host targets (3 of 8 sources)")
                self.assertIn("600 of 1,200 code bytes", private["detail"])

    def test_portable_subset_report_and_missing_report_remain_not_measured(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "port-report.json"
            with mock.patch.object(H, "PORT_REPORT", path):
                self.assertIsNone(H.measure_portable()["progress"])
                path.write_text(json.dumps({"targets": ["i686-pc-windows-msvc"], "sources": 1,
                                            "compiling_all_targets": 1, "code_bytes": 10,
                                            "compiling_bytes": 10}), encoding="utf-8")
                measured = H.measure_portable()
                self.assertIsNone(measured["progress"])
                self.assertIn("not measured yet", measured["detail"])

    def test_compute_private_view_reads_private_portable_report(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            public_path = Path(tmp) / "port-report.json"
            private_path = Path(tmp) / "port-private-report.json"
            targets = ["x86_64-pc-windows-msvc", "i686-pc-windows-msvc", "aarch64-linux-android24",
                       "armv7a-linux-androideabi24", "x86_64-linux-android24"]
            for path, compiling in ((public_path, 2), (private_path, 7)):
                path.write_text(json.dumps({"targets": targets, "sources": 10, "compiling_all_targets": compiling,
                                            "code_bytes": 100, "compiling_bytes": compiling * 10}), encoding="utf-8")
            definitions = {"milestones": [{"id": "portable", "title": "Portable", "criterion": "",
                                           "measure": "portable"}]}
            with mock.patch.object(H, "PORT_REPORT", public_path), \
                    mock.patch.object(H, "PORT_PRIVATE_REPORT", private_path), \
                    mock.patch.object(H, "definitions", return_value=definitions), \
                    mock.patch.object(H.P, "ledger", return_value={"modules": []}), \
                    mock.patch.object(H.P, "build_reports", return_value={}):
                self.assertEqual(H.compute({"modules": {}, "public_paths": set(), "ladder": []})[0]["progress"], 20.0)
                self.assertEqual(H.compute({"modules": {}, "public_paths": None, "ladder": []})[0]["progress"], 70.0)


class TierOneTests(unittest.TestCase):
    def test_all_code_is_not_reached_on_sony_functions_that_no_source_covers(self) -> None:
        """Public view: the game modules are fully matched and one of the two Sony-module functions has a source of an
        excluded class (kept out of this repository); the other has no source anywhere. "Game code matched" is
        reached; "All code matched" ("every function of all six modules matches") is not."""
        import hstviz as V
        led = {"modules": [
            {"name": "EBOOT.BIN", "load_segments": [{"start": 0, "memory_end": 0x100}],
             "functions": [{"start": 0x0, "end": 0x40}]},
            {"name": "libfont.prx", "load_segments": [{"start": 0, "memory_end": 0x100}],
             "functions": [{"start": 0x0, "end": 0x20}, {"start": 0x20, "end": 0x80}]}]}
        public = {("EBOOT.BIN", 0x0): {"path": "src/eboot/a.c", "start": 0x0, "end": 0x40, "module": "EBOOT.BIN"}}
        every = {**public, ("libfont.prx", 0x0): {"path": "src/libfont/b.c", "start": 0x0, "end": 0x20,
                                                  "module": "libfont.prx"}}

        def measure(modules=None, include_excluded=False, **_kw):
            rows = every if include_excluded else public
            return {"sources": len(rows), "source_rows": rows, "total": {"matched_code_bytes": 0x40}}

        definitions = {"milestones": [
            {"id": "game-code", "title": "Game", "criterion": "", "measure": "t1", "modules": ["EBOOT.BIN"]},
            {"id": "all-code", "title": "All", "criterion": "", "measure": "t1", "modules": "all"}]}
        scope = {"scope_bytes": 0x200, "modules": {"EBOOT.BIN": {"image_bytes": 0x100},
                                                   "libfont.prx": {"image_bytes": 0x100}}}
        with mock.patch.object(V.P, "ledger", return_value=led), \
                mock.patch.object(V.hstorigin, "classify", return_value=({}, [])), \
                mock.patch.object(V.P, "scope", return_value=scope), \
                mock.patch.object(V.P, "measure", side_effect=measure), \
                mock.patch.object(V.P, "build_reports", return_value={}), \
                mock.patch.object(V.P, "tier_ladder", return_value={"strict_percent": 0.0}), \
                mock.patch.object(V.A, "load_blocked", return_value=set()), \
                mock.patch.object(V.A, "collect", return_value=[]), \
                mock.patch.object(V.A, "to_json", return_value={"functions": []}), \
                mock.patch.object(V.P, "csv_rows", return_value=[]), \
                mock.patch.object(H, "definitions", return_value=definitions):
            g = V.gather(private=False)
            self.assertEqual([st for _a, _b, st in g["modules"]["libfont.prx"]["funcs"]], ["kept", "kept"])  # drawn
            rows = {r["id"]: r for r in H.compute(g)}
        self.assertEqual(rows["game-code"]["progress"], 100.0)
        self.assertEqual(rows["all-code"]["progress"], 100 * 0x40 / (0x40 + 0x60))
        self.assertIn("32 B of Sony code kept out", rows["all-code"]["detail"])


if __name__ == "__main__":
    unittest.main()
