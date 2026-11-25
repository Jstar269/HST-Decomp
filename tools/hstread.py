#!/usr/bin/env python3
"""Readability work on the exact sources (they all match; this is about how far they read from real source).

    python tools/hstread.py rank [--src-dir DIR ...] [--top N] [--runs] [--json FILE]
    python tools/hstread.py tidy PATH... [--write] [--list]
    python tools/hstread.py twins LANE [--baseline LANE0]
    python tools/hstread.py harvest LANE --stage headers|sources [--dry-run] [--list-out FILE]

A readability lane is a tools/hstlane.py lane directory with READ-BASE.txt holding the main-tree commit it was copied
from. TWINS ports each edited lane source to its unedited twins (the same code at other addresses: the EBOOT/Sharing
pair, clones) by replacing the exemplar's addresses with the twin's. HARVEST copies the lane's edits into the main
tree after checking that the main tree did not change those paths since the base commit.

RANK. Per source it counts:
  addr      address-named symbols (FUN_/DAT_/PTR_/LAB_/SUB_<8 hex>)
  ghidra    Ghidra-style locals and parameters (param_1, uVar3, puVar2, local_10, auStack_20, in_v0, unaff_s0,
            extraout_v0, ...) and Ghidra type names (undefined4, ...) outside the typedef block
  offset    raw pointer-offset accesses: a cast applied to `(x + 0x14)`
  boiler    lines of the Ghidra typedef/define block at the top of generated sources
The score is addr + ghidra + 2*offset + boiler/4 (offset casts hide a struct, which costs most to read). --runs groups
sources by their address neighbourhood (same 0x1000 page of one module: usually one retail object file, so related
functions share structs) and ranks the groups: that is the batch order for readability lanes.

TIDY. The mechanical part of the pass, for MWCC sources made by tools/hstgen.py: the per-file Ghidra typedef block
(undefined4, uint, uchar, ...) is replaced by `#include "hst/types.h"` and the body uses u8/u16/u32/u64/s16/s32/s64
for the same underlying types (so the compiled code cannot change); `bool`, `true` and `false` come from the header in
C; a `code` typedef, the ABS macro and (in C++) true/false defines stay only when the body uses them; a `return;` that
ends a void function is dropped. Sources with a cc= (GCC) profile are skipped (hstmatch compiles those without
include/), and so is a source that already uses one of the new names or declares a block type differently. Without
--write it only reports. Every tidied source must still be EXACT (tools/hstmatch.py) before it is kept.
"""
from __future__ import annotations

import argparse
import json
import re
import subprocess
import sys
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent

ADDR = re.compile(r"\b(?:FUN|DAT|PTR|LAB|SUB)_[0-9a-fA-F]{8}\b")
GHIDRA = re.compile(r"\b(?:param_\d+|[a-z]{1,4}Var\d+|local_[0-9a-fA-F]+[a-z0-9_]*|a[a-z]Stack_[0-9a-fA-F]+"
                    r"|in_[a-z]\w*|unaff_\w+|extraout_\w+|undefined\d?)\b")
# *(T *)(base + 0x14), *(T **)(p + 8), (T *)(x + 0x20): a cast in front of `(name + offset)`
OFFSET = re.compile(r"\(\s*(?:const\s+)?[A-Za-z_]\w*(?:\s+[A-Za-z_]\w*)*\s*\*+\s*\)\s*\(\s*[A-Za-z_][\w.>-]*\s*[+-]\s*"
                    r"(?:0x[0-9a-fA-F]+|\d+)\s*\)")
BOILER = re.compile(r"^(?:typedef\s+(?:unsigned|long|int|char|short)[\w\s]*\b(?:undefined\d?|byte|ushort|uint|ulonglong|"
                    r"longlong|uchar|bool|code|word|dword|ulong|sbyte)\b.*;|#define\s+(?:true|false|ABS)\b.*)\s*$", re.M)
