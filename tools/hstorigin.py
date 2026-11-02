#!/usr/bin/env python3
"""Classify every reconstructed source by where its code came from, and write the origin manifest.

    python tools/hstorigin.py [--out ORIGINS.json] [--check] [--list CLASS]

Every source under src/ (and the near misses under wip/) gets exactly one origin class:

  game              compiled with Metrowerks mwccpsp (a header without `cc=`, or cc=mwcc219) and not a runtime
                    helper: the game's own code
  newlib            a newlib library function (the note says `Library source: newlib ...` or `Identified as
                    newlib ...`), compiled with psp-gcc; its terms are newlib's own (third_party/newlib/)
  mw-runtime        compiled with mwccpsp but the symbol is a Metrowerks runtime name (MW_RUNTIME below, for
                    example __ptmf_scall or __destroy_arr/__construct_array)
  sony-prx          code of a Sony system-library module: sources under src/libfont, src/psmfp, src/psmf and
                    src/sharing_font (SONY_PRX_DIRS)
  sony-middleware-sgx
                    a function of the SGX sound library, Sony middleware linked into EBOOT.BIN and the Sharing module
                    as one block (SGX_BLOCKS): treated like the Sony SDK (owner decision 2026-11-01), whatever compiler
                    profile matches it (a few functions of 16-31 bytes match with mwccpsp too, where both compilers give
                    the same code)
  gcc-unattributed  compiled with psp-gcc (cc=gcc*) in EBOOT.BIN or the Sharing module and not newlib: Sony SDK
                    code until someone attributes it

Order of the rules: a newlib note first, then the sony-prx directories, then the SGX block, then the compiler profile
(gcc -> gcc-unattributed), then the runtime-name check, else game. A clone (`Clone of src/... (hstclone.py ...)`)
inherits the class of its exemplar, followed through chains of clones, except for the SGX class, which is a class
of place: a clone inside the SGX block is SGX, and a clone of an SGX function outside the block takes its own class.

SGX_BLOCKS. From the first function to the end of the last function of the SGX units tools/hsttu.py recovers
(sgxbmg.c, sgxconf.c, sgxport.c, sgxpsp.c, sgxres.c, sgxseq.c, sgxsnd.c, sgxsndnote.c, sgxsys.c, sgxvo.c; their
assertions name them): 592 functions in each module, all psp-gcc code, between the game's mwccpsp C++ units. The
library's strings are SAS, AT3, AT3+ and PCM settings, wave, sequence and BMG names, and "SGX system
initialize/terminate"; it holds no game logic.

The result is written to ORIGINS.json (a JSON object, path relative to the repository root -> class, sorted by
path) and a table of counts, source bytes and retail code bytes per class is printed. tools/hstpublish.py reads the
classes to decide what may leave the private repository.

    --check       compare ORIGINS.json with a fresh classification instead of writing it (exit 1 when stale)
    --list CLASS  print the paths of one class
"""
from __future__ import annotations

import argparse
import json
import re
import sys
from collections import defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hstmatch as M

ROOT = M.ROOT
CLASSES = ("game", "newlib", "mw-runtime", "sony-prx", "sony-middleware-sgx", "gcc-unattributed")
SGX = "sony-middleware-sgx"
# [start, end) of the SGX sound library's block per module (see SGX_BLOCKS above; tools/test_hstorigin.py checks
# them against data/translation-units.json)
SGX_BLOCKS = {"EBOOT.BIN": (0x0006F10C, 0x00088128), "hstp_trans_main.prx": (0x000708CC, 0x000898E8)}
SOURCE_TREES = ("src", "wip")
SOURCE_SUFFIXES = (".c", ".cpp")
# src/<dir> directories built from Sony system-library modules (hstnewlib.SRC_DIRS names the modules)
SONY_PRX_DIRS = frozenset({"libfont", "psmfp", "psmf", "sharing_font"})
# Names of the Metrowerks C++ runtime support functions (pointer-to-member-function helpers, array
# construction/destruction, global object registration). Compiler-generated __ct__/__dt__ member names
# of the game's classes are NOT runtime and stay `game`.
MW_RUNTIME = re.compile(r"^(?:__ptmf_\w+|__destroy_\w+|__construct_\w+|__register_global_object|"
                        r"__destroy_global_chain|__init_cpp_exceptions|__fini_cpp_exceptions)$")
NEWLIB_NOTE = re.compile(r"Library source: newlib|Identified as newlib")
CLONE_NOTE = re.compile(r"^/\* Clone of (src/\S+) \(hstclone\.py tag \w+\)")


def sources(root: Path = ROOT) -> list[Path]:
    return sorted(p for tree in SOURCE_TREES if (root / tree).is_dir()
                  for p in (root / tree).rglob("*") if p.suffix in SOURCE_SUFFIXES)


