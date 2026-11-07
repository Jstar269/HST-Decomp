#!/usr/bin/env python3
"""Synthetic tests for the publish tool's never-lower guard (hstrelease.py) and the generated figure blocks
(hstprogress.py). No game bytes: every figure and document here is made up."""
from __future__ import annotations

import copy
import json
import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hstprogress as P
import hstrelease as R


def badge(strict: float, t1: float) -> str:
    return json.dumps({"schemaVersion": 1, "label": "strict match", "message": f"{strict:.1f}% (tier 1: {t1:.1f}%)",
                       "color": "red"})


def milestones(rebuild: float = 22.1, portable: float = 14.8, reached=None, regressed=False) -> dict:
    return {"schema": "hst-decomp.milestone-record.v1", "generated": "2000-01-01", "milestones": [
        {"id": "psp-rebuild", "title": "PSP rebuild", "progress": rebuild, "detail": "x", "reached": reached,
         "regressed": regressed},
        {"id": "portable-source", "title": "Portable source", "progress": portable,
         "detail": "33 candidate spans open", "reached": None, "regressed": False}]}


def objdiff(matched: int = 400, functions: int = 10) -> dict:
    m = {"fuzzy_match_percent": matched / 40, "total_code": "4000", "matched_code": str(matched),
         "matched_functions": functions, "total_functions": 100}
    return {"measures": dict(m), "units": [{"name": "A.BIN", "measures": dict(m), "metadata": {}}]}


def published() -> dict[str, str]:
    return {"progress.json": badge(0.0, 17.1), "docs/milestones.json": json.dumps(milestones()),
            "docs/objdiff-report.json": json.dumps(objdiff())}


def fn(name: str, address: int, size: int, matched: bool = False) -> dict:
    f = {"name": name, "size": str(size), "metadata": {"virtual_address": str(address)}, "address": str(address)}
    if matched:
        f["fuzzy_match_percent"] = 100.0
    return f


def unit(doc: dict, name: str) -> dict:
    return next(u for u in doc["units"] if u["name"] == name)


def report(units: list[dict], matched: int, functions: int) -> dict:
    return {"measures": {"total_code": "1000", "matched_code": str(matched), "total_functions": 50,
                         "matched_functions": functions, "total_units": len(units)}, "units": units}


def structural() -> tuple[dict[str, str], dict[str, str]]:
    """A published report and a candidate in which FUN_10 left its not-yet-recovered unit for a source unit and
    FUN_30 (SDK class, unmatched in the public figures) left the recovered file sgxa.c for the unlisted SDK unit."""
    meta = {"module_name": "M"}
    src20 = {"name": "src/m/f_20.c", "measures": {"total_code": "12", "matched_code": "12", "total_functions": 1,
                                                  "matched_functions": 1},
             "functions": [fn("FUN_20", 0x20, 12, True)], "metadata": meta}
    old_units = [
        {"name": "M/not-yet-recovered/FUN_10", "measures": {"total_code": "8", "total_functions": 1},
         "functions": [fn("FUN_10", 0x10, 8)], "metadata": dict(meta, auto_generated=True)},
        {"name": "M/sgxa.c", "measures": {"total_code": "24", "total_functions": 2},
         "functions": [fn("FUN_28", 0x28, 8), fn("FUN_30", 0x30, 16)], "metadata": meta},
        {"name": "M/sdk-library-not-published", "measures": {"total_code": "100", "total_functions": 10},
         "metadata": dict(meta, auto_generated=True)},
        src20]
    new_units = [
        {"name": "src/m/f_10.c", "measures": {"total_code": "8", "matched_code": "8", "total_functions": 1,
                                              "matched_functions": 1},
         "functions": [fn("FUN_10", 0x10, 8, True)], "metadata": meta},
        {"name": "M/sgxa.c", "measures": {"total_code": "8", "total_functions": 1},
         "functions": [fn("FUN_28", 0x28, 8)], "metadata": meta},
        {"name": "M/sdk-library-not-published", "measures": {"total_code": "116", "total_functions": 11},
         "metadata": dict(meta, auto_generated=True)},
        src20]
    old = dict(published(), **{"docs/objdiff-report.json": json.dumps(report(old_units, 12, 1))})
    new = dict(published(), **{"docs/objdiff-report.json": json.dumps(report(new_units, 20, 2))})
    # one unit fewer, as when several matches of one source leave their units
    doc = json.loads(new["docs/objdiff-report.json"])
    doc["measures"]["total_units"] = 3
    new["docs/objdiff-report.json"] = json.dumps(doc)
    return old, new


