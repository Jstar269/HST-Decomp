#!/usr/bin/env python3
"""Find non-relocatable numeric addresses in one HSTMATCH source with Clang's AST.

The stable import surface is ``findings(path, lang)``. It returns dictionaries with exactly
``kind``, ``line`` and ``column``; it never returns source text or address values. Callers may
override the checkout root, host Clang, or header overlay with keyword arguments for isolated
synthetic fixtures. The AST is built for x86_64 Windows because the expression and cast kinds
are language properties, while the pointer width and the same freestanding header overlay used
by :mod:`hstport` are held fixed.

``integral-to-pointer`` marks every non-null integer constant expression cast to a pointer,
including folded arithmetic and enumerators. ``module-address-conversion`` marks a constant within
the source module's mapped range that a non-constant integer-to-pointer conversion uses as the
address: added to indices only, with no address-carrying base. A constant added to (or subtracted
from) a base that can hold an address -- a pointer converted to an integer, or a 32-bit value read
from a parameter, local, member, element, dereference or call, as in Ghidra's
``*(T *)(param_1 + 0x524)`` -- is an offset and is not reported. Shapes the rule does not classify
keep the conservative check: every module-range constant inside the conversion is reported.
``module-address-pointer-compare`` marks a pointer comparison against a module-range integer
constant when Clang has not already represented that integer as an IntegralToPointer cast.
"""
from __future__ import annotations

import argparse
import concurrent.futures
import json
import os
import re
import subprocess
import sys
from pathlib import Path
from typing import Iterable

PROJECT_ROOT = Path(__file__).resolve().parent.parent
TARGET = "x86_64-pc-windows-msvc"
CXX_STANDARD = "c++17"
DEFAULT_NDK = Path(os.environ.get("LOCALAPPDATA", r"C:\Users\Default\AppData\Local")) / "Android" / "Sdk" / "ndk" / "30.0.16248370"
NDK_CLANG_RELATIVE = Path("toolchains") / "llvm" / "prebuilt" / "windows-x86_64" / "bin" / "clang.exe"
DEFAULT_HOST_CLANG = Path(r"C:\Program Files\LLVM\bin\clang.exe")
HEADER_RE = re.compile(r"/\*\s*HSTMATCH\s+([^*]+?)\s*\*/")
FIELD_RE = re.compile(r"([A-Za-z_][A-Za-z_0-9]*)=([^\s]+)")
POINTER_COMPARE_OPS = frozenset(("==", "!=", "<", "<=", ">", ">="))
CAST_KINDS = frozenset(("CStyleCastExpr", "CXXFunctionalCastExpr", "CXXReinterpretCastExpr", "ImplicitCastExpr"))


class HstAddrError(RuntimeError):
    """The requested source could not be analyzed into a Clang AST."""


def _expand_path(value: str | Path) -> Path:
    return Path(os.path.expanduser(os.path.expandvars(str(value)))).resolve()


def _find_root(path: Path, root: Path | None) -> Path:
    if root is not None:
        return root.resolve()
    resolved = path.resolve()
    if PROJECT_ROOT in resolved.parents:
        return PROJECT_ROOT
    for parent in resolved.parents:
        if (parent / "include").is_dir() and (parent / "data").is_dir():
            return parent
    return PROJECT_ROOT


def _read_header(path: Path) -> dict[str, str]:
    head = path.read_text(encoding="utf-8", errors="replace")[:2048]
    match = HEADER_RE.search(head)
    fields = dict(FIELD_RE.findall(match.group(1))) if match else {}
    return fields


def _language_args(lang: str) -> list[str]:
    if lang == "c99":
        return ["-x", "c", "-std=c99"]
    if lang == "c++":
        return ["-x", "c++", f"-std={CXX_STANDARD}"]
    raise ValueError(f"unsupported source language: {lang!r}")


def _configured_clang(root: Path) -> Path:
    config_path = root / "hst-config.json"
    config: dict = {}
    if config_path.is_file():
        try:
            config = json.loads(config_path.read_text(encoding="utf-8"))
        except (OSError, json.JSONDecodeError):
            config = {}
    host_value = os.environ.get("HST_HOST_CLANG") or config.get("host_clang") or DEFAULT_HOST_CLANG
    host = _expand_path(host_value)
    if host.is_file():
        return host
    ndk_value = os.environ.get("HST_ANDROID_NDK") or config.get("android_ndk") or DEFAULT_NDK
    ndk = _expand_path(ndk_value)
    ndk_clang = ndk if ndk.is_file() else ndk / NDK_CLANG_RELATIVE
    if ndk_clang.is_file():
        return ndk_clang
    raise FileNotFoundError("neither the configured Windows Clang nor Android NDK Clang exists")