def rel(path: Path, root: Path = ROOT) -> str:
    return path.relative_to(root).as_posix()


def own_class(path: Path, root: Path = ROOT) -> str:
    """Class from the file's own header and note (clones are resolved by the caller)."""
    head = path.read_text(encoding="utf-8", errors="replace")[:3000]
    parts = rel(path, root).split("/")
    if NEWLIB_NOTE.search(head):
        return "newlib"
    if parts[0] == "src" and len(parts) > 2 and parts[1] in SONY_PRX_DIRS:
        return "sony-prx"
    header = M.parse_header(path)
    if in_sgx_block(header):
        return SGX
    cc = header.get("cc", "mwcc219")
    if cc.startswith("gcc"):
        return "gcc-unattributed"
    return "mw-runtime" if MW_RUNTIME.match(header["symbol"]) else "game"


def in_sgx_block(header: dict) -> bool:
    block = SGX_BLOCKS.get(header["module"])
    return bool(block) and block[0] <= header["start"] < block[1]


def classify(root: Path = ROOT) -> tuple[dict[str, str], list[str]]:
    """({path: class}, warnings) for every source under src/ and wip/. SGX is a class of place: a clone inside the
    SGX block is SGX whatever its exemplar, and a clone outside it of an SGX function takes its own class."""
    files = sources(root)
    exemplar_of: dict[str, str] = {}
    result: dict[str, str] = {}
    warnings: list[str] = []
    for path in files:
        key = rel(path, root)
        first = path.read_text(encoding="utf-8", errors="replace").split("\n", 2)
        m = CLONE_NOTE.match(first[1]) if len(first) > 1 else None
        if m:
            exemplar_of[key] = m.group(1)
        else:
            result[key] = own_class(path, root)
    for path in files:
        key = rel(path, root)
        if key in result:
            continue
        seen, cur = [key], exemplar_of[key]
        while cur in exemplar_of and cur not in seen:   # clone of a clone
            seen.append(cur)
            cur = exemplar_of[cur]
        if in_sgx_block(M.parse_header(path)):
            result[key] = SGX
        elif cur in result:
            result[key] = result[cur] if result[cur] != SGX else own_class(path, root)
        else:
            warnings.append(f"{key}: exemplar {cur} is not a source here; classified from the clone's own header")
            result[key] = own_class(path, root)
    return dict(sorted(result.items())), warnings


def summarize(classes: dict[str, str], root: Path = ROOT) -> dict[str, dict[str, int]]:
    out = {c: {"sources": 0, "source_bytes": 0, "code_bytes": 0} for c in CLASSES}
    for key, cls in classes.items():
        path = root / key
        header = M.parse_header(path)
        row = out[cls]
        row["sources"] += 1
        row["source_bytes"] += path.stat().st_size
        row["code_bytes"] += header["end"] - header["start"]
    return out


def load_manifest(path: Path) -> dict[str, str]:
    return json.loads(path.read_text(encoding="utf-8"))


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--out", type=Path, default=ROOT / "ORIGINS.json", help="manifest path (default ORIGINS.json)")
    ap.add_argument("--check", action="store_true", help="compare the manifest with a fresh classification")
    ap.add_argument("--list", choices=CLASSES, metavar="CLASS", help="print the paths of one class")
    args = ap.parse_args()
    classes, warnings = classify()
    for w in warnings:
        print(f"warning: {w}", file=sys.stderr)
    status = 0
    if args.check:
        old = load_manifest(args.out) if args.out.exists() else {}
        if old != classes:
            changed = sorted(k for k in set(old) | set(classes) if old.get(k) != classes.get(k))
            print(f"{args.out.name} is stale: {len(changed)} paths differ (first: {', '.join(changed[:3])})")
            status = 1
        else:
            print(f"{args.out.name} is current")
    else:
        args.out.write_text(json.dumps(classes, indent=0, sort_keys=True) + "\n", encoding="utf-8", newline="\n")
        print(f"wrote {args.out} ({len(classes)} paths)")
    if args.list:
        for key, cls in classes.items():
            if cls == args.list:
                print(key)
        return status
    table = summarize(classes)
    print(f"{'class':18s} {'sources':>8s} {'source bytes':>13s} {'retail code bytes':>18s}")
    for cls in CLASSES:
        row = table[cls]
        print(f"{cls:18s} {row['sources']:8d} {row['source_bytes']:13d} {row['code_bytes']:18d}")
    total = {k: sum(table[c][k] for c in CLASSES) for k in ("sources", "source_bytes", "code_bytes")}
    print(f"{'total':18s} {total['sources']:8d} {total['source_bytes']:13d} {total['code_bytes']:18d}")
    for tree in SOURCE_TREES:
        n = sum(1 for k in classes if k.startswith(tree + "/"))
        print(f"  {tree}/: {n} sources")
    return status


if __name__ == "__main__":
    raise SystemExit(main())
