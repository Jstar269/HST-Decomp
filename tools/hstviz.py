#!/usr/bin/env python3
"""Progress visuals for the README: where the decompilation stands against docs/COMPLETION.md.

    python tools/hstviz.py              write docs/progress.svg, docs/progress-map.svg, docs/progress-history.svg
    python tools/hstviz.py --private    write the same views with every source to try/progress-private/ (never published)

`python tools/hstprogress.py --write` calls this after it writes progress.json and docs/progress.csv, so one command
refreshes everything. The figures come from hstprogress (scope, tiers) and hstattempts (each function's state); this
module only draws them.

progress.svg          the strict headline, the tier ladder, each module's bytes by state, and where each state is
                      tracked
progress-map.svg      each game module's load segments in address order: where matched code, lane work, near misses,
                      blocked functions and data sit
progress-history.svg  the published tier-1 figure over time, beside the strict figure

Public files follow the public accounting: a function whose only source is excluded from the public tree (Sony SDK
classes, tools/hstpublish.py) is drawn as "no source here yet", exactly as the README counts it, and the four Sony
modules are drawn as kept out. `--private` draws the full truth for the owner.
"""
from __future__ import annotations

import argparse
import datetime
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hstattempts as A
import hstmatch as M
import hstorigin
import hstprogress as P

ROOT = M.ROOT
PUBLIC_DIR = ROOT / "docs"
PRIVATE_DIR = ROOT / "try" / "progress-private"
GAME_MODULES = ("EBOOT.BIN", "hstp_trans_main.prx")
MODULE_TITLES = {"EBOOT.BIN": "EBOOT.BIN", "hstp_trans_main.prx": "hstp_trans_main.prx (Sharing Version)"}

# (key, label, light, dark, tracked in). Hues are the reference palette's categorical slots in their validated order
# (blue, orange, aqua, yellow; violet), "blocked" is the reserved critical status red, "todo" a neutral.
STATES = [
    ("matched", "Matched (tier 1)", "#2a78d6", "#3987e5", "src/, each verified by tools/hstmatch.py"),
    ("lane", "In a matching lane now", "#eb6834", "#d95926", "a lane batch in the private workspace"),
    ("near", "Near miss", "#1baf7a", "#199e70", "wip/"),
    ("parked", "Tried, parked", "#eda100", "#c98500", "the attempt ledger and lane notes (private)"),
    ("blocked", "Blocked by the toolchain", "#d03b3b", "#d03b3b", "data/blocked_functions.json"),
    ("todo", "No source here yet", "#c3c2b7", "#4a4a46", "data/functions-<module>.txt (reviewed boundaries)"),
    ("data", "Data, relocations, metadata", "#6250d6", "#9085e9", "needs the whole-module build (docs/COMPLETION.md)"),
    ("kept", "Sony code, kept out of this repository", "#c3c2b7", "#4a4a46", "README: Not published here"),
]
STATE_KEYS = [s[0] for s in STATES]
LABEL = {s[0]: s[1] for s in STATES}

CSS = """
.bg{fill:#fcfcfb}.t1{fill:#0b0b0b}.t2{fill:#52514e}.t3{fill:#898781}.grid{stroke:#e1e0d9}.track{fill:#e1e0d9}
.ln{fill:none;stroke:#2a78d6;stroke-width:2}.ln2{fill:none;stroke:#d03b3b;stroke-width:2;stroke-dasharray:4 3}.dot{fill:#2a78d6}
text{font-family:system-ui,-apple-system,"Segoe UI",sans-serif}.num{font-variant-numeric:tabular-nums}
.hatch-line{stroke:#898781;stroke-width:1.5}
""" + "".join(f".s-{k}{{fill:{light}}}" for k, _l, light, _d, _t in STATES) + """
@media (prefers-color-scheme:dark){
.bg{fill:#1a1a19}.t1{fill:#f0efec}.t2{fill:#c3c2b7}.t3{fill:#898781}.grid{stroke:#2c2c2a}.track{fill:#2c2c2a}
.ln{stroke:#3987e5}.dot{fill:#3987e5}.hatch-line{stroke:#898781}
""" + "".join(f".s-{k}{{fill:{dark}}}" for k, _l, _li, dark, _t in STATES) + "}"