def _load_ranges(root: Path, module: str, path: Path) -> list[tuple[int, int]]:
    modules_path = root / "data" / "modules.json"
    if modules_path.is_file():
        try:
            data = json.loads(modules_path.read_text(encoding="utf-8"))
            entry = next((item for item in data.get("modules", ()) if item.get("name") == module), None)
            if entry is not None:
                ranges = []
                for segment in entry.get("load_segments", ()):
                    start = segment.get("start")
                    end = segment.get("memory_end", segment.get("file_end"))
                    if start is not None and end is not None:
                        lo = int(start, 0) if isinstance(start, str) else int(start)
                        hi = int(end, 0) if isinstance(end, str) else int(end)
                        if hi > lo:
                            ranges.append((lo, hi))
                if ranges:
                    return ranges
        except (OSError, ValueError, json.JSONDecodeError):
            pass

    function_path = root / "data" / f"functions-{module}.txt"
    if function_path.is_file():
        ranges = []
        for line in function_path.read_text(encoding="utf-8", errors="replace").splitlines():
            fields = line.split()
            if len(fields) != 2 or fields[0].startswith("#"):
                continue
            try:
                lo, hi = (int(value, 16) for value in fields)
            except ValueError:
                continue
            if hi > lo:
                ranges.append((lo, hi))
        if ranges:
            return [(min(lo for lo, _ in ranges), max(hi for _, hi in ranges))]

    header = _read_header(path)
    try:
        lo, hi = int(header["start"], 0), int(header["end"], 0)
    except (KeyError, ValueError):
        return []
    return [(lo, hi)] if hi > lo else []


def _source_point(node: dict, source_path: Path, source_bytes: bytes) -> tuple[int, int] | None:
    raw = node.get("range", {}).get("begin") or node.get("loc") or {}
    if not isinstance(raw, dict):
        return None
    # A macro's expansion point belongs to the source using it; its spelling point may be
    # in an included header. Header declarations are excluded from the per-source census.
    while isinstance(raw.get("expansionLoc"), dict):
        raw = raw["expansionLoc"]
    if raw.get("includedFrom"):
        return None
    named_file = raw.get("file")
    if named_file:
        try:
            if _expand_path(named_file) != source_path.resolve():
                return None
        except OSError:
            return None
    offset = raw.get("offset")
    if isinstance(offset, int) and 0 <= offset <= len(source_bytes):
        line = source_bytes.count(b"\n", 0, offset) + 1
        previous_newline = source_bytes.rfind(b"\n", 0, offset)
        column = offset - previous_newline
        return line, column
    line, column = raw.get("line"), raw.get("col")
    if isinstance(line, int) and isinstance(column, int):
        return line, column
    return None


def _field_integer(node: dict) -> int | None:
    value = node.get("value")
    if not isinstance(value, (str, int)):
        return None
    try:
        return int(value, 0) if isinstance(value, str) else value
    except ValueError:
        try:
            return int(value, 10)
        except ValueError:
            return None