HEADER = re.compile(r"module=(\S+)\s+start=0x([0-9a-fA-F]+)")

WIDTH = {"unsigned char": "u8", "unsigned short": "u16", "unsigned int": "u32", "unsigned long long": "u64",
         "short": "s16", "int": "s32", "long long": "s64"}
BLOCK_TYPES = ("undefined", "undefined1", "undefined2", "undefined4", "undefined8", "byte", "uchar", "ushort", "uint",
               "ulonglong", "longlong")
TYPEDEF = re.compile(r"^typedef\s+([a-z][a-z ]*?)\s+(\w+)\s*;\s*$")
CODE_TYPEDEF = re.compile(r"^typedef\s+int\s+code\s*\((?:\.\.\.)?\)\s*;\s*$")
DEFINE = re.compile(r"^#define\s+(true|false|ABS)\b.*$")
NEW_NAMES = re.compile(r"\b[us](?:8|16|32|64)\b")
USES_HEADER = re.compile(r"\b(?:[us](?:8|16|32|64)|bool|true|false)\b")
INCLUDE_LINE = '#include "hst/types.h"'
TAIL_RETURN = re.compile(r"(?<!:)\n[ \t]+return;\n\}(?=\n|$)")   # not after a label: `L: }` is a syntax error in C


def measure(text: str) -> dict:
    boiler = len(BOILER.findall(text))
    body = BOILER.sub("", text)
    body = re.sub(r"/\*.*?\*/", "", body, flags=re.S)
    body = re.sub(r"//[^\n]*", "", body)
    m = {"addr": len(ADDR.findall(body)), "ghidra": len(GHIDRA.findall(body)),
         "offset": len(OFFSET.findall(body)), "boiler": boiler}
    m["score"] = m["addr"] + m["ghidra"] + 2 * m["offset"] + boiler // 4
    return m


def scan(dirs: list[Path]) -> list[dict]:
    rows = []
    for d in dirs:
        for p in sorted(d.glob("*.c*")):
            if p.suffix not in (".c", ".cpp"):
                continue
            text = p.read_text(encoding="utf-8", errors="replace")
            h = HEADER.search(text.split("\n", 1)[0])
            row = measure(text)
            row.update(path=p.relative_to(ROOT).as_posix() if p.is_relative_to(ROOT) else p.as_posix(),
                       module=h.group(1) if h else "?", start=int(h.group(2), 16) if h else -1)
            rows.append(row)
    return rows


def runs(rows: list[dict]) -> list[dict]:
    groups: dict[tuple[str, int], list[dict]] = defaultdict(list)
    for r in rows:
        groups[(r["module"], r["start"] >> 12)].append(r)
    out = []
    for (module, page), rs in groups.items():
        out.append({"module": module, "page": f"0x{page << 12:08x}", "sources": len(rs),
                    "score": sum(r["score"] for r in rs), "offset": sum(r["offset"] for r in rs),
                    "paths": [r["path"] for r in sorted(rs, key=lambda r: r["start"])]})
    return sorted(out, key=lambda g: -g["score"])


def source_text(path: Path) -> str:
    """A source's text with its line endings as they are on disk (read_text would turn CRLF into LF)."""
    return path.read_bytes().decode("utf-8")