class GuardTests(unittest.TestCase):
    def test_equal_or_higher_figures_pass(self) -> None:
        self.assertEqual(R.guard(published(), published()), [])
        higher = {"progress.json": badge(0.1, 17.2), "docs/milestones.json": json.dumps(milestones(30.0, 20.0)),
                  "docs/objdiff-report.json": json.dumps(objdiff(500, 12))}
        self.assertEqual(R.guard(published(), higher), [])

    def test_t2_and_portable_dropping_to_zero_refuse(self) -> None:
        # the 2026-10-22 near miss: a clean worktree without the ignored build and port reports
        cand = dict(published(), **{"docs/milestones.json": json.dumps(milestones(0.0, 0.0))})
        drops = R.guard(published(), cand)
        self.assertEqual(len(drops), 2)
        self.assertTrue(any("psp-rebuild.progress" in d for d in drops))
        self.assertTrue(any("portable-source.progress" in d for d in drops))

    def test_badge_and_report_drops_refuse(self) -> None:
        cand = dict(published(), **{"progress.json": badge(0.0, 17.0),
                                    "docs/objdiff-report.json": json.dumps(objdiff(399, 10))})
        drops = R.guard(published(), cand)
        self.assertIn("progress.json t1_percent: 17.1 published, now 17", drops)
        self.assertTrue(any(d.startswith("docs/objdiff-report.json matched_code:") for d in drops))
        self.assertTrue(any(d.startswith("docs/objdiff-report.json A.BIN.matched_code:") for d in drops))

    def test_null_missing_and_unparseable_figures_refuse(self) -> None:
        doc = objdiff()
        doc["measures"]["matched_code"] = None
        del doc["units"][0]["measures"]["matched_functions"]
        cand = dict(published(), **{"docs/objdiff-report.json": json.dumps(doc), "progress.json": '{"message": "?"}'})
        drops = R.guard(published(), cand)
        self.assertIn("docs/objdiff-report.json matched_code: 400 published, now null", drops)
        self.assertIn("docs/objdiff-report.json A.BIN.matched_functions: 10 published, now missing", drops)
        self.assertIn("progress.json strict_percent: 0 published, now null", drops)
        gone = {k: v for k, v in published().items() if k != "docs/milestones.json"}
        self.assertEqual(R.guard(published(), gone), ["docs/milestones.json: published, now missing"])

    def test_a_reached_milestone_cannot_be_lost_or_regress(self) -> None:
        old = {"docs/milestones.json": json.dumps(milestones(100.0, reached="2000-01-01"))}
        lost = {"docs/milestones.json": json.dumps(milestones(100.0, reached=None))}
        regressed = {"docs/milestones.json": json.dumps(milestones(100.0, reached="2000-01-01", regressed=True))}
        self.assertTrue(any("psp-rebuild.reached" in d for d in R.guard(old, lost)))
        self.assertTrue(any("psp-rebuild.not_regressed" in d for d in R.guard(old, regressed)))

    def test_detail_counts_that_fall_with_progress_are_not_figures(self) -> None:
        cand = milestones()
        cand["milestones"][1]["detail"] = "2 candidate spans open"
        self.assertEqual(R.lowered("docs/milestones.json", milestones(), cand), [])

    def test_unit_layout_changes_from_new_matches_pass(self) -> None:
        # the 2026-11-07 case: matched functions leave their not-yet-recovered units for source units, and SDK-class
        # matches leave a recovered file's unit for the unlisted SDK unit; every match figure goes up
        self.assertEqual(R.guard(*structural()), [])

    def test_a_vanished_unit_whose_function_is_not_matched_elsewhere_refuses(self) -> None:
        old, new = structural()
        doc = json.loads(new["docs/objdiff-report.json"])
        unit(doc, "src/m/f_10.c")["functions"][0]["fuzzy_match_percent"] = 99.0
        drops = R.guard(old, dict(new, **{"docs/objdiff-report.json": json.dumps(doc)}))
        self.assertTrue(any("FUN_10" in d and "not matched" in d for d in drops), drops)
        del unit(doc, "src/m/f_10.c")["functions"][0]
        drops = R.guard(old, dict(new, **{"docs/objdiff-report.json": json.dumps(doc)}))
        self.assertTrue(any("FUN_10" in d and "disappeared" in d for d in drops), drops)

    def test_a_function_the_unlisted_unit_does_not_absorb_refuses(self) -> None:
        old, new = structural()
        doc = json.loads(new["docs/objdiff-report.json"])
        unit(doc, "M/sdk-library-not-published")["measures"] = {"total_code": "100", "total_functions": 10}
        drops = R.guard(old, dict(new, **{"docs/objdiff-report.json": json.dumps(doc)}))
        self.assertTrue(any("FUN_30" in d and "disappeared" in d for d in drops), drops)
        # a function that was matched cannot vanish into the unlisted unit, which counts no matched bytes
        doc = json.loads(old["docs/objdiff-report.json"])
        unit(doc, "M/sgxa.c")["functions"][1]["fuzzy_match_percent"] = 100.0
        drops = R.guard(dict(old, **{"docs/objdiff-report.json": json.dumps(doc)}), new)
        self.assertTrue(any("FUN_30" in d and "disappeared" in d for d in drops), drops)

    def test_a_matched_figure_drop_still_refuses_beside_layout_changes(self) -> None:
        old, new = structural()
        doc = json.loads(new["docs/objdiff-report.json"])
        doc["measures"]["matched_code"] = "4"
        unit(doc, "src/m/f_20.c")["measures"]["matched_code"] = "4"
        drops = R.guard(old, dict(new, **{"docs/objdiff-report.json": json.dumps(doc)}))
        self.assertIn("docs/objdiff-report.json matched_code: 12 published, now 4", drops)
        self.assertIn("docs/objdiff-report.json src/m/f_20.c.matched_code: 12 published, now 4", drops)

    def test_a_file_never_published_has_no_baseline(self) -> None:
        self.assertEqual(R.guard({"progress.json": None, "docs/milestones.json": None,
                                  "docs/objdiff-report.json": None}, published()), [])