def _integer_value(node: dict, enum_values: dict[str, int], constexpr_values: dict[str, int],
                   active: frozenset[str] = frozenset()) -> int | None:
    direct = _field_integer(node)
    if direct is not None and node.get("kind") in (
        "ConstantExpr", "IntegerLiteral", "CharacterLiteral", "CXXBoolLiteralExpr",
        "EnumConstantDecl", "ConstantArrayType",
    ):
        return direct
    kind = node.get("kind")
    if kind == "DeclRefExpr":
        reference = node.get("referencedDecl", {})
        ref_id = reference.get("id")
        ref_kind = reference.get("kind")
        if ref_id in active:
            return None
        if ref_kind == "EnumConstantDecl":
            return enum_values.get(ref_id)
        if ref_kind == "VarDecl":
            # Clang's referencedDecl carries no constexpr flag; constexpr_values holds only constexpr variables
            return constexpr_values.get(ref_id)
        return None

    children = [child for child in node.get("inner", ()) if isinstance(child, dict)]
    if kind in ("ParenExpr", "ImplicitCastExpr", "CStyleCastExpr", "CXXStaticCastExpr",
                "CXXConstCastExpr", "CXXFunctionalCastExpr", "ExprWithCleanups", "FullExpr",
                "ConstantExpr", "MaterializeTemporaryExpr", "CXXBindTemporaryExpr"):
        if kind == "ConstantExpr" and direct is not None:
            return direct
        return _integer_value(children[-1], enum_values, constexpr_values, active) if children else None
    if kind == "UnaryOperator" and children:
        value = _integer_value(children[0], enum_values, constexpr_values, active)
        op = node.get("opcode")
        if value is None:
            return None
        if op == "+":
            return value
        if op == "-":
            return -value
        if op == "~":
            return ~value
        if op == "!":
            return int(not value)
        return None
    if kind == "BinaryOperator" and len(children) >= 2:
        left = _integer_value(children[0], enum_values, constexpr_values, active)
        right = _integer_value(children[1], enum_values, constexpr_values, active)
        if left is None or right is None:
            return None
        op = node.get("opcode")
        operations = {
            "+": lambda: left + right,
            "-": lambda: left - right,
            "*": lambda: left * right,
            "/": lambda: int(left / right),
            "%": lambda: left % right,
            "<<": lambda: left << right,
            ">>": lambda: left >> right,
            "|": lambda: left | right,
            "&": lambda: left & right,
            "^": lambda: left ^ right,
            "==": lambda: int(left == right),
            "!=": lambda: int(left != right),
            "<": lambda: int(left < right),
            "<=": lambda: int(left <= right),
            ">": lambda: int(left > right),
            ">=": lambda: int(left >= right),
            "&&": lambda: int(bool(left) and bool(right)),
            "||": lambda: int(bool(left) or bool(right)),
        }
        operation = operations.get(op)
        if operation is None:
            return None
        try:
            return operation()
        except (ArithmeticError, OverflowError, ValueError):
            return None
    if kind == "ConditionalOperator" and len(children) >= 3:
        condition = _integer_value(children[0], enum_values, constexpr_values, active)
        if condition is None:
            return None
        return _integer_value(children[1] if condition else children[2], enum_values, constexpr_values, active)
    return direct


def _is_pointer(node: dict) -> bool:
    type_info = node.get("type", {})
    qual_type = type_info.get("desugaredQualType") or type_info.get("qualType") or ""
    return "*" in qual_type


def _in_ranges(value: int, ranges: Iterable[tuple[int, int]]) -> bool:
    return any(start <= value < end for start, end in ranges)


def _constant_nodes(node: dict, enum_values: dict[str, int], constexpr_values: dict[str, int]):
    value = _integer_value(node, enum_values, constexpr_values)
    if value is not None:
        yield node, value
        return
    for child in _walk(node):
        if child is node:
            continue
        value = _integer_value(child, enum_values, constexpr_values)
        if value is not None:
            yield child, value


def _pointer_related(node: dict) -> bool:
    return _is_pointer(node) or any(
        item.get("castKind") in ("IntegralToPointer", "PointerToIntegral") for item in _walk(node)
    )


# --------------------------------------------------------------------------------------------------------------
# module-address-conversion: which constants of a non-constant integer-to-pointer conversion are absolute addresses
#
# The converted integer is read as a signed sum of terms, through parentheses, + and -, integral casts to a type
# of at least 32 bits, and the value side of an assignment or comma expression; a conditional inside it gives one
# sum per branch. Each term is
#   const   an integer constant expression (literal, folded arithmetic, enumerator, constexpr)
#   base    something that can hold an address: a pointer converted to an integer of at least 32 bits, or a value
#           of at least 32 bits read from a variable, parameter, member, array element, dereference, call,
#           assignment or increment, or such a base with only low bits cleared (& 0xfffffffe, & ~7)
#   index   a value that cannot be an address by itself: a product, quotient, remainder or shift, a negation or
#           complement, a comparison or logical value, sizeof/alignof, a pointer difference, a floating-point
#           value, or a value narrower than 32 bits
# When the bases of a sum add up to at least one (more added than subtracted), the sum is a base moved by the
# constants: they are offsets, whatever their size, and nothing is reported. Otherwise the constants are the
# address: each module-range constant term is reported, or the first constant term when only their sum lies in the
# range. Any other shape (other bitwise composition, truncation below 32 bits, a statement expression, more than
# MAX_ALTERNATIVES branch combinations, ...) is not classified, and every module-range
# constant inside the converted expression is reported, as the detector did before the rule existed.