def tidy_text(text: str) -> tuple[str | None, str]:
    """The tidied source, or (None, reason) when the file is left alone. A source whose first line ends in CRLF keeps
    CRLF line endings (the rules below are written for LF)."""
    if text.split("\n", 1)[0].endswith("\r"):
        new, why = tidy_text(text.replace("\r\n", "\n"))
        return (new.replace("\n", "\r\n") if new is not None else None), why
    lines = text.split("\n")
    if not lines[0].startswith("/* HSTMATCH "):
        return None, "no header"
    if re.search(r"\bcc=", lines[0]):
        return None, "cc= profile"
    cpp = "lang=c++" in lines[0]
    if NEW_NAMES.search(text):
        return None, "uses a new name"
    rename: dict[str, str] = {}
    optional: dict[int, str] = {}     # block lines kept only when the body uses the name
    drop: set[int] = set()
    for i, ln in enumerate(lines[2:], 2):
        m = TYPEDEF.match(ln)
        if m and m.group(2) in BLOCK_TYPES:
            if m.group(1) not in WIDTH:
                return None, f"{m.group(2)} is {m.group(1)}"
            rename[m.group(2)] = WIDTH[m.group(1)]
            drop.add(i)
        elif m and m.group(2) == "bool":
            if cpp or m.group(1) != "unsigned char":
                return None, f"bool is {m.group(1)}"
            drop.add(i)
        elif CODE_TYPEDEF.match(ln):
            optional[i] = "code"
        elif (d := DEFINE.match(ln)):
            if d.group(1) == "ABS" or cpp:
                optional[i] = d.group(1)
            else:
                drop.add(i)
    if not drop and not optional:
        return None, "no typedef block"
    body = "\n".join(ln for i, ln in enumerate(lines) if i >= 2 and i not in drop and i not in optional)
    for i, name in optional.items():
        if not re.search(rf"\b{name}\b", body):
            drop.add(i)
    first = min(drop | set(optional))
    out = []
    for i, ln in enumerate(lines):
        if i == first:
            out.append(INCLUDE_LINE)
        if i in drop:
            continue
        if i >= 2:
            for old, new in rename.items():
                ln = re.sub(rf"\b{old}\b", new, ln)
        out.append(ln)
    new = "\n".join(out)
    new = TAIL_RETURN.sub("\n}", new)
    if not USES_HEADER.search(new.split(INCLUDE_LINE, 1)[1]):
        new = new.replace(INCLUDE_LINE + "\n", "", 1)
    new = re.sub(r"\n{3,}", "\n\n", new)
    return new, "tidied"


TOKEN = re.compile(r"/\*.*?\*/|//[^\n]*|0[xX][0-9a-fA-F]+|\w+|\S", re.S)
ADDRESSY = re.compile(r"(?:FUN|DAT|PTR|LAB|SUB)_[0-9a-fA-F]{8}|0[xX][0-9a-fA-F]+")


def address_map(exemplar: str, twin: str) -> dict[str, str] | None:
    """Token map from an exemplar's text to its twin's (same code, other addresses): every differing token pair must be
    two address names or two hex literals; comments are compared through the address names inside them. None when
    the two texts are not twins."""
    a = [t for t in TOKEN.findall(exemplar.split("\n", 2)[2])]
    b = [t for t in TOKEN.findall(twin.split("\n", 2)[2])]
    if len(a) != len(b):
        return None
    out: dict[str, str] = {}
    for x, y in zip(a, b):
        if x == y:
            continue
        if x.startswith(("/*", "//")) and y.startswith(("/*", "//")):
            xs, ys = ADDRESSY.findall(x), ADDRESSY.findall(y)
            if len(xs) != len(ys) or ADDRESSY.sub("@", x) != ADDRESSY.sub("@", y):
                return None
            pairs = list(zip(xs, ys))
        elif ADDRESSY.fullmatch(x) and ADDRESSY.fullmatch(y) and x[:4] == y[:4]:
            pairs = [(x, y)]
        else:
            return None
        for p, q in pairs:
            if p != q and out.setdefault(p, q) != q:
                return None
    return out


