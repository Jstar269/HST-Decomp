#!/usr/bin/env python3
"""String-literal pools of matched functions: define them in the function's own source (docs/COMPLETION.md, gate 4).

mwccpsp puts every string literal of a function in its own `.data` section (4-byte aligned, in the order the literals
first appear, identical literals of the function merged). A matched source that names its strings through address
externs (`extern char DAT_002d1234[];`) leaves those bytes to the whole-module build's blob scaffolding. When one
function is the only user of a contiguous run of strings, the run is exactly what the function's own literals
compile to, so the source can define it: replace each extern by the literal, add `data=0x<run start>` to the header,
and hstmatch.py then also compares the placed `.data` bytes with retail, and tools/hstbuild.py counts them in tier 2.

    python tools/hstdatapool.py plan EBOOT.BIN [--json try/datapool/EBOOT.BIN-plan.json] [--src-dir src/eboot]
    python tools/hstdatapool.py apply try/datapool/EBOOT.BIN-plan.json [--only src/eboot/x.c ...]
    python tools/hstdatapool.py keep try/datapool/EBOOT.BIN-plan.json hstmatch.json   # revert the non-EXACT ones
    python tools/hstdatapool.py tables-plan EBOOT.BIN [--json try/datapool/EBOOT.BIN-tables.json]
    python tools/hstdatapool.py tables-apply try/datapool/EBOOT.BIN-tables.json [--only src/eboot/x.c ...]

String-pointer tables (tables-plan, tables-apply; keep works on their plan too). A file-scope `const char *t[] =
{"a", "b"};` compiles to its strings (each its own 4-aligned .data section, in initialiser order) followed by the
pointer array (R_MIPS_32 relocations). A table is planned when, from the retail relocation tables: its words are
consecutive R_MIPS_32 sites; only code inside one matched source refers to its start and nothing refers into it; every
entry points at a string of the data region that only this table's entries refer to; the strings are contiguous in
first-entry order and end where the table starts. A source is rewritten when its tables are contiguous with each
other, its header places no data yet, it has no string literals, and each table is declared exactly once as
`extern ["C"] [const] char *NAME[...];` (or, for a one-entry table, `extern ... char *NAME;`). The declarations become
definitions, in address order, at the first one's place; the uses are not touched.

A unit (one source, one run) is planned only when all of this holds, from the retail relocation tables (exact
references, no heuristics):
  * the run lies in the module's initialised-data region (REGIONS: where the original link put the `.data` input
    sections, after the read-only data and the embedded files);
  * every string starts on a 4-byte boundary, ends with NUL, is followed by zero padding to the next 4-byte boundary,
    and holds text bytes only (tab, newline, carriage return, 0x20-0x7e, or 0x80-0xff for Shift-JIS/UTF-8);
  * every relocation that refers into the run is a LO16 of an `addiu` (an address taken, never a load or a store:
    a literal that the code reads or writes directly would be folded or is a variable) inside the function's span,
    and refers to the start of one of the strings; no data pointer (.rel.data) refers into the run;
  * the strings follow each other without a gap (the run is one placed section group), and the source names each
    one through an extern char array (or `&` of an extern char), so the rewrite is a pure substitution.
The source is not touched when it already places data, defines string literals, or has same-file helpers.

The rewrite keeps the note line's kind and appends what changed. The verdict is hstmatch.py's: `keep` reverts every
rewritten source that is not EXACT (git checkout of that file). No retail bytes are written anywhere but into the
literals of the rewritten sources (which is what a hand-written source does); the plan JSON holds addresses only.
"""
from __future__ import annotations

import argparse
import bisect
import json
import re
import subprocess
import sys
from dataclasses import asdict, dataclass, field
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

ROOT = Path(__file__).resolve().parents[1]

# The initialised-data region of each game module's merged `.data` output section: [start, end). The original link
# merged, in this order, the read-only data, the embedded files, and the initialised data (where mwccpsp puts string
# literals), then the static initialisers, their table and the vtables. Measured 2026-10-26 from the retail images:
# in each region the referencing functions rise with the address (link order), and the region edges are where that
# order restarts (C:/nk/reports/hst-path-to-100-20261026/findings.json, area "data", region C).
REGIONS = {"EBOOT.BIN": (0x2CECA8, 0x303194), "hstp_trans_main.prx": (0x374138, 0x3AC430)}
SRC_DIR = {"EBOOT.BIN": "src/eboot", "hstp_trans_main.prx": "src/sharing_main"}

