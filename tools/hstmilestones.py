#!/usr/bin/env python3
"""Measure the capability milestones of data/milestones.json (defined in docs/COMPLETION.md).

    python tools/hstmilestones.py             print every milestone's progress (nothing is written)

tools/hstviz.py calls `compute()` and `record()`: the dashboard draws the milestones, and docs/milestones.json keeps the
first date each one reached 100%. A reached milestone that later measures below 100% is kept and flagged REGRESSED, so a
regression is visible instead of rewriting history.

Each measure reads its own evidence and never estimates:
  inventory  ledger regions reviewed / all region bytes of the six modules, and no open candidate span
  t1         matched code bytes / function bytes of the named modules (the view's sources: public or every source)
  t2, t3     whole-module build reports (try/build/<module>-report.json from tools/hstbuild.py): compiled-source bytes
             over the image (t2_source_bytes; t3_source_bytes of a shifted build that passed); reached only when every
             named image has the retail SHA-256 with no scaffolding (t3 also needs the shifted build's static check and
             its runtime scenario: shifted_build_passes true and shifted_runtime "passed")
  t4         the strict tier of hstprogress
  portable   try/port-report.json (or try/port-private-report.json with --private); source code bytes that compile for all five host targets
  platform   the PSP system functions the two game executables import (their .lib.stub tables), each looked up in
             Nakagawa Recomp's reviewed import audit (hst-config `nakagawa_root`); only status "complete" counts
  evidence   an entry in data/milestones.json "evidence" naming a recorded scenario
"""
from __future__ import annotations

import argparse
import datetime
import json
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hstconfig as CFG
import hstmatch as M
import hstprogress as P

ROOT = M.ROOT
DEFS = ROOT / "data" / "milestones.json"
RECORD = ROOT / "docs" / "milestones.json"
PORT_REPORT = ROOT / "try" / "port-report.json"
PORT_PRIVATE_REPORT = ROOT / "try" / "port-private-report.json"
GAME = ("EBOOT.BIN", "hstp_trans_main.prx")


def definitions() -> dict:
    return json.loads(DEFS.read_text(encoding="utf-8"))


def module_names(spec, all_names: list[str]) -> list[str]:
    return list(all_names) if spec == "all" else list(spec)


def imports(module: str, led: dict, cache: dict | None = None) -> list[tuple[str, int]]:
    """(library, NID) of every function the module imports, from its .lib.stub table."""
    data, _ = M.module_bytes(cache if cache is not None else {}, module)
    seg = next(m for m in led["modules"] if m["name"] == module)["load_segments"][0]
    shoff, = struct.unpack_from("<I", data, 0x20)
    shentsize, shnum, shstrndx = struct.unpack_from("<HHH", data, 0x2e)
    secs = [struct.unpack_from("<IIIIIIIIII", data, shoff + i * shentsize) for i in range(shnum)]
    strtab = secs[shstrndx][4]

    def cstr(off: int) -> str:
        return data[off:data.index(b"\0", off)].decode("latin-1")

    stub = next((s for s in secs if cstr(strtab + s[0]) == ".lib.stub"), None)
    if stub is None:
        return []
    v2o = lambda v: v - seg["start"] + seg["file_offset"]      # noqa: E731
    out = []
    off, end = stub[4], stub[4] + stub[5]
    while off < end:
        libname, _ver, _attr, words, _vstubs, nfuncs, nidtab, _stubtab = struct.unpack_from("<IHHBBHII", data, off)
        lib = cstr(v2o(libname))
        out += [(lib, struct.unpack_from("<I", data, v2o(nidtab) + 4 * k)[0]) for k in range(nfuncs)]
        off += max(words, 1) * 4
    return out


def nakagawa_audit() -> dict[int, dict] | None:
    """{NID: {"classification", "status", "name"}} from Nakagawa Recomp's committed import audit, or None."""
    if CFG.get("nakagawa_root") is None:
        return None
    path = CFG.host_path("nakagawa_root") / "tools" / "import_audit_baseline.json"
    return {int(k, 16): v for k, v in json.loads(path.read_text(encoding="utf-8")).items()}


