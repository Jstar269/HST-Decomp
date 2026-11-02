#!/usr/bin/env python3
"""Random-mutation search ("permuter") for near-miss generated sources.

    python tools/hstperm.py --tag p1 [--max-diff 8] [--rounds 12] [--mutants 16] [--parallel 16] [--limit N]

Seeds each unmatched function with its few best MISMATCH candidates from try/gen-*/summary-*.json, recompiled
and scored by alignment (words outside the longest common word alignment with retail, so an inserted
instruction costs one word), applies 1-3 random source mutations per mutant (swap adjacent statements,
reorder declarations, commute operands, flip comparisons, rewrite increments, change local signedness,
invert if/else, regroup float chains, inline temporaries, and (2026-11-02) introduce temporaries, add or remove
casts, x = x op y versus x op= y, a[i] versus *(a + i), move declarations, for/while/do-while, `register`,
commute struct-field reads), compiles every mutant with hstmatch.py and keeps
the best by that score (hill climbing; ties may drift). Mutations
need not preserve meaning: only a byte-exact build of the retail function is accepted, and it is written to
src/ with a provenance note.
"""
from __future__ import annotations

import argparse
import difflib
import glob
import json
import shutil
import struct
import random
import re
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hstmatch as M

SRC_DIRS = {"EBOOT.BIN": "eboot", "hstp_trans_main.prx": "sharing_main", "libfont.prx": "libfont",
            "scePsmfP_library.prx": "psmfp", "scePsmf_library.prx": "psmf", "hstp_trans_sceFont_Library.prx": "sharing_font"}
DETAIL = re.compile(r"(\d+)/(\d+) words; (\d+) differ")
SIMPLE = re.compile(r"^\s+[^{}\n]*;\s*$")
DECL = re.compile(r"^(\s+)((?:unsigned |signed )?\w+(?:\s*\*+)?)\s+(\w+(?:\s*\[\w+\])?);\s*$")
COMMUTE = re.compile(r"(?<![\w\]\)])(\w+|\(\w+\)) (\+|\*|&|\||\^|==|!=) (\w+|\(\w+\))(?![\w\(\[])")
COMPARE = re.compile(r"(?<![\w\]\)])(\w+) (<=|>=|<|>) (\w+)(?![\w\(\[])")
FLIP = {"<": ">", ">": "<", "<=": ">=", ">=": "<="}
SIGN = {"int": "uint", "uint": "int", "char": "uchar", "uchar": "char", "short": "ushort", "ushort": "short",
        "undefined4": "int", "undefined2": "short", "undefined1": "char"}


def body_bounds(lines: list[str]) -> tuple[int, int]:
    """Line range of the function body: after the opening brace that follows the signature."""
    start = next(i for i, line in enumerate(lines) if line.strip() == "{" or line.rstrip().endswith(") {")) + 1
    return start, len(lines)


def m_swap(lines, lo, hi, rng):
    cands = [i for i in range(lo, hi - 1) if SIMPLE.match(lines[i]) and SIMPLE.match(lines[i + 1])
             and "return" not in lines[i] + lines[i + 1]
             and len(lines[i]) - len(lines[i].lstrip()) == len(lines[i + 1]) - len(lines[i + 1].lstrip())]
    if not cands:
        return False
    i = rng.choice(cands)
    lines[i], lines[i + 1] = lines[i + 1], lines[i]
    return True


def m_decl(lines, lo, hi, rng):
    decls = [i for i in range(lo, hi) if DECL.match(lines[i]) and not re.search(r"\breturn\b", lines[i])]
    if len(decls) < 2:
        return False
    i, j = rng.sample(decls, 2)
    lines[i], lines[j] = lines[j], lines[i]
    return True


def m_regex(lines, lo, hi, rng, pattern, repl):
    cands = [(i, m) for i in range(lo, hi) for m in pattern.finditer(lines[i]) if not DECL.match(lines[i])]
    if not cands:
        return False
    i, m = rng.choice(cands)
    lines[i] = lines[i][:m.start()] + repl(m) + lines[i][m.end():]
    return True


def m_incdec(lines, lo, hi, rng):
    forms = []
    for i in range(lo, hi):
        line = lines[i]
        m = re.match(r"^(\s+)(\w+) = \2 \+ (-?\d+);\s*$", line)
        if m:
            n = int(m.group(3))
            alts = [f"{m.group(1)}{m.group(2)} += {n};"]
            if n == 1:
                alts.append(f"{m.group(1)}{m.group(2)}++;")
            if n == -1:
                alts.append(f"{m.group(1)}{m.group(2)}--;")
            if n < 0:
                alts.append(f"{m.group(1)}{m.group(2)} = {m.group(2)} - {-n};")
            forms.append((i, alts))
        m = re.search(r"(\w+) \+ -(\d+)", line)
        if m:
            forms.append((i, [line.replace(m.group(0), f"{m.group(1)} - {m.group(2)}", 1)]))
    if not forms:
        return False
    i, alts = rng.choice(forms)
    lines[i] = rng.choice(alts)
    return True


