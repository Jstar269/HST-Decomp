#!/usr/bin/env python3
"""Compile one scratch C/C++ file with the retail MWCC profile and print its disassembly (no header, no link).

    python tools/hstcc.py probe.cpp [--lang c99|c++] [--build 219] [--extra "-opt nopeephole"]

For quick probes (does this spelling fold? which vtable offset does this virtual call use?) where hstmatch.py
would need an HSTMATCH header and a retail address. The relocations are printed, so symbol references read as
`DAT_xxx` against each lui/lo pair instead of linked numbers. Builds other than 219 are for experiments only:
no source may depend on one. Adopted from the ds10 lane's asmdump.sh.
"""
from __future__ import annotations

import argparse
import shlex
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hstmatch as M


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("source", type=Path)
    ap.add_argument("--lang", choices=("c99", "c++"), help="default: c++ for .cpp, c99 otherwise")
    ap.add_argument("--build", default=M.PROFILE["build"], help="mwccpsp build under the native kit (default 219)")
    ap.add_argument("--extra", default="", help="extra compiler flags")
    args = ap.parse_args()
    src = args.source.resolve()
    lang = args.lang or ("c++" if src.suffix == ".cpp" else "c99")
    out = M.ROOT / "build" / "hstcc"
    out.mkdir(parents=True, exist_ok=True)
    obj_path = out / f"{src.stem}.o"
    obj_path.unlink(missing_ok=True)       # an object left by an earlier probe must not pass for this one
    obj = shlex.quote(M.to_wsl(obj_path))
    M.run_wsl([f"cd {shlex.quote(M.to_wsl(src.parent))} && MWCIncludes={shlex.quote(M.to_wsl(M.ROOT / 'include'))} "
               f"$N/wibo $N/builds/{args.build}/mwccpsp.exe {M.PROFILE['flags']} -lang {lang} {args.extra} "
               f"-o {obj} {shlex.quote(src.name)} && {M.pspdev_bin()}/psp-objdump -dr {obj} | tail -n +6"],
              "hstcc", M.native_dir())
    if not obj_path.exists():
        print(f"hstcc: {args.source} did not compile (the compiler's messages are above)", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