R_MIPS_32, R_MIPS_HI16, R_MIPS_LO16 = 2, 5, 6
OP_ADDIU = 0x09
TEXT_BYTES = frozenset([9, 10, 13]) | frozenset(range(0x20, 0x7F)) | frozenset(range(0x80, 0x100))


# ---------------------------------------------------------------------------------------------------------------
# pure helpers (game-free; tools/test_hstdatapool.py)

def align4(x: int) -> int:
    return (x + 3) & ~3


def string_extent(blob: bytes, off: int) -> int | None:
    """Length (without the NUL) of the text string at blob[off], when it is a well-formed literal image: 4-aligned
    start, text bytes, NUL, zero padding to the next 4-byte boundary inside the blob. None otherwise."""
    if off % 4 or off >= len(blob):
        return None
    end = blob.find(b"\0", off)
    if end < 0:
        return None
    if any(b not in TEXT_BYTES for b in blob[off:end]):
        return None
    pad_end = align4(end + 1)
    if pad_end > len(blob) or any(blob[end + 1:pad_end]):
        return None
    return end - off


def c_literal(raw: bytes) -> str:
    """The C/C++ string literal whose bytes (without the implicit NUL) are `raw`. Non-printable and non-ASCII bytes are
    written as three-digit octal escapes (never ambiguous with a following digit, unlike \\x), `?` after `?` is
    escaped (no trigraphs), quotes and backslashes are escaped."""
    out = []
    prev = ""
    for b in raw:
        ch = chr(b)
        if ch == "\\":
            s = "\\\\"
        elif ch == '"':
            s = '\\"'
        elif ch == "\n":
            s = "\\n"
        elif ch == "\t":
            s = "\\t"
        elif ch == "\r":
            s = "\\r"
        elif ch == "?" and prev == "?":
            s = "\\?"
        elif 0x20 <= b < 0x7F:
            s = ch
        else:
            s = f"\\{b:03o}"
        out.append(s)
        prev = ch
    return '"' + "".join(out) + '"'


@dataclass
class Ref:
    target: int          # module address the relocated field refers to
    site: int            # module address of the relocated word
    kind: str            # "addiu" (LO16 of an address computation), "lo-other" (LO16 of a load/store), "ptr" (R_MIPS_32)


@dataclass
class Unit:
    source: str
    module: str
    start: int
    lo: int                                   # run start (the data= address)
    hi: int                                   # end of the last string's NUL (what the placed sections cover)
    strings: list[tuple[int, int]] = field(default_factory=list)    # (address, length without NUL)
    names: dict[str, str] = field(default_factory=dict)             # address (hex) -> identifier used in the source

    @property
    def size(self) -> int:
        return self.hi - self.lo


def site_index(refs_by_target: dict[int, list[Ref]], region: tuple[int, int]) -> list[tuple[int, int]]:
    """Sorted (site, target) of every addiu reference into `region`, for runs_of."""
    return sorted((r.site, t) for t, rs in refs_by_target.items() if region[0] <= t < region[1]
                  for r in rs if r.kind == "addiu")


# id(table) -> (table, sites): the entry keeps its table alive, so the id cannot be reused by another table
_POINTER_SITES: dict[int, tuple[dict, list[int]]] = {}


def pointer_sites(refs_by_target: dict[int, list[Ref]]) -> list[int]:
    """Sorted sites of every R_MIPS_32 relocation (a pointer stored in data), cached per reference table."""
    key = id(refs_by_target)
    cached = _POINTER_SITES.get(key)
    if cached is None or cached[0] is not refs_by_target:
        cached = (refs_by_target, sorted(r.site for rs in refs_by_target.values() for r in rs if r.kind == "ptr"))
        _POINTER_SITES[key] = cached
    return cached[1]


