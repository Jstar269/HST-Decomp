#!/usr/bin/env python3
"""Licensing bookkeeping: REUSE coverage check and the newlib annotation block.

    python tools/hstreuse.py check            # every tracked file is covered by exactly one effective annotation
    python tools/hstreuse.py newlib           # print the generated newlib annotations and the source table
    python tools/hstreuse.py newlib --write   # rewrite the generated parts of REUSE.toml and third_party/newlib/README.md

The repository follows the REUSE specification (3.x): REUSE.toml maps path globs to SPDX-License-Identifier and
SPDX-FileCopyrightText, the licence texts are in LICENSES/. This tool is the project's own check, for machines that
do not have the `reuse` tool installed. `check` does what `reuse lint` is meant to establish:

  * every file git tracks matches at least one [[annotations]] path glob of REUSE.toml, and the LAST matching
    annotation is the effective one (the order of the file decides, so the newlib annotations come after the
    generic src/** one and carry precedence = "override");
  * every licence named by an effective annotation has a text in LICENSES/<id>.txt (warns about unused texts);
  * the generated newlib block of REUSE.toml and the table in third_party/newlib/README.md are current, and the
    paths they cover are exactly the sources tools/hstorigin.py classifies as `newlib`.

Files REUSE itself does not annotate are skipped as the tool does: LICENSES/, licence files named LICENSE* or
COPYING* and *.license files.

The newlib annotations are derived from the sources themselves: each newlib-derived source in src/ is the upstream
file preprocessed with its comments kept, so the notices that appear in the file (from the upstream file and from
the newlib headers it includes) say which licences apply. A source whose upstream file has no notice of its own
(NO_OWN_NOTICE) falls under newlib's default copyright, "Red Hat Incorporated" in COPYING.NEWLIB. A source adapted
by hand (its note names no release) takes the notices of its upstream file from ADAPTED_NOTICES, which
`python tools/hstreuse.py adapted` derives from the newlib trees; a clone without a note of its own takes its
exemplar's.
"""
from __future__ import annotations

import argparse
import os
import re
import subprocess
import sys
import tomllib
from collections import defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hstorigin

ROOT = hstorigin.ROOT
PROJECT_COPYRIGHT = "2026 Jstar269 and contributors"

# sha256 of the newlib release tarballs the sources were reconstructed from (PROVENANCE.json beside them).
TARBALLS = {
    "1.13.0": "c8562ef73ced13940829faa5f6a8ce4fd1d44e45388b3043ce58baedf03830d9",
    "1.14.0": "7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23",
    "1.15.0": "c4496102d38c59d1a47ddd5481af35caa1f65b76e2a94d9607737e17fd9e4465",
}
# Upstream files that carry no copyright or licence comment of their own (found by scanning each upstream file
# of the tarballs for "copyright", "permission to use" and "redistribution"). They fall under the default
# copyright of newlib's COPYING.NEWLIB: Red Hat Incorporated.
NO_OWN_NOTICE = {
    ("1.13.0", "libc/string/strcasecmp.c"),
    ("1.14.0", "libc/search/bsearch.c"), ("1.14.0", "libc/stdlib/abort.c"), ("1.14.0", "libc/stdlib/mbrtowc.c"),
    ("1.14.0", "libc/stdlib/mbtowc_r.c"), ("1.14.0", "libc/stdlib/wcrtomb.c"), ("1.14.0", "libc/stdlib/wcsrtombs.c"),
    ("1.14.0", "libc/string/memchr.c"), ("1.14.0", "libc/string/memcmp.c"), ("1.14.0", "libc/string/strcat.c"),
    ("1.14.0", "libc/string/strchr.c"), ("1.14.0", "libc/string/strcpy.c"), ("1.14.0", "libc/string/strncmp.c"),
    ("1.14.0", "libc/string/strrchr.c"),
}
RED_HAT_DEFAULT = {   # the default copyright of COPYING.NEWLIB in each release
    "1.13.0": "Copyright (c) 1994, 1997, 2001, 2002, 2003, 2004 Red Hat Incorporated",
    "1.14.0": "Copyright (c) 1994, 1997, 2001, 2002, 2003, 2004, 2005 Red Hat Incorporated",
}
# The identifiers in LICENSES/ for the newlib notices, in the order they are written in an expression.
ID_ORDER = ["LicenseRef-Newlib-UCB-BSD-4-Clause", "LicenseRef-Newlib-UCB-BSD-3-Clause",
            "LicenseRef-Newlib-UCB-Advertising-Notice", "LicenseRef-Newlib-Gay-AT-T", "LicenseRef-Newlib-Sun-fdlibm",
            "LicenseRef-Newlib-Red-Hat", "LicenseRef-Newlib-OAR", "LicenseRef-Newlib-Birgmeier"]
