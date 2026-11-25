#!/usr/bin/env python3
"""Instantiate known container templates for every element size found in the retail code.

    python tools/hsttmpl.py --tag t1 [--module EBOOT.BIN] [--parallel 12] [--dry-run]

vector<T> code is instantiated once per element type, and each size compiles to different instructions
(shift, multiply, constants), so hstclone.py cannot carry one exemplar across sizes. For each unmatched
reviewed function whose call sequence fits a template, this tool reads the callees, the string addresses
and the max_size constant from the retail bytes, derives sizeof(T), writes the template source and keeps
only exact builds.

Templates:
  reserve   allocate n elements; length and allocation failures go to fprintf(stderr) + abort
  reserve_t the same with the length check through a throwing helper
  grow      the growth policy (new capacity for n more elements)
"""
from __future__ import annotations

import argparse
import json
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hstclone as C
import hstmatch as M

SRC_DIRS = {"EBOOT.BIN": "eboot", "hstp_trans_main.prx": "sharing_main", "libfont.prx": "libfont",
            "scePsmfP_library.prx": "psmfp", "scePsmf_library.prx": "psmf", "hstp_trans_sceFont_Library.prx": "sharing_font"}

RESERVE = """/* HSTMATCH module={module} start=0x{start:08x} end=0x{end:08x} symbol=FUN_{start:08x} lang=c++ */
/* Container reserve for a {size}-byte element type (template instance, hsttmpl.py tag {tag}): length check
   against max_size, allocate n elements through an allocator temporary, report failures on newlib stderr
   and abort. Names provisional. */
extern "C" {{
struct Reent_{start:08x} {{ int pad[3]; void *err; }};
Reent_{start:08x} *FUN_{reent:08x}(void);
int FUN_{fprintf:08x}(void *stream, const char *format, ...);
void FUN_{abort:08x}(void);
{lenerr_decl}void *FUN_{alloc:08x}(unsigned size);
{str_decls}}}
struct Elem_{start:08x} {{ {elem} }};
struct Alloc_{start:08x} {{ Elem_{start:08x} *allocate(unsigned n) {{ return (Elem_{start:08x} *)FUN_{alloc:08x}(n * sizeof(Elem_{start:08x})); }} }};
struct Vec_{start:08x} {{ Elem_{start:08x} *data; int size; unsigned capacity; }};
extern "C" void FUN_{start:08x}(Vec_{start:08x} *v, unsigned n)
{{
    if (n > 0xffffffffu / sizeof(Elem_{start:08x})){lencheck}
    Elem_{start:08x} *p = Alloc_{start:08x}().allocate(n);
    if (p == 0) {{
        FUN_{fprintf:08x}(FUN_{reent:08x}()->err, DAT_{mem_str:08x});
        FUN_{abort:08x}();
    }}
    v->data = p;
    v->capacity = n;
}}
"""

GROW = """/* HSTMATCH module={module} start=0x{start:08x} end=0x{end:08x} symbol=FUN_{start:08x} lang=c++ */
/* Container growth policy for a {size}-byte element type (template instance, hsttmpl.py tag {tag}):
   max_size = 0xffffffff / sizeof(T); grow by 3/5 below max/3, by 1/2 below 2*max/3, else to max_size;
   never less than n. Names provisional. */
extern "C" void FUN_{lenerr:08x}(void);
template <class T> inline const T &Max_{start:08x}(const T &a, const T &b) {{ return a < b ? b : a; }}
struct Elem_{start:08x} {{ {elem} }};
struct Vec_{start:08x} {{ Elem_{start:08x} *data; unsigned size; unsigned capacity; }};
extern "C" unsigned FUN_{start:08x}(Vec_{start:08x} *v, unsigned n)
{{
    const unsigned max_size = 0xffffffffu / sizeof(Elem_{start:08x});
    unsigned cap = v->capacity;
    if (max_size - cap < n)
        FUN_{lenerr:08x}();
    if (cap < max_size / 3)
        return cap + Max_{start:08x}((cap + 1) * 3 / 5, n);
    if (cap < max_size / 3 * 2)
        return cap + Max_{start:08x}((cap + 1) / 2, n);
    return max_size;
}}
"""