def runs_of(fn: tuple[int, int], refs_by_target: dict[int, list[Ref]], targets_sorted: list[int], blob: bytes,
            base: int, region: tuple[int, int], sites: list[tuple[int, int]] | None = None) -> list[list[tuple[int, int]]]:
    """Maximal contiguous runs of strings that only `fn` = (start, end) refers to, each [(address, length)], in address
    order. `refs_by_target` maps every referenced address of the module to its references; `targets_sorted` is its
    sorted key list (used to find references into a string's interior or padding); `sites` is site_index()."""
    start, end = fn
    if sites is None:
        sites = site_index(refs_by_target, region)
    k0 = bisect.bisect_left(sites, (start, -1))
    k1 = bisect.bisect_left(sites, (end, -1))
    own = sorted({t for _s, t in sites[k0:k1]})
    ptr_sites = pointer_sites(refs_by_target)
    good: list[tuple[int, int]] = []
    for t in own:
        n = string_extent(blob, t - base)
        if n is None:
            continue
        stop = align4(t + n + 1)
        if stop > region[1]:
            continue
        rs = refs_by_target[t]
        if not all(r.kind == "addiu" and start <= r.site < end for r in rs):
            continue
        k = bisect.bisect_right(targets_sorted, t)
        if k < len(targets_sorted) and targets_sorted[k] < stop:
            continue                          # something refers into the string or its padding
        j = bisect.bisect_left(ptr_sites, t)
        if j < len(ptr_sites) and ptr_sites[j] < stop:
            continue                          # a relocated word (a data pointer) lies inside: it is not text
        good.append((t, n))
    runs: list[list[tuple[int, int]]] = []
    for t, n in good:
        if runs and align4(runs[-1][-1][0] + runs[-1][-1][1] + 1) == t:
            runs[-1].append((t, n))
        else:
            runs.append([(t, n)])
    return runs


HEADER_RX = re.compile(r"/\*\s*HSTMATCH\b.*?\*/", re.S)
COMMENT_RX = re.compile(r"/\*.*?\*/|//[^\n]*", re.S)


def strip_comments(text: str) -> str:
    return COMMENT_RX.sub(lambda m: " " * len(m.group(0)) if "\n" not in m.group(0) else
                          re.sub(r"[^\n]", " ", m.group(0)), text)


def decl_rx(name: str) -> re.Pattern:
    return re.compile(r"^[ \t]*extern[ \t]+(?:\"C\"[ \t]+)?(?:const[ \t]+)?(?:(?:unsigned|signed)[ \t]+)?char[ \t]+"
                      + re.escape(name) + r"[ \t]*(\[[ \t]*\w*[ \t]*\])?[ \t]*;[ \t]*(?://[^\n]*|/\*[^\n]*?\*/[ \t]*)?\n",
                      re.M)


def rewrite(text: str, literals: dict[str, bytes], data_addr: int, note: str) -> tuple[str | None, str]:
    """(new source text, '') or (None, why). `literals` maps each identifier to the bytes of its string. Each
    identifier must be declared exactly once as an extern char array (uses: `NAME`) or extern char (uses: `&NAME`),
    and every use must be of that form; the declarations are removed, the uses replaced by the literal, the header
    gains data=0x..., and the note line (the second comment) gains `note`."""
    head = HEADER_RX.search(text)
    if not head or head.start() != 0:
        return None, "no HSTMATCH header on line 1"
    if re.search(r"\b(?:data|rodata|sdata|helpers)=", head.group(0)):
        return None, "the header already places data or has helpers="
    body_nc = strip_comments(text[head.end():])
    if '"' in re.sub(r"^[ \t]*#[^\n]*", "", body_nc, flags=re.M).replace('extern "C"', ""):
        return None, "the source already has string literals"
    out = text
    for name, raw in literals.items():
        decls = list(decl_rx(name).finditer(out))
        if len(decls) != 1:
            return None, f"{name}: {len(decls)} matching extern char declarations"
        is_array = decls[0].group(1) is not None
        out = out[:decls[0].start()] + out[decls[0].end():]
        lit = c_literal(raw)
        nc = strip_comments(out)
        if is_array:
            uses = [m for m in re.finditer(r"\b" + re.escape(name) + r"\b", nc)]
            if any(re.search(r"&\s*\(?\s*$", nc[:m.start()][-8:]) or re.match(r"\s*\)?\s*\[", nc[m.end():m.end() + 8])
                   or re.search(r"sizeof\s*\(?\s*$", nc[:m.start()][-12:]) for m in uses):
                return None, f"{name}: used other than as a pointer to its first char"
            spans = [(m.start(), m.end()) for m in uses]
        else:
            uses = [m for m in re.finditer(r"&\s*" + re.escape(name) + r"\b", nc)]
            bare = len(re.findall(r"\b" + re.escape(name) + r"\b", nc))
            if bare != len(uses):
                return None, f"{name}: scalar extern used other than as &{name}"
            spans = [(m.start(), m.end()) for m in uses]
        if not spans:
            return None, f"{name}: declared but not used"
        for s, e in reversed(spans):
            out = out[:s] + lit + out[e:]
    h = HEADER_RX.search(out)
    hdr = h.group(0)
    out = hdr[:-2].rstrip() + f" data=0x{data_addr:08x} */" + out[h.end():]
    m = re.compile(r"/\*.*?\*/", re.S).search(out, HEADER_RX.search(out).end())
    if not m:
        return None, "no note line"
    out = out[:m.end() - 2].rstrip() + " " + note + " */" + out[m.end():]
    return out, ""


