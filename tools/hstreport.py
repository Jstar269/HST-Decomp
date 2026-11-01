#!/usr/bin/env python3
"""Write the public objdiff progress report (docs/objdiff-report.json) from the same figures as the README table.

    python tools/hstreport.py            write docs/objdiff-report.json (also done by `hstprogress.py --write`)
    python tools/hstreport.py --check    exit 1 when the committed report differs from the figures measured now
    python tools/hstreport.py --proofs   measure from the accepted ledger (like hstprogress.py --proofs)

WHY. bytematch.dev and recomp.fyi show a decomp's share of matched code when the repository's default-branch workflow
uploads an objdiff progress report (`build/report.json`) as the artifact `<version>_report`. Public CI cannot build (no
game, no keys, no Metrowerks compiler), so the maintainers generate the report here, from the public progress figures
(`tools/hstprogress.py`: matched functions and code bytes of the two game modules), commit it as
docs/objdiff-report.json, and .github/workflows/progress-report.yml copies it to build/report.json and uploads it.

FORMAT. objdiff-core's report (objdiff-core/protos/report.proto, REPORT_VERSION 2) in its JSON form (pbjson with
`preserve_proto_field_names`): snake_case names, 64-bit counters as decimal strings, 32-bit counters and percentages as
numbers. The top-level measures and the categories (one per module, the ids of the progress categories in
objdiff.json) carry every field; a unit leaves out its zero fields, as objdiff's own reports do. One unit per line.

UNITS (what bytematch.dev draws as its grid: one tile per unit, one tile per function when a unit is opened), in
this order of precedence:
  <module>/<file>.cpp                   a translation unit of the original program, recovered from evidence
                                        (data/translation-units.json, tools/hsttu.py): all its functions, matched or
                                        not; complete when every one is matched, source_path only when they are all in
                                        one published source
  src/<dir>/<file>                      a published source's matched functions outside any recovered unit, with
                                        metadata.source_path its path
  <module>/not-yet-recovered/FUN_<addr> one function of the reviewed list (data/functions-<module>.txt) in neither;
                                        auto_generated, since it is no source file
  <module>/sdk-library-not-published    the module's Sony SDK library code (origin classes sony-prx and
                                        gcc-unattributed, docs/GUIDE.md#licensing): counted, as in the README, but its
                                        reconstructions are not published, so the unit lists no functions
Units are in address order within a module. Every function of the reviewed list is in exactly one unit, so the units'
code bytes and function counts add up to the module figures (`unit_problems` checks it).

WHAT THE NUMBERS MEAN (honest by construction).
  total_code / matched_code      the reviewed functions' code bytes of the module / the bytes of the functions with an
                                 exact source (tools/hstprogress.py `measure`, the README's figure)
  fuzzy_match_percent            equal to matched_code_percent: partial matches ("near misses") are not counted
  total_data / matched_data      total_data is every byte of the decrypted module image outside the reviewed functions,
                                 reported per module only; matched_data is 0: data is not reproduced yet, so a report
                                 never reads as complete
  complete_*, metadata.complete  the recovered translation units whose every function is matched (code, count);
                                 complete_data stays 0
The report carries what the public tree already publishes: function addresses and sizes (the reviewed lists), source
paths, the names in their headers and the recovered file names. No game bytes, disassembly or decompiler output.
"""
from __future__ import annotations

import argparse
import bisect
import json
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
REPORT_PATH = ROOT / "docs" / "objdiff-report.json"
REPORT_VERSION = 2
NOT_RECOVERED = "not-yet-recovered"
SDK_UNIT = "sdk-library-not-published"


def pct(part: int, whole: int) -> float:
    return round(100 * part / whole, 3) if whole else 0.0


def measures(code: int, matched_code: int, data: int, functions: int, matched_functions: int, units: int,
             complete_code: int = 0, complete_units: int = 0) -> dict:
    """One objdiff `Measures` message (JSON form): u64 as strings, u32 and floats as numbers. complete_*: the code and
    count of the units whose every function matches (metadata.complete)."""
    matched_pct = pct(matched_code, code)
    return {"fuzzy_match_percent": matched_pct,
            "total_code": str(code), "matched_code": str(matched_code), "matched_code_percent": matched_pct,
            "total_data": str(data), "matched_data": "0", "matched_data_percent": 0.0,
            "total_functions": functions, "matched_functions": matched_functions,
            "matched_functions_percent": pct(matched_functions, functions),
            "complete_code": str(complete_code), "complete_code_percent": pct(complete_code, code),
            "complete_data": "0", "complete_data_percent": 0.0, "total_units": units, "complete_units": complete_units}