DTOR_PM = """/* HSTMATCH module={module} start=0x{start:08x} end=0x{end:08x} symbol=__dt__10C_{start:08x}Fv */
/* Polymorphic class owning a polymorphic object at +0x{off:x}: delete it through its virtual destructor and
   clear the pointer; the base destructor (0x{base:x}) is out of line (template instance, hsttmpl.py tag {tag}).
   Names provisional. */
class Poly_{start:08x} {{ public: virtual ~Poly_{start:08x}(); }};
class B_{base:08x} {{ public: virtual void key_{base:08x}(); virtual ~B_{base:08x}(); char pad[0x{pad:x}]; }};
class C_{start:08x} : public B_{base:08x} {{
public:
    virtual void key_{start:08x}();
    Poly_{start:08x} *obj;
    virtual ~C_{start:08x}();
}};
C_{start:08x}::~C_{start:08x}()
{{
    if (obj) {{
        delete obj;
        obj = 0;
    }}
}}
"""


DTOR_ARR = """/* HSTMATCH module={module} start=0x{start:08x} end=0x{end:08x} symbol=__dt__10C_{start:08x}Fv */
/* Polymorphic class owning a heap array at +0x{off:x}: free it and clear the pointer; the inline base
   destructor restores the base vtable (template instance, hsttmpl.py tag {tag}). Names provisional. */
class B_{bvt:08x} {{
public:
    virtual void key_{bvt:08x}();
    virtual ~B_{bvt:08x}() {{}}
}};
class C_{start:08x} : public B_{bvt:08x} {{
public:
    virtual void key_{start:08x}();
{pad}    char *buffer;
    virtual ~C_{start:08x}();
}};
C_{start:08x}::~C_{start:08x}() {{ if (buffer) {{ delete[] buffer; buffer = 0; }} }}
"""


def lui_addiu(hi_word: int, lo_word: int) -> int:
    lo = lo_word & 0xFFFF
    return (((hi_word & 0xFFFF) << 16) + (lo - 0x10000 if lo & 0x8000 else lo)) & 0xFFFFFFFF


def masked(ws: list[int]) -> tuple[int, ...]:
    """Instruction shapes with call targets and immediates removed (for template detection)."""
    return tuple((w >> 26) << 26 if w >> 26 in (2, 3) else (w & 0xFFFF0000 if w >> 26 >= 8 else w) for w in ws)


_EXEMPLARS: dict[str, tuple] = {}


def exemplar(name: str, start: int, end: int) -> tuple:
    if name not in _EXEMPLARS:
        data, seg = M.module_bytes({}, "EBOOT.BIN")
        off = seg["file_offset"] + start - seg["start"]
        ws = C.words(data[off:off + end - start])
        _EXEMPLARS[name] = (masked(ws), ws)
    return _EXEMPLARS[name]


def elem_decl(size: int) -> str:
    return f"int w[{size // 4}];" if size % 4 == 0 else f"char b[{size}];"


def first_constant(ws: list[int]) -> int | None:
    """The first lui-built constant (lui, optionally followed by ori/addiu on the same register)."""
    for i, w in enumerate(ws):
        if w >> 26 == 0x0F:
            rt, value = (w >> 16) & 31, (w & 0xFFFF) << 16
            for nxt in ws[i + 1:i + 4]:
                op, rs = nxt >> 26, (nxt >> 21) & 31
                if rs == rt and op == 0x0D:
                    return value | (nxt & 0xFFFF)
                if rs == rt and op == 0x09:
                    imm = nxt & 0xFFFF
                    return (value + (imm - 0x10000 if imm & 0x8000 else imm)) & 0xFFFFFFFF
            return value
    return None


def sizes_for(max_or_next: int) -> list[int]:
    out = []
    for size in range(1, 1025):
        maximum = 0xFFFFFFFF // size
        if maximum in (max_or_next, max_or_next - 1):
            out.append(size)
    return out