MAX_ALTERNATIVES = 16
LOW_BITS_MASK = 0xffff0000          # an & with a constant at or above this clears only low bits
_TRANSPARENT_KINDS = frozenset(("ParenExpr", "ExprWithCleanups", "FullExpr", "MaterializeTemporaryExpr",
                                "CXXBindTemporaryExpr"))
_INTEGRAL_CAST_KINDS = frozenset(("ImplicitCastExpr", "CStyleCastExpr", "CXXStaticCastExpr",
                                  "CXXFunctionalCastExpr", "CXXConstCastExpr", "CXXReinterpretCastExpr"))
_LOAD_KINDS = frozenset(("DeclRefExpr", "MemberExpr", "ArraySubscriptExpr", "CompoundLiteralExpr"))
_CALL_KINDS = frozenset(("CallExpr", "CXXMemberCallExpr", "CXXOperatorCallExpr"))
_INDEX_BINARY = frozenset(("*", "/", "%", "<<", ">>", "==", "!=", "<", "<=", ">", ">=", "&&", "||"))
_INDEX_UNARY = frozenset(("-", "~", "!"))
_INDEX_CASTS = frozenset(("IntegralToBoolean", "FloatingToIntegral", "BooleanToSignedIntegral"))
_TYPE_BITS = {
    "_Bool": 1, "bool": 1,
    "char": 8, "signed char": 8, "unsigned char": 8,
    "short": 16, "unsigned short": 16, "short int": 16, "unsigned short int": 16, "signed short": 16,
    "wchar_t": 16, "char16_t": 16, "char32_t": 32,
    "int": 32, "unsigned int": 32, "signed int": 32, "unsigned": 32, "signed": 32,
    "long": 32, "unsigned long": 32, "long int": 32, "unsigned long int": 32, "signed long": 32,
    "long long": 64, "unsigned long long": 64, "long long int": 64, "unsigned long long int": 64,
    "signed long long": 64, "__int64": 64, "unsigned __int64": 64,
}

Term = tuple[str, int, dict, "int | None"]          # role, sign, node, constant value


class _Unclassified(Exception):
    """The converted integer has a shape the offset rule does not classify."""


def _type_bits(node: dict) -> int | None:
    """Width in bits of an integral expression type, or None when it is not a known integral type. The AST target
    is LLP64 (long is 32 bits); only the question 'at least 32 bits' matters, and the PSP agrees on it."""
    type_info = node.get("type", {})
    qual_type = type_info.get("desugaredQualType") or type_info.get("qualType") or ""
    name = " ".join(word for word in qual_type.split() if word not in ("const", "volatile", "restrict", "__restrict"))
    if name in _TYPE_BITS:
        return _TYPE_BITS[name]
    if name.startswith("enum "):
        return 32
    return None


def _wide(node: dict) -> bool:
    bits = _type_bits(node)
    return bits is not None and bits >= 32


def _children(node: dict) -> list[dict]:
    return [child for child in node.get("inner", ()) if isinstance(child, dict)]


def _strip(node: dict) -> dict:
    while node.get("kind") in _TRANSPARENT_KINDS and _children(node):
        node = _children(node)[-1]
    return node


def _value_role(node: dict) -> str:
    """base when a value of this node's type can hold an address, else index."""
    return "base" if _wide(node) else "index"