def unit_measures(code: int, matched_code: int, functions: int, matched_functions: int,
                  complete: bool = False) -> dict:
    """A unit's `Measures` without its zero fields (pbjson leaves defaults out)."""
    m = measures(code, matched_code, 0, functions, matched_functions, 0, code if complete else 0, int(complete))
    return {k: v for k, v in m.items() if v not in (0, 0.0, "0")}


def function_item(name: str, start: int, end: int, matched: bool) -> dict:
    item = {"name": name, "size": str(end - start)}
    if matched:
        item["fuzzy_match_percent"] = 100.0
    item["metadata"] = {"virtual_address": str(start)}
    item["address"] = str(start)          # optional fields: written even when 0
    return item


def excluded_spans(classes: dict[str, str], root: Path = ROOT) -> dict[str, list[tuple[int, int]]]:
    """{module: [(start, end)]} of the sources the public tree leaves out (hstpublish.EXCLUDED_CLASSES)."""
    sys.path.insert(0, str(Path(__file__).resolve().parent))
    import hstmatch as M
    import hstpublish
    out: dict[str, list[tuple[int, int]]] = {}
    for rel, cls in classes.items():
        if cls in hstpublish.EXCLUDED_CLASSES and not rel.startswith("wip/"):
            h = M.parse_header(root / rel)
            out.setdefault(h["module"], []).append((h["start"], h["end"]))
    return {m: sorted(v) for m, v in out.items()}


def header_symbols(rows: dict, root: Path = ROOT) -> dict[str, str]:
    """{source path: the symbol its HSTMATCH header names} (the name of the function at the source's start)."""
    sys.path.insert(0, str(Path(__file__).resolve().parent))
    import hstmatch as M
    out = {}
    for r in rows.values():
        try:
            out[r["path"]] = M.parse_header(root / r["path"])["symbol"]
        except (OSError, ValueError, KeyError):
            pass
    return out


def module_units(name: str, functions: list[dict], rows: list[dict], sdk: list[tuple[int, int]],
                 symbols: dict[str, str], tus: dict[int, str] | None = None) -> list[dict]:
    """The units of one module: every reviewed function in exactly one of them (see the module docstring).
    A function is matched when a published source's span covers it, as hstprogress.measure counts it. `tus` is
    {start: unit name} from data/translation-units.json (tools/hsttu.py)."""
    tus = tus or {}
    rows = sorted(rows, key=lambda r: r["start"])
    starts = [r["start"] for r in rows]
    by_start = {r["start"]: r for r in rows}
    widest, best = [], None             # widest[i]: the row reaching furthest among rows[0..i] (hstattempts.exact_spans)
    for r in rows:
        best = r if best is None or r["end"] > best["end"] else best
        widest.append(best)

    def item(f: dict, owner: dict | None) -> dict:
        sym = symbols.get(owner["path"]) if owner is not None and f["start"] == owner["start"] else None
        return function_item(sym or f"FUN_{f['start']:08x}", f["start"], f["end"], owner is not None)

    owned: dict[str, list[dict]] = {}
    in_tu: dict[str, list[tuple[dict, dict | None]]] = {}
    keyed: list[tuple[int, dict]] = []
    sdk_funcs: list[dict] = []
    for f in sorted(functions, key=lambda f: f["start"]):
        i = bisect.bisect_right(starts, f["start"]) - 1
        owner = by_start.get(f["start"]) or (widest[i] if i >= 0 and widest[i]["end"] >= f["end"] else None)
        if f["start"] in tus:
            in_tu.setdefault(tus[f["start"]], []).append((f, owner))
        elif owner is not None:
            owned.setdefault(owner["path"], []).append(f)
        elif any(s <= f["start"] < e for s, e in sdk):
            sdk_funcs.append(f)
        else:
            fname = f"FUN_{f['start']:08x}"
            keyed.append((f["start"], {
                "name": f"{name}/{NOT_RECOVERED}/{fname}",
                "measures": unit_measures(f["end"] - f["start"], 0, 1, 0),
                "functions": [function_item(fname, f["start"], f["end"], False)],
                "metadata": {"complete": False, "module_name": name, "progress_categories": [name],
                             "auto_generated": True}}))
    for tu, fs in in_tu.items():
        code = sum(f["end"] - f["start"] for f, _o in fs)
        matched = [(f, o) for f, o in fs if o is not None]
        mcode = sum(f["end"] - f["start"] for f, _o in matched)
        complete = len(matched) == len(fs)
        meta = {"complete": complete, "module_name": name, "progress_categories": [name]}
        paths = {o["path"] for _f, o in matched}
        if complete and len(paths) == 1:
            meta["source_path"] = paths.pop()
        keyed.append((fs[0][0]["start"], {
            "name": f"{name}/{tu}", "measures": unit_measures(code, mcode, len(fs), len(matched), complete),
            "functions": [item(f, o) for f, o in fs], "metadata": meta}))
    for r in rows:
        fs = owned.get(r["path"])
        if not fs:
            continue
        code = sum(f["end"] - f["start"] for f in fs)
        keyed.append((fs[0]["start"], {
            "name": r["path"], "measures": unit_measures(code, code, len(fs), len(fs)),
            "functions": [item(f, r) for f in fs],
            "metadata": {"complete": False, "module_name": name, "source_path": r["path"],
                         "progress_categories": [name]}}))
    units = [u for _s, u in sorted(keyed, key=lambda t: t[0])]
    if sdk_funcs:
        units.append({"name": f"{name}/{SDK_UNIT}",
                      "measures": unit_measures(sum(f["end"] - f["start"] for f in sdk_funcs), 0, len(sdk_funcs), 0),
                      "metadata": {"complete": False, "module_name": name, "progress_categories": [name],
                                   "auto_generated": True}})
    return units


