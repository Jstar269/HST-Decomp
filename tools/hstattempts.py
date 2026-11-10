#!/usr/bin/env python3
"""Attempt ledger: what has been tried on each function, and which functions are known dead ends.

    python tools/hstattempts.py [--module EBOOT.BIN] [--out try/attempts.json] [--no-write] [--lane ds10]
                                [--list STATUS] [--check]
    python tools/hstattempts.py blocked          # validate data/blocked_functions.json and compare it with src/

Lane results used to live only in each lane's PROGRESS.md and in the trailing comments of wip/ files, and
hstqueue.py treated every function that any earlier batch listed as done. This tool reads those records once and
writes try/attempts.json, one row per function that has ANY record, with a single status:

  exact      a source in src/ covers it (or a lane reported EXACT that the main tree has not harvested yet:
             row key "lane_only")
  blocked    listed in data/blocked_functions.json (a proven dead end with the available toolchain); wins over every
             lane result except an installed source
  near       a wip/ file exists, or the newest lane line is NEAR, and it is within NEAR_WORDS_MAX (20) differing
             words of retail or not measured yet; "words_left" comes from the last measurement
             (try/nearmiss-report.json, written by tools/hstnearmiss.py) or from the lane line when it names a count
  draft      a wip/ file or lane NEAR line that is further away than that, or a wip/ file that does not reach the
             comparison (it does not compile or link). A draft is a hint for fresh work, not a near miss: hstqueue
             offers the function in the fresh tier and names the file (2026-11-09: 605 of the 702 "near misses" of
             EBOOT.BIN were such drafts, with a median of 87 % of their words differing)
  skip       the newest lane line is SKIP; "reason" is the lane's text
  untouched  queued in a batch that produced no result for it: either the lane is finished, kept a PROGRESS.md and
             never reached the function ("evidence": "lane-unreached", safe to offer again), or the batch has no
             per-function record at all, so an attempt cannot be ruled out ("evidence": "queued-no-record"), or the
             lane has not finished ("evidence": "reserved")

A lane is FINISHED when its work has been harvested (try/harvest-<lane>/ exists in the main tree) or it wrote a
REPORT.md. Every address in a batch of a lane that is NOT finished is reserved ("reserved_by" lists the batches, on
any status except exact): hstqueue never offers it again, so batches made at different times stay disjoint. Family
members of a queued item are reserved and recorded with it.

Inputs: every try/agent-queue/*.json batch (not released/), every lane under hst-config.json `ext_root` (LANE.json +
PROGRESS.md: lines `0xADDR EXACT <file>`, `0xADDR NEAR <n> <wip file>`, `0xADDR SKIP <reason>`; a later line for an
address supersedes an earlier one; a `--- ... N EXACT / N NEAR / N SKIP / N untouched of N` line is the lane's own
tally and is checked), src/ and wip/ of the main tree, and data/blocked_functions.json. Without an `ext_root` only
the main tree and the batches are read.

The per-lane tally (`lanes` in the JSON and the printed table) counts the lane's OWN view, so ds10 reports 13 exact /
16 near / 8 skip / 43 untouched of 80 even though five of its skips are blocked in the merged view.
"""
from __future__ import annotations

import argparse
import bisect
import json
import re
import sys
from collections import Counter, defaultdict
from dataclasses import dataclass, field
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hstconfig as CFG
import hstmatch as M

ROOT = M.ROOT
BLOCKED_PATH = ROOT / "data" / "blocked_functions.json"
QUEUE_DIR = ROOT / "try" / "agent-queue"
ATTEMPTS_PATH = ROOT / "try" / "attempts.json"
NEARMISS_REPORT = ROOT / "try" / "nearmiss-report.json"
STATUSES = ("exact", "near", "draft", "skip", "untouched", "blocked")
NEAR_WORDS_MAX = 20     # more differing words than this, and the file is a draft, not a near miss
UNBUILDABLE_CLASSES = ("tool_state",)   # hstnearmiss classes of a file that does not reach the comparison
BLOCKED_SCHEMA = "hst-decomp.blocked-functions.v1"
MODULES = ("EBOOT.BIN", "hstp_trans_main.prx")