GPL = "GPL-3.0-or-later"

LIB_NOTE = re.compile(r"Library source: newlib (?P<ver>\d+\.\d+\.\d+) (?P<dir>\w+)/(?P<file>\w+)\.c function (?P<fn>\w+)")
# Hand-adapted sources name no release: "newlib libm <file>.c" or "newlib libc <directory> <file>.c".
ADAPTED_NOTE = re.compile(r"Library source: newlib (?P<dir>libm|libc \w+) (?P<file>\w+)\.c function (?P<fn>\w+)")
IDENTIFIED_NOTE = re.compile(r"Identified as newlib (?P<dir>libm) (?P<file>\w+)\.c \((?P<fn>\w+)")
NEWLIB_RELEASES = ("1.13.0", "1.14.0", "1.15.0")
RED_HAT_DEFAULT_1_15 = "Copyright (c) 1994-2006 Red Hat, Inc."   # COPYING.NEWLIB of 1.15.0, notice (1)
# The notices of the upstream file of each hand-adapted source. These sources were adapted by hand without
# recording the newlib release and most do not repeat the upstream notice, so the notice comes from the upstream
# file itself, read in all three releases: the licence is the same in each, and the copyright statements are the
# union over the releases (fflush.c added a year in 1.15.0; s_isinf.c and s_isnan.c moved from libm/math to
# libm/common). A file with no notice of its own falls under the default copyright of COPYING.NEWLIB.
# Regenerate with `python tools/hstreuse.py adapted` (needs the newlib_home trees) when a new kind is added.
# key: the note's "<dir>/<file>" -> (upstream file, licence ids, copyright statements)
ADAPTED_NOTICES = {
    "libc/locale/locale": ("libc/locale/locale.c", ("LicenseRef-Newlib-Red-Hat",),
                           (RED_HAT_DEFAULT_1_15, *RED_HAT_DEFAULT.values())),
    "libc/stdio/fflush": ("libc/stdio/fflush.c", ("LicenseRef-Newlib-UCB-Advertising-Notice",),
                          ("Copyright (c) 1990 The Regents of the University of California",
                           "Copyright (c) 1990, 2006 The Regents of the University of California")),
    "libc/stdio/findfp": ("libc/stdio/findfp.c", ("LicenseRef-Newlib-UCB-Advertising-Notice",),
                          ("Copyright (c) 1990 The Regents of the University of California",)),
    "libc/stdio/fwalk": ("libc/stdio/fwalk.c", ("LicenseRef-Newlib-UCB-Advertising-Notice",),
                         ("Copyright (c) 1990 The Regents of the University of California",)),
    "libc/stdio/printf": ("libc/stdio/printf.c", ("LicenseRef-Newlib-UCB-Advertising-Notice",),
                          ("Copyright (c) 1990 The Regents of the University of California",)),
    "libc/stdio/sprintf": ("libc/stdio/sprintf.c", ("LicenseRef-Newlib-UCB-Advertising-Notice",),
                           ("Copyright (c) 1990 The Regents of the University of California",)),
    "libc/stdio/sscanf": ("libc/stdio/sscanf.c", ("LicenseRef-Newlib-UCB-Advertising-Notice",),
                          ("Copyright (c) 1990 The Regents of the University of California",)),
    "libc/stdio/vsprintf": ("libc/stdio/vsprintf.c", ("LicenseRef-Newlib-UCB-Advertising-Notice",),
                            ("Copyright (c) 1990 The Regents of the University of California",)),
    "libc/stdlib/rand": ("libc/stdlib/rand.c", ("LicenseRef-Newlib-Red-Hat",),
                         (RED_HAT_DEFAULT_1_15, *RED_HAT_DEFAULT.values())),
    "libc/stdlib/strtod": ("libc/stdlib/strtod.c", ("LicenseRef-Newlib-Gay-AT-T",), ("Copyright (c) 1991 by AT&T",)),
    **{f"libm/{f}": (f"libm/math/{f}.c", ("LicenseRef-Newlib-Sun-fdlibm",),
                     ("Copyright (C) 1993 by Sun Microsystems, Inc.",))
       for f in ("ef_pow", "kf_cos", "kf_rem_pio2", "s_cos", "s_fabs", "s_sin", "sf_cos", "sf_fabs", "sf_sin",
                 "sf_tan")},
    "libm/s_finite": ("libm/common/s_finite.c", ("LicenseRef-Newlib-Sun-fdlibm",),
                      ("Copyright (C) 1993 by Sun Microsystems, Inc.",)),
    "libm/s_isinf": ("libm/common/s_isinf.c", ("LicenseRef-Newlib-Red-Hat",),
                     (RED_HAT_DEFAULT_1_15, *RED_HAT_DEFAULT.values())),
    "libm/s_isnan": ("libm/common/s_isnan.c", ("LicenseRef-Newlib-Sun-fdlibm",),
                     ("Copyright (C) 1993 by Sun Microsystems, Inc.",)),
}
CLONE_NOTE = re.compile(r"^/\* Clone of (src/\S+) \(hstclone\.py tag \w+\)")
COMMENT = re.compile(r"/\*.*?\*/", re.S)