def candidates(module: str, tag: str):
    ledger = json.loads((M.CAMPAIGN / "ledger" / "campaign.json").read_text(encoding="utf-8-sig"))
    mod = next(m for m in ledger["modules"] if m["name"] == module)
    data, seg = M.module_bytes({}, module)
    import hstattempts as A
    # a source's own start, a function inside a run source or placed as a helper, and a starter function that an open
    # good first issue holds for its contributor (data/starter-issues.json) are not this tool's to write
    have = C.covered_starts(module, [f["start"] for f in mod["functions"]])
    have |= {s for (m, s) in A.load_starters(M.ROOT / A.STARTER_RELPATH) if m == module}
    for f in mod["functions"]:
        if f.get("boundary_status") != "reviewed" or f["start"] in have or not 64 <= f["end"] - f["start"] <= 320:
            continue
        off = seg["file_offset"] + f["start"] - seg["start"]
        ws = C.words(data[off:off + f["end"] - f["start"]])
        _sig, refs = C.analyse(ws)
        calls = [v for k, _i, v in refs if k == "call"]
        pm_mask, _pm_ws = exemplar("dtor_pm", 0xB801C, 0xB80A4)
        if masked(ws) == pm_mask:
            vt = ((ws[7] & 0xFFFF) << 16) + ((ws[8] & 0xFFFF) - 0x10000 if ws[8] & 0x8000 else ws[8] & 0xFFFF)
            off_member = ws[10] & 0xFFFF
            base_dtor = (ws[21] & 0x03FFFFFF) << 2
            symbols = {f"__vt__10C_{f['start']:08x}": vt & 0xFFFFFFFF, f"__dt__10B_{base_dtor:08x}Fv": base_dtor}
            yield "dtor_pm", 0, symbols, DTOR_PM.format(
                module=module, start=f["start"], end=f["end"], tag=tag, off=off_member, pad=off_member - 4, base=base_dtor)
            continue
        arr_mask, _arr_ws = exemplar("dtor_arr", 0x14D14C, 0x14D1CC)
        if masked(ws) == arr_mask:
            vt, off_member, bvt = lui_addiu(ws[7], ws[8]), ws[10] & 0xFFFF, lui_addiu(ws[18], ws[19])
            if off_member >= 4 and off_member % 4 == 0:
                pad = f"    char pad[0x{off_member - 4:x}];\n" if off_member > 4 else ""
                symbols = {f"__vt__10C_{f['start']:08x}": vt, f"__vt__10B_{bvt:08x}": bvt}
                yield "dtor_arr", 0, symbols, DTOR_ARR.format(
                    module=module, start=f["start"], end=f["end"], tag=tag, off=off_member, bvt=bvt, pad=pad)
            continue
        constant = first_constant(ws)
        # string arguments; the max_size constant built with lui/addiu is a data-looking value too
        strings = [v for k, _i, v in refs if k == "data" and v > 0x100000 and v != constant]
        base = {"module": module, "start": f["start"], "end": f["end"], "tag": tag}
        if constant is None:
            continue
        if len(calls) == 7 and calls[0] == calls[4] and calls[1] == calls[5] and calls[2] == calls[6] and len(strings) >= 2:
            for size in sizes_for(constant):
                yield "reserve", size, {}, RESERVE.format(
                    **base, size=size, elem=elem_decl(size), reent=calls[0], fprintf=calls[1], abort=calls[2],
                    alloc=calls[3], mem_str=strings[1], lenerr_decl="",
                    str_decls=f"extern char DAT_{strings[0]:08x}[];\nextern char DAT_{strings[1]:08x}[];\n",
                    lencheck=(" {\n        FUN_%08x(FUN_%08x()->err, DAT_%08x);\n        FUN_%08x();\n    }"
                              % (calls[1], calls[0], strings[0], calls[2])))
        elif len(calls) == 5 and len(strings) >= 1 and calls[2] != calls[0]:
            # length check through a throwing helper: [throw, alloc, reent, fprintf, abort]
            for size in sizes_for(constant):
                yield "reserve_t", size, {}, RESERVE.format(
                    **base, size=size, elem=elem_decl(size), reent=calls[2], fprintf=calls[3], abort=calls[4],
                    alloc=calls[1], mem_str=strings[0], lenerr_decl=f"void FUN_{calls[0]:08x}(void);\n",
                    str_decls=f"extern char DAT_{strings[0]:08x}[];\n",
                    lencheck=f"\n        FUN_{calls[0]:08x}();")
        elif len(calls) == 1 and 200 <= f["end"] - f["start"] <= 240:
            for size in sizes_for(constant):
                yield "grow", size, {}, GROW.format(**base, size=size, elem=elem_decl(size), lenerr=calls[0])


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--tag", required=True)
    ap.add_argument("--module", default="EBOOT.BIN")
    ap.add_argument("--parallel", type=int, default=12)
    ap.add_argument("--dry-run", action="store_true")
    args = ap.parse_args()
    work = M.ROOT / "try" / f"tmpl-{args.tag}"
    if work.exists():
        raise SystemExit(f"{work} exists; use a new tag")
    files = []
    owned: dict[str, dict[str, int]] = {}
    extra: dict[str, int] = {}
    for kind, size, symbols, text in candidates(args.module, args.tag):
        start = int(text.split("start=0x", 1)[1][:8], 16)
        path = work / kind / f"{kind}_{start:08x}_s{size}.cpp"
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(text, encoding="utf-8", newline="\n")
        files.append(path)
        owned[str(path.resolve())] = symbols
        extra.update(symbols)
    print(f"candidates={len(files)}")
    if args.dry_run or not files:
        return 0
    status = {}
    extra_path = work / "symbols-extra.txt"
    base_symbols = M.load_symbols(args.module)
    extra = {k: v for k, v in extra.items() if base_symbols.get(k, v) == v}
    extra_path.write_text("".join(f"{k} = 0x{v:08x};\n" for k, v in sorted(extra.items())), encoding="utf-8", newline="\n")
    cmd = [sys.executable, str(Path(M.__file__)), "--quiet", "--parallel", str(args.parallel),
           "--symbols-extra", str(extra_path), "--json"]
    for k in range(0, len(files), 300):
        part = work / f"summary-{k // 300}.json"
        subprocess.run(cmd + [str(part)] + [str(p) for p in files[k:k + 300]], stdout=subprocess.DEVNULL)
        for row in json.loads(part.read_text(encoding="utf-8")):
            status[str((M.ROOT / row["source"]).resolve())] = row["status"]
    dest = M.ROOT / "src" / SRC_DIRS[args.module]
    dest.mkdir(parents=True, exist_ok=True)
    done, written = set(), 0
    for path in files:
        start = int(path.stem.split("_")[-2], 16)
        if status.get(str(path.resolve())) != "EXACT" or start in done:
            continue
        kind = path.parent.name
        out = dest / (f"{kind}_{start:08x}.cpp" if kind.startswith("dtor") else f"vec_{kind}_{start:08x}.cpp")
        if out.exists():
            continue
        out.write_text(path.read_text(encoding="utf-8"), encoding="utf-8", newline="\n")
        done.add(start)
        written += 1
        new = {k: v for k, v in owned.get(str(path.resolve()), {}).items() if k not in base_symbols}
        if new:
            with (M.ROOT / "symbols" / f"{args.module}.txt").open("a", encoding="utf-8", newline="\n") as stream:
                for k, v in sorted(new.items()):
                    stream.write(f"{k} = 0x{v:08x};   # hsttmpl {args.tag}\n")
            base_symbols.update(new)
    counts: dict[str, int] = {}
    for value in status.values():
        counts[value] = counts.get(value, 0) + 1
    report = {"tag": args.tag, "module": args.module, "candidates": len(files), "written": written, "status_counts": counts}
    (work / "report.json").write_text(json.dumps(report, indent=1) + "\n", encoding="utf-8")
    print(json.dumps(report))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
