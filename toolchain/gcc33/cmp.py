"""Compare a candidate GCC 3.3.3 build with the Sony kit, function by function, on assembly output (-S).

    python -I toolchain/gcc33/cmp.py --variant stock [--base "-mips2 ..."] [--only gcc171-O2] [--files a.c b.c]
           [--jobs 4] [--out try/fable/g33/round-N.json] [--show 3]

For every source in src/ whose header says cc=gcc131*/gcc171* (the proven SDK GCC objects), both compilers produce
assembly with the profile's flags; the instruction lines (directives that only name the compiler dropped) must be
identical. Equal assembly through the same assembler is equal object code, so this is the acceptance test's
measure. Scratch tool: it writes only its --out file.
"""
import argparse, json, re, shlex, subprocess, sys, tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
HDR = re.compile(r"/\*\s*HSTMATCH\s+(.*?)\*/", re.S)
H = "$HOME/hst-gcc"
SONY_DRV = f"{H}/psp-gcc-1.3.1/bin/psp-gcc"
SONY_B = {"131": "", "171": f"-B {H}/psp-gcc-1.7.1/lib/gcc-lib/psp/3.3.3+allegrex-2.2.2-psp-1.7.1/"}
DROP = re.compile(r"^\s*(\.ident|\.file|#|$)")


def to_wsl(p: Path) -> str:
    s = str(p.resolve()).replace("\\", "/")
    return f"/mnt/{s[0].lower()}{s[2:]}"


def header(text: str) -> dict:
    m = HDR.search(text[:2000])
    if not m:
        return {}
    out = {}
    for tok in shlex.split(m.group(1).replace("\n", " ")):
        if "=" in tok:
            k, v = tok.split("=", 1)
            out[k] = v
    return out


def sources(only, files):
    paths = [Path(f) for f in files] if files else sorted(p for p in (ROOT / "src").rglob("*")
                                                              if p.suffix in (".c", ".cpp"))
    for p in paths:
        h = header(p.read_text(encoding="utf-8", errors="replace"))
        cc = h.get("cc", "")
        m = re.fullmatch(r"gcc(131|171)(x?)-(O[0-3s])", cc)
        if not m or (only and cc not in only):
            continue
        yield p, h, m.group(1), m.group(2), m.group(3)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--variant", required=True)
    ap.add_argument("--base", default="")
    ap.add_argument("--ext-flag", default="-mext", help="candidate flag for the x profiles")
    ap.add_argument("--only", nargs="*")
    ap.add_argument("--files", nargs="*")
    ap.add_argument("--jobs", type=int, default=4)
    ap.add_argument("--out")
    ap.add_argument("--show", type=int, default=0)
    a = ap.parse_args()
    cand = f"$HOME/hst-gcc33/{a.variant}/bin/psp-gcc"
    items = list(sources(a.only, a.files))
    work = Path(tempfile.mkdtemp(prefix="g33cmp.", dir=ROOT / "build"))
    cmds = []
    for i, (p, h, v, x, o) in enumerate(items):
        lang = "-x c++" if p.suffix == ".cpp" else "-x c"
        extra = h.get("extra", "")
        fl = f"-{o} -G0 -fno-merge-constants"
        w = to_wsl(work)
        name = shlex.quote(p.name)
        cmds.append(
            f"t=$(mktemp -d /tmp/g33.XXXXXX) && cp {shlex.quote(to_wsl(p))} $t/ && cd $t && "
            f"{SONY_DRV} {SONY_B[v]} -S {fl}{' -mext' if x else ''} {lang} {extra} -o s.s {name} > /dev/null 2>&1; "
            f"{cand} -S {a.base} {fl}{' ' + a.ext_flag if x else ''} {lang} {extra} -o c.s {name} > c.log 2>&1; "
            f"cp s.s {w}/{i}.s.s 2>/dev/null; cp c.s {w}/{i}.c.s 2>/dev/null; cp c.log {w}/{i}.c.log; "
            f"cd / && rm -rf $t")
    jf = work / "jobs"
    jf.write_text("\n".join(cmds) + "\n", encoding="utf-8", newline="\n")
    subprocess.run(["wsl.exe", "-e", "sh", "-c",
                    f"tr '\\n' '\\0' < {shlex.quote(to_wsl(jf))} | xargs -0 -P {a.jobs} -n 1 sh -c"], check=False)
    res = {"same": [], "diff": [], "fail": []}
    tally = {}
    shown = 0
    for i, (p, h, v, x, o) in enumerate(items):
        cc = h["cc"]
        t = tally.setdefault(cc, [0, 0, 0])
        s, c = work / f"{i}.s.s", work / f"{i}.c.s"
        rel = p.relative_to(ROOT).as_posix()
        if not s.exists() or not c.exists():
            res["fail"].append(rel); t[2] += 1
            if a.show and shown < a.show and (work / f"{i}.c.log").exists():
                print("FAIL", rel, (work / f"{i}.c.log").read_text(errors="replace")[:600]); shown += 1
            continue
        ls = [l.rstrip() for l in s.read_text(errors="replace").splitlines() if not DROP.match(l)]
        lc = [l.rstrip() for l in c.read_text(errors="replace").splitlines() if not DROP.match(l)]
        if ls == lc:
            res["same"].append(rel); t[0] += 1
        else:
            res["diff"].append(rel); t[1] += 1
            if a.show and shown < a.show:
                import difflib
                print("DIFF", rel, cc)
                print("\n".join(list(difflib.unified_diff(ls, lc, "sony", "cand", n=1, lineterm=""))[:40]))
                shown += 1
    for k in sorted(tally):
        print(f"{k:14} same {tally[k][0]:5}  diff {tally[k][1]:5}  fail {tally[k][2]:5}")
    print(f"total same {len(res['same'])} diff {len(res['diff'])} fail {len(res['fail'])} of {len(items)}")
    if a.out:
        Path(a.out).write_text(json.dumps({"variant": a.variant, "base": a.base, "tally": tally, **res}, indent=1))
    for f in work.iterdir():
        f.unlink()
    work.rmdir()


if __name__ == "__main__":
    main()