HATCH = ('<defs><pattern id="hatch" patternUnits="userSpaceOnUse" width="6" height="6" patternTransform="rotate(45)">'
         '<rect class="s-kept" width="6" height="6"/><line class="hatch-line" x1="0" y1="0" x2="0" y2="6"/></pattern></defs>')


def esc(s: str) -> str:
    return s.replace("&", "&amp;").replace("<", "&lt;").replace(">", "&gt;")


def fill(state: str) -> str:
    return 'fill="url(#hatch)"' if state == "kept" else f'class="s-{state}"'


def mb(n: int) -> str:
    return f"{n / 1_000_000:.2f} MB"


# ----------------------------------------------------------------------------------------------------------------
# figures

def gather(private: bool = False) -> dict:
    """Everything the three views draw. Public by default (the README's accounting); `private` counts every source."""
    led = P.ledger()
    classes, _warnings = hstorigin.classify(ROOT)
    sc = P.scope(led)
    all_modules = [m["name"] for m in led["modules"]]
    shown = P.measure(all_modules, include_excluded=private, classes=classes, led=led)
    reports = P.build_reports()
    public_paths = None if private else {v["path"] for v in shown["source_rows"].values()}
    ladder = P.tier_ladder(sc["scope_bytes"], shown["total"]["matched_code_bytes"], reports, public_paths)
    readme = P.measure(classes=classes, led=led)
    blocked = A.load_blocked()
    att = A.to_json(A.collect(), "")["functions"]
    attempts = {(r["module"], int(r["start"], 16)): r for r in att}
    src = shown["source_rows"]
    # Public view: a function whose only source is of an excluded origin class is Sony code kept out of this
    # repository; it leaves the public milestones' denominators but stays in the strict scope.
    every = src if private else P.measure(all_modules, include_excluded=True, classes=classes, led=led)["source_rows"]
    excluded = {k: v for k, v in every.items() if k not in src}
    modules = {}
    for m in led["modules"]:
        name = m["name"]
        starts, maxend = A.exact_spans(src, name)
        exact = {s for (mod, s) in src if mod == name}
        xstarts, xmaxend = A.exact_spans(excluded, name)
        xexact = {s for (mod, s) in excluded if mod == name}
        kept = not private and name not in GAME_MODULES
        funcs = []
        for f in m["functions"]:
            a, b = f["start"], f["end"]
            if kept:
                st = "kept"
            elif a in exact or A.covered_by_span(starts, maxend, a, b):
                st = "matched"
            elif a in xexact or A.covered_by_span(xstarts, xmaxend, a, b):
                st = "kept"
            elif (name, a) in blocked:
                st = "blocked"
            else:
                r = attempts.get((name, a))
                if r and r.get("reserved_by"):
                    st = "lane"
                elif r and r["status"] == "near":
                    st = "near"
                elif r and r["status"] == "skip":
                    st = "parked"
                else:
                    st = "todo"
            funcs.append((a, b, st))
        image = sc["modules"][name]["image_bytes"]
        by_state = dict.fromkeys(STATE_KEYS, 0)
        for a, b, st in funcs:
            by_state[st] += b - a
        by_state["kept" if kept else "data"] += image - sum(b - a for a, b, _ in funcs)
        modules[name] = {"image": image, "by_state": by_state, "funcs": sorted(funcs),
                         "segments": [(s["start"], s["memory_end"]) for s in m["load_segments"]]}
    return {"private": private, "ladder": ladder, "scope": sc, "modules": modules, "readme": readme,
            "sources": shown["sources"], "rows": P.csv_rows(), "date": datetime.date.today().isoformat(),
            "public_paths": public_paths}


# ----------------------------------------------------------------------------------------------------------------
# drawing

def svg_open(w: int, h: int, label: str) -> str:
    return (f'<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 {w} {h}" width="{w}" height="{h}" role="img" '
            f'aria-label="{esc(label)}">\n<style>{CSS}</style>\n{HATCH}\n<rect class="bg" width="{w}" height="{h}" rx="8"/>\n')