PROGRESS_LINE = re.compile(r"^\s*(0x[0-9a-fA-F]+)\s+(EXACT|NEAR|SKIP)\b\s*(.*?)\s*$", re.I)
TALLY_LINE = re.compile(r"(\d+)\s+EXACT\s*/\s*(\d+)\s+NEAR\s*/\s*(\d+)\s+SKIP\s*/\s*(\d+)\s+untouched\s+of\s+(\d+)", re.I)
# "NEAR 4 wip/x.cpp", "NEAR 3-words (64/64 words) wip/x.c", "NEAR 1-word-short 66/67 (...) wip/x.c" name a count;
# "NEAR 58/70 (...) wip/x.c" does not (A/B is ambiguous), so its words_left stays unknown until measured.
NEAR_COUNT = re.compile(r"^(\d+)(?:-words?(?:-short)?)?(?![\d/])")
WIP_REF = re.compile(r"\b(wip/[\w.\-]+\.(?:cpp|c))\b")
SRC_REF = re.compile(r"\b(src/[\w./\-]+\.(?:cpp|c))\b")


@dataclass
class LaneRecord:
    status: str            # EXACT / NEAR / SKIP (upper case, as written)
    detail: str            # the rest of the line
    words_left: int | None = None
    wip: str | None = None
    src: str | None = None


@dataclass
class Lane:
    name: str
    batch: str                       # batch file stem, e.g. ds10-00
    path: Path
    progress: Path | None
    records: dict[int, LaneRecord] = field(default_factory=dict)
    tally_line: tuple[int, int, int, int, int] | None = None   # exact, near, skip, untouched, total as written
    mtime: float = 0.0
    finished: bool = False           # harvested into the main tree, or the lane wrote a REPORT.md

    @property
    def recorded(self) -> bool:
        return self.progress is not None


@dataclass
class Batch:
    name: str
    module: str
    kind: str
    mtime: float
    items: list[dict]
    lane: Lane | None = None

    @property
    def recorded(self) -> bool:
        """The batch's lane is finished and kept a per-function progress file."""
        return self.lane is not None and self.lane.finished and self.lane.recorded

    @property
    def reserved(self) -> bool:
        """The batch has a lane that is not finished: its addresses belong to that lane."""
        return self.lane is not None and not self.lane.finished


def _hex(value: int) -> str:
    return f"0x{value:08x}"


def read_text(path: Path) -> str:
    data = path.read_bytes()
    try:
        return data.decode("utf-8-sig")
    except UnicodeDecodeError:
        return data.decode("cp1252", errors="replace")


# --------------------------------------------------------------------------------------------------------------
# blocked list

def load_blocked(path: Path = BLOCKED_PATH) -> dict[tuple[str, int], dict]:
    """{(module, start): entry}. The file is address-only: module, start, reason, notes (and an optional class)."""
    if not path.exists():
        return {}
    doc = json.loads(path.read_text(encoding="utf-8"))
    if doc.get("schema") != BLOCKED_SCHEMA or not isinstance(doc.get("entries"), list):
        raise ValueError(f"{path}: not a {BLOCKED_SCHEMA} document")
    out: dict[tuple[str, int], dict] = {}
    for entry in doc["entries"]:
        extra = set(entry) - {"module", "start", "reason", "class", "notes"}
        if extra or not all(isinstance(entry.get(k), str) and entry[k] for k in ("module", "start", "reason")):
            raise ValueError(f"{path}: malformed entry {entry!r}")
        key = (entry["module"], int(entry["start"], 16))
        if key in out:
            raise ValueError(f"{path}: duplicate entry for {entry['module']} {entry['start']}")
        out[key] = entry
    return out


def blocked_keys(path: Path = BLOCKED_PATH) -> set[tuple[str, int]]:
    return set(load_blocked(path))


# --------------------------------------------------------------------------------------------------------------
# lanes and batches