def first_use_order(text: str, literals: list[str]) -> bool:
    """True when the literals appear in the rewritten source's text in the given (address) order."""
    pos = [text.find(lit) for lit in literals]
    return all(p >= 0 for p in pos) and pos == sorted(pos)


# ---------------------------------------------------------------------------------------------------------------
# the module side (retail relocation tables via tools/hstbuild.py)

def module_refs(module: str):
    import hstbuild as B
    retail, _cache = B.load_retail(module)
    funcs = B.ledger_functions(module)
    recs, _tables = B.retail_records(retail, funcs)
    data_sec = next(s for s in retail.loaded if s.name == ".data")
    blob = retail.data[data_sec.off:data_sec.off + data_sec.size]
    refs: dict[int, list[Ref]] = {}
    for r in recs:
        if r.target is None:
            continue
        if r.type == R_MIPS_32:
            kind = "ptr"
        elif r.type == R_MIPS_LO16:
            kind = "addiu" if B.retail_word(retail, r.site) >> 26 == OP_ADDIU else "lo-other"
        else:
            continue
        refs.setdefault(r.target, []).append(Ref(r.target, r.site, kind))
    return blob, data_sec.addr, refs


def load_symbol_names(module: str) -> dict[int, list[str]]:
    out: dict[int, list[str]] = {}
    path = ROOT / "symbols" / f"{module}.txt"
    if path.exists():
        for line in path.read_text(encoding="utf-8").splitlines():
            m = re.fullmatch(r"\s*(\S+)\s*=\s*(0x[0-9A-Fa-f]+)\s*;?\s*(?:#.*)?", line)
            if m:
                out.setdefault(int(m.group(2), 16), []).append(m.group(1))
    return out


def plan(module: str, src_dir: Path) -> dict:
    import hstmatch as M
    blob, base, refs = module_refs(module)
    region = REGIONS[module]
    targets_sorted = sorted(refs)
    sites = site_index(refs, region)
    symnames = load_symbol_names(module)
    units, skipped = [], {}
    stats = {"sources": 0, "with_runs": 0}
    for p in sorted(x for x in src_dir.iterdir() if x.suffix in (".c", ".cpp")):
        try:
            h = M.parse_header(p)
        except ValueError:
            continue
        if h["module"] != module:
            continue
        stats["sources"] += 1
        runs = runs_of((h["start"], h["end"]), refs, targets_sorted, blob, base, region, sites)
        if not runs:
            continue
        stats["with_runs"] += 1
        text = p.read_text(encoding="utf-8")
        best = max(runs, key=lambda r: r[-1][0] + r[-1][1] + 1 - r[0][0])
        names = {}
        for t, _n in best:
            cands = [f"DAT_{t:08x}"] + symnames.get(t, [])
            used = [c for c in cands if re.search(r"\b" + re.escape(c) + r"\b", text)]
            if len(used) != 1:
                break
            names[f"0x{t:08x}"] = used[0]
        rel = p.relative_to(ROOT).as_posix()
        if len(names) != len(best):
            skipped[rel] = "a string of the run is not named by one identifier in the source"
            continue
        u = Unit(rel, module, h["start"], best[0][0], best[-1][0] + best[-1][1] + 1, best, names)
        lits = {names[f"0x{t:08x}"]: blob[t - base:t - base + n] for t, n in best}
        new, why = rewrite(text, lits, u.lo, "x")
        if new is None:
            skipped[rel] = why
            continue
        units.append(asdict(u) | {"size": u.size, "in_order": first_use_order(new, [c_literal(v) for v in lits.values()])})
    return {"module": module, "region": [f"0x{region[0]:08x}", f"0x{region[1]:08x}"], "stats": stats,
            "units": units, "skipped": skipped,
            "unit_bytes": sum(u["size"] for u in units)}