def fig() -> dict:
    row = lambda f, m, b, mb: {"functions": f, "matched_functions": m, "code_bytes": b, "matched_code_bytes": mb}  # noqa: E731
    return {"modules": {"EBOOT.BIN": row(100, 10, 4000, 400), "hstp_trans_main.prx": row(50, 5, 1000, 50)},
            "total": dict(row(150, 15, 5000, 450), percent_code_bytes=9.0), "sources": 14, "near_misses": 3}


def ladder() -> dict:
    return P.tier_ladder(10000, 450, {"A": {"module": "A", "t2_source_bytes": {"src/a.c": 1234}}}, None)


KINDS = {"sources": 14, "kinds": {"clone": 6, "generated": 3, "corrected": 1, "hand": 2, "template": 1, "library": 1},
         "classes": {"game": 12, "newlib": 2}, "newlib_clones": 1,
         "profiles": {"mwcc219": 11, "gcc171-O2": 2, "gcc171-O0": 1}}

DOCS = {
    "README.md": "# T\n\n<!-- hstprogress:begin readme-glance -->\n\nold\n\n<!-- hstprogress:end -->\n\n"
                 "<!-- hstprogress:begin readme-progress -->\n\nold\n\n| a |\n|---|\n| 1 |\n\n"
                 "<!-- hstprogress:end -->\n\nAfter.\n",
    "docs/COMPLETION.md": "| Tier | when | Measured <!-- hstprogress:begin completion-date -->1999-01-01"
                          "<!-- hstprogress:end --> (public) |\n| --- | --- | --- |\n"
                          "| T1 | x | <!-- hstprogress:begin completion-t1 -->1% (1 B<!-- hstprogress:end -->; code) |\n"
                          "| T2 | x | <!-- hstprogress:begin completion-t2 -->2% (2 B<!-- hstprogress:end -->; all) |\n"
                          "| T3 | x | <!-- hstprogress:begin completion-t3 -->3% (3 B<!-- hstprogress:end -->; s) |\n"
                          "| T4 | x | <!-- hstprogress:begin completion-t4 -->0%<!-- hstprogress:end --> |\n",
    "docs/GUIDE.md": "- First item.\n  <!-- hstprogress:begin guide-source-kinds -->\n- old one\n- old two\n"
                     "  <!-- hstprogress:end -->\n- Last item.\n- `cc=` item.\n"
                     "  <!-- hstprogress:begin guide-profiles -->\n  old\n  <!-- hstprogress:end -->\n- After.\n",
    "docs/PROGRESS.md": "# P\n\n<!-- hstprogress:begin progress-figures -->\n\nold\n\n<!-- hstprogress:end -->\n\n"
                        "## Next\n"}