def build_report(fig: dict, sc: dict, led: dict | None = None, sdk: dict[str, list[tuple[int, int]]] | None = None,
                 symbols: dict[str, str] | None = None, tus: dict[str, dict[int, str]] | None = None) -> dict:
    """The report for `fig` (hstprogress.measure over the public modules), `sc` (hstprogress.scope), `led` (the campaign
    ledger: the reviewed functions), `sdk` (excluded_spans) and `tus` (hsttu.read). Without `led` each module is one
    unit, as before."""
    rows = fig["modules"]
    units, categories = [], []
    sums = {"code": 0, "matched": 0, "data": 0, "functions": 0, "matched_functions": 0, "complete": 0,
            "complete_units": 0}
    funcs = {m["name"]: m["functions"] for m in (led or {}).get("modules", [])}
    for name, r in rows.items():
        data = sc["modules"][name]["image_bytes"] - r["code_bytes"]
        if data < 0:
            raise ValueError(f"{name}: the reviewed functions ({r['code_bytes']} B) exceed the image")
        if led is None:
            mod_units = [{"name": name, "measures": measures(r["code_bytes"], r["matched_code_bytes"], data,
                                                            r["functions"], r["matched_functions"], 1),
                          "metadata": {"module_name": name, "progress_categories": [name]}}]
        else:
            src = [v for (mod, _s), v in fig["source_rows"].items() if mod == name]
            mod_units = module_units(name, funcs[name], src, (sdk or {}).get(name, []), symbols or {},
                                     (tus or {}).get(name))
        done = [u for u in mod_units if u["metadata"].get("complete")]
        complete = sum(int(u["measures"]["total_code"]) for u in done)
        m = measures(r["code_bytes"], r["matched_code_bytes"], data, r["functions"], r["matched_functions"],
                     len(mod_units), complete, len(done))
        units.extend(mod_units)
        categories.append({"id": name, "name": name, "measures": m})
        sums["code"] += r["code_bytes"]
        sums["matched"] += r["matched_code_bytes"]
        sums["data"] += data
        sums["functions"] += r["functions"]
        sums["matched_functions"] += r["matched_functions"]
        sums["complete"] += complete
        sums["complete_units"] += len(done)
    total = measures(sums["code"], sums["matched"], sums["data"], sums["functions"], sums["matched_functions"],
                     len(units), sums["complete"], sums["complete_units"])
    return {"measures": total, "units": units, "version": REPORT_VERSION, "categories": categories}


def unit_problems(report: dict) -> list[str]:
    """Why the units do not add up to their module (category) figures ([] when they do)."""
    out = []
    for c in report["categories"]:
        us = [u for u in report["units"] if u["metadata"]["module_name"] == c["id"]]
        m = c["measures"]
        for key, field in (("total_code", "total_code"), ("matched_code", "matched_code")):
            got = sum(int(u["measures"].get(field, "0")) for u in us)
            if got != int(m[key]):
                out.append(f"{c['id']}: units' {key} {got}, module {m[key]}")
        for key in ("total_functions", "matched_functions"):
            got = sum(u["measures"].get(key, 0) for u in us)
            if got != m[key]:
                out.append(f"{c['id']}: units' {key} {got}, module {m[key]}")
        if len(us) != m["total_units"]:
            out.append(f"{c['id']}: {len(us)} units, module total_units {m['total_units']}")
        for u in us:
            fs = u.get("functions", [])
            if fs and (len(fs) != u["measures"].get("total_functions", 0)
                       or sum(int(f["size"]) for f in fs) != int(u["measures"].get("total_code", "0"))):
                out.append(f"{u['name']}: its functions do not add up to its measures")
    names = [u["name"] for u in report["units"]]
    if len(set(names)) != len(names):
        out.append("two units share a name")
    return out