def parse_progress(text: str) -> tuple[dict[int, LaneRecord], tuple[int, int, int, int, int] | None]:
    records: dict[int, LaneRecord] = {}
    tally = None
    for line in text.splitlines():
        m = TALLY_LINE.search(line)
        if m and line.lstrip().startswith("-"):
            tally = tuple(int(g) for g in m.groups())  # type: ignore[assignment]
            continue
        m = PROGRESS_LINE.match(line)
        if not m:
            continue
        address, status, detail = int(m.group(1), 16), m.group(2).upper(), m.group(3)
        words = None
        wip = None
        src = None
        if status == "NEAR":
            n = NEAR_COUNT.match(detail)
            words = int(n.group(1)) if n else None
            w = WIP_REF.search(detail)
            wip = w.group(1) if w else None
        elif status == "EXACT":
            s = SRC_REF.search(detail)
            src = s.group(1) if s else None
        records[address] = LaneRecord(status, detail, words, wip, src)  # later lines supersede earlier ones
    return records, tally


def ext_root_or_none() -> Path | None:
    try:
        return CFG.host_path("ext_root")
    except SystemExit:
        return None


def read_lanes(ext_root: Path | None, root: Path = ROOT) -> dict[str, Lane]:
    """{batch stem: Lane} for every lane directory with a LANE.json."""
    lanes: dict[str, Lane] = {}
    if ext_root is None or not ext_root.is_dir():
        return lanes
    for d in sorted(p for p in ext_root.iterdir() if p.is_dir()):
        meta = d / "LANE.json"
        if not meta.exists():
            continue
        try:
            info = json.loads(meta.read_text(encoding="utf-8-sig"))
        except ValueError:
            continue
        batch = str(info.get("batch", "")).removesuffix(".json")
        if not batch:
            continue
        progress = d / "PROGRESS.md"
        lane = Lane(name=str(info.get("lane", d.name)), batch=batch, path=d,
                    progress=progress if progress.exists() else None)
        lane.finished = (root / "try" / f"harvest-{lane.name}").is_dir() or (d / "REPORT.md").exists()
        if lane.progress is not None:
            lane.records, lane.tally_line = parse_progress(read_text(lane.progress))
            lane.mtime = lane.progress.stat().st_mtime
        lanes[batch] = lane
    return lanes


def read_batches(lanes: dict[str, Lane], queue_dir: Path = QUEUE_DIR) -> dict[str, Batch]:
    batches: dict[str, Batch] = {}
    if not queue_dir.is_dir():
        return batches
    for q in sorted(queue_dir.glob("*.json")):          # not released/: hstqueue never counted those either
        doc = json.loads(q.read_text(encoding="utf-8"))
        batches[q.stem] = Batch(q.stem, doc["module"], doc.get("kind", ""), q.stat().st_mtime, doc["items"],
                                lanes.get(q.stem))
    return batches


# --------------------------------------------------------------------------------------------------------------
# main tree

def scan_sources(root: Path = ROOT) -> tuple[dict[tuple[str, int], dict], dict[str, dict[int, list[dict]]]]:
    """(src rows by (module, start), wip rows by module -> start -> [row])."""
    src: dict[tuple[str, int], dict] = {}
    wip: dict[str, dict[int, list[dict]]] = defaultdict(lambda: defaultdict(list))
    for tree in ("src", "wip"):
        base = root / tree
        if not base.is_dir():
            continue
        for p in sorted(base.rglob("*")):
            if p.suffix not in (".c", ".cpp"):
                continue
            try:
                h = M.parse_header(p)
            except ValueError:
                continue
            row = {"path": p.relative_to(root).as_posix(), "start": h["start"], "end": h["end"], "module": h["module"]}
            if tree == "src":
                src[(h["module"], h["start"])] = row
            else:
                wip[h["module"]][h["start"]].append(row)
    return src, wip


def exact_spans(src: dict[tuple[str, int], dict], module: str) -> tuple[list[int], list[int]]:
    """Sorted span starts and the running maximum of span ends, to ask 'does a source span cover [a, b)' quickly."""
    spans = sorted((r["start"], r["end"]) for (m, _s), r in src.items() if m == module)
    starts, maxend, cur = [], [], 0
    for s, e in spans:
        cur = max(cur, e)
        starts.append(s)
        maxend.append(cur)
    return starts, maxend