def classify_imports(nids, audit: dict[int, dict]) -> dict[str, int]:
    """Count imported NIDs by Nakagawa's audit: only a dedicated handler reviewed 'complete' counts as done."""
    counts = {"complete": 0, "partial or unreviewed": 0, "refused or fake": 0, "missing": 0}
    for nid in nids:
        row = audit.get(nid)
        if row is None:
            counts["missing"] += 1
        elif row.get("classification") != "dedicated":
            counts["refused or fake"] += 1
        elif row.get("status") == "complete":
            counts["complete"] += 1
        else:
            counts["partial or unreviewed"] += 1
    return counts


def measure_platform(led: dict) -> dict:
    nids: dict[int, str] = {}
    cache: dict = {}
    for mod in GAME:
        for lib, nid in imports(mod, led, cache):
            nids.setdefault(nid, lib)
    audit = nakagawa_audit()
    if audit is None:
        return {"progress": None, "detail": f"{len(nids)} imported PSP system functions; set nakagawa_root to measure"}
    counts = classify_imports(nids, audit)
    detail = (f"{counts['complete']} of {len(nids)} imported system functions complete in Nakagawa; "
              f"{counts['partial or unreviewed']} partial or unreviewed, {counts['refused or fake']} refused or fake, "
              f"{counts['missing']} missing")
    return {"progress": 100 * counts["complete"] / len(nids) if nids else None, "detail": detail, "counts": counts,
            "imports": len(nids)}


def measure_inventory(led: dict) -> dict:
    total = reviewed = 0
    open_candidates = 0
    for m in led["modules"]:
        for r in m["regions"]:
            size = r["end"] - r["start"]
            total += size
            if r.get("review_status") == "reviewed":
                reviewed += size
        open_candidates += sum(1 for c in m.get("function_candidates", []) if c.get("boundary_status") != "reviewed")
    progress = 100 * reviewed / total if total else 0.0
    if open_candidates:
        progress = min(progress, 99.999)
    return {"progress": progress, "detail": f"{reviewed:,} of {total:,} region bytes reviewed; "
                                            f"{open_candidates} candidate spans open"}


def measure_build(names: list[str], g: dict, reports: dict, shifted: bool) -> dict:
    """In the public view the Sony code kept out of this repository may come from the owner's own executable
    (`owner_supplied_bytes` in the report) and is not counted; the private view needs every byte from source."""
    public = g.get("public_paths")
    image = sum(g["modules"][n]["image"] - (g["modules"][n]["by_state"]["kept"] if public is not None else 0)
                for n in names)
    done = 0
    complete = True
    for n in names:
        rep = reports.get(n)
        if rep is None:
            complete = False
            continue
        identical = rep.get("identical") is True or rep.get("image_sha256_match") is True
        sc = rep.get("scaffolding")
        scaffold = (sum(v for v in sc.values() if isinstance(v, int)) if isinstance(sc, dict)
                    else rep.get("scaffolding_bytes", 1))
        scaffold += (rep.get("container") or {}).get("bytes_from_retail", 0)
        owner = rep.get("owner_supplied_bytes", (sc or {}).get("owner_supplied_bytes", 0) if isinstance(sc, dict) else 0)
        ok = identical and scaffold - (owner if public is not None else 0) == 0
        if shifted:
            # the static half (shifted_build_passes) and the runtime half: the relocated build loads and runs the
            # documented scenario (recorded by whoever ran it as shifted_runtime "passed")
            ok = ok and rep.get("shifted_build_passes") is True and rep.get("shifted_runtime") == "passed"
        complete = complete and ok
        key = "t3_source_bytes" if shifted else "t2_source_bytes"
        if not shifted or rep.get("shifted_build_passes") is True:
            # only accepted sources count: a build of a scratch sample (try/...) is a test, not progress
            done += sum(v for p, v in (rep.get(key) or {}).items()
                        if p.startswith("src/") and (public is None or p in public))
    progress = 100.0 if complete else min(100 * done / image, 99.999) if image else 0.0
    what = "relocated build" if shifted else "whole-module build"
    detail = (f"{done:,} of {image:,} image bytes from source in the {what}" if reports
              else f"no {what} report yet (tools/hstbuild.py)")
    return {"progress": progress, "detail": detail}