BEGIN_TOML, END_TOML = "# BEGIN generated newlib annotations", "# END generated newlib annotations"
BEGIN_MD, END_MD = "<!-- BEGIN generated source table -->", "<!-- END generated source table -->"
README = ROOT / "third_party" / "newlib" / "README.md"
REUSE_TOML = ROOT / "REUSE.toml"
LICENSES = ROOT / "LICENSES"


# ---- notices -----------------------------------------------------------------------------------------------------
def families(text: str) -> dict[str, set[str]]:
    """{licence id: {copyright statements}} for the newlib notices found in the comments of a source."""
    found: dict[str, set[str]] = defaultdict(set)
    for comment in COMMENT.findall(text):
        flat = re.sub(r"\s*\n\s*\*?\s*", " ", comment)
        if "Martin Birgmeier" in flat:
            found["LicenseRef-Newlib-Birgmeier"].add("Copyright (c) 1993 Martin Birgmeier")
        elif "On-Line Applications Research" in flat:
            found["LicenseRef-Newlib-OAR"].add("Copyright (c) 1989-2000 On-Line Applications Research Corporation (OAR)")
        elif "David M. Gay" in flat:
            found["LicenseRef-Newlib-Gay-AT-T"].add("Copyright (c) 1991 by AT&T")
        elif "Sun Microsystems" in flat:
            found["LicenseRef-Newlib-Sun-fdlibm"].add("Copyright (C) 1993 by Sun Microsystems, Inc.")
        elif "Regents of the University of California" in flat and "Copyright" in flat:
            years = re.search(r"Copyright \(c\) ([\d, ]+?)\s+(?:The )?(Regents of the University of California)", flat)
            if years is None:
                continue
            holder = f"Copyright (c) {years.group(1)} " + ("The " if "The Regents" in flat else "") + years.group(2)
            if "All advertising materials mentioning features or use of this software" in flat:
                family = "LicenseRef-Newlib-UCB-BSD-4-Clause"
            elif "Neither the name of the University nor the names of its contributors" in flat:
                family = "LicenseRef-Newlib-UCB-BSD-3-Clause"
            elif "provided that the above copyright notice and this paragraph are" in flat:
                family = "LicenseRef-Newlib-UCB-Advertising-Notice"
            else:
                raise SystemExit(f"unrecognised University of California notice: {flat[:200]}")
            found[family].add(holder)
    return found


