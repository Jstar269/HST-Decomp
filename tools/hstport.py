#!/usr/bin/env python3
"""Measure strict source portability for the EBOOT and Sharing game executables.

    python tools/hstport.py [--root DIR] [--config FILE] [--parallel N]
                            [--targets TRIPLE ...] [--json] [--write-doc]
    python tools/hstport.py --doc-only      rewrite docs/PORTING.md's measurement block from the last reports

--write-doc (after a run of all five targets) and --doc-only rewrite the block between
`<!-- hstport:begin measurement -->` and `<!-- hstport:end -->` in docs/PORTING.md; never edit it by hand.

Strict success means Clang exits zero and no fixed portability diagnostic below fires. Translation units are
compiled with -c to unique system-temporary object paths, never into the repository. C is C99. C++ is C++17,
fixed so results do not depend on a compiler default and all measured targets use the same language baseline.
The HSTMATCH lang= field selects the language; older headers without it follow the matcher's .c/.cpp suffix rule.
The freestanding compile uses -nostdinc and port/include before the PSP include directory; no host or PSP
runtime is linked.

Promoted diagnostics (each is one line of the portability contract):
  int-to-pointer-cast             catches non-constant integer-to-pointer casts when the integer is narrower
                                  than the target pointer; integer constant expressions use the AST
                                  numeric-address detector below because Clang may suppress this warning for them.
  pointer-to-int-cast             catches pointer-to-integer casts when the integer type is narrower than a pointer.
  int-conversion                  catches implicit incompatible pointer/integer conversions.
  incompatible-pointer-types      catches assignments, arguments, and returns using incompatible data pointers.
  implicit-function-declaration  catches calls to functions without a visible declaration in C.
  implicit-int                    catches declarations that rely on C's obsolete implicit int rule.
  return-type                     catches non-void functions that can fall through without returning a value.
  shorten-64-to-32                catches implicit 64-to-32 bit integer narrowing on 64-bit targets only.
  pointer-integer-compare         catches comparisons between pointer and integer operands.
  incompatible-function-pointer-types catches incompatible function pointer assignments and arguments.
Clang's return-type-c-linkage subdiagnostic is retained under return-type because it rejects non-C-compatible
return types crossing a C ABI boundary.
The AST numeric-address policy catches every non-null integer constant expression cast to a pointer, plus
module-range integer constants converted to or compared with pointers. It covers literal, folded, and enum values
even where Clang suppresses an integer-to-pointer warning.

Other compiler warnings are omitted from the fixed list because style, optimization, and unrelated target
warnings do not establish a source portability defect; any compiler error still fails. MWCC/MIPS-only inline
assembly, register-bound assembly, section attributes, known MWCC pragmas, and MWCC/MIPS intrinsics are rejected
by an explicit source policy even if Clang accepts or ignores them. The report stores no diagnostic source text, and
its census counts each failed source once by its first failing target and diagnostic.
"""
from __future__ import annotations

import argparse
import concurrent.futures
import datetime as dt
import json
import os
import re
import subprocess
import sys
import tempfile
import time
from collections import Counter
from dataclasses import dataclass
from pathlib import Path
from typing import Callable, Iterable

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
import hstorigin
import hstpublish
import hstaddr

GAME_TREES = {
    "src/eboot": "EBOOT.BIN",
    "src/sharing_main": "hstp_trans_main.prx",
}
TARGETS = (
    "x86_64-pc-windows-msvc",
    "i686-pc-windows-msvc",
    "aarch64-linux-android24",
    "armv7a-linux-androideabi24",
    "x86_64-linux-android24",
)
BITS_64 = frozenset((TARGETS[0], TARGETS[2], TARGETS[4]))
CXX_STANDARD = "c++17"
PORTABILITY_WARNINGS = (
    "int-to-pointer-cast",
    "pointer-to-int-cast",
    "int-conversion",
    "incompatible-pointer-types",
    "implicit-function-declaration",
    "implicit-int",
    "return-type",
    "pointer-integer-compare",
    "incompatible-function-pointer-types",
)
WARNING_CATEGORY_ALIASES = {
    "int-to-void-pointer-cast": "int-to-pointer-cast",
    "void-pointer-to-int-cast": "pointer-to-int-cast",
}

DEFAULT_NDK = (Path(os.environ.get("LOCALAPPDATA", r"C:\Users\Default\AppData\Local")) /
               "Android" / "Sdk" / "ndk" / "30.0.16248370")