def m_sign(lines, lo, hi, rng):
    decls = [(i, DECL.match(lines[i])) for i in range(lo, hi) if DECL.match(lines[i])]
    decls = [(i, m) for i, m in decls if m.group(2).replace("*", "").strip() in SIGN]
    if not decls:
        return False
    i, m = rng.choice(decls)
    base = m.group(2).replace("*", "").strip()
    lines[i] = lines[i].replace(base, SIGN[base], 1)
    return True


def m_invert(lines, lo, hi, rng):
    """if (C) { A } else { B }  ->  if (!(C)) { B } else { A } (single-level brace matching)."""
    cands = []
    for i in range(lo, hi):
        m = re.match(r"^(\s+)if \((.*)\) \{\s*$", lines[i])
        if not m:
            continue
        depth, j = 1, i + 1
        while j < hi and depth:
            depth += lines[j].count("{") - lines[j].count("}")
            j += 1
        if j < hi + 1 and re.match(r"^\s+\} else \{\s*$", lines[j - 1]):
            depth, k = 1, j
            while k < hi and depth:
                depth += lines[k].count("{") - lines[k].count("}")
                k += 1
            cands.append((i, j - 1, k - 1, m.group(1), m.group(2)))
    if not cands:
        return False
    i, mid, end, indent, cond = rng.choice(cands)
    a, b = lines[i + 1:mid], lines[mid + 1:end]
    lines[i:end + 1] = [f"{indent}if (!({cond})) {{"] + b + [f"{indent}}} else {{"] + a + [f"{indent}}}"]
    return True


ASSIGN_STMT = re.compile(r"^(\s*[^=;]*?[^=!<>+\-*/%&|^]= )(.+);\s*$")


def split_top(expr: str, op: str) -> list[str]:
    """Split expr at top-level ` op ` (outside parentheses and brackets)."""
    parts, depth, start, i = [], 0, 0, 0
    token = f" {op} "
    while i < len(expr):
        ch = expr[i]
        if ch in "([":
            depth += 1
        elif ch in ")]":
            depth -= 1
        elif depth == 0 and expr.startswith(token, i):
            parts.append(expr[start:i])
            i += len(token)
            start = i
            continue
        i += 1
    parts.append(expr[start:])
    return parts


def m_reassoc(lines, lo, hi, rng):
    """a * b * c -> a * (b * c): Ghidra drops the parentheses of float product/sum chains, which are not
    associative, so the printed grouping can differ from the computed one."""
    cands = []
    for i in range(lo, hi):
        m = ASSIGN_STMT.match(lines[i])
        if not m:
            continue
        for op in ("*", "+"):
            parts = split_top(m.group(2), op)
            if len(parts) >= 3:
                cands.append((i, m, op, parts))
    if not cands:
        return False
    i, m, op, parts = rng.choice(cands)
    k = rng.randrange(1, len(parts) - 1)
    left = f" {op} ".join(parts[:k])
    right = f" {op} ".join(parts[k:])
    lines[i] = f"{m.group(1)}{left} {op} ({right});"
    return True


PURE = re.compile(r"^[\w\s+\-*<>&|^()]+$")


def m_inline(lines, lo, hi, rng):
    """t = <pure expression>; ... t ...  ->  ... (<expression>) ...: Ghidra hoists repeated subexpressions
    (array index products, field addresses) into temporaries the source wrote inline (0xee44c)."""
    cands = []
    for i in range(lo, hi):
        m = re.match(r"^\s+(\w+) = ([^;=]+);\s*$", lines[i])
        if not m or not PURE.match(m.group(2)) or re.search(r"\b\w+\s*\(", m.group(2)):
            continue
        var, expr = m.group(1), m.group(2).strip()
        names = set(re.findall(r"\b[A-Za-z_]\w*\b", expr))
        if var in names:
            continue
        later = lines[i + 1:hi]
        if any(re.match(rf"^\s+{var}\s*(?:[-+*/|&^]|<<|>>)?=(?!=)", ln) for ln in later):
            continue  # reassigned
        if any(re.match(rf"^\s+{n}\s*(?:[-+*/|&^]|<<|>>)?=(?!=)|.*\b{n}\s*(?:\+\+|--)", ln)
               for n in names for ln in later):
            continue  # an operand changes afterwards
        if not any(re.search(rf"\b{var}\b", ln) for ln in later):
            continue
        cands.append((i, var, expr))
    if not cands:
        return False
    i, var, expr = rng.choice(cands)
    simple = re.fullmatch(r"\w+", expr) is not None
    rep = expr if simple else f"({expr})"
    for j in range(i + 1, hi):
        lines[j] = re.sub(rf"\b{var}\b", rep, lines[j])
    del lines[i]
    return True


BOUND = re.compile(r"(?<![\w\]\)])(\w+) (<|<=|>|>=) (-?(?:0x[0-9a-f]+|\d+))(?![\w.])")


