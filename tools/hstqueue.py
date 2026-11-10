#!/usr/bin/env python3
"""Build hand-matching batches for matching agents.

    python tools/hstqueue.py --name fam1 [--module EBOOT.BIN] [--kind families|singles] [--per-batch 8]
                             [--max-size N] [--min-size 64] [--batches 4] [--exclude 0xb49ec,...]
                             [--execution-rank try/execution-rank.json]
                             [--near first|off|only] [--near-max-left N] [--near-max-size N] [--lever body,structure,...]
                             [--include-skipped] [--include-unrecorded]
                             [--fresh-order legacy|bytes] [--lanes N --lane-size M] [--public-first [--min-p 0.9]] [--dry-run]

families: one representative per unmatched clone family (same masked code as hstclone.py), largest total
bytes first; a match is propagated to the other members by hstclone.py. singles: unmatched reviewed
functions that are not in any family, smallest alignment distance first when a generated candidate exists
(try/best-mismatch.json), then by size. Writes try/agent-queue/<name>-NN.json.

What is offered is decided by the attempt ledger (tools/hstattempts.py, which reads every lane's PROGRESS.md, the
earlier batches, src/, wip/ and data/blocked_functions.json), in this order of tiers:

  near        a near miss (wip/ file or lane NEAR line within 20 differing words, hstattempts.NEAR_WORDS_MAX),
              ranked by code bytes unlocked per word left (a family or Sharing twin multiplies the bytes); the item
              carries "wip" and "words_left"
  untouched   queued by a lane that kept a PROGRESS.md but never reached the function: offered again
  fresh       never queued, or only a far draft exists (hstattempts status "draft": a wip/ file more than 20 words
              from retail, or one that does not build). Such an item says so: "draft" names the file, with
              "draft_words_left" and "draft_words", as a hint for writing the function again, not as a seed to permute
  skipped     a lane SKIP, only with --include-skipped
  unrecorded  queued by a batch with no per-function record (an attempt cannot be ruled out), only with
              --include-unrecorded

A function covered by a source in src/ or listed in data/blocked_functions.json is never offered, and neither is
any address of a batch whose lane is not finished (hstattempts.py: "reserved"; a lane is finished once try/harvest-
<lane>/ exists or it wrote a REPORT.md), so batches made at different times stay disjoint. --near off keeps near
misses out, --near only offers nothing else, --near-max-left N drops near misses with more than N words left (and
unmeasured ones). Within a tier the order is the old one (twins first, then the generated-candidate distance; with
--execution-rank, observed entry-weighted functions lead); --fresh-order bytes ranks the fresh tier by code bytes
unlocked instead. --dry-run prints the plan and writes nothing.

Size. There is no upper size limit unless --max-size is given (until 2026-11-09 the default was 1600 and the lanes
passed 4000, so the 63 game functions of 4 KB or more, a quarter of the unmatched bytes, were never offered). Give
each lane the size band it is meant for with --min-size and --max-size.

--public-first ranks by compiler family (tools/hstfamily.py, a classifier whose accuracy is reported in its
docstring and by `hstfamily.py evaluate`): inside each tier the functions predicted to be Metrowerks code (the part of
the game the public repository can ship) come before the uncertain ones, and functions predicted to be Sony psp-gcc
SDK code (probability below 0.5) are left out. The address floor (--min-start) is lifted for functions below it that
are predicted Metrowerks with probability of at least --min-p (default 0.9). Predictions come from
try/family-predictions.json (written by `hstfamily.py predict`; made on demand when missing).

--lanes N --lane-size M writes N disjoint batches <name>1-00.json .. <name>N-00.json of M items each (for lanes
sn1..snN): the first N*M rows in rank order are dealt out so that every lane gets a similar number of items of each
group (near misses by lever from try/nearmiss-report.json, untouched, fresh), and each batch keeps the rank order,
near misses first.

Levers. tools/hstnearmiss.py gives every measured near miss a lever: the kind of work it needs (body, structure,
referent, permute, types, frame, expression; its docstring has the rules). A near-miss item carries "near_class",
"lever", "lever_why" (the evidence) and "lever_digest" (the docs/MWCC-DIGEST.md sections to read), so a lane works
on the right problem instead of guessing from the word count. --lever L1,L2 offers only near misses with one of those
levers (other tiers are unaffected); a near miss with no measurement has no lever and is left out by --lever.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import re
import sys
from collections import defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hstattempts as A
import hstclone as C
import hstgen as G
import hstmatch as M


def load_execution_rank(path: Path) -> tuple[dict[str, dict[int, dict[str, int]]], dict[str, object]]:
    """Load a complete bounded hstexec result tied to this exact campaign ledger."""
    try:
        raw = path.read_bytes()
        if len(raw) > 64 * 1024 * 1024:
            raise ValueError("file exceeds 64 MiB")
        document = json.loads(raw.decode("utf-8-sig"))
    except (OSError, UnicodeError, json.JSONDecodeError, ValueError) as exc:
        raise ValueError(f"cannot read execution rank ({type(exc).__name__}: {exc})") from exc
    if not isinstance(document, dict) or document.get("schema_version") != 1:
        raise ValueError("unsupported execution-rank schema")
    capture = document.get("capture")
    if not isinstance(capture, dict) or capture.get("complete") is not True:
        raise ValueError("execution rank is not a complete bounded trace")
    if capture.get("unseen_is_not_zero_usage") is not True:
        raise ValueError("execution rank does not preserve unseen-function uncertainty")
    source_commit = capture.get("source_commit")
    trace_sha256 = capture.get("trace_sha256")
    record_count, record_limit = capture.get("record_count"), capture.get("record_limit")
    if (not isinstance(source_commit, str) or not re.fullmatch(r"[0-9a-f]{7,64}", source_commit)
            or not isinstance(trace_sha256, str) or not re.fullmatch(r"[0-9a-f]{64}", trace_sha256)
            or not isinstance(capture.get("scope"), str)
            or isinstance(record_count, bool) or not isinstance(record_count, int)
            or isinstance(record_limit, bool) or not isinstance(record_limit, int)
            or record_count <= 0 or record_count != record_limit):
        raise ValueError("execution rank lacks its source, trace, or scope identity")
    ledger_path = M.CAMPAIGN / "ledger" / "campaign.json"
    ledger_sha256 = hashlib.sha256(ledger_path.read_bytes()).hexdigest()
    if document.get("ledger_sha256") != ledger_sha256:
        raise ValueError("execution rank belongs to a different campaign ledger")
    raw_modules = document.get("modules")
    if not isinstance(raw_modules, list):
        raise ValueError("execution rank has no module list")
    result: dict[str, dict[int, dict[str, int]]] = {}
    for module in raw_modules:
        if not isinstance(module, dict) or not isinstance(module.get("name"), str):
            raise ValueError("execution rank contains a malformed module")
        name = module["name"]
        if name in result:
            raise ValueError(f"execution rank repeats module {name}")
        rows = module.get("functions")
        if not isinstance(rows, list):
            raise ValueError(f"execution rank has no function rows for {name}")
        weights: dict[int, dict[str, int]] = {}
        for row in rows:
            if not isinstance(row, dict) or not isinstance(row.get("address"), str):
                raise ValueError(f"execution rank has a malformed function row for {name}")
            try:
                address = int(row["address"], 16)
                count = row["execution_count"]
                size = row["code_bytes"]
                score = row["weighted_score"]
            except (KeyError, TypeError, ValueError) as exc:
                raise ValueError(f"execution rank has malformed weight fields for {name}") from exc
            if (address < 0 or isinstance(count, bool) or not isinstance(count, int) or count < 0
                    or isinstance(size, bool) or not isinstance(size, int) or size <= 0
                    or isinstance(score, bool) or not isinstance(score, int) or score != count * size):
                raise ValueError(f"execution rank has inconsistent weights for {name}")
            if address in weights:
                raise ValueError(f"execution rank repeats a function address in {name}")
            weights[address] = {"execution_count": count, "weighted_score": score}
        result[name] = weights
    bringup = capture.get("bringup") if isinstance(capture.get("bringup"), dict) else {}
    presentation = bringup.get("presentation")
    frame_submissions = presentation.get("frame_submissions") if isinstance(presentation, dict) else None
    if (isinstance(frame_submissions, bool) or not isinstance(frame_submissions, int)
            or frame_submissions <= 0):
        raise ValueError("execution rank has no rendered frame submissions")
    capture_summary = {
        key: capture.get(key)
        for key in ("source_commit", "source_branch", "trace_kind", "trace_sha256",
                    "scope", "record_count", "record_limit", "unseen_is_not_zero_usage")
    }
    capture_summary["bringup_failure_class"] = bringup.get("failure_class")
    capture_summary["bringup_reached_stage"] = bringup.get("reached_stage")
    presentation = bringup.get("presentation")
    capture_summary["frame_submissions"] = (
        presentation.get("frame_submissions") if isinstance(presentation, dict) else None
    )
    return result, capture_summary


def _execution_queue_key(row: dict, enabled: bool) -> tuple:
    """Observed entry-weighted rows lead; unseen rows keep their legacy order."""
    legacy = row["legacy_priority"]
    tie_size = row["tie_size"]
    if not enabled:
        return (0, legacy, tie_size)
    if row["execution_count"] > 0:
        return (0, -row["execution_score"], legacy, tie_size)
    return (1, legacy, tie_size)


TIER_NAMES = ("near", "untouched", "fresh", "skipped", "unrecorded")


def offer_tier(members: list[dict | None], *, exact: bool = False, near_mode: str = "first",
               include_skipped: bool = False, include_unrecorded: bool = False, twin_near: bool = False) -> int | None:
    """Index into TIER_NAMES at which a family (its members' attempt rows; None = no record) is offered, or None
    when it must not be offered. A source in src/, a blocked entry and a reservation always win; one near member
    makes the whole family a near-miss item, and so does a near miss of its Sharing/EBOOT twin (`twin_near`: its wip
    file is a starting point for this module's function, which hstclone.py copies back once either one matches)."""
    if exact:
        return None
    if any(m and m.get("reserved_by") for m in members):
        return None
    # a far draft is fresh work: its file is a hint, not a near miss
    states = [("fresh" if m["status"] == "draft" else m["status"]) if m else "fresh" for m in members]
    if any(state in ("exact", "blocked") for state in states):
        return None
    if "near" in states or (twin_near and "skip" not in states):
        return None if near_mode == "off" else 0
    if near_mode == "only":
        return None
    if "skip" in states:
        return 3 if include_skipped else None
    if any(m and m["status"] == "untouched" and m["evidence"] == "queued-no-record" for m in members):
        return 4 if include_unrecorded else None
    return 1 if "untouched" in states else 2


def near_yield(unlocked_bytes: int, words_left: int | None) -> float:
    """Code bytes per word left (higher first); an unmeasured near miss ranks after every measured one."""
    return unlocked_bytes / max(words_left, 1) if words_left is not None else 0.0


def family_rank(p: float | None) -> int:
    """0 = Metrowerks (probability >= 0.9, or the function's profile is known), 1 = uncertain, 2 = probably psp-gcc."""
    if p is None or p >= 0.9:
        return 0
    return 1 if p >= 0.5 else 2


def queue_sort_key(row: dict, execution_enabled: bool, fresh_by_bytes: bool = False, public_first: bool = False) -> tuple:
    """Tier first (and with public_first, the compiler family right after it); near misses by yield; every other tier
    keeps the old order (_execution_queue_key)."""
    tier = row.get("tier", 2)
    fam = (family_rank(row.get("family_p")),) if public_first else ()
    if tier == 0:
        measured = row.get("words_left") is not None
        return (0, *fam, 0 if measured else 1, -row.get("near_yield", 0.0), -row.get("unlocked_bytes", 0), row["tie_size"])
    if tier == 2 and fresh_by_bytes:
        return (2, *fam, -row.get("unlocked_bytes", 0), row["tie_size"])
    return (tier, *fam, *_execution_queue_key(row, execution_enabled))


LEVER_NAMES = ("install", "none", "tooling", "body", "structure", "referent", "permute", "types", "frame", "expression")


def load_near_info(report: Path) -> dict[str, dict]:
    """wip path -> its row of try/nearmiss-report.json (class, lever, ...); empty when there is no report."""
    if not report.exists():
        return {}
    return {r["wip"]: r for r in json.loads(report.read_text(encoding="utf-8"))["files"]}


def lever_fields(row: dict | None) -> dict:
    """The routing fields a near-miss item carries (none for an unmeasured file or a report without levers)."""
    if not row or "lever" not in row:
        return {}
    import hstnearmiss as N
    return {"near_class": row["class"], "lever": row["lever"], "lever_why": row.get("lever_why", ""),
            "lever_digest": N.LEVERS.get(row["lever"], ("", ""))[0]}


def deal_lanes(rows: list[dict], lanes: int, size: int, group_of) -> list[list[dict]]:
    """Deal the first lanes*size rows (already in rank order) out to `lanes` batches so that every lane gets a similar
    mix: each group's rows (in rank order) go round-robin to the lanes with the fewest items, so the lanes' counts of
    any one group differ by at most one and their totals by at most one. A lane keeps the rank order."""
    taken = list(enumerate(rows[:lanes * size]))
    groups: dict = {}
    for rank, row in taken:
        groups.setdefault(group_of(row), []).append((rank, row))
    out: list[list[tuple[int, dict]]] = [[] for _ in range(lanes)]
    for members in groups.values():
        order = sorted(range(lanes), key=lambda i: (len(out[i]), i))
        for k, entry in enumerate(members):
            out[order[k % lanes]].append(entry)
    return [[row for _rank, row in sorted(lane, key=lambda e: e[0])] for lane in out]


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--name", help="queue name (files try/agent-queue/<name>-NN.json); not needed with --dry-run")
    ap.add_argument("--module", default="EBOOT.BIN")
    ap.add_argument("--kind", choices=("families", "singles"), default="families")
    ap.add_argument("--per-batch", type=int, default=8)
    ap.add_argument("--batches", type=int, default=4)
    ap.add_argument("--min-size", type=int, default=64)
    ap.add_argument("--max-size", type=int, default=None, help="largest function offered, in bytes (default: no limit)")
    ap.add_argument("--min-start", type=lambda s: int(s, 0), default=0x88000)
    ap.add_argument("--exclude", default="", help="comma-separated addresses (and their families) to skip")
    ap.add_argument("--execution-rank", type=Path, default=None,
                    help="private hstexec JSON; observed entry hits x code bytes lead, unseen functions keep the legacy order")
    ap.add_argument("--near", choices=("first", "off", "only"), default="first",
                    help="near misses first (default), not at all, or nothing else")
    ap.add_argument("--near-max-left", type=int, default=None,
                    help="offer only near misses with at most this many words left (unmeasured ones are dropped)")
    ap.add_argument("--public-first", action="store_true",
                    help="rank likely-Metrowerks functions first and drop likely psp-gcc ones (tools/hstfamily.py)")
    ap.add_argument("--min-p", type=float, default=0.9,
                    help="with --public-first: lift the --min-start floor for functions predicted Metrowerks at least this surely")
    ap.add_argument("--near-max-size", type=int, default=None,
                    help="size limit for near misses (default: --max-size); a near miss is cheap whatever its size")
    ap.add_argument("--fresh-order", choices=("legacy", "bytes"), default="legacy",
                    help="order of the never-queued tier: the old one, or code bytes unlocked")
    ap.add_argument("--lanes", type=int, default=0, help="write this many disjoint batches <name>1-00.json ..")
    ap.add_argument("--lane-size", type=int, default=40, help="items per batch with --lanes")
    ap.add_argument("--lever", default="",
                    help="offer only near misses with one of these comma-separated levers (tools/hstnearmiss.py)")
    ap.add_argument("--include-skipped", action="store_true", help="also offer functions a lane marked SKIP")
    ap.add_argument("--include-unrecorded", action="store_true",
                    help="also offer functions queued by a batch that kept no per-function record")
    ap.add_argument("--dry-run", action="store_true", help="print the plan; write nothing")
    args = ap.parse_args()
    if not args.name and not args.dry_run:
        ap.error("--name is required unless --dry-run")
    args.name = args.name or "dryrun"
    try:
        if args.execution_rank:
            execution_ranks, execution_capture = load_execution_rank(args.execution_rank)
        else:
            execution_ranks, execution_capture = {}, None
    except ValueError as exc:
        raise SystemExit(f"execution rank refused: {exc}") from exc
    if args.execution_rank:
        selected_rank = execution_ranks.get(args.module, {})
        observed = sum(1 for weight in selected_rank.values() if weight["execution_count"] > 0)
        print(f"execution rank: module={args.module} observed_functions={observed}; unseen functions retain legacy order")
    else:
        selected_rank = {}
    queue_dir = M.ROOT / "try" / "agent-queue"
    if not args.dry_run:
        queue_dir.mkdir(parents=True, exist_ok=True)
        if list(queue_dir.glob(f"{args.name}-*.json")):
            raise SystemExit(f"queue {args.name} exists; use a new name")
    attempts = A.collect()
    ledger = json.loads((M.CAMPAIGN / "ledger" / "campaign.json").read_text(encoding="utf-8-sig"))
    module = next(m for m in ledger["modules"] if m["name"] == args.module)
    data, seg = M.module_bytes({}, args.module)
    span_index = A.exact_spans(attempts.src, args.module)
    exclude = {int(a, 16) for a in args.exclude.split(",") if a}
    pseudo, _ = G.load_pseudocode(M.CAMPAIGN, args.module.replace(".", "_"))
    predictions: dict[str, float] = {}
    if args.public_first:
        pred_path = M.ROOT / "try" / "family-predictions.json"
        if not pred_path.exists():
            import hstfamily
            print("family predictions:", hstfamily.predict(pred_path))
        predictions = json.loads(pred_path.read_text(encoding="utf-8"))["predictions"]

    def family_p(start: int) -> float | None:
        return predictions.get(f"{args.module}:0x{start:08x}")      # None: the function's profile is known

    families = defaultdict(list)
    ends = {}
    for f in module["functions"]:
        if f.get("boundary_status") != "reviewed":
            continue
        if f["start"] < args.min_start and not (args.public_first and (family_p(f["start"]) or 0.0) >= args.min_p):
            continue
        ends[f["start"]] = f["end"]
        off = seg["file_offset"] + f["start"] - seg["start"]
        sig, _refs = C.analyse(C.words(data[off:off + f["end"] - f["start"]]))
        families[sig].append(f["start"])
    # twins in the other game module (EBOOT <-> Sharing) are cloned across by hstclone.py --from-module
    other_name = "hstp_trans_main.prx" if args.module == "EBOOT.BIN" else "EBOOT.BIN"
    other = next(m for m in ledger["modules"] if m["name"] == other_name)
    odata, oseg = M.module_bytes({}, other_name)
    twins = defaultdict(list)
    for f in other["functions"]:
        if f.get("boundary_status") == "reviewed":
            off = oseg["file_offset"] + f["start"] - oseg["start"]
            signature = C.analyse(C.words(odata[off:off + f["end"] - f["start"]]))[0]
            twins[signature].append((f["start"], f["end"]))
    other_rank = execution_ranks.get(other_name, {})
    best_path = M.ROOT / "try" / "best-mismatch.json"
    best = json.loads(best_path.read_text(encoding="utf-8")) if best_path.exists() else {}
    rows = []
    withheld: dict[str, int] = defaultdict(int)
    for sig, starts in families.items():
        starts = sorted(starts)
        size = 4 * len(sig)
        if any(s in exclude for s in starts):
            continue
        members = [attempts.status(args.module, s) for s in starts]
        fp = min((p for p in (family_p(s) for s in starts) if p is not None), default=None)
        if args.public_first and fp is not None and fp < 0.5:
            withheld["probably-psp-gcc"] += 1
            continue
        twin_functions = twins.get(sig, [])
        twin_rows = [attempts.status(other_name, ts) for ts, _te in twin_functions]
        twin_rows = [t for t in twin_rows if t and t["status"] == "near" and not t.get("reserved_by")
                     and (args.near_max_left is None or (t["words_left"] is not None and t["words_left"] <= args.near_max_left))]
        is_near = any(m and m["status"] == "near" for m in members) or bool(twin_rows)
        limit = args.near_max_size if (is_near and args.near_max_size is not None) else args.max_size
        if size < args.min_size or (limit is not None and size > limit):
            continue
        if args.near_max_left is not None and any(m and m["status"] == "near" and (
                m["words_left"] is None or m["words_left"] > args.near_max_left) for m in members):
            withheld["near>max-left"] += 1
            continue
        covered = any((args.module, s) in attempts.src or A.covered_by_span(*span_index, s, ends[s]) for s in starts)
        tier = offer_tier(members, exact=covered, near_mode=args.near, include_skipped=args.include_skipped,
                          include_unrecorded=args.include_unrecorded, twin_near=bool(twin_rows))
        if tier is None:
            if covered:
                withheld["exact"] += 1
            else:
                label = next((m["status"] for m in members if m and m["status"] in ("blocked", "near", "draft", "skip", "untouched")), "other")
                withheld[label] += 1
            continue
        n_twins = len(twin_functions)
        execution_count = 0
        execution_score = 0
        for start in starts:
            weight = selected_rank.get(start)
            if weight and weight["execution_count"] > 0:
                execution_count += weight["execution_count"]
                execution_score += weight["weighted_score"]
        for start, _end in twin_functions:
            weight = other_rank.get(start)
            if weight and weight["execution_count"] > 0:
                execution_count += weight["execution_count"]
                execution_score += weight["weighted_score"]
        near_rows = [m for m in members if m and m["status"] == "near"]
        words_left = min((m["words_left"] for m in near_rows if m["words_left"] is not None), default=None)
        if not near_rows and twin_rows:                 # a near miss only in the other module
            near_rows = twin_rows
            words_left = min((t["words_left"] for t in twin_rows if t["words_left"] is not None), default=None)
        previous = next((m for m in members if m and m["status"] in ("near", "draft", "skip", "untouched")), None)
        draft = next((m for m in members if m and m["status"] == "draft"), None)
        extra = {"tier": tier, "family_p": fp, "unlocked_bytes": size * (len(starts) + n_twins), "draft": draft,
                 "words_left": words_left, "wip": near_rows[0]["wip"] if near_rows else None, "previous": previous,
                 "twin_near": None if any(m and m["status"] == "near" for m in members) or not twin_rows else twin_rows[0]}
        extra["near_yield"] = near_yield(extra["unlocked_bytes"], words_left)
        if args.kind == "families" and len(starts) >= 2:
            legacy_priority = -size * (len(starts) + n_twins)
            rows.append({
                "legacy_priority": legacy_priority, "tie_size": ends[starts[0]] - starts[0],
                "starts": starts, "n_twins": n_twins,
                "execution_count": execution_count, "execution_score": execution_score, **extra,
            })
        elif args.kind == "singles" and len(starts) == 1:
            b = best.get(f"{args.module}:0x{starts[0]:08x}")
            # functions with twins first (their match counts twice), then by generated-candidate distance
            legacy_priority = (0 if n_twins else 1, (b[0] / max(b[1], 1)) if b else 1.0)
            rows.append({
                "legacy_priority": legacy_priority, "tie_size": ends[starts[0]] - starts[0],
                "starts": starts, "n_twins": n_twins,
                "execution_count": execution_count, "execution_score": execution_score, **extra,
            })
    near_info = load_near_info(M.ROOT / "try" / "nearmiss-report.json")
    if args.lever:
        wanted = set(args.lever.split(","))
        unknown = wanted - set(LEVER_NAMES)
        if unknown:
            raise SystemExit(f"unknown lever(s) {sorted(unknown)}; known: {', '.join(LEVER_NAMES)}")
        kept = []
        for row in rows:
            if row["tier"] == 0 and near_info.get(row["wip"] or "", {}).get("lever") not in wanted:
                withheld["near-other-lever"] += 1
                continue
            kept.append(row)
        rows = kept
    rows.sort(key=lambda row: queue_sort_key(row, args.execution_rank is not None, args.fresh_order == "bytes", args.public_first))
    tiers = {name: sum(1 for r in rows if r["tier"] == i) for i, name in enumerate(TIER_NAMES)}
    n_drafts = sum(1 for r in rows if r.get("draft"))
    print(f"offerable {args.kind}: " + ", ".join(f"{n} {name}" for name, n in tiers.items() if n)
          + (f" ({n_drafts} of them have a far draft)" if n_drafts else "")
          + (f"; withheld: {dict(withheld)}" if withheld else ""))
    if args.lanes:
        batches = deal_lanes(rows, args.lanes, args.lane_size,
                             lambda r: (r["tier"], near_info.get(r["wip"] or "", {}).get("lever", "") if r["tier"] == 0 else ""))
        batch_names = [f"{args.name}{i + 1}-00" for i in range(args.lanes)]
        if not args.dry_run and any((queue_dir / f"{n}.json").exists() for n in batch_names):
            raise SystemExit(f"a queue file {args.name}<N>-00.json exists; use a new name")
    else:
        batches = [rows[i:i + args.per_batch] for i in range(0, len(rows), args.per_batch)][:args.batches]
        batch_names = [f"{args.name}-{n:02d}" for n in range(len(batches))]
    if args.dry_run:
        for n, batch in enumerate(batches):
            for row in batch:
                s0 = row["starts"][0]
                print(f"  {batch_names[n]} {TIER_NAMES[row['tier']]:<10} 0x{s0:08x} {row['tie_size']:5d} B"
                      f"{' words_left=' + str(row['words_left']) if row['words_left'] is not None else ''}"
                      f"{' ' + row['wip'] if row['wip'] else ''}"
                      + (f" draft={row['draft']['wip']} ({row['draft']['words_left']} words off)" if row.get("draft") else ""))
        return 0
    for n, batch in enumerate(batches):
        items = []
        for row in batch:
            starts, n_twins = row["starts"], row["n_twins"]
            rep = starts[0]
            b = best.get(f"{args.module}:0x{rep:08x}")
            item = {
                "address": f"0x{rep:08x}", "end": f"0x{ends[rep]:08x}", "size": ends[rep] - rep,
                "family": [f"0x{s:08x}" for s in starts[1:]],
                "twins_in_other_module": n_twins,
                "pseudocode": str(pseudo[rep][1]) if rep in pseudo else None,
                "ghidra_name": pseudo[rep][0] if rep in pseudo else f"FUN_{rep:08x}",
                "best_generated": ({"source": b[2], "differing_words": b[0], "words": b[1]} if b else None),
            }
            if row["tier"] != 2:
                item["queue_tier"] = TIER_NAMES[row["tier"]]
            if args.public_first and row.get("family_p") is not None:
                item["family_p_metrowerks"] = row["family_p"]
            if row["wip"] and row["twin_near"] is None:
                item["wip"] = row["wip"]
                item["words_left"] = row["words_left"]
                item.update(lever_fields(near_info.get(row["wip"])))
            if row["twin_near"] is not None:
                t = row["twin_near"]
                item["twin_near_miss"] = {"module": other_name, "address": t["start"], "wip": t["wip"],
                                          "words_left": t["words_left"], **lever_fields(near_info.get(t["wip"]))}
            if row.get("draft"):
                d = row["draft"]
                item["draft"] = d["wip"]
                item["draft_words_left"] = d["words_left"]
                if d.get("words"):
                    item["draft_words"] = d["words"]
                item["draft_note"] = ("far draft: a hint for the types, calls and constants only; write the function "
                                      "again from the disassembly instead of permuting this file")
            prev = row["previous"]
            if prev and prev["status"] in ("skip", "near", "draft", "untouched"):
                item["previous_attempt"] = {"status": prev["status"], "lanes": prev["lanes"],
                                            "reason": prev["reason"] or prev["evidence"]}
            if args.execution_rank:
                item["execution_weight"] = (
                    {"entry_hits_in_window": row["execution_count"],
                     "weighted_code_bytes": row["execution_score"]}
                    if row["execution_count"] > 0 else None
                )
            items.append(item)
        out = queue_dir / f"{batch_names[n]}.json"
        queue_document = {"module": args.module, "kind": args.kind, "items": items}
        if execution_capture is not None:
            queue_document["execution_capture"] = execution_capture
        out.write_text(json.dumps(queue_document, indent=1) + "\n", encoding="utf-8")
        print(f"{out.relative_to(M.ROOT).as_posix()}: {len(items)} items, "
              f"{sum(i['size'] * (1 + len(i['family']) + i['twins_in_other_module']) for i in items)} bytes "
              "incl. family members and twins" +
              (f", {sum(1 for row in batch if row['execution_count'] > 0)} observed execution-weighted"
               if args.execution_rank else ""))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