DEFAULT_HOST_CLANG = Path(r"C:\Program Files\LLVM\bin\clang.exe")
NDK_CLANG_RELATIVE = Path("toolchains") / "llvm" / "prebuilt" / "windows-x86_64" / "bin" / "clang.exe"
DEFAULT_OVERLAY = ROOT / "port" / "include"

HEADER_RE = re.compile(r"/\*\s*HSTMATCH\s+([^*]+?)\s*\*/")
FIELD_RE = re.compile(r"([A-Za-z_][A-Za-z_0-9]*)=([^\s]+)")
DIAGNOSTIC_RE = re.compile(
    r"^(?P<file>.+):(?P<line>\d+):(?P<column>\d+):\s*"
    r"(?P<severity>fatal error|error|warning|note):\s*(?P<message>.*?)"
    r"(?:\s+\[(?P<flag>-W[^\]]+)\])?$"
)


@dataclass(frozen=True)
class Source:
    path: Path
    relative: str
    module: str
    start: int
    end: int
    lang: str
    origin: str


@dataclass(frozen=True)
class PolicyIssue:
    reason: str
    line: int


@dataclass(frozen=True)
class CompileResult:
    ok: bool
    diagnostic: dict | None
    reason: str | None


def _expand_path(value: str | Path) -> Path:
    expanded = os.path.expanduser(os.path.expandvars(str(value)))
    return Path(expanded).resolve()


def _clang_in_ndk(value: str | Path) -> Path:
    path = _expand_path(value)
    return path if path.is_file() else path / NDK_CLANG_RELATIVE


def configured_compilers(config_path: Path | None = None, host_clang: str | Path | None = None,
                         android_ndk: str | Path | None = None) -> dict[str, Path]:
    """Choose Windows Clang when installed; otherwise use the NDK Clang for all five targets."""
    settings: dict = {}
    if config_path is not None and config_path.exists():
        settings = json.loads(config_path.read_text(encoding="utf-8"))
    ndk_setting = (android_ndk or os.environ.get("HST_ANDROID_NDK") or settings.get("android_ndk") or DEFAULT_NDK)
    ndk_clang = _clang_in_ndk(ndk_setting)
    host_setting = (host_clang or os.environ.get("HST_HOST_CLANG") or settings.get("host_clang") or DEFAULT_HOST_CLANG)
    host_path = _expand_path(host_setting)
    windows_clang = host_path if host_path.is_file() else ndk_clang
    if not ndk_clang.is_file():
        raise FileNotFoundError(f"Android NDK Clang not found: {ndk_clang}")
    return {target: (windows_clang if target in TARGETS[:2] else ndk_clang) for target in TARGETS}


def read_source_header(path: Path) -> dict:
    head = path.read_text(encoding="utf-8", errors="replace")[:2048]
    match = HEADER_RE.search(head)
    if not match:
        raise ValueError(f"missing HSTMATCH header: {path}")
    fields = dict(FIELD_RE.findall(match.group(1)))
    required = ("module", "start", "end")
    missing = [key for key in required if key not in fields]
    if missing:
        raise ValueError(f"HSTMATCH header in {path} lacks {', '.join(missing)}")
    try:
        fields["start"] = int(fields["start"], 0)
        fields["end"] = int(fields["end"], 0)
    except ValueError as exc:
        raise ValueError(f"invalid HSTMATCH range in {path}") from exc
    if fields["end"] <= fields["start"]:
        raise ValueError(f"empty or reversed HSTMATCH range in {path}")
    # Older HSTMATCH headers omit lang; hstmatch's established rule is .c -> c99 and .cpp -> c++.
    if "lang" not in fields:
        fields["lang"] = "c++" if path.suffix == ".cpp" else "c99"
    if fields["lang"] not in ("c99", "c++"):
        raise ValueError(f"unsupported HSTMATCH language {fields['lang']!r} in {path}")
    return fields


def collect_sources(root: Path = ROOT, origin_classes: dict[str, str] | None = None) -> list[Source]:
    root = root.resolve()
    if origin_classes is None:
        origin_classes, _warnings = hstorigin.classify(root)
    sources: list[Source] = []
    for tree, expected_module in GAME_TREES.items():
        source_root = root / tree
        if not source_root.is_dir():
            raise FileNotFoundError(f"missing source tree: {source_root}")
        for path in sorted(p for p in source_root.rglob("*") if p.suffix in (".c", ".cpp")):
            header = read_source_header(path)
            if header["module"] != expected_module:
                raise ValueError(f"{path.relative_to(root).as_posix()} names {header['module']!r}, expected {expected_module!r}")
            relative = path.relative_to(root).as_posix()
            if relative not in origin_classes:
                raise ValueError(f"no origin class for {relative}")
            sources.append(Source(path, relative, header["module"], header["start"], header["end"],
                                  header["lang"], origin_classes[relative]))
    return sorted(sources, key=lambda source: source.relative)