def _sums(node: dict, sign: int, enum_values: dict[str, int], constexpr_values: dict[str, int]) -> list[list[Term]]:
    """The alternative sums (one per combination of conditional branches) that `node` evaluates to."""
    value = _integer_value(node, enum_values, constexpr_values)
    if value is not None:
        return [[("const", sign, node, value)]]
    kind = node.get("kind")
    children = _children(node)
    if kind in _TRANSPARENT_KINDS and children:
        return _sums(children[-1], sign, enum_values, constexpr_values)
    if kind in _INTEGRAL_CAST_KINDS and children:
        cast_kind = node.get("castKind")
        if cast_kind == "PointerToIntegral":
            return [[(_value_role(node), sign, node, None)]]
        if cast_kind in ("IntegralCast", "NoOp"):
            if not _wide(node):
                raise _Unclassified("truncating cast")
            return _sums(children[-1], sign, enum_values, constexpr_values)
        if cast_kind == "LValueToRValue":
            target = _strip(children[-1])
            if target.get("kind") in _LOAD_KINDS or (target.get("kind") == "UnaryOperator"
                                                     and target.get("opcode") == "*"):
                return [[(_value_role(node), sign, node, None)]]
            raise _Unclassified("load of an unclassified lvalue")
        if cast_kind in _INDEX_CASTS:
            return [[("index", sign, node, None)]]
        raise _Unclassified(f"cast {cast_kind}")
    if kind == "BinaryOperator" and len(children) >= 2:
        opcode = node.get("opcode")
        if opcode in ("+", "-"):
            if _is_pointer(children[0]) and _is_pointer(children[1]):
                return [[("index", sign, node, None)]]          # a pointer difference is a displacement
            left = _sums(children[0], sign, enum_values, constexpr_values)
            right = _sums(children[1], sign if opcode == "+" else -sign, enum_values, constexpr_values)
            if len(left) * len(right) > MAX_ALTERNATIVES:
                raise _Unclassified("too many alternatives")
            return [a + b for a in left for b in right]
        if opcode in _INDEX_BINARY:
            return [[("index", sign, node, None)]]
        if opcode in ("=", ","):
            # an assignment's value is its right side; so is a comma expression's
            return _sums(children[1], sign, enum_values, constexpr_values)
        if opcode == "&":
            # clearing only low bits (a tag or alignment mask) of an address leaves an address
            for mask_side, other in ((children[1], children[0]), (children[0], children[1])):
                mask = _integer_value(mask_side, enum_values, constexpr_values)
                if mask is not None and (mask & 0xffffffff) >= LOW_BITS_MASK:
                    if all(_net_bases(terms) >= 1 for terms in _sums(other, 1, enum_values, constexpr_values)):
                        return [[("base", sign, node, None)]]
            raise _Unclassified("bitwise and")
        raise _Unclassified(f"binary {opcode}")
    if kind == "CompoundAssignOperator" and children:
        return [[(_value_role(node), sign, node, None)]]
    if kind == "UnaryOperator" and children:
        opcode = node.get("opcode")
        if opcode in _INDEX_UNARY:
            return [[("index", sign, node, None)]]
        if opcode == "+":
            return _sums(children[0], sign, enum_values, constexpr_values)
        if opcode in ("++", "--"):
            return [[(_value_role(node), sign, node, None)]]
        raise _Unclassified(f"unary {opcode}")
    if kind == "ConditionalOperator" and len(children) >= 3:
        alternatives = (_sums(children[1], sign, enum_values, constexpr_values)
                        + _sums(children[2], sign, enum_values, constexpr_values))
        if len(alternatives) > MAX_ALTERNATIVES:
            raise _Unclassified("too many alternatives")
        return alternatives
    if kind in _CALL_KINDS:
        return [[(_value_role(node), sign, node, None)]]
    if kind == "UnaryExprOrTypeTraitExpr":
        return [[("index", sign, node, None)]]
    raise _Unclassified(f"expression {kind}")


def _net_bases(terms: list[Term]) -> int:
    return sum(sign for role, sign, _node, _value in terms if role == "base")


def _absolute_constants(operand: dict, ranges: list[tuple[int, int]], enum_values: dict[str, int],
                        constexpr_values: dict[str, int]) -> list[dict]:
    """The constant nodes of the converted integer `operand` that are used as module addresses (see above)."""
    try:
        alternatives = _sums(operand, 1, enum_values, constexpr_values)
    except _Unclassified:
        return [child for child in _walk(operand)
                if (value := _integer_value(child, enum_values, constexpr_values)) is not None
                and value != 0 and _in_ranges(value, ranges)]
    hits: list[dict] = []
    for terms in alternatives:
        if _net_bases(terms) >= 1:
            continue
        constants = [(sign, node, value) for role, sign, node, value in terms if role == "const"]
        found = [node for _sign, node, value in constants if value != 0 and _in_ranges(value, ranges)]
        if not found and constants:
            total = sum(sign * value for sign, _node, value in constants)
            if total != 0 and _in_ranges(total, ranges):
                found = [constants[0][1]]
        hits.extend(found)
    return hits


