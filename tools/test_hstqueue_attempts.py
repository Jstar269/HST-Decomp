#!/usr/bin/env python3
"""Synthetic tests for the attempt ledger (hstattempts.py) and the tiers hstqueue.py offers."""
from __future__ import annotations

import json
import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hstattempts as A
import hstqueue as Q

HEADER = "/* HSTMATCH module=EBOOT.BIN start=0x{start:08x} end=0x{end:08x} symbol=FUN_{start:08x} lang=c99 */\n"


def item(start: int, size: int = 320) -> dict:
    return {"address": f"0x{start:08x}", "end": f"0x{start + size:08x}", "size": size, "family": []}


class ProgressParsingTests(unittest.TestCase):
    def test_later_lines_supersede_earlier_ones(self) -> None:
        text = "\n".join([
            "0x00001000 NEAR 4 wip/a_00001000.cpp",
            "0x00001000 EXACT src/eboot/a_00001000.cpp (supersedes NEAR)",
            "0x00002000 SKIP retail uses the non-folding lui/addiu/lw form",
            "0x00003000 NEAR 1-word-short 66/67 (prologue param-copy order) wip/c_00003000.c",
            "0x00004000 NEAR 3-words (64/64 words) wip/d_00004000.c",
            "0x00005000 NEAR 58/70 (out-of-line case bodies) wip/e_00005000.c",
            "--- 2026-10-21 round total: 1 EXACT / 3 NEAR / 1 SKIP / 2 untouched of 7",
        ])
        records, tally = A.parse_progress(text)
        self.assertEqual(records[0x1000].status, "EXACT")
        self.assertEqual(records[0x1000].src, "src/eboot/a_00001000.cpp")
        self.assertEqual(records[0x2000].status, "SKIP")
        self.assertIn("non-folding", records[0x2000].detail)
        self.assertEqual(records[0x3000].words_left, 1)
        self.assertEqual(records[0x3000].wip, "wip/c_00003000.c")
        self.assertEqual(records[0x4000].words_left, 3)
        self.assertIsNone(records[0x5000].words_left)   # "58/70" is ambiguous, so it stays unknown
        self.assertEqual(tally, (1, 3, 1, 2, 7))


class BlockedListTests(unittest.TestCase):
    def write(self, directory: str, doc: dict) -> Path:
        path = Path(directory) / "blocked.json"
        path.write_text(json.dumps(doc), encoding="utf-8")
        return path

    def test_rejects_duplicates_extra_keys_and_wrong_schema(self) -> None:
        entry = {"module": "EBOOT.BIN", "start": "0x00001000", "reason": "x"}
        with tempfile.TemporaryDirectory() as tmp:
            self.assertEqual(list(A.load_blocked(self.write(tmp, {"schema": A.BLOCKED_SCHEMA, "entries": [entry]}))),
                             [("EBOOT.BIN", 0x1000)])
            with self.assertRaises(ValueError):
                A.load_blocked(self.write(tmp, {"schema": A.BLOCKED_SCHEMA, "entries": [entry, entry]}))
            with self.assertRaises(ValueError):
                A.load_blocked(self.write(tmp, {"schema": A.BLOCKED_SCHEMA, "entries": [dict(entry, bytes="deadbeef")]}))
            with self.assertRaises(ValueError):
                A.load_blocked(self.write(tmp, {"schema": "other", "entries": [entry]}))

    def test_the_committed_list_is_well_formed_and_holds_the_seven_dead_ends(self) -> None:
        blocked = A.load_blocked()
        for start in (0x22060c, 0x21f0f8, 0xf1828, 0xf2024, 0x2239ec, 0x21f5c4, 0x15dbc8):
            self.assertIn(("EBOOT.BIN", start), blocked)
            self.assertEqual(blocked[("EBOOT.BIN", start)]["reason"], "unfolded-global-read")
        self.assertTrue(all(module in A.MODULES for module, _start in blocked))
        text = A.BLOCKED_PATH.read_text(encoding="utf-8")
        self.assertNotRegex(text, r"[A-Za-z]:[\\/]")    # address-only: no workstation paths