def m_bound(lines, lo, hi, rng):
    """i < C  <->  i <= C-1 (and > / >=): Ghidra normalises bounds, but MWCC picks different compare
    registers for the two spellings (slti at vs v0, 0x1afcd4)."""
    cands = [(i, m) for i in range(lo, hi) for m in BOUND.finditer(lines[i]) if not DECL.match(lines[i])]
    if not cands:
        return False
    i, m = rng.choice(cands)
    var, op, lit = m.groups()
    n = int(lit, 0)
    new = {"<": ("<=", n - 1), "<=": ("<", n + 1), ">": (">=", n + 1), ">=": (">", n - 1)}[op]
    text = hex(new[1]) if lit.lower().startswith(("0x", "-0x")) and new[1] >= 0 else str(new[1])
    lines[i] = lines[i][:m.start()] + f"{var} {new[0]} {text}" + lines[i][m.end():]
    return True


UCMP = re.compile(r"(?<![\w\])])([\w*]+(?: [-+] (?:0x[0-9a-f]+|\d+))?) (<|<=|>|>=) (0x[0-9a-f]+|\d+)\b")


def m_ucmp(lines, lo, hi, rng):
    """x - 4 < 2  ->  (unsigned int)(x - 4) < 2: Ghidra drops the unsigned cast of range checks (sltiu vs
    slti, 0x1a4824)."""
    cands = [(i, m) for i in range(lo, hi) for m in UCMP.finditer(lines[i]) if not DECL.match(lines[i])]
    if not cands:
        return False
    i, m = rng.choice(cands)
    lines[i] = lines[i][:m.start()] + f"(unsigned int)({m.group(1)}) {m.group(2)} {m.group(3)}" + lines[i][m.end():]
    return True


NEG = {"<": ">=", ">=": "<", ">": "<=", "<=": ">", "==": "!=", "!=": "=="}


def negate(cond: str) -> str:
    m = re.fullmatch(r"\s*([\w*.\[\]()+ -]+?) (<|<=|>|>=|==|!=) ([\w*.\[\]()+ -]+?)\s*", cond)
    if m and m.group(1).count("(") == m.group(1).count(")") and m.group(3).count("(") == m.group(3).count(")"):
        return f"{m.group(1)} {NEG[m.group(2)]} {m.group(3)}"
    return f"!({cond})"


def chain_arms(lines, i, hi):
    """Parse a Ghidra if / else if / else chain starting at line i: ([(cond, body)], else_body, end)."""
    arms, j = [], i
    m = re.match(r"^(\s+)if \((.*)\) \{\s*$", lines[j])
    if not m:
        return None
    indent = m.group(1)
    cond = m.group(2)
    while True:
        depth, k = 1, j + 1
        while k < hi and depth:
            depth += lines[k].count("{") - lines[k].count("}")
            k += 1
        if depth:
            return None
        arms.append((cond, lines[j + 1:k - 1]))
        nxt = lines[k] if k < hi else ""
        if lines[k - 1].strip() == "}" and re.match(rf"^{indent}else if \((.*)\) \{{\s*$", nxt):
            cond = re.match(rf"^{indent}else if \((.*)\) \{{\s*$", nxt).group(1)
            j = k
            continue
        if lines[k - 1].strip() == "}" and re.match(rf"^{indent}else \{{\s*$", nxt):
            depth, e = 1, k + 1
            while e < hi and depth:
                depth += lines[e].count("{") - lines[e].count("}")
                e += 1
            return arms, lines[k + 1:e - 1], e, indent
        return None


def m_chain_merge(lines, lo, hi, rng):
    """if (A) {E} else if (B) {X} else {E}  ->  if (!A && B) {X} else {E}: Ghidra splits range checks into
    chains that repeat the else body (0x1a47e8, 0xb13c4)."""
    cands = []
    for i in range(lo, hi):
        parsed = chain_arms(lines, i, len(lines))
        if not parsed:
            continue
        arms, else_body, end, indent = parsed
        for a in range(len(arms) - 1):
            if [s.strip() for s in arms[a][1]] == [s.strip() for s in else_body]:
                cands.append((i, arms, else_body, end, indent, a))
    if not cands:
        return False
    i, arms, else_body, end, indent, a = rng.choice(cands)
    merged = (f"{negate(arms[a][0])} && ({arms[a + 1][0]})", arms[a + 1][1])
    arms = arms[:a] + [merged] + arms[a + 2:]
    out = []
    for n, (cond, body) in enumerate(arms):
        out.append(f"{indent}{'if' if n == 0 else 'else if'} ({cond}) {{")
        out += body
        out.append(f"{indent}}}")
    out += [f"{indent}else {{"] + else_body + [f"{indent}}}"]
    lines[i:end] = out
    return True