def read_function_ranges(root: Path = ROOT) -> dict[str, list[tuple[int, int]]]:
    ranges: dict[str, list[tuple[int, int]]] = {}
    for module in GAME_TREES.values():
        path = root / "data" / f"functions-{module}.txt"
        if not path.is_file():
            raise FileNotFoundError(f"missing function boundary list: {path}")
        module_ranges = []
        for number, line in enumerate(path.read_text(encoding="utf-8").splitlines(), 1):
            line = line.strip()
            if not line or line.startswith("#"):
                continue
            fields = line.split()
            if len(fields) != 2:
                raise ValueError(f"invalid function range at {path}:{number}")
            start, end = (int(field, 16) for field in fields)
            if end <= start:
                raise ValueError(f"empty or reversed function range at {path}:{number}")
            module_ranges.append((start, end))
        ranges[module] = module_ranges
    return ranges


def _mask_noncode(text: str) -> str:
    pattern = re.compile(r"/\*.*?\*/|//[^\n]*|\"(?:\\.|[^\"\\])*\"|'(?:\\.|[^'\\])*'", re.S)
    return pattern.sub(lambda match: "".join("\n" if ch == "\n" else " " for ch in match.group()), text)


def policy_issue(source: Source) -> PolicyIssue | None:
    text = source.path.read_text(encoding="utf-8", errors="replace")
    code = _mask_noncode(text)
    patterns = (
        ("mips-register-variable", re.compile(r"\bregister\b[^;{}]*?\b(?:__asm__?|asm)\b\s*(?:(?:__volatile__|volatile)\s*)?\([^)]*\$[A-Za-z0-9_]+", re.S)),
        ("target-specific-section", re.compile(r"__attribute__\s*\(\([^)]*\bsection\s*\(", re.S)),
        ("mwcc-pragma", re.compile(r"(?m)^\s*#\s*pragma\s+(?:exceptions|inline_depth|opt_loop_invariants|opt_strength_reduction|opt_unroll_loops)\b")),
        ("mwcc-or-mips-intrinsic", re.compile(r"\b(?:__builtin_mips_[A-Za-z0-9_]*|__MWERKS__|MWC_[A-Za-z0-9_]*)\b")),
        ("mips-inline-asm", re.compile(r"\b(?:__asm__?|asm)\b\s*(?:(?:__volatile__|volatile)\s*)?\(")),
    )
    found: list[tuple[int, int, str]] = []
    for priority, (reason, pattern) in enumerate(patterns):
        match = pattern.search(code)
        if match:
            found.append((match.start(), priority, reason))
    if not found:
        return None
    position, _priority, reason = min(found)
    return PolicyIssue(reason, code.count("\n", 0, position) + 1)


def target_flags(target: str, lang: str, root: Path, overlay: Path) -> list[str]:
    if lang == "c99":
        language_args = ["-x", "c", "-std=c99"]
    else:
        language_args = ["-x", "c++", f"-std={CXX_STANDARD}"]
    flags = [
        f"--target={target}", *language_args,
        "-ffreestanding", "-nostdinc", "-fno-color-diagnostics", "-fdiagnostics-show-option",
        "-I", str(overlay.resolve()), "-I", str((root / "include").resolve()), "-I", str((root / "src").resolve()),
    ]
    flags.extend(f"-Werror={warning}" for warning in PORTABILITY_WARNINGS)
    if target in BITS_64:
        flags.append("-Werror=shorten-64-to-32")
    flags.append("-c")
    return flags


def _normalize_file(path: str, root: Path) -> str:
    try:
        return Path(path).resolve().relative_to(root.resolve()).as_posix()
    except (OSError, ValueError):
        return Path(path).name if path not in ("<stdin>", "") else "<compiler>"


def _category_from_flag(flag: str | None) -> str | None:
    if not flag:
        return None
    for part in flag.split(","):
        if part.startswith("-W") and part != "-Werror":
            if part.startswith("-Werror="):
                return part[len("-Werror="):]
            return part[2:]
    if flag.startswith("-Werror="):
        return flag[len("-Werror="):]
    if flag.startswith("-W"):
        return flag[2:]
    return flag