class SpanTests(unittest.TestCase):
    def test_a_multi_function_source_covers_the_functions_inside_its_span(self) -> None:
        src = {("EBOOT.BIN", 0x100): {"start": 0x100, "end": 0x200}, ("EBOOT.BIN", 0x400): {"start": 0x400, "end": 0x440}}
        starts, maxend = A.exact_spans(src, "EBOOT.BIN")
        self.assertTrue(A.covered_by_span(starts, maxend, 0x180, 0x1c0))
        self.assertTrue(A.covered_by_span(starts, maxend, 0x100, 0x200))
        self.assertFalse(A.covered_by_span(starts, maxend, 0x1f0, 0x210))
        self.assertFalse(A.covered_by_span(starts, maxend, 0x80, 0x90))
        self.assertFalse(A.covered_by_span(starts, maxend, 0x300, 0x340))


class LedgerTests(unittest.TestCase):
    """A synthetic tree: one recorded lane (ds-a), one batch with no record (old-b), src/, wip/ and a blocked list."""

    def build(self, tmp: Path, *, finished: bool = True) -> A.Attempts:
        (tmp / "queue").mkdir()
        (tmp / "root" / "src" / "eboot").mkdir(parents=True)
        (tmp / "root" / "wip").mkdir()
        if finished:
            (tmp / "root" / "try" / "harvest-ds-a").mkdir(parents=True)     # the lane has been harvested
        (tmp / "ext" / "ds-a").mkdir(parents=True)
        starts = [0x1000, 0x2000, 0x3000, 0x4000, 0x5000, 0x6000, 0x7000, 0x8000]
        (tmp / "queue" / "ds-a-00.json").write_text(json.dumps(
            {"module": "EBOOT.BIN", "kind": "singles", "items": [item(s) for s in starts]}), encoding="utf-8")
        (tmp / "queue" / "old-b-00.json").write_text(json.dumps(
            {"module": "EBOOT.BIN", "kind": "singles", "items": [item(0x9000), item(0x4000)]}), encoding="utf-8")
        (tmp / "ext" / "ds-a" / "LANE.json").write_text(json.dumps({"lane": "ds-a", "batch": "ds-a-00.json"}), encoding="utf-8")
        (tmp / "ext" / "ds-a" / "PROGRESS.md").write_text("\n".join([
            "0x00001000 EXACT src/eboot/f_00001000.c",
            "0x00002000 NEAR 5 wip/g_00002000.c",
            "0x00003000 SKIP parked",
            "0x00005000 NEAR 9 wip/h_00005000.c",
            "0x00005000 SKIP gave up",
            "0x00006000 SKIP retail uses the non-folding lui/addiu/lw form",
        ]) + "\n", encoding="utf-8")
        (tmp / "root" / "src" / "eboot" / "f_00001000.c").write_text(HEADER.format(start=0x1000, end=0x1140) + "int x;\n", encoding="utf-8")
        (tmp / "root" / "wip" / "g_00002000.c").write_text(HEADER.format(start=0x2000, end=0x2140) + "int x;\n", encoding="utf-8")
        (tmp / "blocked.json").write_text(json.dumps({"schema": A.BLOCKED_SCHEMA, "entries": [
            {"module": "EBOOT.BIN", "start": "0x00006000", "reason": "unfolded-global-read"}]}), encoding="utf-8")
        return A.collect(("EBOOT.BIN",), queue_dir=tmp / "queue", blocked_path=tmp / "blocked.json",
                         ext_root=tmp / "ext", measurements={}, root=tmp / "root",
                         sizes={"EBOOT.BIN": {s: s + 320 for s in starts + [0x9000]}})

    def test_statuses_lane_tally_and_evidence(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            att = self.build(Path(tmp))
            status = {s: att.status("EBOOT.BIN", s)["status"] for s in (0x1000, 0x2000, 0x3000, 0x4000, 0x5000, 0x6000, 0x7000, 0x8000, 0x9000)}
            self.assertEqual(status, {0x1000: "exact", 0x2000: "near", 0x3000: "skip", 0x4000: "untouched",
                                      0x5000: "skip", 0x6000: "blocked", 0x7000: "untouched", 0x8000: "untouched",
                                      0x9000: "untouched"})
            self.assertEqual(att.status("EBOOT.BIN", 0x5000)["reason"], "gave up")      # the later line wins
            self.assertEqual(att.status("EBOOT.BIN", 0x7000)["evidence"], "lane-unreached")
            # 0x4000 is also in a batch with no per-function record: an attempt cannot be ruled out
            self.assertEqual(att.status("EBOOT.BIN", 0x4000)["evidence"], "queued-no-record")
            self.assertEqual(att.status("EBOOT.BIN", 0x9000)["evidence"], "queued-no-record")
            lane = att.lanes["ds-a"]
            # the lane's own view counts the dead end as a skip; the merged view calls it blocked
            self.assertEqual((lane["exact"], lane["near"], lane["skip"], lane["untouched"], lane["total"]), (1, 1, 3, 3, 8))
            self.assertEqual(lane["now"]["blocked"], 1)
            self.assertEqual(att.status("EBOOT.BIN", 0x2000)["words_left"], 5)

    def test_a_measurement_replaces_the_lane_count(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            att = self.build(Path(tmp))
            self.assertEqual(att.status("EBOOT.BIN", 0x2000)["words_left_source"], "lane line")
            root = Path(tmp) / "root"
            att = A.collect(("EBOOT.BIN",), queue_dir=Path(tmp) / "queue", blocked_path=Path(tmp) / "blocked.json",
                            ext_root=Path(tmp) / "ext", root=root,
                            measurements={"wip/g_00002000.c": {"words_left": 2, "measured": "2026-10-21"}},
                            sizes={"EBOOT.BIN": {0x2000: 0x2140}})
            self.assertEqual(att.status("EBOOT.BIN", 0x2000)["words_left"], 2)
            self.assertIn("2026-10-21", att.status("EBOOT.BIN", 0x2000)["words_left_source"])

    def test_a_far_or_unbuildable_wip_file_is_a_draft_not_a_near_miss(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            self.build(Path(tmp))

            def status(measurements: dict) -> dict:
                return A.collect(("EBOOT.BIN",), queue_dir=Path(tmp) / "queue", blocked_path=Path(tmp) / "blocked.json",
                                 ext_root=Path(tmp) / "ext", root=Path(tmp) / "root", measurements=measurements,
                                 sizes={"EBOOT.BIN": {0x2000: 0x2140}}).status("EBOOT.BIN", 0x2000)

            at_limit = status({"wip/g_00002000.c": {"words_left": A.NEAR_WORDS_MAX, "words": 80, "class": "unknown"}})
            self.assertEqual((at_limit["status"], at_limit["evidence"]), ("near", "main-tree"))
            far = status({"wip/g_00002000.c": {"words_left": A.NEAR_WORDS_MAX + 1, "words": 80, "class": "missing_extra_insn"}})
            self.assertEqual((far["status"], far["evidence"], far["words_left"], far["words"]),
                             ("draft", "main-tree-far", A.NEAR_WORDS_MAX + 1, 80))
            self.assertEqual(far["wip"], "wip/g_00002000.c")                 # the draft stays named, as a hint
            broken = status({"wip/g_00002000.c": {"words_left": None, "words": 80, "class": "tool_state"}})
            self.assertEqual((broken["status"], broken["evidence"]), ("near", "main-tree"))   # the lane line says 5 words
            # a far draft is offered as fresh work, and never as a near miss
            self.assertEqual(Q.TIER_NAMES[Q.offer_tier([far])], "fresh")
            self.assertEqual(Q.TIER_NAMES[Q.offer_tier([far], near_mode="off")], "fresh")
            self.assertIsNone(Q.offer_tier([far], near_mode="only"))

    def test_an_unbuildable_wip_file_without_a_count_is_a_draft(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            tmp = Path(tmp)
            (tmp / "queue").mkdir()
            (tmp / "root" / "wip").mkdir(parents=True)
            (tmp / "root" / "wip" / "g_00002000.c").write_text(HEADER.format(start=0x2000, end=0x2140) + "int x;\n", encoding="utf-8")

            def status(cls: str) -> dict:
                return A.collect(("EBOOT.BIN",), queue_dir=tmp / "queue", blocked_path=tmp / "none.json", ext_root=None,
                                 root=tmp / "root", sizes={"EBOOT.BIN": {0x2000: 0x2140}},
                                 measurements={"wip/g_00002000.c": {"words_left": None, "words": 80, "class": cls}}
                                 ).status("EBOOT.BIN", 0x2000)

            self.assertEqual((status("tool_state")["status"], status("tool_state")["evidence"]), ("draft", "main-tree-unbuildable"))
            self.assertEqual(status("jump_table_rodata")["status"], "near")   # the code may match; only the table is unplaced

    def test_a_far_lane_near_line_is_a_draft(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            tmp = Path(tmp)
            (tmp / "queue").mkdir()
            (tmp / "root" / "try" / "harvest-ds-a").mkdir(parents=True)
            (tmp / "ext" / "ds-a").mkdir(parents=True)
            (tmp / "queue" / "ds-a-00.json").write_text(json.dumps(
                {"module": "EBOOT.BIN", "kind": "singles", "items": [item(0x1000), item(0x2000)]}), encoding="utf-8")
            (tmp / "ext" / "ds-a" / "LANE.json").write_text(json.dumps({"lane": "ds-a", "batch": "ds-a-00.json"}), encoding="utf-8")
            (tmp / "ext" / "ds-a" / "PROGRESS.md").write_text(
                "0x00001000 NEAR 21 wip/a_00001000.c\n0x00002000 NEAR 20 wip/b_00002000.c\n", encoding="utf-8")
            att = A.collect(("EBOOT.BIN",), queue_dir=tmp / "queue", blocked_path=tmp / "none.json", ext_root=tmp / "ext",
                            measurements={}, root=tmp / "root", sizes={"EBOOT.BIN": {0x1000: 0x1140, 0x2000: 0x2140}})
            self.assertEqual((att.status("EBOOT.BIN", 0x1000)["status"], att.status("EBOOT.BIN", 0x1000)["evidence"]),
                             ("draft", "lane-progress-far"))
            self.assertEqual(att.status("EBOOT.BIN", 0x2000)["status"], "near")
            self.assertEqual(att.summary["EBOOT.BIN"]["draft"]["functions"], 1)

    def test_no_ext_root_reads_only_the_main_tree_and_the_batches(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            self.build(Path(tmp))
            att = A.collect(("EBOOT.BIN",), queue_dir=Path(tmp) / "queue", blocked_path=Path(tmp) / "blocked.json",
                            ext_root=None, measurements={}, root=Path(tmp) / "root",
                            sizes={"EBOOT.BIN": {s: s + 320 for s in (0x1000, 0x2000, 0x3000, 0x6000, 0x7000)}})
            self.assertEqual(att.status("EBOOT.BIN", 0x7000)["evidence"], "queued-no-record")
            self.assertEqual(att.status("EBOOT.BIN", 0x1000)["status"], "exact")
            self.assertEqual(att.lanes, {})


    def test_an_unfinished_lane_reserves_every_address_of_its_batch(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            att = self.build(Path(tmp), finished=False)
            for start in (0x2000, 0x3000, 0x5000, 0x7000, 0x8000):
                self.assertEqual(att.status("EBOOT.BIN", start)["reserved_by"], ["ds-a-00"], hex(start))
            self.assertEqual(att.status("EBOOT.BIN", 0x7000)["evidence"], "reserved")
            self.assertNotIn("reserved_by", att.status("EBOOT.BIN", 0x1000))       # an installed source is never reserved
            self.assertFalse(att.lanes["ds-a"]["finished"])
            self.assertIsNone(Q.offer_tier([att.status("EBOOT.BIN", 0x7000)]))
            self.assertIsNone(Q.offer_tier([att.status("EBOOT.BIN", 0x2000)]))   # not even a near miss

    def test_a_report_file_also_finishes_a_lane(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            self.build(Path(tmp), finished=False)
            (Path(tmp) / "ext" / "ds-a" / "REPORT.md").write_text("done", encoding="utf-8")
            att = A.collect(("EBOOT.BIN",), queue_dir=Path(tmp) / "queue", blocked_path=Path(tmp) / "blocked.json",
                            ext_root=Path(tmp) / "ext", measurements={}, root=Path(tmp) / "root",
                            sizes={"EBOOT.BIN": {s: s + 320 for s in (0x7000,)}})
            self.assertTrue(att.lanes["ds-a"]["finished"])
            self.assertEqual(att.status("EBOOT.BIN", 0x7000)["evidence"], "lane-unreached")

    def test_family_members_are_queued_and_reserved_with_their_item(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            tmp = Path(tmp)
            (tmp / "queue").mkdir()
            (tmp / "ext" / "sn1").mkdir(parents=True)
            (tmp / "root").mkdir()
            fam = dict(item(0x1000), family=["0x00002000", "0x00003000"])
            (tmp / "queue" / "sn1-00.json").write_text(json.dumps(
                {"module": "EBOOT.BIN", "kind": "families", "items": [fam]}), encoding="utf-8")
            (tmp / "ext" / "sn1" / "LANE.json").write_text(json.dumps({"lane": "sn1", "batch": "sn1-00.json"}), encoding="utf-8")
            att = A.collect(("EBOOT.BIN",), queue_dir=tmp / "queue", blocked_path=tmp / "none.json", ext_root=tmp / "ext",
                            measurements={}, root=tmp / "root", sizes={"EBOOT.BIN": {}})
            for start in (0x1000, 0x2000, 0x3000):
                self.assertEqual(att.status("EBOOT.BIN", start)["reserved_by"], ["sn1-00"])
            self.assertIsNone(Q.offer_tier([att.status("EBOOT.BIN", 0x3000)]))


class TierTests(unittest.TestCase):
    def row(self, status: str, evidence: str = "") -> dict:
        return {"status": status, "evidence": evidence}

    def test_exact_and_blocked_are_never_offered(self) -> None:
        self.assertIsNone(Q.offer_tier([self.row("exact")]))
        self.assertIsNone(Q.offer_tier([None], exact=True))
        self.assertIsNone(Q.offer_tier([self.row("blocked"), self.row("near")]))
        self.assertIsNone(Q.offer_tier([self.row("blocked")], include_skipped=True, include_unrecorded=True, near_mode="first"))

    def test_untouched_is_offered_again_only_when_a_lane_proves_it_was_not_reached(self) -> None:
        self.assertEqual(Q.TIER_NAMES[Q.offer_tier([self.row("untouched", "lane-unreached")])], "untouched")
        self.assertIsNone(Q.offer_tier([self.row("untouched", "queued-no-record")]))
        self.assertEqual(Q.TIER_NAMES[Q.offer_tier([self.row("untouched", "queued-no-record")], include_unrecorded=True)], "unrecorded")
        self.assertEqual(Q.TIER_NAMES[Q.offer_tier([None])], "fresh")

    def test_near_misses_lead_and_the_options_switch_tiers_on_and_off(self) -> None:
        self.assertEqual(Q.TIER_NAMES[Q.offer_tier([self.row("near")])], "near")
        self.assertEqual(Q.TIER_NAMES[Q.offer_tier([None, self.row("near")])], "near")
        self.assertIsNone(Q.offer_tier([self.row("near")], near_mode="off"))
        self.assertIsNone(Q.offer_tier([None], near_mode="only"))
        self.assertIsNone(Q.offer_tier([self.row("skip")]))
        self.assertEqual(Q.TIER_NAMES[Q.offer_tier([self.row("skip")], include_skipped=True)], "skipped")

    def test_near_misses_rank_by_code_bytes_per_word_left(self) -> None:
        self.assertEqual(Q.near_yield(400, 4), 100.0)
        self.assertEqual(Q.near_yield(400, 0), 400.0)        # an exact-looking wip file is the cheapest of all
        self.assertEqual(Q.near_yield(400, None), 0.0)
        rows = [
            {"tier": 2, "legacy_priority": (1, 0.5), "tie_size": 4, "execution_count": 0, "execution_score": 0, "name": "fresh"},
            {"tier": 0, "words_left": None, "near_yield": 0.0, "unlocked_bytes": 900, "tie_size": 900, "name": "unmeasured"},
            {"tier": 0, "words_left": 8, "near_yield": Q.near_yield(800, 8), "unlocked_bytes": 800, "tie_size": 800, "name": "800B/8"},
            {"tier": 0, "words_left": 1, "near_yield": Q.near_yield(300, 1), "unlocked_bytes": 300, "tie_size": 300, "name": "300B/1"},
            {"tier": 0, "words_left": 40, "near_yield": Q.near_yield(1600, 40), "unlocked_bytes": 1600, "tie_size": 1600, "name": "1600B/40"},
            {"tier": 1, "legacy_priority": (0, 0.9), "tie_size": 4, "execution_count": 0, "execution_score": 0, "name": "untouched"},
        ]
        ordered = sorted(rows, key=lambda r: Q.queue_sort_key(r, False))
        self.assertEqual([r["name"] for r in ordered], ["300B/1", "800B/8", "1600B/40", "unmeasured", "untouched", "fresh"])

    def test_execution_rank_still_orders_within_the_untouched_and_fresh_tiers(self) -> None:
        rows = [
            {"tier": 2, "legacy_priority": -1000, "tie_size": 4, "execution_count": 0, "execution_score": 0, "name": "large-but-unseen"},
            {"tier": 2, "legacy_priority": -5, "tie_size": 12, "execution_count": 2, "execution_score": 24, "name": "observed"},
            {"tier": 1, "legacy_priority": -1, "tie_size": 4, "execution_count": 0, "execution_score": 0, "name": "untouched"},
        ]
        ordered = sorted(rows, key=lambda r: Q.queue_sort_key(r, True))
        self.assertEqual([r["name"] for r in ordered], ["untouched", "observed", "large-but-unseen"])

    def test_lanes_are_dealt_evenly_per_group_and_keep_rank_order(self) -> None:
        rows = [{"n": i, "g": "near-a" if i % 3 == 0 else ("near-b" if i % 3 == 1 else "fresh")} for i in range(30)]
        lanes = Q.deal_lanes(rows, 3, 8, lambda r: r["g"])
        self.assertEqual([len(lane) for lane in lanes], [8, 8, 8])
        taken = [r["n"] for lane in lanes for r in lane]
        self.assertEqual(sorted(taken), list(range(24)))             # the first 3*8 rows, each exactly once
        for lane in lanes:
            self.assertEqual([r["n"] for r in lane], sorted(r["n"] for r in lane))
        counts = [{g: sum(1 for r in lane if r["g"] == g) for g in ("near-a", "near-b", "fresh")} for lane in lanes]
        for g in ("near-a", "near-b", "fresh"):
            values = [c[g] for c in counts]
            self.assertLessEqual(max(values) - min(values), 1, g)

    def test_public_first_ranks_by_compiler_family_inside_a_tier(self) -> None:
        self.assertEqual([Q.family_rank(p) for p in (None, 0.95, 0.7, 0.4)], [0, 0, 1, 2])
        rows = [
            {"tier": 2, "legacy_priority": (0, 0.1), "tie_size": 1, "unlocked_bytes": 1, "execution_count": 0, "execution_score": 0, "family_p": 0.7, "n": "unsure"},
            {"tier": 2, "legacy_priority": (1, 0.9), "tie_size": 2, "unlocked_bytes": 2, "execution_count": 0, "execution_score": 0, "family_p": 0.99, "n": "metrowerks"},
            {"tier": 1, "legacy_priority": (1, 0.9), "tie_size": 2, "unlocked_bytes": 2, "execution_count": 0, "execution_score": 0, "family_p": 0.6, "n": "untouched"},
        ]
        off = [r["n"] for r in sorted(rows, key=lambda r: Q.queue_sort_key(r, False))]
        on = [r["n"] for r in sorted(rows, key=lambda r: Q.queue_sort_key(r, False, False, True))]
        self.assertEqual(off, ["untouched", "unsure", "metrowerks"])           # the old order
        self.assertEqual(on, ["untouched", "metrowerks", "unsure"])            # the tier still comes first

    def test_fresh_tier_can_rank_by_code_bytes(self) -> None:
        rows = [{"tier": 2, "legacy_priority": (0, 0.1), "tie_size": 100, "unlocked_bytes": 100, "execution_count": 0, "execution_score": 0, "n": "small"},
                {"tier": 2, "legacy_priority": (1, 0.9), "tie_size": 900, "unlocked_bytes": 900, "execution_count": 0, "execution_score": 0, "n": "big"}]
        self.assertEqual([r["n"] for r in sorted(rows, key=lambda r: Q.queue_sort_key(r, False, True))], ["big", "small"])
        self.assertEqual([r["n"] for r in sorted(rows, key=lambda r: Q.queue_sort_key(r, False, False))], ["small", "big"])


if __name__ == "__main__":
    unittest.main()