def port_text(old: str, new: str, twin: str) -> str | None:
    """The twin's text for an edited exemplar: `new` with the exemplar's addresses replaced by the twin's (the map comes
    from `old`, the exemplar before the edit), under the twin's own header and note lines. None when `old` and `twin`
    are not twins."""
    amap = address_map(old, twin)
    if amap is None:
        return None
    head = twin.split("\n", 2)
    old_head, new_head = old.split("\n", 2), new.split("\n", 2)
    s_old, s_new = (re.search(r"\bsymbol=(\S+)", h[0]).group(1) for h in (old_head, new_head))
    if s_old != s_new:       # the exemplar's function was renamed: the twin takes the same name
        head[0] = re.sub(r"\bsymbol=\S+", f"symbol={s_new}", head[0])
    if old_head[1].rstrip("\r") != new_head[1].rstrip("\r") and not re.search(r"^/\* Clone of |\(port of ", head[1]):
        head[1] = new_head[1]   # the twin's own note described the exemplar's old text
    body = ADDRESSY.sub(lambda m: amap.get(m.group(0), m.group(0)), new_head[2])
    out = "\n".join(head[:2] + [body]).replace("\r\n", "\n")
    return out.replace("\n", "\r\n") if head[0].endswith("\r") else out    # the twin keeps its own line endings


def twin_key(text: str) -> str | None:
    """Sources with equal keys are the same code at other addresses (an EBOOT/Sharing twin, a clone family)."""
    parts = text.split("\n", 2)
    if len(parts) < 3 or not parts[2].strip():
        return None
    body = ADDRESSY.sub("@", parts[2])
    m = re.search(r"\blang=(\S+)", parts[0])
    return (m.group(1) if m else "?") + "\n" + re.sub(r"\s+", " ", body).strip()


def git_texts(repo: Path, rev: str, rels: list[str]) -> dict[str, str]:
    """The text of each path at `rev` (one `git cat-file --batch`); a path missing there is left out."""
    query = "".join(f"{rev}:{x}\n" for x in rels).encode()
    r = subprocess.run(["git", "-C", str(repo), "cat-file", "--batch"], input=query, capture_output=True, check=True)
    out, data, pos = {}, r.stdout, 0
    for rel in rels:
        nl = data.index(b"\n", pos)
        head = data[pos:nl].split()
        pos = nl + 1
        if len(head) == 2 and head[1] == b"missing":
            continue
        size = int(head[2])
        out[rel] = data[pos:pos + size].decode("utf-8")
        pos += size + 1
    return out


def lane_changes(lane: Path, main: Path, base: str) -> tuple[list[str], list[str], list[str]]:
    """(changed or new sources, changed or new include/ files, symbols/ files with appended lines) of a lane against
    the main tree's commit `base` (the commit the lane was copied from), as repository-relative paths."""
    listed = subprocess.run(["git", "-C", str(main), "ls-tree", "-r", "--name-only", base, "src", "include", "symbols"],
                            capture_output=True, text=True, check=True).stdout.split()
    old = git_texts(main, base, listed)
    srcs, incs, syms = [], [], []
    for d in ("src", "include", "symbols"):
        for p in sorted((lane / d).rglob("*")):
            if not p.is_file() or "__pycache__" in p.parts:
                continue
            rel = p.relative_to(lane).as_posix()
            if rel in old and old[rel].encode("utf-8") == p.read_bytes():
                continue
            (srcs if d == "src" else incs if d == "include" else syms).append(rel)
    return srcs, incs, syms


def true_twins(rel: str, group: list[str], lane: Path) -> list[str]:
    """The same function in the other module, out of the sources with the same code shape. A clone family (many
    different functions of one shape, e.g. every two-level deleting destructor) shares a key, but a readability edit
    names one function's types, so it is ported only to the sources that record themselves as this one's clone or port
    (their note says they are its text with addresses substituted, which stays true only if they follow it), or else
    to the single other-module member of a group that has a single member in this module."""
    module = rel.split("/")[1]
    others = [t for t in group if t.split("/")[1] != module]
    named = [t for t in group if t != rel and re.search(rf"(?:Clone of|port of) {re.escape(rel)}\b",
                                                        (lane / t).read_text(encoding="utf-8").split("\n", 2)[1])]
    if named:
        return named
    if len(others) == 1 and sum(t.split("/")[1] == module for t in group) == 1:
        return others
    return []