def stacked_bar(x: float, y: float, w: float, h: float, parts: list[tuple[str, int]], total: int, title: str) -> str:
    """Segments left to right with 2px surface gaps; 4px rounded outer ends."""
    out = []
    widths = [(st, w * n / total) for st, n in parts if n > 0]
    cx = x
    for i, (st, sw) in enumerate(widths):
        gap = 2 if i < len(widths) - 1 and sw > 3 else 0
        rx = 4 if i in (0, len(widths) - 1) else 0
        out.append(f'<rect {fill(st)} x="{cx:.2f}" y="{y}" width="{max(sw - gap, 0.5):.2f}" height="{h}" rx="{rx}">'
                   f'<title>{esc(title)}: {esc(LABEL[st])} {dict(parts)[st]:,} B ({100 * dict(parts)[st] / total:.2f}%)</title></rect>')
        cx += sw
    return "".join(out)


def dashboard(g: dict) -> str:
    W, L = 920, 28
    lad, sc = g["ladder"], g["scope"]
    who = "every source, including the unpublished Sony SDK reconstructions" if g["private"] else "public sources"
    y = 0
    t1 = P.shown(lad["t1_percent"], lad.get("t1_bytes", 0) >= sc["scope_bytes"])
    parts = [svg_open(W, 600, f"Decompilation progress: strict {lad['strict_percent']:.1f} percent; tier 1 "
                              f"{t1} percent of all {sc['scope_bytes']:,} bytes of six modules")]
    parts.append(f'<text class="t1" x="{L}" y="34" font-size="17" font-weight="600">Hot Shots Tennis: Get a Grip, '
                 f'matching decompilation{" (PRIVATE VIEW)" if g["private"] else ""}</text>'
                 f'<text class="t3" x="{W - L}" y="34" font-size="12" text-anchor="end">{g["date"]}, {esc(who)}</text>')
    # hero figures
    parts.append(f'<text class="t1" x="{L}" y="104" font-size="54" font-weight="600">{lad["strict_percent"]:.1f}%</text>'
                 f'<text class="t1" x="{L + 170}" y="76" font-size="14" font-weight="600">Strict 100%</text>'
                 f'<text class="t2" x="{L + 170}" y="95" font-size="12">the whole game rebuilt from source, byte for byte,</text>'
                 f'<text class="t2" x="{L + 170}" y="111" font-size="12">relocatable and reviewed (docs/COMPLETION.md)</text>')
    parts.append(f'<text class="t1" x="{L + 520}" y="98" font-size="34" font-weight="600">{t1}%</text>'
                 f'<text class="t1" x="{L + 640}" y="78" font-size="14" font-weight="600">Tier 1</text>'
                 f'<text class="t2" x="{L + 640}" y="96" font-size="12">functions matched one at a time</text>'
                 f'<text class="t2" x="{L + 640}" y="112" font-size="12">{g["sources"]:,} sources</text>')
    parts.append(f'<text class="t3" x="{L}" y="140" font-size="12">Every percentage on this page is a share of all '
                 f'{sc["scope_bytes"]:,} bytes of the six decrypted modules; the scope never shrinks.</text>')
    # tier ladder
    y = 168
    parts.append(f'<text class="t1" x="{L}" y="{y}" font-size="14" font-weight="600">Tiers</text>')
    tiers = [("T1 function matches", lad["t1_percent"], lad["t1_bytes"]),
             ("T2 whole-module build", lad["t2_percent"], lad["t2_bytes"]),
             ("T3 relocated build", lad["t3_percent"], lad["t3_bytes"]),
             ("T4 strict (reviewed)", lad["t4_percent"], lad["t4_bytes"])]
    bx, bw = L + 170, 560
    for i, (name, pct, nbytes) in enumerate(tiers):
        yy = y + 14 + i * 24
        fw = bw * pct / 100
        parts.append(f'<text class="t2" x="{L}" y="{yy + 11}" font-size="12">{name}</text>'
                     f'<rect class="track" x="{bx}" y="{yy}" width="{bw}" height="14" rx="4"/>'
                     + (f'<rect class="s-matched" x="{bx}" y="{yy}" width="{fw:.2f}" height="14" rx="4"/>' if fw > 0 else "")
                     + f'<text class="t1 num" x="{bx + bw + 12}" y="{yy + 11}" font-size="12">{pct:.2f}%  '
                       f'<tspan class="t3">{nbytes:,} B</tspan></text>')
    # where every byte is
    y = 296
    parts.append(f'<text class="t1" x="{L}" y="{y}" font-size="14" font-weight="600">Where every byte is</text>'
                 f'<text class="t3" x="{L}" y="{y + 18}" font-size="12">Each bar is one module\'s whole decrypted image; '
                 'segment widths are shares of that module. docs/progress-map.svg shows the same states by address.</text>')
    mods = g["modules"]
    rows = [(MODULE_TITLES[n], [n]) for n in GAME_MODULES if n in mods]
    others = [n for n in mods if n not in GAME_MODULES]
    if others:
        rows.append((f"{len(others)} Sony modules (libfont, scePsmf, scePsmfP, font)", others))
    for i, (title, names) in enumerate(rows):
        yy = y + 50 + i * 40
        agg = dict.fromkeys(STATE_KEYS, 0)
        image = 0
        for n in names:
            image += mods[n]["image"]
            for k, v in mods[n]["by_state"].items():
                agg[k] += v
        partlist = [(k, agg[k]) for k in STATE_KEYS]
        value = "kept out" if agg["kept"] == image else f"{100 * agg['matched'] / image:.1f}% matched"
        parts.append(f'<text class="t2" x="{L}" y="{yy - 5}" font-size="12">{esc(title)}  <tspan class="t3">{mb(image)}</tspan></text>'
                     + stacked_bar(L, yy, 740, 18, partlist, image, title)
                     + f'<text class="t1 num" x="{L + 752}" y="{yy + 13}" font-size="12">{value}</text>')
    # legend: label, total, where tracked
    y = y + 50 + len(rows) * 40 + 6
    parts.append(f'<text class="t1" x="{L}" y="{y}" font-size="14" font-weight="600">States, and where each is tracked</text>')
    totals = {k: sum(mods[n]["by_state"][k] for n in mods) for k in STATE_KEYS}
    shown = [s for s in STATES if totals[s[0]] > 0 or s[0] in ("lane", "near", "blocked")]
    for i, (k, label, _l, _d, tracked) in enumerate(shown):
        col, row = i % 2, i // 2
        xx, yy = L + col * 440, y + 14 + row * 34
        parts.append(f'<rect {fill(k)} x="{xx}" y="{yy}" width="12" height="12" rx="3"/>'
                     f'<text class="t1" x="{xx + 20}" y="{yy + 10}" font-size="12">{esc(label)}  '
                     f'<tspan class="t3 num">{totals[k]:,} B</tspan></text>'
                     f'<text class="t3" x="{xx + 20}" y="{yy + 25}" font-size="11">{esc(tracked)}</text>')
    height = y + 14 + ((len(shown) + 1) // 2) * 34 + 10
    parts[0] = parts[0].replace('height="600"', f'height="{height}"').replace('0 0 920 600', f'0 0 920 {height}') \
        .replace('<rect class="bg" width="920" height="600"', f'<rect class="bg" width="920" height="{height}"')
    return "".join(parts) + "</svg>\n"


def cell_states(funcs: list[tuple[int, int, str]], lo: int, hi: int, cells: int) -> list[str]:
    """The state with the most bytes in each of `cells` equal slices of [lo, hi); bytes outside functions are data, and
    a tie goes to the function state (data is counted last)."""
    step = (hi - lo) / cells
    out = []
    fi = 0
    for c in range(cells):
        c0, c1 = lo + c * step, lo + (c + 1) * step
        acc: dict[str, float] = {}
        covered = 0.0
        while fi < len(funcs) and funcs[fi][1] <= c0:
            fi += 1
        j = fi
        while j < len(funcs) and funcs[j][0] < c1:
            a, b, st = funcs[j]
            ov = min(b, c1) - max(a, c0)
            if ov > 0:
                acc[st] = acc.get(st, 0) + ov
                covered += ov
            j += 1
        acc["data"] = acc.get("data", 0) + (c1 - c0) - covered
        out.append(max(acc.items(), key=lambda kv: kv[1])[0])
    return out


def address_map(g: dict) -> str:
    W, L, CW, ROWS, RH, RG = 920, 28, 864, 14, 9, 2
    mods = g["modules"]
    shown = [n for n in GAME_MODULES if n in mods]
    block_h = 26 + ROWS * (RH + RG) + 18
    height = 76 + len(shown) * block_h + 64
    parts = [svg_open(W, height, "Address map of the two game modules: matched code, lane work, near misses, "
                                 "blocked functions and data, in address order")]
    parts.append(f'<text class="t1" x="{L}" y="34" font-size="17" font-weight="600">Where in the executables'
                 f'{" (PRIVATE VIEW)" if g["private"] else ""}</text>'
                 f'<text class="t3" x="{L}" y="54" font-size="12">Each module\'s load segments in address order, left to right '
                 'and top to bottom; each cell shows the state that holds most of its bytes.</text>')
    y = 76
    for n in shown:
        m = mods[n]
        lo = min(a for a, _ in m["segments"])
        hi = max(b for _, b in m["segments"])
        cells = CW * ROWS
        states = cell_states(m["funcs"], lo, hi, cells)
        per = (hi - lo) / cells
        parts.append(f'<text class="t2" x="{L}" y="{y + 14}" font-size="12">{esc(MODULE_TITLES[n])}  '
                     f'<tspan class="t3">0x{lo:x} to 0x{hi:x}, about {per:,.0f} bytes per cell</tspan></text>')
        for r in range(ROWS):
            ry = y + 24 + r * (RH + RG)
            row = states[r * CW:(r + 1) * CW]
            start = 0
            for c in range(1, CW + 1):
                if c == CW or row[c] != row[start]:
                    a0 = lo + (r * CW + start) * per
                    parts.append(f'<rect {fill(row[start])} x="{L + start}" y="{ry}" width="{c - start}" height="{RH}">'
                                 f'<title>0x{int(a0):x}: {esc(LABEL[row[start]])}</title></rect>')
                    start = c
        y += block_h
    legend = [s for s in STATES if s[0] != "kept"]
    xx = L
    for k, label, *_ in legend:
        parts.append(f'<rect {fill(k)} x="{xx}" y="{y + 4}" width="12" height="12" rx="3"/>'
                     f'<text class="t2" x="{xx + 18}" y="{y + 14}" font-size="12">{esc(label)}</text>')
        xx += 18 + int(6.4 * len(label)) + 18
        if xx > W - 160:
            xx, y = L, y + 22
    return "".join(parts) + "</svg>\n"


def history(g: dict) -> str:
    W, H, L, R = 920, 300, 64, 28
    by_day: dict[str, float] = {}
    strict: dict[str, float] = {}
    for r in sorted(g["rows"], key=P.row_order):
        if r.get("percent_code_bytes"):
            by_day[r["date"]] = float(r["percent_code_bytes"])
        if r.get("strict_percent"):
            strict[r["date"]] = float(r["strict_percent"])
    pts = sorted(by_day.items())
    if not pts:
        return svg_open(W, 80, "No history yet") + "</svg>\n"
    dates = [datetime.date.fromisoformat(d) for d, _ in pts]
    d0, d1 = dates[0], dates[-1]
    span = max((d1 - d0).days, 1)
    cy0, cy1 = 84, H - 48
    ymax = max(30, (int(max(p for _, p in pts) / 10) + 1) * 10)

    def x(d):
        return L + (W - L - R) * ((d - d0).days / span if len(pts) > 1 else 0.5)

    def y(p):
        return cy0 + (cy1 - cy0) * (1 - p / ymax)

    parts = [svg_open(W, H, "History of the published tier-1 figure and the strict figure")]
    parts.append(f'<text class="t1" x="{L - 36}" y="34" font-size="17" font-weight="600">Progress over time</text>'
                 f'<text class="t3" x="{L - 36}" y="54" font-size="12">Blue: the README figure (tier 1, public sources, '
                 "share of the two game modules' code bytes). Red, dashed: strict, share of all bytes of the six modules.</text>")
    for v in range(0, ymax + 1, 10):
        parts.append(f'<line class="grid" x1="{L}" x2="{W - R}" y1="{y(v):.1f}" y2="{y(v):.1f}" stroke-width="1"/>'
                     f'<text class="t3 num" x="{L - 8}" y="{y(v) + 4:.1f}" font-size="11" text-anchor="end">{v}%</text>')
    for d in dates:
        parts.append(f'<text class="t3 num" x="{x(d):.1f}" y="{cy1 + 20}" font-size="11" text-anchor="middle">{d.strftime("%m-%d")}</text>')
    path = " ".join(f"{'M' if i == 0 else 'L'}{x(d):.1f},{y(p):.1f}" for i, (d, (_s, p)) in enumerate(zip(dates, pts)))
    parts.append(f'<path class="ln" d="{path}"/>')
    sp = sorted((datetime.date.fromisoformat(d), p) for d, p in strict.items())
    if sp:
        spath = " ".join(f"{'M' if i == 0 else 'L'}{x(d):.1f},{y(p):.1f}" for i, (d, p) in enumerate(sp))
        if len(sp) == 1:
            spath = f"M{L},{y(sp[0][1]):.1f} L{W - R},{y(sp[0][1]):.1f}"
        parts.append(f'<path class="ln2" d="{spath}"/>')
    for d, (ds, p) in zip(dates, pts):
        parts.append(f'<circle class="dot" cx="{x(d):.1f}" cy="{y(p):.1f}" r="4"><title>{ds}: {p:.3f}%</title></circle>')
    last_d, last_p = dates[-1], pts[-1][1]
    parts.append(f'<text class="t1 num" x="{x(last_d) - 8:.1f}" y="{y(last_p) - 10:.1f}" font-size="12" text-anchor="end">'
                 f'{last_p:.2f}%</text>')
    return "".join(parts) + "</svg>\n"


def milestones_chart(g: dict, rows: list[dict]) -> str:
    """One row per capability milestone: its progress bar, the first date it reached 100%, or 'not measured yet'."""
    W, L, RH = 920, 28, 46
    height = 84 + len(rows) * RH + 18
    parts = [svg_open(W, height, "Milestones: " + "; ".join(
        f"{r['title']} {'not measured' if r['progress'] is None else format(r['progress'], '.1f') + ' percent'}" for r in rows))]
    parts.append(f'<text class="t1" x="{L}" y="34" font-size="17" font-weight="600">Milestones'
                 f'{" (PRIVATE VIEW)" if g["private"] else ""}</text>'
                 f'<text class="t3" x="{L}" y="54" font-size="12">Each is 100% only when its own criterion fully holds '
                 '(docs/COMPLETION.md); the first date it was reached is kept in docs/milestones.json.</text>')
    bx, bw = L + 300, 400
    for i, r in enumerate(rows):
        y = 84 + i * RH
        p = r["progress"]
        num = f"{i + 1:>2}"
        parts.append(f'<text class="t3 num" x="{L}" y="{y + 12}" font-size="12">{num}</text>'
                     f'<text class="t1" x="{L + 24}" y="{y + 12}" font-size="13" font-weight="600">{esc(r["title"])}</text>'
                     f'<text class="t3" x="{L + 24}" y="{y + 29}" font-size="11">{esc(r["detail"][:140])}'
                     f'{"..." if len(r["detail"]) > 140 else ""}<title>{esc(r["criterion"])}. {esc(r["detail"])}</title></text>')
        parts.append(f'<rect class="track" x="{bx}" y="{y + 2}" width="{bw}" height="12" rx="4"/>')
        if p:
            parts.append(f'<rect class="s-matched" x="{bx}" y="{y + 2}" width="{max(bw * p / 100, 2):.2f}" height="12" rx="4"/>')
        if r.get("reached") and not r.get("regressed"):
            label = f"100%, reached {r['reached']}"
        elif r.get("regressed"):
            label = f"REGRESSED (reached {r['reached']}): {p:.2f}%" if p is not None else "REGRESSED"
        elif p is None:
            label = "not measured yet"
        else:
            label = f"{p:.2f}%"
        parts.append(f'<text class="t1 num" x="{bx + bw + 12}" y="{y + 12}" font-size="12">{esc(label)}</text>')
    return "".join(parts) + "</svg>\n"


def write(private: bool = False) -> list[Path]:
    import hstmilestones
    g = gather(private)
    out_dir = PRIVATE_DIR if private else PUBLIC_DIR
    out_dir.mkdir(parents=True, exist_ok=True)
    rows = hstmilestones.record(hstmilestones.compute(g),
                                path=(out_dir / "milestones.json") if private else hstmilestones.RECORD)
    files = {"progress.svg": dashboard(g), "progress-map.svg": address_map(g), "progress-history.svg": history(g),
             "progress-milestones.svg": milestones_chart(g, rows)}
    written = []
    for name, text in files.items():
        p = out_dir / name
        p.write_text(text, encoding="utf-8", newline="\n")
        written.append(p)
    return written


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--private", action="store_true", help="draw every source, into try/progress-private/")
    args = ap.parse_args()
    for p in write(args.private):
        print(f"wrote {p.relative_to(ROOT).as_posix()}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