def covered_by_span(starts: list[int], maxend: list[int], a: int, b: int) -> bool:
    i = bisect.bisect_right(starts, a) - 1
    return i >= 0 and maxend[i] >= b


def load_measurements(path: Path = NEARMISS_REPORT) -> dict[str, dict]:
    """{wip path: row} from the last tools/hstnearmiss.py run; {} when there is none."""
    if not path.exists():
        return {}
    try:
        doc = json.loads(path.read_text(encoding="utf-8"))
    except ValueError:
        return {}
    out = {}
    for row in doc.get("files", []):
        if isinstance(row, dict) and row.get("wip"):
            out[row["wip"]] = dict(row, measured=doc.get("generated", ""))
    return out


# --------------------------------------------------------------------------------------------------------------
# the ledger

@dataclass
class Attempts:
    rows: dict[tuple[str, int], dict]
    lanes: dict[str, dict]
    blocked: dict[tuple[str, int], dict]
    src: dict[tuple[str, int], dict]
    summary: dict

    def status(self, module: str, start: int) -> dict | None:
        return self.rows.get((module, start))

    def exact_or_blocked(self, module: str, start: int) -> bool:
        row = self.rows.get((module, start))
        return bool(row and row["status"] in ("exact", "blocked"))


def ledger_functions(module: str) -> dict[int, int]:
    ledger = json.loads((M.CAMPAIGN / "ledger" / "campaign.json").read_text(encoding="utf-8-sig"))
    mod = next(m for m in ledger["modules"] if m["name"] == module)
    return {f["start"]: f["end"] for f in mod["functions"]}