class Entry:
    def __init__(self, path: Path):
        self.rel = hstorigin.rel(path)
        self.text = path.read_text(encoding="utf-8", errors="replace")
        lines = self.text.split("\n", 3)
        self.kind, self.exemplar = "direct", None
        clone = CLONE_NOTE.match(lines[1])
        if clone:
            self.kind, self.exemplar = "clone", clone.group(1)
        head = "\n".join(lines[:3])
        adapted = None
        m = LIB_NOTE.search(head)
        if m:
            self.ver, self.function = m["ver"], m["fn"]
            self.upstream = m["dir"].replace("_", "/", 1) + "/" + m["file"] + ".c"
        else:
            m = ADAPTED_NOTE.search(head) or IDENTIFIED_NOTE.search(self.text[:3000])
            if m is None and clone:
                # a clone made before its exemplar carried the library note: the exemplar's note decides
                exemplar = ROOT / clone.group(1)
                if not exemplar.is_file():
                    raise SystemExit(f"{self.rel}: cannot read the newlib note, and its exemplar {clone.group(1)} "
                                     "is not a source here")
                source = Entry(exemplar)
                self.ver, self.function, self.upstream = source.ver, source.function, source.upstream
                self.ids, self.copyrights, self.in_src = source.ids, source.copyrights, self.rel.startswith("src/")
                return
            if m is None:
                raise SystemExit(f"{self.rel}: cannot read the newlib note")
            key = m["dir"].replace(" ", "/") + "/" + m["file"]
            if key not in ADAPTED_NOTICES:
                raise SystemExit(f"{self.rel}: hand-adapted newlib source {key}.c has no ADAPTED_NOTICES entry "
                                 "(run: hstreuse.py adapted)")
            adapted = ADAPTED_NOTICES[key]
            self.ver, self.function, self.upstream = None, m["fn"], adapted[0]
            if not clone:
                self.kind = "hand-adapted" if self.rel.startswith("src/") else "near miss"
        found = families(self.text)
        copyrights = {c for cs in found.values() for c in cs}
        ids = set(found)
        if adapted:
            ids |= set(adapted[1])
            copyrights |= set(adapted[2])
        if self.ver and (self.ver, self.upstream) in NO_OWN_NOTICE:
            ids.add("LicenseRef-Newlib-Red-Hat")
            copyrights.add(RED_HAT_DEFAULT[self.ver])
        self.ids = [i for i in ID_ORDER if i in ids]
        self.copyrights = sorted(copyrights)
        if set(ids) - set(ID_ORDER) or not self.ids:
            raise SystemExit(f"{self.rel}: no recognised newlib notice")
        self.in_src = self.rel.startswith("src/")

    @property
    def expression(self) -> str:
        return " AND ".join(self.ids + ([] if self.in_src else [GPL]))

    @property
    def statements(self) -> list[str]:
        return self.copyrights + ([] if self.in_src else [PROJECT_COPYRIGHT])


def newlib_entries() -> list[Entry]:
    classes, _warnings = hstorigin.classify()
    return [Entry(ROOT / p) for p, c in classes.items() if c == "newlib"]


def render_toml(entries: list[Entry]) -> str:
    groups: dict[tuple, list[str]] = defaultdict(list)
    for e in entries:
        groups[(e.expression, tuple(e.statements))].append(e.rel)
    out = [BEGIN_TOML + " (tools/hstreuse.py newlib --write); do not edit by hand",
           "# One annotation per distinct set of notices. The files are the upstream newlib files preprocessed with",
           "# their comments kept, so the licences of the notices inside them (and of the newlib headers they include)",
           "# apply; precedence = \"override\" makes these win over the generic src/** annotation above."]
    for (expression, statements), paths in sorted(groups.items()):
        out += ["", "[[annotations]]", "path = ["] + [f'  "{p}",' for p in sorted(paths)] + ["]"]
        out += ['precedence = "override"', "SPDX-FileCopyrightText = ["] + [f'  "{s}",' for s in statements] + ["]"]
        out += [f'SPDX-License-Identifier = "{expression}"']
    return "\n".join(out) + "\n" + END_TOML + "\n"


def render_table(entries: list[Entry]) -> str:
    rows = ["| Source | Kind | Newlib | Upstream file | Function | Terms (LICENSES/) | Tarball SHA-256 |",
            "|---|---|---|---|---|---|---|"]
    for e in sorted(entries, key=lambda e: e.rel):
        if e.ver:
            version, sha = e.ver, TARBALLS[e.ver]
        else:   # hand-adapted: release not recorded; the notices are those of the file in all three (ADAPTED_NOTICES)
            version, sha = "not recorded (notices of 1.13.0, 1.14.0, 1.15.0)", "all three"
        terms = ", ".join(i.removeprefix("LicenseRef-Newlib-") for i in e.ids)
        rows.append(f"| `{e.rel}` | {e.kind} | {version} | `newlib/{e.upstream}` | `{e.function}` | {terms} | "
                    f"{'`' + sha + '`' if sha != 'all three' else sha} |")
    return "\n".join(rows) + "\n"