NOTE = ("Data: its string literals ({n} strings, 0x{lo:08x}-0x{hi:08x}) are defined in place and placed with data= "
        "(tools/hstdatapool.py); before, the source named them through address externs.")


def apply(plan_json: dict, only: set[str] | None) -> list[str]:
    module = plan_json["module"]
    blob, base, _refs = module_refs(module)
    done = []
    for u in plan_json["units"]:
        if only and u["source"] not in only:
            continue
        p = ROOT / u["source"]
        text = p.read_text(encoding="utf-8")
        lits = {u["names"][f"0x{t:08x}"]: blob[t - base:t - base + n] for t, n in u["strings"]}
        new, why = rewrite(text, lits, u["lo"], NOTE.format(n=len(u["strings"]), lo=u["lo"], hi=u["hi"]))
        if new is None:
            print(f"skip {u['source']}: {why}")
            continue
        p.write_text(new, encoding="utf-8", newline="\n")
        done.append(u["source"])
    return done


def keep(plan_json: dict, match_json: list[dict]) -> tuple[list[str], list[str]]:
    status = {r["source"].replace("\\", "/"): r["status"] for r in match_json}  # hstmatch writes backslashes on Windows
    kept, reverted = [], []
    for u in plan_json["units"]:
        s = status.get(u["source"].replace("\\", "/"))
        if s is None:
            continue
        if s == "EXACT":
            kept.append(u["source"])
        else:
            subprocess.run(["git", "-C", str(ROOT), "checkout", "--", u["source"]], check=True)
            reverted.append(u["source"])
    return kept, reverted


# ---------------------------------------------------------------------------------------------------------------
# string-pointer tables

TABLE_NOTE = ("Data: its string-pointer tables ({k} tables, {n} strings, 0x{lo:08x}-0x{hi:08x}) are defined in place "
              "and placed with data= (tools/hstdatapool.py tables); before, the source named them through address "
              "externs.")


def string_tables(module: str, blob: bytes, base: int, refs: dict[int, list[Ref]]) -> tuple[list[dict], dict[str, int]]:
    """Every pointer table of the data region whose entries are strings that only it refers to, laid out as one
    file-scope initialiser compiles: [strings in first-entry order][the pointer array]. Returns (tables, why-not)."""
    lo, hi = REGIONS[module]
    ptr = {r.site: t for t, rs in refs.items() for r in rs if r.kind == "ptr"}
    out, why = [], {}

    def no(reason: str) -> None:
        why[reason] = why.get(reason, 0) + 1
    for start in sorted(ptr):
        if not lo <= start < hi or (start - 4) in ptr:
            continue
        n = 0
        while (start + 4 * n) in ptr:
            n += 1
        users = refs.get(start, [])
        if not users or any(r.kind == "ptr" for r in users):
            no("table referenced by data or not at all")
            continue
        if any(refs.get(start + 4 * i) for i in range(1, n)):
            no("reference into the table interior")
            continue
        entries = [ptr[start + 4 * i] for i in range(n)]
        order = list(dict.fromkeys(entries))
        cur, ok = order[0], True
        for t in order:
            ext = string_extent(blob, t - base) if lo <= t < hi else None
            if ext is None or t != cur or any(r.kind != "ptr" or not start <= r.site < start + 4 * n
                                              for r in refs.get(t, [])):
                ok = False
                break
            cur = align4(t + ext + 1)
        if not ok or cur != start:
            no("entries are not this table's own strings laid out before it")
            continue
        out.append({"table": start, "entries": entries, "lo": order[0], "hi": start + 4 * n,
                    "users": sorted(r.site for r in users)})
    return out, why


def table_decl_rx(name: str) -> re.Pattern:
    return re.compile(r'^[ \t]*extern[ \t]+("C"[ \t]+)?(const[ \t]+)?char[ \t]*\*[ \t]*' + re.escape(name)
                      + r"[ \t]*(\[[ \t]*\w*[ \t]*\])?[ \t]*;[^\n]*\n", re.M)