def collect(modules: tuple[str, ...] = MODULES, *, queue_dir: Path = QUEUE_DIR, blocked_path: Path = BLOCKED_PATH,
            ext_root: Path | None | str = "config", measurements: dict[str, dict] | None = None,
            root: Path = ROOT, sizes: dict[str, dict[int, int]] | None = None) -> Attempts:
    """Build the ledger. `sizes` ({module: {start: end}}) defaults to the campaign ledger; tests pass their own."""
    if ext_root == "config":
        ext_root = ext_root_or_none()
    lanes = read_lanes(ext_root if isinstance(ext_root, Path) else None, root)
    batches = read_batches(lanes, queue_dir)
    blocked = load_blocked(blocked_path)
    src, wip = scan_sources(root)
    measured = load_measurements() if measurements is None else measurements
    if sizes is None:
        sizes = {m: ledger_functions(m) for m in modules}
    rows: dict[tuple[str, int], dict] = {}

    def row_for(module: str, start: int) -> dict:
        key = (module, start)
        if key not in rows:
            end = sizes.get(module, {}).get(start)
            rows[key] = {"module": module, "start": _hex(start), "end": _hex(end) if end else None,
                         "size": (end - start) if end else None, "status": None, "words_left": None, "wip": None,
                         "src": None, "lanes": [], "batches": [], "reason": "", "evidence": "", "family": []}
        return rows[key]

    # batch membership
    by_function: dict[tuple[str, int], list[tuple[Batch, dict]]] = defaultdict(list)
    rep_of: dict[tuple[str, int, str], int] = {}           # (module, member, batch) -> the item's own address
    for batch in batches.values():
        for item in batch.items:
            address = int(item["address"], 16)
            by_function[(batch.module, address)].append((batch, item))
            for fam in item.get("family", []):             # members are queued, and reserved, with their item
                member = int(fam, 16)
                if member != address:
                    by_function[(batch.module, member)].append((batch, item))
                    rep_of[(batch.module, member, batch.name)] = address
    for (module, start), members in by_function.items():
        if module not in modules:
            continue
        row = row_for(module, start)
        for batch, item in members:
            row["batches"].append(batch.name)
            if batch.lane is not None:
                row["lanes"].append(batch.lane.name)
            if row["size"] is None and item.get("size"):
                row["size"] = item["size"]
                row["end"] = item.get("end")
            for fam in item.get("family", []):
                if fam not in row["family"]:
                    row["family"].append(fam)
    # lane lines (including helpers outside the batch)
    for lane in lanes.values():
        batch = batches.get(lane.batch)
        module = batch.module if batch else None
        if module is None or module not in modules:
            continue
        for address, rec in lane.records.items():
            row = row_for(module, address)
            if lane.name not in row["lanes"]:
                row["lanes"].append(lane.name)

    # near misses that no batch or lane ever mentioned (generated candidates, hstperm leftovers)
    for module, by_start in wip.items():
        if module not in modules:
            continue
        for start, wrows in by_start.items():
            r = row_for(module, start)
            if r["size"] is None:
                r["size"] = wrows[0]["end"] - start
                r["end"] = _hex(wrows[0]["end"])

    # resolve
    for (module, start), row in sorted(rows.items(), key=lambda kv: (kv[0][0], kv[0][1])):
        members = by_function.get((module, start), [])
        recorded = [(b.lane, b.lane.records[rep_of.get((module, start, b.name), start)]) for b, _i in members
                    if b.lane is not None and rep_of.get((module, start, b.name), start) in b.lane.records]
        # a record may also come from a lane whose batch does not list the address (a helper matched on the way)
        for lane in lanes.values():
            b = batches.get(lane.batch)
            if b and b.module == module and start in lane.records and (lane, lane.records[start]) not in recorded:
                recorded.append((lane, lane.records[start]))
        recorded.sort(key=lambda lr: lr[0].mtime)           # newest PROGRESS.md last
        lane_rec = recorded[-1][1] if recorded else None
        unreached = [b for b, _i in members
                     if b.recorded and rep_of.get((module, start, b.name), start) not in b.lane.records]
        unrecorded = [b for b, _i in members if not b.recorded and not b.reserved]
        reserved = [b.name for b, _i in members if b.reserved]
        wip_rows = wip.get(module, {}).get(start, [])
        src_row = src.get((module, start))
        if src_row is not None:
            row.update(status="exact", src=src_row["path"], evidence="main-tree", reason="")
        elif (module, start) in blocked:
            ent = blocked[(module, start)]
            row.update(status="blocked", reason=ent["reason"], evidence="blocked-list")
        elif wip_rows:
            best = None
            for w in wip_rows:
                m = measured.get(w["path"])
                if m and m.get("words_left") is not None and (best is None or m["words_left"] < best[0]):
                    best = (m["words_left"], w["path"], m.get("measured", ""))
            row.update(status="near", wip=(best[1] if best else wip_rows[0]["path"]), evidence="main-tree")
            if best:
                row["words_left"] = best[0]
                row["words_left_source"] = f"measured {best[2]}".strip()
            elif lane_rec is not None and lane_rec.words_left is not None:
                row["words_left"] = lane_rec.words_left
                row["words_left_source"] = "lane line"
            row["wip_files"] = [w["path"] for w in wip_rows]
            rows_measured = [measured[w["path"]] for w in wip_rows if w["path"] in measured]
            if row["words_left"] is not None and row["words_left"] > NEAR_WORDS_MAX:
                row.update(status="draft", evidence="main-tree-far")
            elif (row["words_left"] is None and rows_measured and len(rows_measured) == len(wip_rows)
                  and all(m.get("class_if_live", m.get("class")) in UNBUILDABLE_CLASSES for m in rows_measured)):
                row.update(status="draft", evidence="main-tree-unbuildable")   # every file fails before the comparison
            if row["status"] == "draft":
                words = next((m["words"] for m in rows_measured if m.get("words")), None)
                if words:
                    row["words"] = words
        elif lane_rec is not None:
            if lane_rec.status == "EXACT":
                row.update(status="exact", src=lane_rec.src, evidence="lane-progress", lane_only=True)
            elif lane_rec.status == "NEAR":
                far = lane_rec.words_left is not None and lane_rec.words_left > NEAR_WORDS_MAX
                row.update(status="draft" if far else "near", wip=lane_rec.wip, words_left=lane_rec.words_left,
                           evidence="lane-progress-far" if far else "lane-progress",
                           reason=lane_rec.detail, words_left_source="lane line" if lane_rec.words_left is not None else "")
            elif re.search(r"\bnot attempted\b", lane_rec.detail, re.I):
                # a budget skip (fm11, 2026-10-24: "not attempted: lane session budget was spent ..."): the lane never
                # tried the function, so it is unreached, not a dead end
                row.update(status="untouched", reason=lane_rec.detail,
                           evidence="reserved" if reserved else "lane-unreached")
            else:
                row.update(status="skip", reason=lane_rec.detail, evidence="lane-progress")
        else:
            row.update(status="untouched", reason="",
                       evidence=("reserved" if reserved else
                                 "queued-no-record" if unrecorded or not unreached else "lane-unreached"))
            if unrecorded:
                row["pending_batches"] = [b.name for b in unrecorded]
        if reserved and row["status"] != "exact":
            row["reserved_by"] = reserved

    # span coverage: functions inside a multi-function source count as exact (as hstcov does)
    for module in modules:
        starts, maxend = exact_spans(src, module)
        for (m, start), row in rows.items():
            if m != module or row["status"] == "exact" or not row["end"]:
                continue
            if covered_by_span(starts, maxend, start, int(row["end"], 16)):
                row.update(status="exact", evidence="main-tree-span", reason="")

    # underlying status of blocked rows, for the report
    for (module, start), row in rows.items():
        if row["status"] == "blocked":
            lane_view = [lr.status.lower() for lane in lanes.values() for a_, lr in lane.records.items() if a_ == start]
            row["lane_view"] = lane_view[-1] if lane_view else ("near" if wip.get(module, {}).get(start) else "")
            row["notes"] = blocked[(module, start)].get("notes", "")
    # blocked-list entries that no queue or lane ever mentioned still get a row
    for (module, start), ent in blocked.items():
        if module in modules and (module, start) not in rows:
            r = row_for(module, start)
            r.update(status="exact" if (module, start) in src else "blocked", reason=ent["reason"],
                     evidence="blocked-list", notes=ent.get("notes", ""))

    lane_table = {}
    for batch in batches.values():
        lane = batch.lane
        if lane is None or not lane.recorded:
            continue
        addresses = [int(i["address"], 16) for i in batch.items]
        tally = Counter()
        for address in addresses:
            rec = lane.records.get(address)
            tally[rec.status.lower() if rec else "untouched"] += 1
        now = Counter(rows[(batch.module, a)]["status"] for a in addresses if (batch.module, a) in rows)
        reported = lane.tally_line
        computed = (tally["exact"], tally["near"], tally["skip"], tally["untouched"], len(addresses))
        lane_table[lane.name] = {
            "batch": batch.name, "module": batch.module, "finished": lane.finished, "total": len(addresses),
            "exact": tally["exact"],
            "near": tally["near"], "skip": tally["skip"], "untouched": tally["untouched"],
            "reported_tally": list(reported) if reported else None,
            "tally_matches_reported": (reported == computed) if reported else None,
            "outside_batch": sum(1 for a in lane.records if a not in set(addresses)),
            "now": {s: now.get(s, 0) for s in STATUSES},
        }
    summary = _summarize(rows, modules)
    summary["main_tree"] = {"src_sources": len(src), "wip_files": sum(len(v) for m in wip.values() for v in m.values()),
                            "blocked_entries": len(blocked)}
    return Attempts(rows, lane_table, blocked, src, summary)