def _generic_error_category(message: str) -> str:
    text = message.casefold()
    if "file not found" in text or "include file" in text:
        return "missing-header"
    if "unknown type name" in text:
        return "unknown-type"
    if "undeclared identifier" in text or "implicit declaration" in text:
        return "undeclared-name"
    if "no member named" in text:
        return "missing-member"
    if "redefinition" in text or "redefined" in text or "duplicate definition" in text:
        return "duplicate-definition"
    if "no matching" in text or "candidate function" in text:
        return "no-matching-overload"
    if "operator" in text and any(token in text for token in ("argument", "operand", "takes", "arity")):
        return "operator-arity"
    if "operator" in text:
        return "operator-diagnostic"
    if "invalid operands" in text or "invalid conversion" in text:
        return "invalid-operation-or-conversion"
    if "inline assembly" in text or "invalid instruction" in text or "invalid operand for instruction" in text:
        return "unsupported-assembly"
    if any(token in text for token in ("expected ", "unterminated", "extraneous", "too many errors")):
        return "syntax-error"
    return "compiler-error"


def parse_diagnostics(stderr: str, source: Source, root: Path) -> list[dict]:
    diagnostics = []
    for line in stderr.splitlines():
        match = DIAGNOSTIC_RE.match(line.strip())
        if not match:
            continue
        severity = match.group("severity")
        flag = match.group("flag")
        category = _category_from_flag(flag) or _generic_error_category(match.group("message"))
        category = WARNING_CATEGORY_ALIASES.get(category, category)
        diagnostics.append({
            "severity": severity,
            "category": category,
            "flag": flag or "none",
            "location": f"{_normalize_file(match.group('file'), root)}:{int(match.group('line'))}",
            "line": int(match.group("line")),
        })
    return diagnostics


def compile_source(source: Source, target: str, compiler: Path, flags: list[str], root: Path,
                   object_path: Path, issue: PolicyIssue | None = None) -> CompileResult:
    command = [str(compiler), *flags, str(source.path.resolve()), "-o", str(object_path)]
    try:
        result = subprocess.run(command, cwd=root, capture_output=True, text=True, encoding="utf-8", errors="replace",
                                stdin=subprocess.DEVNULL, check=False)
    except OSError:
        diag = {"category": "compiler-launch-failure", "flag": "none", "location": f"{source.relative}:1"}
        return CompileResult(False, diag, "compiler-launch-failure")

    diagnostics = parse_diagnostics(result.stderr, source, root)
    errors = [d for d in diagnostics if d["severity"] in ("error", "fatal error")]
    strict = [d for d in diagnostics if d["category"] in PORTABILITY_WARNINGS + ("shorten-64-to-32",)]
    selected = errors or strict
    if result.returncode != 0 and not selected:
        selected = diagnostics
    compiler_diag = selected[0] if selected else None
    if result.returncode == 0 and compiler_diag is None and issue is None:
        return CompileResult(True, None, None)

    policy_diag = None
    if issue is not None:
        policy_diag = {"category": issue.reason, "flag": f"policy:{issue.reason}",
                       "location": f"{source.relative}:{issue.line}", "line": issue.line}
    if policy_diag is not None and (compiler_diag is None or policy_diag["line"] <= compiler_diag["line"]):
        diagnostic = policy_diag
        reason = issue.reason
    elif compiler_diag is not None:
        diagnostic = compiler_diag
        reason = compiler_diag["flag"] if compiler_diag["flag"] != "none" else compiler_diag["category"]
    else:
        diagnostic = {"category": "compiler-error", "flag": "none", "location": f"{source.relative}:1"}
        reason = "compiler-error"
    diagnostic = {key: diagnostic[key] for key in ("category", "flag", "location")}
    return CompileResult(False, diagnostic, reason)


def _compiler_version(compiler: Path) -> str:
    result = subprocess.run([str(compiler), "--version"], capture_output=True, text=True, encoding="utf-8",
                            errors="replace", stdin=subprocess.DEVNULL, check=False)
    if result.returncode:
        raise RuntimeError(f"clang --version failed for {compiler}")
    first = next((line.strip() for line in result.stdout.splitlines() if line.strip()), "")
    if not first:
        raise RuntimeError(f"clang --version returned no version for {compiler}")
    return first