def render(report: dict) -> str:
    """JSON with one unit per line (a day's change touches only the units that changed)."""
    dump = lambda v: json.dumps(v, separators=(",", ":"))
    head = {k: v for k, v in report.items() if k != "units"}
    return ("{\n" + f'"measures":{dump(head["measures"])},\n"units":[\n'
            + ",\n".join(dump(u) for u in report["units"])
            + f'\n],\n"version":{dump(head["version"])},\n"categories":[\n'
            + ",\n".join(dump(c) for c in head["categories"]) + "\n]\n}\n")


def write(fig: dict, sc: dict, led: dict | None = None, sdk: dict | None = None, symbols: dict | None = None,
          tus: dict | None = None, path: Path = REPORT_PATH) -> Path:
    report = build_report(fig, sc, led, sdk, symbols, tus)
    problems = unit_problems(report)
    if problems:
        raise ValueError("objdiff report units do not add up: " + "; ".join(problems[:5]))
    path.write_text(render(report), encoding="utf-8", newline="\n")
    return path


def differences(fig: dict, sc: dict, led: dict | None = None, sdk: dict | None = None, symbols: dict | None = None,
                tus: dict | None = None, path: Path = REPORT_PATH) -> list[str]:
    """Why the committed report does not match the measured figures ([] when it does)."""
    if not path.exists():
        return [f"{path.name} does not exist"]
    want = build_report(fig, sc, led, sdk, symbols, tus)
    try:
        have = json.loads(path.read_text(encoding="utf-8"))
    except ValueError as exc:
        return [f"{path.name} is not valid JSON: {exc}"]
    if have == want:
        return []
    h, w = have.get("measures", {}), want["measures"]
    out = [f"{path.name} {k}: {h.get(k)}, measured {w[k]}" for k in ("total_code", "matched_code", "total_functions",
                                                                    "matched_functions", "total_data", "total_units")
           if h.get(k) != w[k]]
    return out or [f"{path.name} differs from the measured figures (run tools/hstreport.py)"]


def inputs(proofs: bool = False) -> tuple:
    """(fig, sc, led, sdk, symbols, tus) measured now, as hstprogress.py --write passes them."""
    sys.path.insert(0, str(Path(__file__).resolve().parent))
    import hstorigin
    import hstprogress as P
    import hsttu
    led = P.ledger()
    classes, _warnings = hstorigin.classify(P.ROOT)
    fig = P.measure(classes=classes, led=led, from_proofs=proofs)
    return (fig, P.scope(led), led, excluded_spans(classes, P.ROOT), header_symbols(fig["source_rows"], P.ROOT),
            hsttu.read())


def inputs_from(fig: dict, sc: dict, led: dict, classes: dict[str, str]) -> tuple:
    """The report's inputs beside figures already measured (tools/hstprogress.py)."""
    import hsttu
    return fig, sc, led, excluded_spans(classes), header_symbols(fig["source_rows"]), hsttu.read()


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--check", action="store_true")
    ap.add_argument("--proofs", action="store_true", help="measure from the accepted ledger, like hstprogress.py --proofs")
    args = ap.parse_args()
    got = inputs(args.proofs)
    if args.check:
        diffs = differences(*got)
        print("objdiff report: " + ("agrees" if not diffs else "DIFFERS"))
        for d in diffs:
            print("  " + d)
        return 1 if diffs else 0
    path = write(*got)
    rep = json.loads(path.read_text(encoding="utf-8"))
    t = rep["measures"]
    print(f"wrote {path.relative_to(ROOT).as_posix()}: {t['matched_functions']}/{t['total_functions']} functions, "
          f"{t['matched_code']}/{t['total_code']} code bytes ({t['matched_code_percent']:.3f} %), {t['total_units']} units")
    for c in rep["categories"]:
        us = [u for u in rep["units"] if u["metadata"]["module_name"] == c["id"]]
        print(f"  {c['id']}: {len(us)} units ({sum(1 for u in us if 'source_path' in u['metadata'])} sources, "
              f"{sum(1 for u in us if NOT_RECOVERED in u['name'])} not yet recovered, "
              f"{sum(1 for u in us if u['name'].endswith(SDK_UNIT))} SDK), "
              f"{sum(len(u.get('functions', [])) for u in us)} function entries")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