def m_ternary(lines, lo, hi, rng):
    """if (C) { v = A; } else { v = B; }  ->  v = C ? A : B; (0xb13c4)"""
    cands = []
    for i in range(lo, hi - 4):
        m = re.match(r"^(\s+)if \((.*)\) \{\s*$", lines[i])
        a = re.match(r"^\s+(\w+) = (.+);\s*$", lines[i + 1])
        b = re.match(r"^\s+(\w+) = (.+);\s*$", lines[i + 4]) if i + 4 < hi else None
        if (m and a and b and lines[i + 2].strip() == "}" and lines[i + 3].strip() == "else {"
                and lines[i + 5].strip() == "}" and a.group(1) == b.group(1)):
            cands.append((i, m.group(1), m.group(2), a.group(1), a.group(2), b.group(2)))
    if not cands:
        return False
    i, indent, cond, var, x, y = rng.choice(cands)
    lines[i:i + 6] = [f"{indent}{var} = ({cond}) ? {x} : {y};"]
    return True


def m_hoist(lines, lo, hi, rng):
    """if (C) { T } else { X; T } -> if (!C) { X } T (Ghidra's duplicated tails; hstgen.hoist_common_tails)."""
    import hstgen
    body = "\n".join(lines[lo:])
    new = hstgen.hoist_common_tails(body)
    if new == body:
        return False
    lines[lo:] = new.split("\n")
    return True


INNER_SUM = re.compile(r"\(([^()]+)\)")


def m_sumorder(lines, lo, hi, rng):
    """(base + i * 4 + 0x3c) -> (base + 0x3c + i * 4): the order of address terms decides operand order and
    strength reduction; struct member arrays put the field offset before the index (0x2535f0)."""
    cands = []
    for i in range(lo, hi):
        if DECL.match(lines[i]):
            continue
        for m in INNER_SUM.finditer(lines[i]):
            parts = m.group(1).split(" + ")
            if len(parts) >= 3:
                cands.append((i, m, parts))
    if not cands:
        return False
    i, m, parts = rng.choice(cands)
    rest = parts[1:]
    rng.shuffle(rest)
    if rest == parts[1:]:
        rest = rest[1:] + rest[:1]
    lines[i] = lines[i][:m.start()] + "(" + " + ".join([parts[0]] + rest) + ")" + lines[i][m.end():]
    return True


# ---- operators added 2026-11-02 (decomp-permuter style); each keeps the text a single readable C function ----
KEYWORDS = frozenset("if else for while do return switch case default goto break continue sizeof int unsigned signed char "
                     "short long void struct union enum const static register volatile float double".split())
TYPE_WORDS = frozenset("int unsigned signed char short long uint ushort uchar byte undefined undefined1 undefined2 "
                       "undefined4 undefined8 float double void struct const".split())
CAST = re.compile(r"\((?:unsigned int|unsigned char|unsigned short|int|uint|short|ushort|char|uchar|byte|undefined[124])\) ?")
STMT = re.compile(r"^(\s+)(?!return\b|for\b|while\b|if\b|else\b|do\b|switch\b|case\b)[^{};]*;\s*$")


def is_decl(line: str) -> bool:
    """A local declaration (DECL also matches `return a[i];`)."""
    return bool(DECL.match(line)) and not re.match(r"\s+return\b", line)


def indent_of(line: str) -> int:
    return len(line) - len(line.lstrip())


def block_end(lines, i, hi):
    """Index of the line holding the brace that closes the block opened on line i, or None."""
    depth = 0
    for k in range(i, min(hi, len(lines))):
        depth += lines[k].count("{") - lines[k].count("}")
        if depth == 0 and k > i:
            return k
    return None


def m_temp(lines, lo, hi, rng):
    """... (A + B) ... -> int tN = A + B; ... tN ...: a temporary for a parenthesised, call-free sub-expression."""
    cands = []
    for i in range(max(lo, 1), hi):
        ln = lines[i]
        if not STMT.match(ln) or is_decl(ln) or lines[i - 1].strip() not in ("",) and not lines[i - 1].rstrip().endswith(("{", ";", "}")):
            continue
        for m in INNER_SUM.finditer(ln):
            inner = m.group(1).strip()
            if (" " in inner and PURE.match(inner) and not CAST.fullmatch("(" + inner + ")") and "," not in inner
                    and not re.search(r"[\w\]]$", ln[:m.start()].rstrip()[-1:])
                    and not ln[:m.start()].rstrip().endswith(("sizeof", "-"))):
                cands.append((i, m, inner))
    if not cands:
        return False
    i, m, inner = rng.choice(cands)
    text = "\n".join(lines)
    n = 0
    while re.search(rf"\bt{n}\b", text):
        n += 1
    ind = lines[i][:indent_of(lines[i])]
    lines[i] = lines[i][:m.start()] + f"t{n}" + lines[i][m.end():]
    lines.insert(i, f"{ind}int t{n} = {inner};")
    return True