def _named_constants(nodes: list[dict]) -> tuple[dict[str, int], dict[str, int]]:
    """({enumerator id: value}, {constexpr variable id: value}) of the AST `nodes` in pre-order. An enumerator
    without an initialiser is one more than the enumerator before it in its enum (the first one is 0)."""
    enum_values: dict[str, int] = {}
    constexpr_values: dict[str, int] = {}

    def initial(node: dict) -> int | None:
        children = [child for child in node.get("inner", ()) if isinstance(child, dict)]
        return next((value for child in reversed(children)
                     if (value := _integer_value(child, enum_values, constexpr_values)) is not None), None)

    for node in nodes:
        kind = node.get("kind")
        if kind == "EnumDecl":
            implicit: int | None = 0
            for child in node.get("inner", ()):
                if not isinstance(child, dict) or child.get("kind") != "EnumConstantDecl":
                    continue
                explicit = [item for item in child.get("inner", ()) if isinstance(item, dict)]
                value = initial(child) if explicit else implicit
                if value is not None and child.get("id"):
                    enum_values[child["id"]] = value
                implicit = value + 1 if value is not None else None
        elif kind == "VarDecl" and node.get("constexpr") and node.get("id"):
            value = initial(node)
            if value is not None:
                constexpr_values[node["id"]] = value
    return enum_values, constexpr_values


def _ast_command(source: Path, lang: str, root: Path, overlay: Path, compiler: Path) -> list[str]:
    return [
        str(compiler), f"--target={TARGET}", *_language_args(lang), "-ffreestanding", "-nostdinc",
        "-fno-color-diagnostics", "-fdiagnostics-show-option",
        "-I", str(overlay.resolve()), "-I", str((root / "include").resolve()),
        "-I", str((root / "src").resolve()),
        "-Xclang", "-ast-dump=json", "-fsyntax-only", str(source.resolve()),
    ]


def findings(path: str | Path, lang: str, *, root: str | Path | None = None,
             overlay: str | Path | None = None, compiler: str | Path | None = None) -> list[dict]:
    """Return numeric-address findings for ``path`` without retaining source text.

    ``lang`` is ``c99`` or ``c++``. Optional keyword arguments select the checkout,
    host header overlay, and Clang executable; the two-argument form uses this checkout's
    configuration and is stable for external census consumers.
    """
    source = Path(path).resolve()
    checkout = _find_root(source, Path(root) if root is not None else None)
    host_overlay = _expand_path(overlay or (checkout / "port" / "include"))
    if not host_overlay.is_dir():
        raise FileNotFoundError(f"Clang host header overlay not found: {host_overlay}")
    clang = _expand_path(compiler) if compiler is not None else _configured_clang(checkout)
    if not clang.is_file():
        raise FileNotFoundError(f"Clang executable not found: {clang}")
    source_bytes = source.read_bytes()
    header = _read_header(source)
    module = header.get("module", "")
    ranges = _load_ranges(checkout, module, source) if module else []
    command = _ast_command(source, lang, checkout, host_overlay, clang)
    try:
        proc = subprocess.run(command, cwd=checkout, capture_output=True, text=True, encoding="utf-8",
                              errors="replace", stdin=subprocess.DEVNULL, check=False)
    except OSError as exc:
        raise HstAddrError("could not launch Clang AST analysis") from exc
    try:
        ast = json.loads(proc.stdout)
    except json.JSONDecodeError as exc:
        # Never expose Clang's diagnostic text here: it can contain source excerpts.
        raise HstAddrError(f"Clang did not produce an AST for {source.name}") from exc

    nodes: list[dict] = []

    def collect(node: dict) -> None:
        nodes.append(node)
        for child in node.get("inner", ()):
            if isinstance(child, dict):
                collect(child)

    collect(ast)
    enum_values, constexpr_values = _named_constants(nodes)

    found: set[tuple[str, int, int]] = set()

    def add(kind: str, node: dict) -> None:
        point = _source_point(node, source, source_bytes)
        if point is not None:
            found.add((kind, point[0], point[1]))

    for node in nodes:
        cast_kind = node.get("castKind")
        node_kind = node.get("kind")
        children = [child for child in node.get("inner", ()) if isinstance(child, dict)]
        if cast_kind == "IntegralToPointer" and node_kind in CAST_KINDS and children:
            operand = children[-1]
            value = _integer_value(operand, enum_values, constexpr_values)
            if value is not None:
                if value != 0:
                    add("integral-to-pointer", node)
            elif ranges:
                # A module-range constant used as the address in a non-constant conversion is still a
                # fixed module address; an offset from an address-carrying base is not (see above).
                for constant in _absolute_constants(operand, ranges, enum_values, constexpr_values):
                    add("module-address-conversion", constant)
        if node_kind == "BinaryOperator" and node.get("opcode") in POINTER_COMPARE_OPS and len(children) >= 2:
            left, right = children[0], children[1]
            if _is_pointer(left) and not _is_pointer(right):
                integer_side = right
            elif _is_pointer(right) and not _is_pointer(left):
                integer_side = left
            else:
                integer_side = None
            if integer_side is not None:
                if any(child.get("castKind") == "IntegralToPointer" for child in _walk(integer_side)):
                    continue
                for constant_node, value in _constant_nodes(integer_side, enum_values, constexpr_values):
                    if value != 0 and _in_ranges(value, ranges):
                        add("module-address-pointer-compare", constant_node)
                continue

            # A source can first convert a pointer to uintptr_t and then compare it with a
            # numeric module address. In that case the binary operands are both integers;
            # preserve the pointer provenance carried by the cast in the AST.
            left_related, right_related = _pointer_related(left), _pointer_related(right)
            if left_related == right_related:
                continue
            if any(child.get("castKind") == "IntegralToPointer" for child in _walk(left)) or any(
                child.get("castKind") == "IntegralToPointer" for child in _walk(right)
            ):
                continue
            integer_side = right if left_related else left
            for constant_node, value in _constant_nodes(integer_side, enum_values, constexpr_values):
                if value != 0 and _in_ranges(value, ranges):
                    add("module-address-pointer-compare", constant_node)

    return [
        {"kind": kind, "line": line, "column": column}
        for kind, line, column in sorted(found, key=lambda item: (item[1], item[2], item[0]))
    ]