def _summarize(rows: dict[tuple[str, int], dict], modules: tuple[str, ...]) -> dict:
    out: dict = {}
    for module in modules:
        mine = [r for (m, _s), r in rows.items() if m == module]
        out[module] = {s: {"functions": sum(1 for r in mine if r["status"] == s),
                           "code_bytes": sum(r["size"] or 0 for r in mine if r["status"] == s)} for s in STATUSES}
        out[module]["untouched_by_evidence"] = dict(Counter(r["evidence"] for r in mine if r["status"] == "untouched"))
    return out


def to_json(att: Attempts, generated: str) -> dict:
    return {
        "schema": "hst-decomp.attempts.v1", "generated": generated,
        "note": "try/ is gitignored: regenerate with python tools/hstattempts.py",
        "summary": att.summary, "lanes": att.lanes,
        "functions": [dict(r) for _k, r in sorted(att.rows.items(), key=lambda kv: (kv[0][0], kv[0][1]))],
    }


# --------------------------------------------------------------------------------------------------------------
# CLI

def cmd_blocked() -> int:
    blocked = load_blocked()
    src, _wip = scan_sources()
    sizes = {m: ledger_functions(m) for m in MODULES}
    bad = 0
    by_reason = Counter()
    for (module, start), ent in sorted(blocked.items()):
        by_reason[ent["reason"]] += 1
        known = start in sizes.get(module, {})
        problems = []
        if not known:
            problems.append("not a ledger function start")
        if (module, start) in src:
            problems.append(f"has a source ({src[(module, start)]['path']}): remove it from the blocked list")
        if problems:
            bad += 1
            print(f"PROBLEM {module} {_hex(start)}: {'; '.join(problems)}")
    print(f"{len(blocked)} blocked functions: " + ", ".join(f"{n} {r}" for r, n in by_reason.most_common()))
    print("OK" if not bad else f"{bad} problem(s)")
    return 1 if bad else 0