def _merge_intervals(intervals: Iterable[tuple[int, int]]) -> list[tuple[int, int]]:
    merged: list[list[int]] = []
    for start, end in sorted(intervals):
        if not merged or start > merged[-1][1]:
            merged.append([start, end])
        else:
            merged[-1][1] = max(merged[-1][1], end)
    return [(start, end) for start, end in merged]


def _subtract_intervals(left: Iterable[tuple[int, int]], right: Iterable[tuple[int, int]]) -> list[tuple[int, int]]:
    right_merged = _merge_intervals(right)
    out = []
    for start, end in _merge_intervals(left):
        cursor = start
        for other_start, other_end in right_merged:
            if other_end <= cursor:
                continue
            if other_start >= end:
                break
            if other_start > cursor:
                out.append((cursor, min(other_start, end)))
            cursor = max(cursor, other_end)
            if cursor >= end:
                break
        if cursor < end:
            out.append((cursor, end))
    return out


def covered_bytes(functions: dict[str, list[tuple[int, int]]], intervals: dict[str, Iterable[tuple[int, int]]]) -> int:
    total = 0
    for module, ranges in functions.items():
        merged = _merge_intervals(intervals.get(module, ()))
        for start, end in ranges:
            total += sum(max(0, min(end, right) - max(start, left))
                         for left, right in merged if left < end and right > start)
    return total


def _summary_view(view: str, sources: list[Source], results: dict[str, dict[str, CompileResult]],
                  functions: dict[str, list[tuple[int, int]]], targets: tuple[str, ...],
                  compilers: dict[str, Path], versions: dict[Path, str], flags: dict[str, dict[str, list[str]]],
                  excluded_classes: tuple[str, ...], numeric_address_counts: dict[str, int]) -> dict:
    private = view == "private"
    included = [s for s in sources if private or s.origin not in excluded_classes]
    included_paths = {s.relative for s in included}
    passing_intervals: dict[str, list[tuple[int, int]]] = {module: [] for module in GAME_TREES.values()}
    excluded_intervals: dict[str, list[tuple[int, int]]] = {module: [] for module in GAME_TREES.values()}
    per_target = {target: {"ok": 0, "fail": 0} for target in targets}
    compiling_all = 0
    failures = []

    for source in sources:
        outcomes = results[source.relative]
        complete = all(outcomes[target].ok for target in targets)
        if source.relative in included_paths:
            for target in targets:
                if outcomes[target].ok:
                    per_target[target]["ok"] += 1
                else:
                    per_target[target]["fail"] += 1
            if complete:
                compiling_all += 1
                passing_intervals[source.module].append((source.start, source.end))
            else:
                first_target = next(target for target in targets if not outcomes[target].ok)
                failure = {"source": source.relative, "target": first_target,
                           "diagnostic": outcomes[first_target].diagnostic,
                           "reason": outcomes[first_target].reason}
                failures.append(failure)
        elif not private and source.origin in excluded_classes:
            excluded_intervals[source.module].append((source.start, source.end))

    code_bytes = sum(end - start for ranges in functions.values() for start, end in ranges)
    if not private:
        # Subtract excluded-class-only coverage; an overlapping public source keeps the bytes in scope.
        for module in GAME_TREES.values():
            public_by_module = _merge_intervals((s.start, s.end) for s in sources
                                                if s.module == module and s.origin not in excluded_classes)
            excluded_intervals[module] = _subtract_intervals(excluded_intervals[module], public_by_module)
        code_bytes -= covered_bytes(functions, excluded_intervals)
    compiling_bytes = covered_bytes(functions, passing_intervals)

    census = Counter()
    for failure in failures:
        diag = failure["diagnostic"] or {}
        reason = failure["reason"] or diag.get("category", "compiler-error")
        census[reason] += 1
    compiler_info = {target: {"path": str(compilers[target]), "version": versions[compilers[target]]}
                     for target in targets}
    return {
        "schema": "hst-decomp.port-report.v1",
        "view": view,
        "targets": list(targets),
        "sources": len(included),
        "compiling_all_targets": compiling_all,
        "code_bytes": code_bytes,
        "compiling_bytes": compiling_bytes,
        "per_target": per_target,
        "compilers": compiler_info,
        "flags": {target: flags[target] for target in targets},
        "failures": failures,
        "census": dict(sorted(census.items())),
        "numeric_address_findings": {source.relative: numeric_address_counts[source.relative]
                                      for source in included},
    }