def _walk(node: dict):
    yield node
    for child in node.get("inner", ()):
        if isinstance(child, dict):
            yield from _walk(child)


def _language_for(path: Path, override: str | None) -> str:
    if override:
        return override
    lang = _read_header(path).get("lang")
    if lang in ("c99", "c++"):
        return lang
    if path.suffix.lower() == ".cpp":
        return "c++"
    return "c99"


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("paths", nargs="+", type=Path, help="source paths to analyze")
    parser.add_argument("--root", type=Path, help="checkout root (default: discovered from the source path)")
    parser.add_argument("--overlay", type=Path, help="host header overlay (default: ROOT/port/include)")
    parser.add_argument("--clang", type=Path, help="Clang executable (default: configured Windows Clang)")
    parser.add_argument("--lang", choices=("c99", "c++"), help="override language for every input path")
    parser.add_argument("--parallel", type=int, default=4, help="maximum concurrent Clang AST processes")
    parser.add_argument("--json", action="store_true", help="emit path and findings objects as JSON")
    args = parser.parse_args(argv)
    if args.parallel < 1:
        parser.error("--parallel must be at least 1")
    root = args.root.resolve() if args.root else None

    def analyze(path: Path) -> tuple[str, list[dict]]:
        lang = _language_for(path, args.lang)
        result = findings(path, lang, root=root, overlay=args.overlay, compiler=args.clang)
        return str(path), result

    try:
        with concurrent.futures.ThreadPoolExecutor(max_workers=args.parallel) as executor:
            results = list(executor.map(analyze, args.paths))
    except (OSError, ValueError, HstAddrError) as exc:
        print(f"hstaddr: {exc}", file=sys.stderr)
        return 2
    payload = [{"path": path, "findings": result} for path, result in results]
    if args.json:
        print(json.dumps(payload, indent=2, sort_keys=True))
    else:
        for item in payload:
            if not item["findings"]:
                print(f"{item['path']}: no numeric-address findings")
            for finding in item["findings"]:
                print(f"{item['path']}:{finding['line']}:{finding['column']}: {finding['kind']}")
    return 1 if any(item["findings"] for item in payload) else 0


if __name__ == "__main__":
    raise SystemExit(main())