def measure_portable(private: bool = False, path: Path | None = None) -> dict:
    """Read a complete five-target portable-source report; subset runs are not milestone evidence."""
    report_path = path or (PORT_PRIVATE_REPORT if private else PORT_REPORT)
    if not report_path.exists():
        try:
            display_path = report_path.relative_to(ROOT).as_posix()
        except ValueError:
            display_path = str(report_path)
        return {"progress": None,
                "detail": f"not measured yet: needs the host-compile check ({display_path})"}
    rep = json.loads(report_path.read_text(encoding="utf-8"))
    from hstport import TARGETS
    if set(rep.get("targets", [])) != set(TARGETS) or len(rep.get("targets", [])) != len(TARGETS):
        return {"progress": None, "detail": "not measured yet: report does not cover all five host targets"}
    code_bytes = int(rep["code_bytes"])
    compiling_bytes = int(rep["compiling_bytes"])
    sources = int(rep["sources"])
    compiling_sources = int(rep["compiling_all_targets"])
    progress = 100 * compiling_bytes / code_bytes if code_bytes else 0.0
    detail = (f"{compiling_bytes:,} of {code_bytes:,} code bytes are source that compiles for all 5 host targets "
              f"({compiling_sources:,} of {sources:,} sources)")
    return {"progress": progress, "detail": detail}


def compute(g: dict) -> list[dict]:
    """One row per milestone: id, title, criterion, progress (0-100, or None when not measured yet), detail."""
    d = definitions()
    led = P.ledger()
    names = list(g["modules"])
    reports = P.build_reports()
    evidence = {e["id"]: e for e in d.get("evidence", [])}
    rows = []
    for m in d["milestones"]:
        kind = m["measure"]
        if kind == "inventory":
            r = measure_inventory(led)
        elif kind == "t1":
            mods = module_names(m["modules"], names)
            matched = sum(g["modules"][n]["by_state"]["matched"] for n in mods)
            # Sony code kept out of this repository: only what an excluded source covers (hstviz.gather kept_sourced)
            kept = sum(g["modules"][n].get("kept_sourced",
                                           sum(b - a for a, b, st in g["modules"][n]["funcs"] if st == "kept"))
                       for n in mods)
            code = sum(sum(b - a for a, b, _ in g["modules"][n]["funcs"]) for n in mods) - kept
            r = {"progress": 100 * matched / code if code else 0.0,
                 "detail": f"{matched:,} of {code:,} code bytes"
                           + (f" ({kept:,} B of Sony code kept out of this repository are not counted)" if kept else "")}
        elif kind in ("t2", "t3"):
            r = measure_build(module_names(m["modules"], names), g, reports, kind == "t3")
        elif kind == "t4":
            r = {"progress": g["ladder"]["strict_percent"], "detail": "every gate of docs/COMPLETION.md"}
        elif kind == "portable":
            r = measure_portable(private=g.get("public_paths", set()) is None)
        elif kind == "platform":
            r = measure_platform(led)
        elif kind == "evidence":
            e = evidence.get(m["id"])
            r = ({"progress": 100.0, "detail": f"scenario recorded: {e['evidence']}"} if e
                 else {"progress": 0.0, "detail": "no recorded scenario yet"})
        else:
            raise ValueError(f"unknown measure {kind!r} for {m['id']}")
        rows.append({"id": m["id"], "title": m["title"], "criterion": m["criterion"], **r})
    return rows


def record(rows: list[dict], path: Path = RECORD, today: str | None = None) -> list[dict]:
    """Add each row's first date at 100% (kept forever) and a REGRESSED flag; write `path`; return the rows."""
    today = today or datetime.date.today().isoformat()
    old = {}
    if path.exists():
        old = {r["id"]: r for r in json.loads(path.read_text(encoding="utf-8")).get("milestones", [])}
    for r in rows:
        reached = old.get(r["id"], {}).get("reached")
        if reached is None and r["progress"] is not None and r["progress"] >= 100.0:
            reached = today
        r["reached"] = reached
        r["regressed"] = bool(reached) and (r["progress"] is None or r["progress"] < 100.0)
    path.parent.mkdir(parents=True, exist_ok=True)
    doc = {"schema": "hst-decomp.milestone-record.v1", "generated": today,
           "milestones": [{k: r[k] for k in ("id", "title", "progress", "detail", "reached", "regressed")} for r in rows]}
    path.write_text(json.dumps(doc, indent=1) + "\n", encoding="utf-8", newline="\n")
    return rows


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--private", action="store_true", help="measure with every source")
    args = ap.parse_args()
    import hstviz
    for r in compute(hstviz.gather(args.private)):
        p = "not measured" if r["progress"] is None else f"{r['progress']:7.3f} %"
        print(f"{r['id']:16} {p:>14}  {r['detail']}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