class BlockTests(unittest.TestCase):
    def render(self, date: str = "2000-02-03") -> dict[str, str]:
        return P.render_blocks(fig(), ladder(), KINDS, date)

    def test_render_matches_the_published_formats(self) -> None:
        b = self.render()
        self.assertIn("**14 exact sources** (15 functions) and 3 near misses, measured 2000-02-03", b["readme-progress"])
        self.assertIn("| `EBOOT.BIN` | 10 / 100 | 400 / 4,000 | 10.000 % |", b["readme-progress"])
        self.assertIn("| `hstp_trans_main.prx` (Sharing Version) | 5 / 50 | 50 / 1,000 | 5.000 % |", b["readme-progress"])
        self.assertTrue(b["readme-progress"].endswith("| **Both** | 15 / 150 | 450 / 5,000 | 9.000 % |"))
        self.assertEqual((b["completion-date"], b["completion-t1"], b["completion-t2"], b["completion-t4"]),
                         ("2000-02-03", "4.500% (450 B", "12.340% (1,234 B", "0.000% (0 B)"))
        glance = b["readme-glance"]
        self.assertIn("| **Code matched** (T1) | **9.0%** | 450 of the 5,000 code bytes", glance)
        self.assertIn("| **Functions matched** (T1) | **10.0%** | 15 of the 150 game functions. |", glance)
        self.assertIn("| **Built from source** (T2) | **12.3%** |", glance)
        self.assertTrue(glance.endswith("That figure is 0.0%. Measured 2000-02-03 from the maintainer's private ledger of accepted proofs by "
                                       "`tools/hstprogress.py`, which needs that ledger."))
        built = P.tier_ladder(10000, 450, {"A": {"module": "A", "t2_source_bytes": {"src/a.c": 1234},
                                                 "built": "2000-01-31"}}, None)
        dated = P.render_blocks(fig(), built, KINDS, "2000-02-03")["readme-glance"]
        self.assertTrue(dated.endswith("Measured 2000-02-03 from the maintainer's private ledger of accepted proofs by `tools/hstprogress.py`, which "
                                       "needs that ledger; built from source by "
                                       "the whole-module build of 2000-01-31."))
        self.assertEqual(P.DATE.search(dated).group(0), "2000-02-03")     # --check-readme reads the first date
        guide = b["guide-source-kinds"]
        self.assertTrue(guide.startswith("- Of the 14 sources (measured 2000-02-03), 6 are address-substituted"))
        self.assertIn("(1 more are clones of them", guide.replace("\n  ", " "))
        self.assertIn("- 2 of the sources are newlib library functions", guide)
        self.assertIn("all others are game code", guide.replace("\n  ", " "))
        self.assertTrue(all(len(line) <= 118 for line in guide.splitlines()))
        self.assertEqual(b["guide-profiles"], "  Measured 2000-02-03, the public sources use `mwcc219` (11 sources), "
                                              "`gcc171-O2` (2 sources) and `gcc171-O0` (1\n  source).")
        figures = b["progress-figures"]
        self.assertTrue(figures.startswith("Measured 2000-02-03 from the public sources: 14 exact sources and 3 near"))
        self.assertIn("| Code matched, both game modules | 450 B (15 functions) | 5,000 B (150 functions) | 9.000% |",
                      figures)
        self.assertIn("| `EBOOT.BIN` code | 400 B (10 functions) | 4,000 B (100 functions) | 10.000% |", figures)
        self.assertIn("| Tier 2: whole-module build | 1,234 B | the six modules in scope, 10,000 B | 12.340% |", figures)
        self.assertTrue(figures.endswith("| Tier 4: strict (reviewed) | 0 B | the six modules in scope, 10,000 B | 0.000% |"))

    def test_unexpected_classes_and_kinds_are_named_not_hidden(self) -> None:
        kinds = copy.deepcopy(KINDS)
        kinds["classes"]["mw-runtime"] = 1
        kinds["kinds"]["unknown"] = 2
        guide = P.render_blocks(fig(), ladder(), kinds, "2000-02-03")["guide-source-kinds"].replace("\n  ", " ")
        self.assertIn("12 are game code compiled with Metrowerks `mwccpsp`, and 1 are of origin class `mw-runtime`", guide)
        self.assertIn("and 2 have no known note kind", guide)

    def test_rewrite_keeps_the_layout_of_each_block(self) -> None:
        b = self.render()
        readme, missing = P.rewrite_blocks(DOCS["README.md"], b)
        self.assertEqual(missing, [n for n in b if n not in ("readme-glance", "readme-progress")])
        self.assertTrue(readme.startswith("# T\n\n<!-- hstprogress:begin readme-glance -->\n\n| | Progress |"))
        self.assertIn("<!-- hstprogress:begin readme-progress -->\n\n**14 exact", readme)
        self.assertIn("9.000 % |\n\n<!-- hstprogress:end -->\n\nAfter.\n", readme)      # the table ends before the marker
        completion, _ = P.rewrite_blocks(DOCS["docs/COMPLETION.md"], b)
        self.assertIn("| T1 | x | <!-- hstprogress:begin completion-t1 -->4.500% (450 B<!-- hstprogress:end -->; code) |",
                      completion)
        self.assertIn("Measured <!-- hstprogress:begin completion-date -->2000-02-03<!-- hstprogress:end --> (public)",
                      completion)
        guide, _ = P.rewrite_blocks(DOCS["docs/GUIDE.md"], b)
        self.assertTrue(guide.startswith("- First item.\n  <!-- hstprogress:begin guide-source-kinds -->\n- Of the 14"))
        self.assertIn("`mwccpsp`.\n  <!-- hstprogress:end -->\n- Last item.\n", guide)          # one list, no gap
        self.assertTrue(guide.endswith("<!-- hstprogress:begin guide-profiles -->\n  Measured 2000-02-03"
                                       ", the public sources use `mwcc219` (11 sources), `gcc171-O2` (2 sources) and "
                                       "`gcc171-O0` (1\n  source).\n  <!-- hstprogress:end -->\n- After.\n"))
        progress, _ = P.rewrite_blocks(DOCS["docs/PROGRESS.md"], b)
        self.assertIn("<!-- hstprogress:begin progress-figures -->\n\nMeasured 2000-02-03", progress)
        self.assertIn("0.000% |\n\n<!-- hstprogress:end -->\n\n## Next\n", progress)       # the table ends before the marker
        again, _ = P.rewrite_blocks(guide, b)
        self.assertEqual(again, guide)                                                      # idempotent
        self.assertEqual(P.blocks_in(guide)["guide-source-kinds"], b["guide-source-kinds"])
        self.assertEqual(P.blocks_in(readme)["readme-progress"], b["readme-progress"])

    def test_text_outside_blocks_and_unknown_blocks_are_untouched(self) -> None:
        text = "a <!-- hstprogress:begin other -->keep<!-- hstprogress:end --> b"
        self.assertEqual(P.rewrite_blocks(text, self.render())[0], text)

    def test_write_then_check_every_block(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            for rel, text in DOCS.items():
                (root / rel).parent.mkdir(parents=True, exist_ok=True)
                (root / rel).write_text(text, encoding="utf-8")
            render = lambda d: P.render_blocks(fig(), ladder(), KINDS, d)          # noqa: E731
            self.assertEqual(len(P.compare_blocks(render, root)), 9)    # every block but the date, which is checked as itself
            self.assertEqual(sorted(P.write_blocks(render("2000-02-03"), root)),
                             ["README.md", "docs/COMPLETION.md", "docs/GUIDE.md", "docs/PROGRESS.md"])
            self.assertEqual(P.compare_blocks(render, root), [])
            self.assertEqual(P.write_blocks(render("2000-02-03"), root), [])     # nothing left to rewrite
            # a later day is not a difference by itself (each block is checked with the date it shows) ...
            self.assertEqual(P.compare_blocks(lambda d: P.render_blocks(fig(), ladder(), KINDS, d), root), [])
            # ... but a changed figure is, and a removed block is reported by name
            more = copy.deepcopy(KINDS)
            more["kinds"]["hand"] += 1
            diffs = P.compare_blocks(lambda d: P.render_blocks(fig(), ladder(), more, d), root)
            self.assertEqual(diffs, ["docs/GUIDE.md: block guide-source-kinds differs from the figures measured now"])
            (root / "docs/GUIDE.md").write_text("- no block\n", encoding="utf-8")
            self.assertEqual(P.compare_blocks(render, root), ["docs/GUIDE.md: no hstprogress block guide-source-kinds",
                                                          "docs/GUIDE.md: no hstprogress block guide-profiles"])
            with self.assertRaises(SystemExit):
                P.write_blocks(render("2000-02-03"), root)


if __name__ == "__main__":
    unittest.main()