def m_cast(lines, lo, hi, rng):
    """Add or remove a value-preserving cast on an operand."""
    removals = [(i, m) for i in range(lo, hi) for m in CAST.finditer(lines[i]) if not is_decl(lines[i])]
    adds = []
    for i in range(lo, hi):
        ln = lines[i]
        if is_decl(ln) or not STMT.match(ln) or "=" not in ln:
            continue
        rhs = ln.index("=") + 1
        for m in re.finditer(r"(?<![\w.>&*(])([A-Za-z_]\w*)(?![\w(\[.>])", ln):
            if (m.start() >= rhs and m.group(1) not in KEYWORDS and m.group(1) not in TYPE_WORDS
                    and ln[:m.start()].count("(") <= ln[:m.start()].count(")")):
                adds.append((i, m))
    if not removals and not adds:
        return False
    if removals and (not adds or rng.random() < 0.5):
        i, m = rng.choice(removals)
        lines[i] = lines[i][:m.start()] + lines[i][m.end():]
    else:
        i, m = rng.choice(adds)
        t = rng.choice(("int", "unsigned int", "int"))
        lines[i] = lines[i][:m.start()] + f"({t}){m.group(1)}" + lines[i][m.end():]
    return True


COMPOUND = re.compile(r"(?<![\w.\]>])(\w+) = \1 (\+|-|\*|\||&|\^|<<|>>) ([^;)]+?)(?=;|\))")
COMPOUND_ASSIGN = re.compile(r"(?<![\w.\]>])(\w+) (\+|-|\*|\||&|\^|<<|>>)= ([^;)]+?)(?=;|\))")


def m_compound(lines, lo, hi, rng):
    """x = x op y  <->  x op= y"""
    cands = []
    for i in range(lo, hi):
        if is_decl(lines[i]):
            continue
        cands += [(i, m, True) for m in COMPOUND.finditer(lines[i])]
        cands += [(i, m, False) for m in COMPOUND_ASSIGN.finditer(lines[i])]
    if not cands:
        return False
    i, m, forward = rng.choice(cands)
    if forward:
        new = f"{m.group(1)} {m.group(2)}= {m.group(3)}"
    else:
        y = m.group(3)
        if re.search(r" (\+|-|\||&|\^|<<|>>|\?) ", y) and not (y.startswith("(") and y.endswith(")")):
            y = f"({y})"
        new = f"{m.group(1)} = {m.group(1)} {m.group(2)} {y}"
    lines[i] = lines[i][:m.start()] + new + lines[i][m.end():]
    return True


INDEX = re.compile(r"(?<![\w.>])([A-Za-z_]\w*)\[(\w+)\]")
DEREF_SUM = re.compile(r"\*\(([A-Za-z_]\w*) \+ (\w+)\)")


def m_index(lines, lo, hi, rng):
    """a[i] <-> *(a + i)"""
    cands = []
    for i in range(lo, hi):
        if is_decl(lines[i]):
            continue
        cands += [(i, m, True) for m in INDEX.finditer(lines[i])]
        cands += [(i, m, False) for m in DEREF_SUM.finditer(lines[i])]
    if not cands:
        return False
    i, m, forward = rng.choice(cands)
    new = f"*({m.group(1)} + {m.group(2)})" if forward else f"{m.group(1)}[{m.group(2)}]"
    lines[i] = lines[i][:m.start()] + new + lines[i][m.end():]
    return True


def m_decl_scope(lines, lo, hi, rng):
    """Move a declaration to just before its first use (or into the block that holds every use), or up to the top."""
    base = min((indent_of(lines[i]) for i in range(lo, hi) if lines[i].strip() and indent_of(lines[i])), default=0)
    cands = []
    for i in range(lo, hi):
        m = DECL.match(lines[i]) if is_decl(lines[i]) else None
        if not m:
            continue
        var = re.match(r"\w+", m.group(3)).group(0)
        uses = [k for k in range(lo, hi) if k != i and re.search(rf"\b{var}\b", lines[k])]
        if not uses:
            continue
        d, first = indent_of(lines[i]), uses[0]
        if indent_of(lines[first]) == d and first > i + 1:
            cands.append(("sink", i, first, d))
        elif indent_of(lines[first]) > d and first > i:
            k = first
            while k > i and not (lines[k].rstrip().endswith("{") and indent_of(lines[k]) < indent_of(lines[first])):
                k -= 1
            e = block_end(lines, k, hi) if k > i else None
            if e is not None and all(k < u < e for u in uses):
                cands.append(("into", i, k + 1, indent_of(lines[k + 1])))
        if d > base:
            cands.append(("up", i, lo, base))
    if not cands:
        return False
    kind, i, at, ind = rng.choice(cands)
    text = " " * ind + lines[i].strip()
    del lines[i]
    if at > i:
        at -= 1
    lines.insert(at, text)
    return True


FOR_HDR = re.compile(r"^(\s*)for \((.*?);(.*?);(.*?)\) \{\s*$")
WHILE_HDR = re.compile(r"^(\s*)while \((.+)\) \{\s*$")