def _scan_numeric_addresses(sources: list[Source], root: Path, overlay: Path, compiler: Path,
                            parallel: int, progress: Callable[[str, int, int], None] | None) -> tuple[dict[str, list[dict]], float]:
    """Run the shared x86_64 AST detector once per source before target compilation."""
    started = time.monotonic()
    results: dict[str, list[dict]] = {}
    completed = 0
    with concurrent.futures.ThreadPoolExecutor(max_workers=parallel) as executor:
        futures = {
            executor.submit(hstaddr.findings, source.path, source.lang, root=root, overlay=overlay,
                            compiler=compiler): source
            for source in sources
        }
        for future in concurrent.futures.as_completed(futures):
            source = futures[future]
            results[source.relative] = future.result()
            completed += 1
            if progress and (completed == len(sources) or completed % max(1, len(sources) // 20) == 0):
                progress("numeric-address", completed, len(sources))
    return results, time.monotonic() - started


def run_check(root: Path = ROOT, targets: Iterable[str] = TARGETS, parallel: int = 4,
              config_path: Path | None = None, host_clang: str | Path | None = None,
              android_ndk: str | Path | None = None, overlay: Path | None = None,
              origin_classes: dict[str, str] | None = None,
              progress: Callable[[str, int, int], None] | None = None,
              temp_root: Path | None = None) -> dict:
    """Compile the source inventory and return the public and private reports."""
    started = time.monotonic()
    root = root.resolve()
    requested_targets = set(targets)
    target_tuple = tuple(target for target in TARGETS if target in requested_targets)
    unknown = requested_targets - set(TARGETS)
    if unknown:
        raise ValueError(f"unknown target(s): {', '.join(sorted(unknown))}")
    if not target_tuple:
        raise ValueError("at least one target is required")
    if parallel < 1:
        raise ValueError("parallelism must be at least 1")

    compiler_map = configured_compilers(config_path, host_clang, android_ndk)
    compilers = {target: compiler_map[target] for target in target_tuple}
    versions_by_path: dict[Path, str] = {}
    for compiler in dict.fromkeys(compilers.values()):
        versions_by_path[compiler] = _compiler_version(compiler)
    overlay = (overlay or DEFAULT_OVERLAY).resolve()
    if not overlay.is_dir():
        raise FileNotFoundError(f"missing host header overlay: {overlay}")

    sources = collect_sources(root, origin_classes)
    if not sources:
        raise ValueError("no EBOOT or Sharing sources found")
    functions = read_function_ranges(root)
    policy = {source.relative: policy_issue(source) for source in sources}
    numeric_findings, numeric_address_seconds = _scan_numeric_addresses(
        sources, root, overlay, compiler_map[TARGETS[0]], parallel, progress)
    numeric_counts = {relative: len(items) for relative, items in numeric_findings.items()}
    result_flags = {
        target: {
            "c99": target_flags(target, "c99", root, overlay),
            "c++": target_flags(target, "c++", root, overlay),
        }
        for target in target_tuple
    }

    outcomes: dict[str, dict[str, CompileResult]] = {source.relative: {} for source in sources}
    with tempfile.TemporaryDirectory(prefix="hstport-", dir=str(temp_root.resolve()) if temp_root else None) as tmp:
        temp_dir = Path(tmp)
        for target_index, target in enumerate(target_tuple):
            target_flags_actual = {}
            for source in sources:
                target_flags_actual[source.relative] = target_flags(target, source.lang, root, overlay)
            completed = 0
            with concurrent.futures.ThreadPoolExecutor(max_workers=parallel) as executor:
                futures = {}
                for source_index, source in enumerate(sources):
                    found = numeric_findings[source.relative]
                    if found:
                        first = found[0]
                        outcomes[source.relative][target] = CompileResult(
                            False,
                            {"category": "numeric-address", "flag": "policy:numeric-address",
                             "location": f"{source.relative}:{first['line']}"},
                            "numeric-address",
                        )
                        completed += 1
                        if progress and (completed == len(sources) or completed % max(1, len(sources) // 20) == 0):
                            progress(target, completed, len(sources))
                        continue
                    object_path = temp_dir / f"{target_index:02d}-{source_index:06d}.o"
                    future = executor.submit(compile_source, source, target, compilers[target],
                                             target_flags_actual[source.relative], root, object_path,
                                             policy[source.relative])
                    futures[future] = source
                for future in concurrent.futures.as_completed(futures):
                    source = futures[future]
                    outcomes[source.relative][target] = future.result()
                    completed += 1
                    if progress and (completed == len(sources) or completed % max(1, len(sources) // 20) == 0):
                        progress(target, completed, len(sources))

    elapsed = time.monotonic() - started
    excluded = tuple(hstpublish.EXCLUDED_CLASSES)
    public = _summary_view("public", sources, outcomes, functions, target_tuple, compilers,
                           versions_by_path, result_flags, excluded, numeric_counts)
    private = _summary_view("private", sources, outcomes, functions, target_tuple, compilers,
                            versions_by_path, result_flags, excluded, numeric_counts)
    for report in (public, private):
        report["generated_at"] = dt.datetime.now(dt.timezone.utc).replace(microsecond=0).isoformat()
        report["elapsed_seconds"] = round(elapsed, 3)
        report["numeric_address_scan_seconds"] = round(numeric_address_seconds, 3)
        report["all_five_targets"] = set(target_tuple) == set(TARGETS)
    return {"public": public, "private": private, "elapsed_seconds": round(elapsed, 3),
            "numeric_address_scan_seconds": round(numeric_address_seconds, 3)}


def write_report(path: Path, report: dict) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(report, indent=2, sort_keys=True) + "\n", encoding="utf-8", newline="\n")


PORTING_DOC = Path("docs/PORTING.md")
DOC_BLOCK = re.compile(r"(<!-- hstport:begin measurement -->)(.*?)(<!-- hstport:end -->)", re.S)


def _duration(seconds: float) -> str:
    minutes, rest = divmod(seconds, 60)
    unit = "minute" if int(minutes) == 1 else "minutes"
    return f"{seconds:,.3f} seconds ({int(minutes)} {unit} {rest:.3f} seconds)" if minutes else f"{seconds:.3f} seconds"


def render_measurement(public: dict, private: dict) -> str:
    """The docs/PORTING.md measurement block from one full run's public and private reports."""
    if not (public.get("all_five_targets") and private.get("all_five_targets")):
        raise ValueError("a target subset is iteration-only: the measurement needs all five targets")
    findings = public["numeric_address_findings"]
    flagged = sum(1 for n in findings.values() if n)

    def share(report: dict) -> str:
        return f"{100 * report['compiling_bytes'] / report['code_bytes']:.3f}%" if report["code_bytes"] else "0.000%"

    def row(name: str, report: dict) -> str:
        return (f"| {name} | {report['sources']:,} | {report['compiling_all_targets']:,} | {report['code_bytes']:,} | "
                f"{report['compiling_bytes']:,} | {share(report)} |")

    lines = [
        f"The last full run, on {private['generated_at'][:10]}, analyzed and measured all {private['sources']:,} "
        f"sources for all five targets and took {_duration(private['elapsed_seconds'])}, including "
        f"{_duration(private['numeric_address_scan_seconds'])} for the AST numeric-address scan. The detector "
        f"reported {sum(findings.values()):,} findings across {flagged:,} published sources.",
        "",
        "| View | Sources | Compile all 5 | Code bytes | Compiling bytes | Byte progress |",
        "| --- | ---: | ---: | ---: | ---: | ---: |",
        row("Public", public),
        row("Private", private),
        "",
        "Per-target source counts are shown as `ok / fail`:",
        "",
        "| Target | Public | Private |",
        "| --- | ---: | ---: |",
        *(f"| `{t}` | {public['per_target'][t]['ok']:,} / {public['per_target'][t]['fail']:,} | "
          f"{private['per_target'][t]['ok']:,} / {private['per_target'][t]['fail']:,} |" for t in public["targets"])]
    return "\n".join(lines)


def write_doc(root: Path, public: dict, private: dict) -> bool:
    """Rewrite the measurement block of docs/PORTING.md; True when the file changed. A missing block is an error."""
    path = root / PORTING_DOC
    text = path.read_text(encoding="utf-8")
    if not DOC_BLOCK.search(text):
        raise SystemExit(f"{PORTING_DOC.as_posix()} has no hstport measurement block")
    body = "\n\n" + render_measurement(public, private) + "\n\n"
    new = DOC_BLOCK.sub(lambda m: m.group(1) + body + m.group(3), text, count=1)
    if new != text:
        path.write_text(new, encoding="utf-8", newline="\n")
    return new != text


def _print_progress(target: str, completed: int, total: int) -> None:
    print(f"{target}: {completed}/{total} sources", file=sys.stderr, flush=True)


def main(argv: list[str] | None = None) -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--root", type=Path, default=ROOT, help="repository root (default: this checkout)")
    ap.add_argument("--config", type=Path, help="hst-config.json path (optional host_clang/android_ndk keys)")
    ap.add_argument("--overlay", type=Path, default=DEFAULT_OVERLAY, help="host header overlay directory")
    ap.add_argument("--output-dir", type=Path, help="directory for default reports (default: ROOT/try)")
    ap.add_argument("--public-report", type=Path, help="public report output path")
    ap.add_argument("--private-report", type=Path, help="private report output path")
    ap.add_argument("--host-clang", type=Path, help="preferred Clang binary for the Windows targets")
    ap.add_argument("--android-ndk", type=Path, help="Android NDK root or its clang executable")
    ap.add_argument("--temp-dir", type=Path, help="parent directory for temporary object files")
    ap.add_argument("--parallel", type=int, default=4, help="maximum concurrent compiler processes (default: 4)")
    ap.add_argument("--targets", nargs="+", choices=TARGETS, default=list(TARGETS),
                    help="targets to measure (default: all five; subsets are iteration-only)")
    ap.add_argument("--json", action="store_true", help="print a JSON summary to stdout")
    ap.add_argument("--write-doc", action="store_true",
                    help="after a run of all five targets, rewrite the measurement block of docs/PORTING.md")
    ap.add_argument("--doc-only", action="store_true",
                    help="rewrite the docs/PORTING.md measurement block from the existing reports; measure nothing")
    args = ap.parse_args(argv)
    if args.doc_only:
        root = args.root.resolve()
        output_dir = args.output_dir.resolve() if args.output_dir else root / "try"
        reports = [json.loads(p.read_text(encoding="utf-8")) for p in (
            args.public_report.resolve() if args.public_report else output_dir / "port-report.json",
            args.private_report.resolve() if args.private_report else output_dir / "port-private-report.json")]
        changed = write_doc(root, *reports)
        print(f"{PORTING_DOC.as_posix()}: " + ("measurement rewritten" if changed else "measurement unchanged"))
        return 0

    if args.write_doc and set(args.targets) != set(TARGETS):
        ap.error("--write-doc needs a run of all five targets (a --targets subset is iteration-only)")
    root = args.root.resolve()
    config = args.config.resolve() if args.config else root / "hst-config.json"
    output_dir = args.output_dir.resolve() if args.output_dir else root / "try"
    public_path = args.public_report.resolve() if args.public_report else output_dir / "port-report.json"
    private_path = args.private_report.resolve() if args.private_report else output_dir / "port-private-report.json"
    started = dt.datetime.now(dt.timezone.utc).replace(microsecond=0).isoformat()
    result = run_check(root=root, targets=args.targets, parallel=args.parallel, config_path=config,
                       host_clang=args.host_clang, android_ndk=args.android_ndk, overlay=args.overlay,
                       progress=None if args.json else _print_progress, temp_root=args.temp_dir)
    write_report(public_path, result["public"])
    write_report(private_path, result["private"])
    summary = {
        "generated_at": started,
        "elapsed_seconds": result["elapsed_seconds"],
        "numeric_address_scan_seconds": result["numeric_address_scan_seconds"],
        "reports": {"public": str(public_path), "private": str(private_path)},
        "public": {key: result["public"][key] for key in
                   ("sources", "compiling_all_targets", "code_bytes", "compiling_bytes", "per_target", "census")},
        "private": {key: result["private"][key] for key in
                    ("sources", "compiling_all_targets", "code_bytes", "compiling_bytes", "per_target", "census")},
    }
    if args.json:
        print(json.dumps(summary, sort_keys=True))
    else:
        print(f"public:  {result['public']['sources']} sources; {result['public']['compiling_all_targets']} compile "
              f"all selected target(s); {result['public']['compiling_bytes']}/{result['public']['code_bytes']} code bytes")
        print(f"private: {result['private']['sources']} sources; {result['private']['compiling_all_targets']} compile "
              f"all selected target(s); {result['private']['compiling_bytes']}/{result['private']['code_bytes']} code bytes")
        print(f"reports: {public_path} ; {private_path}")
    if args.write_doc:
        changed = write_doc(root, result["public"], result["private"])
        print(f"{PORTING_DOC.as_posix()}: " + ("measurement rewritten" if changed else "measurement unchanged"))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