def rewrite_tables(text: str, tables: list[tuple[str, list[bytes]]], data_addr: int, note: str) -> tuple[str | None, str]:
    """`tables`: (identifier, the strings of its entries in order), in address order. Each extern pointer declaration is
    removed; the definitions go, in address order, where the first declaration was; the header gains data=."""
    head = HEADER_RX.search(text)
    if not head or head.start() != 0:
        return None, "no HSTMATCH header on line 1"
    if re.search(r"\b(?:data|rodata|sdata|helpers)=", head.group(0)):
        return None, "the header already places data or has helpers="
    body_nc = strip_comments(text[head.end():])
    if '"' in re.sub(r"^[ \t]*#[^\n]*", "", body_nc, flags=re.M).replace('extern "C"', ""):
        return None, "the source already has string literals"
    out, first, defs = text, None, []
    for name, strings in tables:
        decls = list(table_decl_rx(name).finditer(out))
        if len(decls) != 1:
            return None, f"{name}: {len(decls)} matching extern char-pointer declarations"
        d = decls[0]
        if d.group(3) is None and len(strings) != 1:
            return None, f"{name}: declared as one pointer but the table has {len(strings)} entries"
        if not re.search(r"\b" + re.escape(name) + r"\b", strip_comments(out[:d.start()] + out[d.end():])):
            return None, f"{name}: declared but not used"
        linkage = 'extern "C" ' if d.group(1) else ""
        const = "const " if d.group(2) else ""
        lits = [c_literal(raw) for raw in strings]
        if d.group(3) is None:
            defs.append(f"{linkage}{const}char *{name} = {lits[0]};\n")
        else:
            defs.append(f"{linkage}{const}char *{name}{d.group(3).replace(' ', '')} = {{\n"
                        + "".join(f"    {lit},\n" for lit in lits) + "};\n")
        first = d.start() if first is None else min(first, d.start())
        out = out[:d.start()] + "\0" * (d.end() - d.start()) + out[d.end():]   # keep offsets while removing
    out = out[:first] + "".join(defs) + out[first:]
    out = out.replace("\0", "")
    h = HEADER_RX.search(out)
    out = h.group(0)[:-2].rstrip() + f" data=0x{data_addr:08x} */" + out[h.end():]
    m = re.compile(r"/\*.*?\*/", re.S).search(out, HEADER_RX.search(out).end())
    if not m:
        return None, "no note line"
    out = out[:m.end() - 2].rstrip() + " " + note + " */" + out[m.end():]
    return out, ""


def plan_tables(module: str, src_dir: Path) -> dict:
    import hstmatch as M
    blob, base, refs = module_refs(module)
    tables, why = string_tables(module, blob, base, refs)
    symnames = load_symbol_names(module)
    spans = []
    for p in sorted(x for x in src_dir.iterdir() if x.suffix in (".c", ".cpp")):
        try:
            h = M.parse_header(p)
        except ValueError:
            continue
        if h["module"] == module:
            spans.append((h["start"], h["end"], p))
    spans.sort()
    starts = [a for a, _b, _p in spans]
    by_source: dict[Path, list[dict]] = {}
    for t in tables:
        owners = set()
        for site in t["users"]:
            k = bisect.bisect_right(starts, site) - 1
            owners.add(spans[k][2] if k >= 0 and spans[k][0] <= site < spans[k][1] else None)
        if len(owners) != 1 or None in owners:
            key = "table not used by exactly one matched source"
            why[key] = why.get(key, 0) + 1
            continue
        by_source.setdefault(owners.pop(), []).append(t)
    units, skipped = [], {}
    for p, ts in sorted(by_source.items()):
        rel = p.relative_to(ROOT).as_posix()
        ts.sort(key=lambda t: t["lo"])
        if any(a["hi"] != b["lo"] for a, b in zip(ts, ts[1:])):
            skipped[rel] = "its tables are not contiguous"
            continue
        text = p.read_text(encoding="utf-8")
        names = []
        for t in ts:
            cands = [f"DAT_{t['table']:08x}"] + symnames.get(t["table"], [])
            used = [c for c in cands if re.search(r"\b" + re.escape(c) + r"\b", text)]
            if len(used) != 1:
                break
            names.append(used[0])
        if len(names) != len(ts):
            skipped[rel] = "a table is not named by one identifier in the source"
            continue
        strings = [[blob[e - base:e - base + string_extent(blob, e - base)] for e in t["entries"]] for t in ts]
        new, why_not = rewrite_tables(text, list(zip(names, strings)), ts[0]["lo"], "x")
        if new is None:
            skipped[rel] = why_not
            continue
        units.append({"source": rel, "module": module, "lo": ts[0]["lo"], "hi": ts[-1]["hi"], "names": names,
                      "tables": [{"table": t["table"], "entries": t["entries"]} for t in ts],
                      "size": ts[-1]["hi"] - ts[0]["lo"]})
    return {"module": module, "stats": {"tables": len(tables)}, "why_not": why, "units": units, "skipped": skipped,
            "unit_bytes": sum(u["size"] for u in units)}