def m_loop_form(lines, lo, hi, rng):
    """for <-> while <-> if + do-while with an equivalent guard."""
    cands = []
    for i in range(lo, hi):
        e = block_end(lines, i, hi) if lines[i].rstrip().endswith("{") else None
        if e is None:
            continue
        if any(re.search(r"\bcontinue\b", b) for b in lines[i + 1:e]):
            continue
        if FOR_HDR.match(lines[i]):
            cands.append(("for2while", i, e))
        elif WHILE_HDR.match(lines[i]):
            cands += [("while2for", i, e), ("while2do", i, e)]
    if not cands:
        return False
    kind, i, e = rng.choice(cands)
    if kind == "for2while":
        ind, init, cond, step = FOR_HDR.match(lines[i]).groups()
        init, cond, step = init.strip(), cond.strip() or "1", step.strip()
        head = ([f"{ind}{init};"] if init else []) + [f"{ind}while ({cond}) {{"]
        tail = ([f"{ind}  {step};"] if step else []) + [lines[e]]
        lines[i:e + 1] = head + lines[i + 1:e] + tail
    elif kind == "while2for":
        ind, cond = WHILE_HDR.match(lines[i]).groups()
        lines[i] = f"{ind}for (; {cond}; ) {{"
    else:
        ind, cond = WHILE_HDR.match(lines[i]).groups()
        body = [f"  {b}" if b.strip() else b for b in lines[i + 1:e]]
        lines[i:e + 1] = ([f"{ind}if ({cond}) {{", f"{ind}  do {{"] + body +
                          [f"{ind}  }} while ({cond});", f"{ind}}}"])
    return True


def m_register(lines, lo, hi, rng):
    """Add or remove `register` on a local declaration."""
    plain = [i for i in range(lo, hi) if is_decl(lines[i]) and "register" not in lines[i] and "[" not in lines[i]]
    regs = [i for i in range(lo, hi) if re.match(r"^\s+register ", lines[i])]
    if not plain and not regs:
        return False
    if regs and (not plain or rng.random() < 0.4):
        i = rng.choice(regs)
        lines[i] = lines[i].replace("register ", "", 1)
    else:
        i = rng.choice(plain)
        lines[i] = lines[i][:indent_of(lines[i])] + "register " + lines[i].lstrip()
    return True


FIELD = r"(?:\w+(?:->|\.))+\w+|\w+"
FIELD_COMMUTE = re.compile(rf"(?<![\w\]\)>.])({FIELD}) (\+|\*|&|\||\^|==|!=) ({FIELD})(?![\w(\[.])")


def m_field_commute(lines, lo, hi, rng):
    """a->x + b->y -> b->y + a->x: reorders independent struct-field reads (COMMUTE only sees plain names)."""
    return m_regex(lines, lo, hi, rng, FIELD_COMMUTE, lambda m: f"{m.group(3)} {m.group(2)} {m.group(1)}")


NEW_MUTATIONS = [(2, m_temp), (2, m_cast), (2, m_compound), (2, m_index), (2, m_decl_scope), (2, m_loop_form),
                 (1, m_register), (2, m_field_commute)]

MUTATIONS = [
    (3, m_swap), (2, m_decl), (2, lambda l, lo, hi, r: m_regex(l, lo, hi, r, COMMUTE, lambda m: f"{m.group(3)} {m.group(2)} {m.group(1)}")),
    (2, lambda l, lo, hi, r: m_regex(l, lo, hi, r, COMPARE, lambda m: f"{m.group(3)} {FLIP[m.group(2)]} {m.group(1)}")),
    (1, m_incdec), (2, m_sign), (2, m_invert), (2, m_reassoc), (2, m_inline), (2, m_sumorder), (2, m_bound),
    (1, m_ucmp), (2, m_chain_merge), (1, m_ternary), (1, m_hoist),
] + NEW_MUTATIONS


def mutate(text: str, rng: random.Random) -> str | None:
    lines = text.split("\n")
    try:
        lo, hi = body_bounds(lines)
    except StopIteration:
        return None
    applied = 0
    for _ in range(rng.choice((1, 1, 2, 3))):
        weights = [w for w, _ in MUTATIONS]
        fn = rng.choices([f for _, f in MUTATIONS], weights)[0]
        if fn(lines, lo, len(lines), rng):
            applied += 1
    return "\n".join(lines) if applied else None


def write_accepted(tag: str, key: tuple[str, int], path: Path, origin: str) -> int:
    module, start = key
    dest_dir = M.ROOT / "src" / SRC_DIRS[module]
    if any(M.parse_header(p)["start"] == start for p in dest_dir.glob("*") if p.suffix in (".c", ".cpp")):
        return 0
    text = path.read_text(encoding="utf-8")
    head, rest = text.split("\n", 1)
    rest = re.sub(r"^/\*\s*Mechanical translation.*?\*/\n", "", rest, count=1)
    gen = re.search(r"gen-(\w+)[\\/]", origin)
    note = (f"/* Mechanical translation of Ghidra pseudocode (hstgen.py tag {gen.group(1) if gen else 'unknown'}), "
            f"refined by hstperm.py tag {tag} random mutation from {origin}; exact match verified; names and types "
            "await human review. */\n")
    out = dest_dir / (re.sub(r"\.(c|cpp)$", "", Path(origin).name) + path.suffix)
    if out.exists():
        return 0
    out.write_text(head + "\n" + note + rest, encoding="utf-8", newline="\n")
    return 1