def print_report(att: Attempts, lane_filter: str | None, list_status: str | None, module_filter: str | None) -> None:
    for name, t in sorted(att.lanes.items()):
        if lane_filter and name != lane_filter:
            continue
        ok = {True: "tally line agrees", False: "TALLY LINE DISAGREES", None: "no tally line in PROGRESS.md"}[t["tally_matches_reported"]]
        state = "finished" if t["finished"] else "NOT finished (reserved)"
        print(f"{name} ({t['batch']}): {t['exact']} exact, {t['near']} near, {t['skip']} skip, {t['untouched']} untouched "
              f"of {t['total']}  [{ok}; {state}]")
        now = ", ".join(f"{t['now'][s]} {s}" for s in STATUSES if t["now"][s])
        print(f"    merged view now: {now}" + (f"; {t['outside_batch']} lane line(s) outside the batch" if t["outside_batch"] else ""))
    print()
    for module, data in att.summary.items():
        if module == "main_tree" or (module_filter and module != module_filter):
            continue
        line = ", ".join(f"{data[s]['functions']} {s} ({data[s]['code_bytes']} B)" for s in STATUSES)
        print(f"{module}: {line}")
        print(f"    untouched by evidence: {data['untouched_by_evidence']}")
    mt = att.summary["main_tree"]
    print(f"main tree: {mt['src_sources']} sources in src/, {mt['wip_files']} files in wip/, {mt['blocked_entries']} blocked")
    if list_status:
        for (module, start), r in sorted(att.rows.items()):
            if r["status"] == list_status and (not module_filter or module == module_filter):
                extra = r["wip"] or r["reason"] or r["evidence"]
                print(f"  {module} {r['start']} {r['size']} {r['words_left'] if r['words_left'] is not None else '-'} {extra}")


def main() -> int:
    if len(sys.argv) > 1 and sys.argv[1] == "blocked":
        return cmd_blocked()
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--module", choices=MODULES, help="limit the printout to one module (the JSON always has both)")
    ap.add_argument("--out", type=Path, default=ATTEMPTS_PATH)
    ap.add_argument("--no-write", action="store_true")
    ap.add_argument("--lane")
    ap.add_argument("--list", choices=STATUSES, help="print every function of one status")
    ap.add_argument("--check", action="store_true", help="exit 1 when a lane's own tally line disagrees with its records")
    args = ap.parse_args()
    import datetime
    att = collect()
    print_report(att, args.lane, args.list, args.module)
    if not args.no_write:
        args.out.parent.mkdir(parents=True, exist_ok=True)
        args.out.write_text(json.dumps(to_json(att, datetime.date.today().isoformat()), indent=1) + "\n", encoding="utf-8")
        try:
            shown = args.out.resolve().relative_to(ROOT).as_posix()
        except ValueError:
            shown = str(args.out)
        print(f"wrote {shown}")
    if args.check and any(t["tally_matches_reported"] is False for t in att.lanes.values()):
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