def cmd_twins(a) -> int:
    """Port every edited lane source to its unedited twins (same code at other addresses) in the lane."""
    lane, main = a.lane.resolve(), ROOT
    if a.baseline:      # the lane was copied from another lane directory, not from a main-tree commit
        listed = sorted(p.relative_to(a.baseline).as_posix() for p in (a.baseline / "src").rglob("*.c*"))
        originals = {}
        for rel in listed:
            if (lane / rel).exists() and (lane / rel).read_bytes() != (a.baseline / rel).read_bytes():
                originals[rel] = source_text(a.baseline / rel)
        edited = set(originals)
    else:
        base = (lane / "READ-BASE.txt").read_text().strip()
        srcs, _, _ = lane_changes(lane, main, base)
        edited = set(srcs)
        listed = subprocess.run(["git", "-C", str(main), "ls-tree", "-r", "--name-only", base, "src"],
                                capture_output=True, text=True, check=True).stdout.split()
        originals = git_texts(main, base, sorted(edited))
    groups: dict[str, list[str]] = defaultdict(list)
    for rel in listed:
        if rel.endswith((".c", ".cpp")) and (lane / rel).exists():
            t = originals.get(rel) if rel in edited else source_text(lane / rel)
            if t is not None and (k := twin_key(t)):
                groups[k].append(rel)
    ported, failed = [], []
    for rel in sorted(edited):
        old = originals.get(rel)
        if old is None:
            continue
        new = source_text(lane / rel)
        for twin in true_twins(rel, groups.get(twin_key(old) or "", []), lane):
            if twin in edited:
                continue
            out = port_text(old, new, source_text(lane / twin))
            if out is None:
                failed.append(twin)
                continue
            (lane / twin).write_bytes(out.encode("utf-8"))
            edited.add(twin)
            ported.append(twin)
    for t in ported:
        print(t)
    print(f"twins: {len(ported)} ported, {len(failed)} not portable" + (": " + ", ".join(failed[:10]) if failed else ""),
          file=sys.stderr)
    return 0


def cmd_harvest(a) -> int:
    """Copy a readability lane's edits into the main tree, refusing every path the main tree changed since the lane's
    base commit. Stage `headers` copies include/ only (then migrate the toolchain lock: tools/hstpromote.py
    migrate-toolchain); stage `sources` copies src/ and the lines appended to symbols/ (then hstmatch and promote)."""
    lane, main = a.lane.resolve(), ROOT
    base = (lane / "READ-BASE.txt").read_text().strip()
    srcs, incs, syms = lane_changes(lane, main, base)
    paths = incs if a.stage == "headers" else srcs + syms
    old = git_texts(main, base, paths)
    problems = []
    for rel in paths:
        cur = main / rel
        if rel in old:
            if not cur.exists() or cur.read_bytes() != old[rel].encode("utf-8"):
                problems.append(f"{rel}: changed in the main tree since {base[:8]}")
        elif rel.startswith(("src/", "symbols/")):
            problems.append(f"{rel}: not in the main tree (a readability lane only edits existing sources)")
        elif cur.exists():
            problems.append(f"{rel}: created in the main tree since {base[:8]}")
    appended: dict[str, list[str]] = {}
    for rel in syms:
        before, after = old.get(rel, ""), (lane / rel).read_text(encoding="utf-8")
        if not after.startswith(before):
            problems.append(f"{rel}: the lane edited existing symbol lines (append only)")
        else:
            appended[rel] = [ln for ln in after[len(before):].split("\n") if ln.strip()]
    if problems:
        print("harvest refused:\n  " + "\n  ".join(problems[:40]), file=sys.stderr)
        return 1
    if not a.dry_run:
        for rel in paths:
            dst = main / rel
            dst.parent.mkdir(parents=True, exist_ok=True)
            if rel in appended:
                with dst.open("a", encoding="utf-8", newline="\n") as f:
                    f.write("".join(ln + "\n" for ln in appended[rel]))
            else:
                dst.write_bytes((lane / rel).read_bytes())
    listing = a.list_out or (lane / f"harvest-{a.stage}.txt")
    listing.write_text("".join(p + "\n" for p in paths if p.startswith(("src/", "include/"))), encoding="utf-8")
    print(f"harvest {a.stage}: {len(paths)} path(s) {'would be ' if a.dry_run else ''}copied "
          f"({sum(p.startswith('src/') for p in paths)} sources, {len(appended)} symbol file(s)); list: {listing}")
    return 0