def score(status: str, detail: str) -> int:
    """Positional score from an hstmatch row (used only to pick seed candidates)."""
    if status == "EXACT":
        return 0
    m = DETAIL.match(detail or "")
    if status == "MISMATCH" and m:
        ours, n, d = map(int, m.groups())
        return d if ours == n else 1000 + abs(ours - n) * 10 + d
    return 10_000


_RETAIL: dict = {}


def retail_words(key: tuple[str, int], end: int) -> list[int]:
    module, start = key
    data, seg = M.module_bytes(_RETAIL, module)
    off = seg["file_offset"] + start - seg["start"]
    raw = data[off:off + end - start]
    return list(struct.unpack(f"<{len(raw) // 4}I", raw[:len(raw) // 4 * 4]))


def align_score(row: dict, theirs: list[int]) -> int:
    """Words not covered by the longest common word alignment with retail: one inserted or missing
    instruction costs one, not every word after it. 0 only for an EXACT row."""
    if row["status"] == "EXACT":
        return 0
    if row["status"] != "MISMATCH":
        return 10_000
    src = M.ROOT / row["source"]
    text = M.ROOT / "build" / row["module"] / M.build_key(src) / "text.bin"
    if not text.exists():
        return 10_000
    raw = text.read_bytes()
    ours = list(struct.unpack(f"<{len(raw) // 4}I", raw[:len(raw) // 4 * 4]))
    sm = difflib.SequenceMatcher(None, ours, theirs, autojunk=False)
    return max(1, max(len(ours), len(theirs)) - sum(b.size for b in sm.get_matching_blocks()))


def run_scored(cmd: list[str], files: list[Path], part_dir: Path, ends: dict, owner: dict) -> dict:
    """Compile and score files; returns {resolved path: alignment score}."""
    results = {}
    crashed = unscored = 0
    for c in range(0, len(files), 400):
        todo = [str(p) for p in files[c:c + 400]]
        for attempt in range(2):
            part = part_dir / f"summary-{c // 400}-{attempt}.json"
            done = subprocess.run(cmd + [str(part)] + todo, stdout=subprocess.DEVNULL)
            if done.returncode == 3 or not part.exists():
                raise SystemExit(f"hstmatch stopped (exit {done.returncode}): wsl.exe or the compiler machinery failed; "
                                 "not scoring this part. Re-run after WSL answers again.")
            again = []
            for row in json.loads(part.read_text(encoding="utf-8")):
                path = str((M.ROOT / row["source"]).resolve())
                if row["status"] == "COMPILE_CRASHED":
                    # the compiler process died (not a diagnostic): one more compile of exactly these files, counted
                    crashed += attempt == 0
                    if attempt == 0:
                        again.append(path)
                        continue
                    unscored += 1
                    continue
                key = owner[path]
                results[path] = align_score(row, retail_words(key, ends[key]))
            todo = again
            if not todo:
                break
    if crashed:
        print(f"compiler crashes: {crashed} compiled again, {unscored} still crashed and left unscored", flush=True)
    return results


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--tag", required=True)
    ap.add_argument("--max-diff", type=int, default=8)
    ap.add_argument("--rounds", type=int, default=12)
    ap.add_argument("--mutants", type=int, default=16)
    ap.add_argument("--parallel", type=int, default=16)
    ap.add_argument("--limit", type=int, default=0)
    ap.add_argument("--pre-diff", type=int, default=0,
                    help="seed only targets whose best recorded mismatch differs in at most this many words")
    ap.add_argument("--seed", type=int, default=1)
    ap.add_argument("--seeds", type=int, default=3, help="seed candidates per target, recompiled and aligned")
    ap.add_argument("--max-words", type=int, default=400, help="skip retail functions longer than this")
    ap.add_argument("--start-min", type=lambda s: int(s, 0), default=0)
    ap.add_argument("--start-max", type=lambda s: int(s, 0), default=0xFFFFFFFF)
    args = ap.parse_args()
    rng = random.Random(args.seed)
    covered = {(M.parse_header(p)["module"], M.parse_header(p)["start"])
               for p in (M.ROOT / "src").rglob("*") if p.suffix in (".c", ".cpp")}
    work = M.ROOT / "try" / f"perm-{args.tag}"
    if work.exists():
        raise SystemExit(f"{work} exists; use a new tag")
    # Seeds: each target's few best generated MISMATCH candidates (any length), recompiled under unique
    # names (generation runs share build directories) and scored by alignment.
    pool: dict[tuple[str, int], dict[str, int]] = {}
    for summary in glob.glob(str(M.ROOT / "try" / "gen-*" / "summary-*.json")):
        for row in json.loads(Path(summary).read_text(encoding="utf-8")):
            key = (row["module"], int(row["start"], 16))
            if key in covered or row["status"] != "MISMATCH" or not args.start_min <= key[1] <= args.start_max:
                continue
            m = DETAIL.match(row.get("detail") or "")
            if not m or int(m.group(2)) > args.max_words:
                continue
            if (M.ROOT / row["source"]).exists():
                pool.setdefault(key, {})[row["source"]] = score(row["status"], row.get("detail", ""))
    if args.pre_diff:  # recompiling every seed of thousands of targets takes hours
        pool = {k: v for k, v in pool.items() if min(v.values()) <= args.pre_diff}
    seed_files, owner, ends, origin = [], {}, {}, {}
    seen_text: dict = {}
    for idx, key in enumerate(sorted(pool)):
        for k, source in enumerate(sorted(pool[key], key=pool[key].get)[:args.seeds]):
            src = M.ROOT / source
            text = src.read_text(encoding="utf-8")
            if text in seen_text.setdefault(key, set()):
                continue
            seen_text[key].add(text)
            ends[key] = M.parse_header(src)["end"]
            path = work / "seed" / f"{args.tag}_s{idx:05d}_{k}{src.suffix}"
            path.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(src, path)
            seed_files.append(path)
            owner[str(path.resolve())] = key
            origin[str(path.resolve())] = source
    cmd = [sys.executable, str(Path(M.__file__)), "--quiet", "--parallel", str(args.parallel), "--json"]
    print(f"seeding targets={len(pool)} seed_files={len(seed_files)}", flush=True)
    seeded = run_scored(cmd, seed_files, work / "seed", ends, owner)
    best: dict[tuple[str, int], tuple[int, str, str]] = {}
    for path_text, s in seeded.items():
        key = owner[path_text]
        if s <= args.max_diff and (key not in best or s < best[key][0]):
            best[key] = (s, Path(path_text).read_text(encoding="utf-8"), origin[path_text])
    keys = sorted(best, key=lambda k: best[k][0])
    if args.limit:
        keys = keys[:args.limit]
    state = {k: [best[k][0], best[k][1], best[k][2]] for k in keys}
    print(f"targets={len(state)} by_diff=" + json.dumps({d: sum(1 for v in state.values() if v[0] == d) for d in range(1, args.max_diff + 1)}), flush=True)
    accepted = {}
    written_keys: set = set()
    written = 0
    for rnd in range(args.rounds):
        active = [k for k in keys if k not in accepted]
        if not active:
            break
        files, owner = [], {}
        for idx, key in enumerate(active):
            seen = set()
            for k in range(args.mutants):
                text = mutate(state[key][1], rng)
                if text is None or text in seen or text == state[key][1]:
                    continue
                seen.add(text)
                suffix = ".cpp" if "lang=c++" in text.split("\n", 1)[0] else ".c"
                path = work / f"r{rnd:02d}" / f"{args.tag}_f{idx:04d}_{key[1]:08x}_m{k:02d}{suffix}"
                path.parent.mkdir(parents=True, exist_ok=True)
                path.write_text(text, encoding="utf-8", newline="\n")
                files.append(path)
                owner[str(path.resolve())] = key
        results = run_scored(cmd, files, work / f"r{rnd:02d}", ends, owner)
        improved = 0
        for path_text, s in results.items():
            key = owner[path_text]
            if key in accepted:
                continue
            if s == 0:
                accepted[key] = Path(path_text)
            elif s < state[key][0] or (s == state[key][0] and rng.random() < 0.3):
                improved += s < state[key][0]
                state[key][0] = s
                state[key][1] = Path(path_text).read_text(encoding="utf-8")
        # write this round's exact results right away, so a stopped run loses nothing
        for key in [k for k in accepted if k not in written_keys]:
            written_keys.add(key)
            written += write_accepted(args.tag, key, accepted[key], state[key][2])
        print(f"round={rnd} mutants={len(files)} exact_total={len(accepted)} improved={improved} written={written}", flush=True)
    # keep each unfinished target's best variant, so agents can start from it
    best_dir = work / "best"
    best_dir.mkdir(exist_ok=True)
    best_index = {}
    for key, (score_, text, origin) in state.items():
        if key in accepted:
            continue
        suffix = ".cpp" if "lang=c++" in text.split("\n", 1)[0] else ".c"
        path = best_dir / f"{key[0]}_{key[1]:08x}{suffix}"
        path.write_text(text, encoding="utf-8", newline="\n")
        best_index[f"{key[0]}:0x{key[1]:08x}"] = {"score": score_, "source": path.relative_to(M.ROOT).as_posix(),
                                                  "origin": origin}
    (work / "best.json").write_text(json.dumps(best_index, indent=1) + "\n", encoding="utf-8")
    report = {"tag": args.tag, "targets": len(state), "accepted": len(accepted), "written": written,
              "remaining_by_diff": {d: sum(1 for k, v in state.items() if k not in accepted and v[0] == d)
                                    for d in range(1, args.max_diff + 1)}}
    (work / "report.json").write_text(json.dumps(report, indent=1) + "\n", encoding="utf-8")
    print(json.dumps(report))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