def splice(text: str, begin: str, end: str, block: str) -> str:
    i, j = text.index(begin), text.index(end) + len(end)
    return text[:i] + block.rstrip("\n") + text[j:]


def newlib_command(write: bool) -> int:
    entries = newlib_entries()
    toml, table = render_toml(entries), render_table(entries)
    if not write:
        print(toml)
        print(table)
        return 0
    REUSE_TOML.write_text(splice(REUSE_TOML.read_text(encoding="utf-8"), BEGIN_TOML, END_TOML, toml),
                          encoding="utf-8", newline="\n")
    README.write_text(splice(README.read_text(encoding="utf-8"), BEGIN_MD, END_MD,
                             BEGIN_MD + "\n\n" + table + "\n" + END_MD), encoding="utf-8", newline="\n")
    print(f"wrote {len(entries)} newlib-derived sources into REUSE.toml and {README.relative_to(ROOT).as_posix()}")
    return 0


# ---- coverage check ----------------------------------------------------------------------------------------------
def glob_regex(pattern: str) -> re.Pattern:
    """REUSE.toml path globs: `*` matches anything but `/`, `**` matches anything including `/`."""
    out, i = [], 0
    while i < len(pattern):
        if pattern.startswith("**", i):
            out.append(".*")
            i += 2
        elif pattern[i] == "*":
            out.append("[^/]*")
            i += 1
        elif pattern[i] == "\\" and i + 1 < len(pattern):
            out.append(re.escape(pattern[i + 1]))
            i += 2
        else:
            out.append(re.escape(pattern[i]))
            i += 1
    return re.compile("".join(out) + r"\Z")


IGNORED_NAMES = (re.compile(r"^LICEN[CS]E([-.].*)?$"), re.compile(r"^COPYING([-.].*)?$"), re.compile(r".*\.license$"))


def reuse_ignored(path: str) -> bool:
    return path.startswith("LICENSES/") or any(p.match(path.rsplit("/", 1)[-1]) for p in IGNORED_NAMES)


def tracked_files() -> list[str]:
    out = subprocess.run(["git", "ls-files", "-z"], cwd=ROOT, check=True, capture_output=True,
                         env={**os.environ, "GIT_TERMINAL_PROMPT": "0"}).stdout
    return [p.decode("utf-8") for p in out.split(b"\0") if p]


def license_ids(expression: str) -> set[str]:
    return {t for t in re.split(r"[\s()]+", expression) if t and t not in ("AND", "OR", "WITH")}