def cmd_tidy(a) -> int:
    reasons: dict[str, int] = defaultdict(int)
    changed = 0
    for p in a.paths:
        text = source_text(p)
        new, why = tidy_text(text)
        if new is None or new == text:
            reasons[why if new is None else "unchanged"] += 1
            continue
        changed += 1
        if a.write:
            p.write_bytes(new.encode("utf-8"))
        if a.list:
            print(p.as_posix())
    print(f"tidy: {changed} source(s) {'rewritten' if a.write else 'would change'}; left alone: "
          + ", ".join(f"{k} {v}" for k, v in sorted(reasons.items())), file=sys.stderr)
    return 0


def cmd_rank(a) -> int:
    dirs = [d if d.is_absolute() else ROOT / d for d in a.src_dir] if a.src_dir else sorted(
        d for d in (ROOT / "src").iterdir() if d.is_dir())
    rows = scan(dirs)
    total = defaultdict(int)
    for r in rows:
        for k in ("addr", "ghidra", "offset", "boiler", "score"):
            total[k] += r[k]
    clean = sum(1 for r in rows if r["score"] == 0)
    print(f"{len(rows)} sources, {clean} with score 0; totals: " +
          ", ".join(f"{k} {total[k]}" for k in ("score", "addr", "ghidra", "offset", "boiler")))
    if a.runs:
        result = runs(rows)
        for g in result[:a.top]:
            print(f"{g['score']:7d}  {g['module']:24s} {g['page']}  {g['sources']:3d} sources  offset {g['offset']}")
    else:
        result = sorted(rows, key=lambda r: -r["score"])
        for r in result[:a.top]:
            print(f"{r['score']:6d}  addr {r['addr']:4d} ghidra {r['ghidra']:4d} offset {r['offset']:4d}  {r['path']}")
    if a.json:
        a.json.parent.mkdir(parents=True, exist_ok=True)
        a.json.write_text(json.dumps({"totals": dict(total), "sources": len(rows), "clean": clean, "rows": result},
                                     indent=1), encoding="utf-8")
    return 0


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    sub = ap.add_subparsers(dest="command", required=True)
    rk = sub.add_parser("rank")
    rk.add_argument("--src-dir", action="append", type=Path, help="directory of sources (default: every src/ module)")
    rk.add_argument("--top", type=int, default=20)
    rk.add_argument("--runs", action="store_true", help="rank address-neighbourhood groups instead of single sources")
    rk.add_argument("--json", type=Path, help="write every row (or group) as JSON")
    td = sub.add_parser("tidy")
    td.add_argument("paths", nargs="+", type=Path)
    td.add_argument("--write", action="store_true")
    td.add_argument("--list", action="store_true", help="print each source that changes")
    tw = sub.add_parser("twins")
    tw.add_argument("lane", type=Path)
    tw.add_argument("--baseline", type=Path, help="the lane directory this lane was copied from (instead of READ-BASE.txt)")
    hv = sub.add_parser("harvest")
    hv.add_argument("lane", type=Path)
    hv.add_argument("--stage", choices=("headers", "sources"), required=True)
    hv.add_argument("--dry-run", action="store_true")
    hv.add_argument("--list-out", type=Path, help="where to list the copied paths (default <lane>/harvest-<stage>.txt)")
    a = ap.parse_args()
    return {"tidy": cmd_tidy, "rank": cmd_rank, "twins": cmd_twins, "harvest": cmd_harvest}[a.command](a)


if __name__ == "__main__":
    sys.exit(main())