def apply_tables(plan_json: dict, only: set[str] | None) -> list[str]:
    blob, base, _refs = module_refs(plan_json["module"])
    done = []
    for u in plan_json["units"]:
        if only and u["source"] not in only:
            continue
        p = ROOT / u["source"]
        tables = [(name, [blob[e - base:e - base + string_extent(blob, e - base)] for e in t["entries"]])
                  for name, t in zip(u["names"], u["tables"])]
        note = TABLE_NOTE.format(k=len(tables), n=len({e for t in u["tables"] for e in t["entries"]}), lo=u["lo"],
                                 hi=u["hi"])
        new, why = rewrite_tables(p.read_text(encoding="utf-8"), tables, u["lo"], note)
        if new is None:
            print(f"skip {u['source']}: {why}")
            continue
        p.write_text(new, encoding="utf-8", newline="\n")
        done.append(u["source"])
    return done


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)
    a = sub.add_parser("plan")
    a.add_argument("module", choices=sorted(REGIONS))
    a.add_argument("--src-dir", type=Path)
    a.add_argument("--json", type=Path)
    b = sub.add_parser("apply")
    b.add_argument("plan", type=Path)
    b.add_argument("--only", nargs="*")
    t = sub.add_parser("tables-plan")
    t.add_argument("module", choices=sorted(REGIONS))
    t.add_argument("--src-dir", type=Path)
    t.add_argument("--json", type=Path)
    ta = sub.add_parser("tables-apply")
    ta.add_argument("plan", type=Path)
    ta.add_argument("--only", nargs="*")
    c = sub.add_parser("keep")
    c.add_argument("plan", type=Path)
    c.add_argument("match_json", type=Path)
    args = ap.parse_args()
    if args.cmd == "plan":
        src = args.src_dir or ROOT / SRC_DIR[args.module]
        out = plan(args.module, src if src.is_absolute() else ROOT / src)
        print(f"{args.module}: {out['stats']['sources']} sources, {out['stats']['with_runs']} own a string run; "
              f"{len(out['units'])} units, {out['unit_bytes']:,} B; {len(out['skipped'])} skipped; "
              f"{sum(1 for u in out['units'] if not u['in_order'])} units whose literals are not in address order")
        if args.json:
            args.json.parent.mkdir(parents=True, exist_ok=True)
            args.json.write_text(json.dumps(out, indent=1) + "\n", encoding="utf-8")
        return 0
    if args.cmd == "tables-plan":
        src = args.src_dir or ROOT / SRC_DIR[args.module]
        out = plan_tables(args.module, src if src.is_absolute() else ROOT / src)
        print(f"{args.module}: {out['stats']['tables']} single-owner string tables; {len(out['units'])} sources, "
              f"{out['unit_bytes']:,} B; {len(out['skipped'])} skipped")
        if args.json:
            args.json.parent.mkdir(parents=True, exist_ok=True)
            args.json.write_text(json.dumps(out, indent=1) + "\n", encoding="utf-8")
        return 0
    pj = json.loads(args.plan.read_text(encoding="utf-8"))
    if args.cmd == "tables-apply":
        done = apply_tables(pj, set(args.only) if args.only else None)
        print(f"rewrote {len(done)} sources")
        for d in done:
            print(d)
        return 0
    if args.cmd == "apply":
        done = apply(pj, set(args.only) if args.only else None)
        print(f"rewrote {len(done)} sources")
        for d in done:
            print(d)
        return 0
    kept, reverted = keep(pj, json.loads(args.match_json.read_text(encoding="utf-8")))
    print(f"kept {len(kept)} EXACT rewrites, reverted {len(reverted)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