def check_command() -> int:
    problems: list[str] = []
    config = tomllib.loads(REUSE_TOML.read_text(encoding="utf-8"))
    annotations = config.get("annotations", [])
    compiled = []
    for n, a in enumerate(annotations):
        paths = [a["path"]] if isinstance(a["path"], str) else a["path"]
        for key in ("SPDX-License-Identifier", "SPDX-FileCopyrightText"):
            if not a.get(key):
                problems.append(f"annotation #{n + 1} ({paths[0]}...) lacks {key}")
        compiled.append([glob_regex(p) for p in paths])
    files = tracked_files()
    effective: dict[str, int] = {}
    uncovered, skipped = [], 0
    for path in files:
        if reuse_ignored(path):
            skipped += 1
            continue
        matches = [n for n, pats in enumerate(compiled) if any(p.match(path) for p in pats)]
        if not matches:
            uncovered.append(path)
        else:
            effective[path] = matches[-1]
    if uncovered:
        problems.append(f"{len(uncovered)} tracked files are not covered by any annotation (first: {uncovered[0]})")
    used = set()
    for n in set(effective.values()):
        used |= license_ids(annotations[n]["SPDX-License-Identifier"])
    for lid in sorted(used):
        if not (LICENSES / f"{lid}.txt").is_file():
            problems.append(f"licence {lid} is used but LICENSES/{lid}.txt is missing")
    present = {p.stem for p in LICENSES.glob("*.txt")}
    for lid in sorted(present - used):
        print(f"warning: LICENSES/{lid}.txt is not used by any effective annotation")
    # the newlib block must be current and cover exactly the sources hstorigin calls newlib
    entries = newlib_entries()
    toml_text = REUSE_TOML.read_text(encoding="utf-8")
    if BEGIN_TOML not in toml_text or splice(toml_text, BEGIN_TOML, END_TOML, render_toml(entries)) != toml_text:
        problems.append("the generated newlib block of REUSE.toml is stale (run: hstreuse.py newlib --write)")
    readme_text = README.read_text(encoding="utf-8") if README.exists() else ""
    if BEGIN_MD not in readme_text or splice(readme_text, BEGIN_MD, END_MD,
                                             BEGIN_MD + "\n\n" + render_table(entries) + "\n" + END_MD) != readme_text:
        problems.append("the source table of third_party/newlib/README.md is stale (run: hstreuse.py newlib --write)")
    newlib_expected = {e.rel for e in entries}
    for path, n in effective.items():
        if "LicenseRef-Newlib" in annotations[n]["SPDX-License-Identifier"].replace("LicenseRef-Newlib-COPYING-NEWLIB", ""):
            if path not in newlib_expected:
                problems.append(f"{path} carries a newlib licence but hstorigin does not classify it newlib")
    for path in sorted(newlib_expected):
        if path not in effective or "LicenseRef-Newlib" not in annotations[effective[path]]["SPDX-License-Identifier"]:
            problems.append(f"{path} is newlib-derived but its effective annotation has no newlib licence")
    by_expr: dict[str, int] = defaultdict(int)
    for n in effective.values():
        by_expr[annotations[n]["SPDX-License-Identifier"]] += 1
    print(f"tracked files: {len(files)}; covered: {len(effective)}; skipped as REUSE does (licence files): {skipped}")
    for expr, count in sorted(by_expr.items(), key=lambda kv: -kv[1])[:12]:
        print(f"  {count:5d}  {expr}")
    if len(by_expr) > 12:
        print(f"  ... {len(by_expr) - 12} more distinct expressions")
    for p in problems:
        print(f"PROBLEM: {p}")
    print("hstreuse check: " + ("COMPLIANT" if not problems else f"{len(problems)} problem(s)"))
    return 1 if problems else 0


def adapted_command() -> int:
    """Print, for every hand-adapted newlib source, the notices of its upstream file in each release (the input of
    ADAPTED_NOTICES). Reads the newlib trees under newlib_home through WSL."""
    import hstconfig
    home = hstconfig.wsl_path("newlib_home")
    classes, _warnings = hstorigin.classify()
    keys: dict[str, list[str]] = defaultdict(list)
    for p, c in sorted(classes.items()):
        text = (ROOT / p).read_text(encoding="utf-8", errors="replace") if c == "newlib" else ""
        head = "\n".join(text.split("\n", 3)[:3])
        m = None if LIB_NOTE.search(head) else (ADAPTED_NOTE.search(head) or IDENTIFIED_NOTE.search(text[:3000]))
        if m:
            keys[m["dir"].replace(" ", "/") + "/" + m["file"]].append(p)
    for key, paths in sorted(keys.items()):
        top, name = key.split("/", 1)[0], key.rsplit("/", 1)[1]
        dirs = ["libm/math", "libm/common"] if top == "libm" else [key.rsplit("/", 1)[0]]
        print(f"{key}: {len(paths)} source(s){'' if key in ADAPTED_NOTICES else '  (NOT IN ADAPTED_NOTICES)'}")
        for ver in NEWLIB_RELEASES:
            for d in dirs:
                r = subprocess.run(["wsl.exe", "-e", "sh", "-c", f'cat "{home}/{ver}/newlib-{ver}/newlib/{d}/{name}.c"'],
                                   capture_output=True)
                if r.returncode == 0:
                    found = families(r.stdout.decode("utf-8", "replace"))
                    print(f"  {ver} {d}/{name}.c: " + ("; ".join(f"{i}: {sorted(s)}" for i, s in sorted(found.items()))
                                                       or "no notice of its own (COPYING.NEWLIB default)"))
                    break
            else:
                print(f"  {ver}: not in {' or '.join(dirs)}")
    return 0


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)
    sub.add_parser("check")
    n = sub.add_parser("newlib")
    n.add_argument("--write", action="store_true")
    sub.add_parser("adapted", help="print the upstream notices of the hand-adapted sources (needs newlib_home)")
    args = ap.parse_args()
    if args.cmd == "adapted":
        return adapted_command()
    return check_command() if args.cmd == "check" else newlib_command(args.write)


if __name__ == "__main__":
    raise SystemExit(main())
