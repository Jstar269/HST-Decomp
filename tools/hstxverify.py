#!/usr/bin/env python3
"""Second implementation of byte verification for hstmatch function artifacts.

This module deliberately does not import hstmatch. It parses ELF headers, sections,
symbols, relocations, source headers, reviewed ranges and symbol maps itself.
"""

from __future__ import annotations

import argparse
import concurrent.futures
import hashlib
import json
import os
import re
import shlex
import struct
import subprocess
import sys
import time
from dataclasses import dataclass
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(Path(__file__).resolve().parent))
import hstconfig as CFG  # path lookup only


def build_key(path: Path) -> str:
    """hstmatch.py's build directory name for a source (`<folder>__<stem>__<digest>`), written out again here because
    this module must not import hstmatch; tools/test_hstxverify.py checks the two agree."""
    p = Path(path).resolve()
    try:
        ident = p.relative_to(ROOT).as_posix()
    except ValueError:
        ident = p.as_posix()
    return f"{p.parent.name}__{p.stem}__{hashlib.sha1(os.path.normcase(ident).encode('utf-8')).hexdigest()[:10]}"


LOAD = 1
SHT_SYMTAB = 2
SHT_STRTAB = 3
SHT_RELA = 4
SHT_NOBITS = 8
SHT_REL = 9
SHT_DYNSYM = 11
SHF_ALLOC = 2
TEXT_DATA_SECTIONS = (".rodata", ".data", ".sdata", ".sbss", ".bss")
GCC_OPTIMIZATIONS = ("O1", "O2", "O3", "Os")


class VerificationRefusal(Exception):
    """The input is not eligible for this verifier's proof."""


class ELFFormatError(Exception):
    """An ELF image is malformed or outside the supported minimal ELF32 format."""


@dataclass(frozen=True)
class ProgramHeader:
    kind: int
    offset: int
    vaddr: int
    paddr: int
    filesz: int
    memsz: int
    flags: int
    align: int


@dataclass(frozen=True)
class Section:
    index: int
    name: str
    kind: int
    flags: int
    addr: int
    offset: int
    size: int
    link: int
    info: int
    align: int
    entsize: int


@dataclass(frozen=True)
class Symbol:
    name: str
    value: int
    size: int
    info: int
    other: int
    shndx: int


class ELFImage:
    """Small ELF32 little-endian parser with address-to-file mapping via PT_LOAD."""

    def __init__(self, data: bytes, label: str = "ELF") -> None:
        self.data = data
        self.label = label
        if len(data) < 52 or data[:4] != b"\x7fELF":
            raise ELFFormatError(f"{label}: missing ELF32 header")
        ident = data[:16]
        if ident[4] != 1 or ident[5] != 1 or ident[6] != 1:
            raise ELFFormatError(f"{label}: expected ELF32 little-endian version 1")
        (
            self.etype,
            self.machine,
            version,
            self.entry,
            phoff,
            shoff,
            self.flags,
            ehsize,
            phentsize,
            phnum,
            shentsize,
            shnum,
            shstrndx,
        ) = struct.unpack_from("<HHIIIIIHHHHHH", data, 16)
        if version != 1 or ehsize < 52:
            raise ELFFormatError(f"{label}: invalid ELF header version or size")
        if self.machine != 8:
            raise ELFFormatError(
                f"{label}: expected MIPS machine 8, got {self.machine}"
            )
        if phnum and (phentsize < 32 or phoff + phentsize * phnum > len(data)):
            raise ELFFormatError(f"{label}: program-header table is out of bounds")
        if shnum and (shentsize < 40 or shoff + shentsize * shnum > len(data)):
            raise ELFFormatError(f"{label}: section-header table is out of bounds")
        self.program_headers: list[ProgramHeader] = []
        for index in range(phnum):
            values = struct.unpack_from("<IIIIIIII", data, phoff + index * phentsize)
            header = ProgramHeader(*values)
            if header.offset + header.filesz > len(data):
                raise ELFFormatError(
                    f"{label}: program segment {index} is out of bounds"
                )
            if (
                header.filesz > header.memsz
                or header.vaddr + header.memsz > 0x100000000
            ):
                raise ELFFormatError(
                    f"{label}: program segment {index} has invalid memory bounds"
                )
            self.program_headers.append(header)
        raw_sections = []
        for index in range(shnum):
            raw_sections.append(
                struct.unpack_from("<IIIIIIIIII", data, shoff + index * shentsize)
            )
        self.sections: list[Section] = []
        if raw_sections:
            if shstrndx >= len(raw_sections):
                raise ELFFormatError(f"{label}: invalid section-name table index")
            names_header = raw_sections[shstrndx]
            if names_header[1] != SHT_STRTAB:
                raise ELFFormatError(
                    f"{label}: section-name table is not a string table"
                )
            names = self._slice(names_header[4], names_header[5], "section-name table")
            for index, raw in enumerate(raw_sections):
                name = self._string(names, raw[0], "section name")
                section = Section(
                    index,
                    name,
                    raw[1],
                    raw[2],
                    raw[3],
                    raw[4],
                    raw[5],
                    raw[6],
                    raw[7],
                    raw[8],
                    raw[9],
                )
                if section.kind != SHT_NOBITS and section.size:
                    self._slice(section.offset, section.size, f"section {name}")
                self.sections.append(section)
        self._symbol_tables: dict[int, list[Symbol]] = {}

    def _slice(self, offset: int, size: int, what: str) -> bytes:
        if offset < 0 or size < 0 or offset + size > len(self.data):
            raise ELFFormatError(f"{self.label}: {what} is out of bounds")
        return self.data[offset : offset + size]

    @staticmethod
    def _string(table: bytes, offset: int, what: str) -> str:
        if offset < 0 or offset >= len(table):
            raise ELFFormatError(f"invalid {what} offset")
        end = table.find(b"\0", offset)
        if end < 0:
            raise ELFFormatError(f"unterminated {what}")
        return table[offset:end].decode("utf-8", errors="replace")

    def section(self, name: str) -> Section | None:
        found = [section for section in self.sections if section.name == name]
        if len(found) > 1:
            raise ELFFormatError(f"{self.label}: duplicate section {name}")
        return found[0] if found else None

    def section_bytes(self, section: Section) -> bytes:
        if section.kind == SHT_NOBITS:
            raise ELFFormatError(
                f"{self.label}: section {section.name} has no file bytes"
            )
        return self._slice(section.offset, section.size, f"section {section.name}")

    def file_offset(self, address: int, size: int) -> int:
        if size < 0 or address < 0 or address + size > 0x100000000:
            raise ELFFormatError(
                f"{self.label}: invalid address range 0x{address:x}+0x{size:x}"
            )
        candidates = []
        for segment in self.program_headers:
            if segment.kind != LOAD:
                continue
            if (
                address >= segment.vaddr
                and address + size <= segment.vaddr + segment.filesz
            ):
                candidates.append(segment.offset + address - segment.vaddr)
        if not candidates:
            raise ELFFormatError(
                f"{self.label}: 0x{address:08x}+0x{size:x} is not file-backed by PT_LOAD"
            )
        if len(set(candidates)) != 1:
            raise ELFFormatError(
                f"{self.label}: address range has ambiguous PT_LOAD mappings"
            )
        return candidates[0]

    def bytes_at(self, address: int, size: int) -> bytes:
        return self._slice(
            self.file_offset(address, size), size, "mapped address range"
        )

    def symbols(self, table_index: int | None = None) -> list[Symbol]:
        tables = [
            s
            for s in self.sections
            if s.kind in (SHT_SYMTAB, SHT_DYNSYM)
            and (table_index is None or s.index == table_index)
        ]
        result: list[Symbol] = []
        for table in tables:
            if table.link >= len(self.sections):
                raise ELFFormatError(
                    f"{self.label}: symbol table has invalid string-table link"
                )
            strings = self.section_bytes(self.sections[table.link])
            entsize = table.entsize or 16
            if entsize < 16 or table.size % entsize:
                raise ELFFormatError(
                    f"{self.label}: malformed symbol table {table.name}"
                )
            for pos in range(table.offset, table.offset + table.size, entsize):
                nameoff, value, size, info, other, shndx = struct.unpack_from(
                    "<IIIBBH", self.data, pos
                )
                result.append(
                    Symbol(
                        self._string(strings, nameoff, "symbol name"),
                        value,
                        size,
                        info,
                        other,
                        shndx,
                    )
                )
            self._symbol_tables[table.index] = (
                result[-(table.size // entsize) :] if table.size else []
            )
        return result

    def symbols_for_table(self, table_index: int) -> list[Symbol]:
        if table_index not in self._symbol_tables:
            self.symbols(table_index)
        return self._symbol_tables.get(table_index, [])

    def relocation_targets(
        self, section_index: int, low: int, high: int
    ) -> tuple[set[str], set[str]]:
        """Return undefined symbol and defined-section references within one object range."""
        undefined: set[str] = set()
        referenced_sections: set[str] = set()
        for relocation in self.sections:
            if (
                relocation.kind not in (SHT_REL, SHT_RELA)
                or relocation.info != section_index
            ):
                continue
            if relocation.link >= len(self.sections):
                raise ELFFormatError(
                    f"{self.label}: invalid relocation symbol-table link"
                )
            entry_size = relocation.entsize or (8 if relocation.kind == SHT_REL else 12)
            minimum = 8 if relocation.kind == SHT_REL else 12
            if entry_size < minimum or relocation.size % entry_size:
                raise ELFFormatError(
                    f"{self.label}: malformed relocation table {relocation.name}"
                )
            symbols = self.symbols_for_table(relocation.link)
            for pos in range(
                relocation.offset, relocation.offset + relocation.size, entry_size
            ):
                offset, info = struct.unpack_from("<II", self.data, pos)
                if not low <= offset < high:
                    continue
                symbol_index = info >> 8
                if symbol_index >= len(symbols):
                    raise ELFFormatError(
                        f"{self.label}: relocation has invalid symbol index"
                    )
                symbol = symbols[symbol_index]
                if symbol.shndx == 0 and symbol.name:
                    undefined.add(symbol.name)
                elif 0 < symbol.shndx < len(self.sections):
                    referenced_sections.add(self.sections[symbol.shndx].name)
        return undefined, referenced_sections


def parse_header(source: Path) -> dict[str, object]:
    text = source.read_text(encoding="utf-8", errors="replace")[:4096]
    marker = text.find("HSTMATCH")
    if marker < 0:
        raise VerificationRefusal(f"{source}: missing HSTMATCH header")
    opening = text.rfind("/*", 0, marker)
    closing = text.find("*/", marker)
    if opening < 0 or closing < 0:
        raise VerificationRefusal(f"{source}: malformed HSTMATCH comment")
    try:
        tokens = shlex.split(
            text[marker + len("HSTMATCH") : closing], comments=False, posix=True
        )
    except ValueError as exc:
        raise VerificationRefusal(f"{source}: malformed header fields: {exc}") from None
    fields: dict[str, str] = {}
    for token in tokens:
        if "=" not in token:
            raise VerificationRefusal(f"{source}: malformed header token {token!r}")
        key, value = token.split("=", 1)
        if key in fields:
            raise VerificationRefusal(f"{source}: duplicate header key {key}")
        fields[key] = value
    required = ("module", "start", "end", "symbol")
    missing = [key for key in required if not fields.get(key)]
    if missing:
        raise VerificationRefusal(f"{source}: header lacks {', '.join(missing)}")
    placement_keys = {name.strip(".") for name in TEXT_DATA_SECTIONS}
    accepted = {
        "module",
        "start",
        "end",
        "symbol",
        "lang",
        "cc",
        "profile",
        "asm",
        *placement_keys,
    }
    unknown = sorted(set(fields) - accepted)
    if unknown:
        raise VerificationRefusal(
            f"{source}: unsupported header key(s): {', '.join(unknown)}"
        )
    if fields.get("asm", "vfpu") != "vfpu":
        raise VerificationRefusal(f"{source}: asm= may only be vfpu (the VFPU inline-asm rule, hstgate.py)")
    if "cc" in fields and "profile" in fields:
        raise VerificationRefusal(f"{source}: use only one of cc= or profile=")
    try:
        start, end = int(fields["start"], 16), int(fields["end"], 16)
        placements = {
            key: int(fields[key], 16) for key in placement_keys if key in fields
        }
    except ValueError:
        raise VerificationRefusal(
            f"{source}: start, end and section placements must be hexadecimal"
        ) from None
    if start >= end or start < 0 or end > 0x100000000:
        raise VerificationRefusal(
            f"{source}: invalid function span 0x{start:x}..0x{end:x}"
        )
    language = fields.get("lang", "c++" if source.suffix == ".cpp" else "c99")
    if language not in ("c99", "c++"):
        raise VerificationRefusal(f"{source}: unsupported lang={language}")
    profile = fields.get("profile", fields.get("cc", "mwcc219"))
    return {
        "module": fields["module"],
        "start": start,
        "end": end,
        "symbol": fields["symbol"],
        "lang": language,
        "profile": profile,
        "placements": placements,
    }


def load_ranges(path: Path) -> list[tuple[int, int]]:
    ranges: list[tuple[int, int]] = []
    for number, raw in enumerate(path.read_text(encoding="ascii").splitlines(), 1):
        line = raw.split("#", 1)[0].strip()
        if not line:
            continue
        match = re.fullmatch(r"([0-9a-fA-F]{1,8})\s+([0-9a-fA-F]{1,8})", line)
        if not match:
            raise VerificationRefusal(
                f"{path}:{number}: malformed reviewed function range"
            )
        start, end = int(match.group(1), 16), int(match.group(2), 16)
        if start >= end or (ranges and start < ranges[-1][1]):
            raise VerificationRefusal(
                f"{path}:{number}: invalid or overlapping reviewed function range"
            )
        ranges.append((start, end))
    return ranges


def validate_reviewed_span(
    ranges: list[tuple[int, int]], start: int, end: int
) -> list[tuple[int, int]]:
    if start >= end:
        raise VerificationRefusal("empty or reversed claimed span")
    cursor = start
    included: list[tuple[int, int]] = []
    for low, high in ranges:
        if high <= cursor:
            continue
        if low > cursor:
            break
        included.append((low, high))
        cursor = min(end, high)
        if cursor == end:
            return included
    raise VerificationRefusal(
        f"span 0x{start:08x}..0x{end:08x} is not fully covered by contiguous reviewed functions"
    )


def parse_symbol_map(path: Path) -> dict[str, int]:
    symbols: dict[str, int] = {}
    if not path.exists():
        return symbols
    for number, raw in enumerate(path.read_text(encoding="utf-8").splitlines(), 1):
        line = raw.split("#", 1)[0].strip()
        if not line:
            continue
        match = re.fullmatch(r"([^\s=]+)\s*=\s*(0x[0-9a-fA-F]+)\s*;?", line)
        if not match:
            raise VerificationRefusal(f"{path}:{number}: malformed symbol map line")
        name, value = match.group(1), int(match.group(2), 16)
        if name in symbols and symbols[name] != value:
            raise VerificationRefusal(
                f"{path}:{number}: conflicting address for {name}"
            )
        symbols[name] = value
    return symbols


def address_symbol(name: str) -> int | None:
    match = re.fullmatch(r"(?:FUN|DAT|LAB|PTR|jtbl)_([0-9a-fA-F]{8})", name)
    return int(match.group(1), 16) if match else None


def load_retail_modules() -> tuple[dict[str, dict], dict[str, ELFImage]]:
    inventory = json.loads(
        (ROOT / "data" / "modules.json").read_text(encoding="utf-8-sig")
    )
    expected = {entry["name"]: entry for entry in inventory["modules"]}
    campaign = CFG.host_path("campaign_root")
    ledger = json.loads(
        (campaign / "ledger" / "campaign.json").read_text(encoding="utf-8-sig")
    )
    selection = json.loads(
        (campaign / "inputs" / "revision-selection.json").read_text(
            encoding="utf-8-sig"
        )
    )
    actual_modules: dict[str, dict] = {}
    images: dict[str, ELFImage] = {}
    for item in ledger["modules"]:
        name = item["name"]
        if name not in expected:
            continue
        source = Path(selection["roots"][item["input"]["root"]]) / item["input"]["path"]
        raw = source.read_bytes()
        digest = hashlib.sha256(raw).hexdigest()
        if len(raw) != expected[name]["size"] or digest != expected[name]["sha256"]:
            raise ELFFormatError(
                f"retail input for {name} does not match data/modules.json identity"
            )
        actual_modules[name] = {"bytes": raw, "path": source, "sha256": digest}
        images[name] = ELFImage(raw, f"retail {name}")
    missing = sorted(set(expected) - set(actual_modules))
    if missing:
        raise ELFFormatError(
            f"campaign input is missing module(s): {', '.join(missing)}"
        )
    return expected, images


def compare_bytes(expected: bytes, actual: bytes, address: int, label: str) -> dict:
    size = min(len(expected), len(actual))
    first = next((i for i in range(size) if expected[i] != actual[i]), None)
    if first is None and len(expected) != len(actual):
        first = size
    differing = sum(a != b for a, b in zip(expected[:size], actual[:size])) + abs(
        len(expected) - len(actual)
    )
    return {
        "label": label,
        "address": address,
        "expected_bytes": len(expected),
        "actual_bytes": len(actual),
        "compared_bytes": size,
        "differing_bytes": differing,
        "expected_sha256": hashlib.sha256(expected).hexdigest(),
        "actual_sha256": hashlib.sha256(actual).hexdigest(),
        "first_differing_offset": first,
        "first_differing_address": address + first if first is not None else None,
    }


def image_comparison(retail: ELFImage, linked: ELFImage, header: dict) -> dict:
    start, end = int(header["start"]), int(header["end"])
    span = end - start
    text = linked.section(".text")
    if (
        text is None
        or not text.size
        or not (text.flags & SHF_ALLOC)
        or start < text.addr
        or end > text.addr + text.size
    ):
        raise VerificationRefusal(
            "linked image has no .text section covering the claimed span"
        )
    symbols = [
        symbol
        for symbol in linked.symbols()
        if symbol.name == header["symbol"] and symbol.shndx != 0
    ]
    if not symbols:
        raise VerificationRefusal(
            f"linked image has no definition for {header['symbol']}"
        )
    if symbols[0].value != start:
        raise VerificationRefusal(
            f"{header['symbol']} is linked at 0x{symbols[0].value:08x}, not 0x{start:08x}"
        )
    for section in linked.sections:
        if (
            section.flags & SHF_ALLOC
            and section.size
            and section.name != ".text"
            and section.addr < end
            and start < section.addr + section.size
        ):
            raise VerificationRefusal(
                f"section {section.name} overlaps claimed span 0x{start:08x}..0x{end:08x}"
            )
    try:
        mapped_text = linked.file_offset(start, span)
    except ELFFormatError as exc:
        raise VerificationRefusal(str(exc)) from None
    if mapped_text != text.offset + start - text.addr:
        raise VerificationRefusal(".text section offset disagrees with PT_LOAD mapping")
    text_bytes = linked._slice(mapped_text, span, "linked text span")
    retail_bytes = retail.bytes_at(start, span)
    regions = [compare_bytes(retail_bytes, text_bytes, start, ".text")]
    for section_name in TEXT_DATA_SECTIONS:
        section = linked.section(section_name)
        placement = header["placements"].get(section_name.strip("."))
        if section is None or not section.size or placement is None:
            continue
        if section.addr != placement:
            raise VerificationRefusal(
                f"{section_name} is at 0x{section.addr:08x}, header places it at 0x{placement:08x}"
            )
        if section.kind == SHT_NOBITS:
            regions.append(
                {
                    "label": section_name,
                    "address": section.addr,
                    "expected_bytes": None,
                    "actual_bytes": section.size,
                    "compared_bytes": 0,
                    "differing_bytes": None,
                    "first_differing_offset": None,
                    "first_differing_address": None,
                    "note": "NOBITS has no retail file bytes to compare",
                }
            )
            continue
        try:
            mapped = linked.file_offset(section.addr, section.size)
        except ELFFormatError as exc:
            raise VerificationRefusal(str(exc)) from None
        actual = linked.section_bytes(section)
        if mapped != section.offset:
            raise VerificationRefusal(
                f"{section_name} section file offset disagrees with PT_LOAD mapping"
            )
        expected = retail.bytes_at(section.addr, section.size)
        regions.append(compare_bytes(expected, actual, section.addr, section_name))
    status = (
        "DIFFER" if any(region["differing_bytes"] for region in regions) else "MATCH"
    )
    compared = sum(int(region["compared_bytes"]) for region in regions)
    different = sum(int(region["differing_bytes"] or 0) for region in regions)
    first_region = next(
        (region for region in regions if region["first_differing_address"] is not None),
        None,
    )
    return {
        "status": status,
        "compared_bytes": compared,
        "differing_bytes": different,
        "first_differing_offset": (first_region["first_differing_address"] - start)
        if first_region
        else None,
        "first_differing_address": first_region["first_differing_address"]
        if first_region
        else None,
        "regions": regions,
    }


def check_artifact(path: Path, source: Path, retail: ELFImage, header: dict) -> dict:
    linked_path = path / "linked.elf"
    if not linked_path.is_file():
        raise VerificationRefusal(
            f"missing hstmatch artifact: {linked_path.relative_to(ROOT) if linked_path.is_relative_to(ROOT) else linked_path.name}"
        )
    if linked_path.stat().st_mtime_ns < source.stat().st_mtime_ns:
        raise VerificationRefusal("linked.elf is older than its source")
    linked = ELFImage(linked_path.read_bytes(), "hstmatch linked.elf")
    return image_comparison(retail, linked, header)


def wsl_path(path: Path) -> str:
    value = str(path.resolve()).replace("\\", "/")
    if not re.match(r"^[A-Za-z]:/", value):
        raise VerificationRefusal(f"cannot map host path to WSL: {path}")
    return f"/mnt/{value[0].lower()}{value[2:]}"


def gcc_profiles(home: str, gcc4_home: str | None = None) -> dict[str, dict[str, str]]:
    home = shell_config_path(home)
    driver = f"{home}/psp-gcc-1.3.1/bin/psp-gcc"
    prefix171 = f"{home}/psp-gcc-1.7.1/lib/gcc-lib/psp/3.3.3+allegrex-2.2.2-psp-1.7.1/"
    profiles: dict[str, dict[str, str]] = {}
    for version, prefix in (("131", ""), ("171", prefix171)):
        for extension in ("", "x"):
            for optimization in GCC_OPTIMIZATIONS:
                name = f"gcc{version}{extension}-{optimization}"
                profiles[name] = {
                    "driver": driver,
                    "prefix": prefix,
                    "flags": f"-{optimization} -G0 -fno-merge-constants"
                    + (" -mext" if extension else ""),
                }
        profiles[f"gcc{version}-O0"] = {  # unoptimized Sony SDK objects; no -mext variant
            "driver": driver,
            "prefix": prefix,
            "flags": "-O0 -G0 -fno-merge-constants",
        }
    if gcc4_home:  # secondary profile (owner decision 2026-10-27): GCC 4.0.2 + pspdev patch + Allegrex tuning
        g4 = shell_config_path(gcc4_home)
        for optimization in ("O0",) + GCC_OPTIMIZATIONS:
            profiles[f"gcc402x-{optimization}"] = {
                "driver": f"{g4}/psp-gcc-4.0.2x/bin/psp-gcc",
                "prefix": "",
                "flags": f"-{optimization} -G0 -fno-merge-constants -mpreferred-stack-boundary=4",
            }
            # SDK-only profile (owner decision 2026-11-09): psp-gcc-4.0.4x, lbp-decomp's kit with ins-from-zero
            profiles[f"gcc404x-{optimization}"] = {
                "driver": f"{g4}/psp-gcc-4.0.4x/bin/psp-gcc",
                "prefix": "",
                "flags": f"-{optimization} -G0 -fno-merge-constants -mpreferred-stack-boundary=4",
            }
    return profiles


def shell_config_path(value: str) -> str:
    """Quote trusted WSL config paths while preserving the documented $HOME prefix."""
    if not re.fullmatch(r"(?:\$HOME|/)[A-Za-z0-9_./+\-]*", value):
        raise VerificationRefusal(
            "compiler config path contains unsupported shell characters"
        )
    return value


def compile_command(
    source: Path,
    obj: Path,
    log: Path,
    header: dict,
    native: str,
    include: Path,
    gcc_home: str,
) -> str:
    profile = str(header["profile"])
    lang = str(header["lang"])
    if profile == "mwcc219":
        return (
            f"cd {shlex.quote(wsl_path(source.parent))} && "
            f"MWCIncludes={shlex.quote(wsl_path(include))} "
            f"{shell_config_path(native)}/wibo {shell_config_path(native)}/builds/219/mwccpsp.exe -c -O4,p -sdatathreshold 0 "
            f"-lang {shlex.quote(lang)} -o {shlex.quote(wsl_path(obj))} {shlex.quote(source.name)} "
            f"> {shlex.quote(wsl_path(log))} 2>&1"
        )
    profiles = gcc_profiles(gcc_home, CFG.get("gcc4_home"))
    if profile not in profiles:
        raise VerificationRefusal(f"unsupported compiler profile {profile}")
    selected = profiles[profile]
    b_prefix = (
        f"-B {shell_config_path(selected['prefix'])} " if selected["prefix"] else ""
    )
    language = "c++" if lang == "c++" else "c"
    source_wsl = wsl_path(source)
    return (
        f"t=$(mktemp -d /tmp/hstxverify-gcc.XXXXXX) && "
        f'cp {shlex.quote(source_wsl)} "$t/{source.name}" && cd "$t" && '
        f"if {selected['driver']} {b_prefix}-c {selected['flags']} -x {language} "
        f"-o obj.o {shlex.quote(source.name)} > log 2>&1; then rc=0; else rc=$?; fi; "
        f"cp log {shlex.quote(wsl_path(log))}; "
        f"if [ -f obj.o ]; then cp obj.o {shlex.quote(wsl_path(obj))}; fi; "
        f'rm -rf "$t"; exit $rc'
    )


def make_link_job(
    obj_path: Path, output: Path, header: dict, symbol_map: dict[str, int], pspdev: str
) -> tuple[str, Path]:
    obj = ELFImage(obj_path.read_bytes(), "compiler object")
    sections_by_name: dict[str, list[Section]] = {}
    for section in obj.sections:
        sections_by_name.setdefault(section.name, []).append(section)
    if ".text" not in sections_by_name:
        raise VerificationRefusal("object has no .text section")
    symbols = obj.symbols()
    target = next(
        (
            sym
            for sym in symbols
            if sym.name == header["symbol"]
            and 0 < sym.shndx < len(obj.sections)
            and obj.sections[sym.shndx].name == ".text"
        ),
        None,
    )
    if target is None:
        raise VerificationRefusal(f"object does not define {header['symbol']} in .text")
    text_section = obj.sections[target.shndx]
    referenced_undefined, referenced_sections = obj.relocation_targets(
        text_section.index,
        target.value,
        target.value + max(target.size, int(header["end"]) - int(header["start"])),
    )
    undefined = sorted(
        {symbol.name for symbol in symbols if symbol.shndx == 0 and symbol.name}
    )
    definitions = []
    missing = []
    for name in undefined:
        address = symbol_map.get(name, address_symbol(name))
        if address is None:
            if name in referenced_undefined:
                missing.append(name)
            else:
                address = 0
        if address is not None:
            if not re.fullmatch(r"[^\s=;]+", name):
                raise VerificationRefusal(
                    f"cannot emit linker definition for symbol {name!r}"
                )
            definitions.append(f"{name} = 0x{address:08x};")
    if missing:
        raise VerificationRefusal(f"unresolved target symbol(s): {' '.join(missing)}")
    script = output / "symbols.ld"
    script.write_text("\n".join(definitions) + "\n", encoding="ascii", newline="\n")
    start = int(header["start"])
    text_base = start - target.value
    if text_base < 0:
        raise VerificationRefusal(
            "target symbol offset exceeds its requested retail address"
        )
    placements: list[str] = [f"-Ttext=0x{text_base:08x}"]
    for section_name in TEXT_DATA_SECTIONS:
        sections = [s for s in sections_by_name.get(section_name, []) if s.size]
        if not sections:
            continue
        header_key = section_name.strip(".")
        if (
            section_name in referenced_sections
            and header_key not in header["placements"]
        ):
            raise VerificationRefusal(
                f"target function references {section_name} but header has no {header_key}= placement"
            )
        if header_key in header["placements"]:
            placements.append(
                f"--section-start {section_name}=0x{header['placements'][header_key]:08x}"
            )
    linked = output / "linked.elf"
    log = output / "link.log"
    command = (
        f"{shell_config_path(pspdev)}/psp-ld -EL -e 0x{start:08x} {' '.join(placements)} "
        f"-o {shlex.quote(wsl_path(linked))} {shlex.quote(wsl_path(obj_path))} "
        f"{shlex.quote(wsl_path(script))} > {shlex.quote(wsl_path(log))} 2>&1"
    )
    return command, linked


def run_shell(command: str, script: Path, log: Path) -> None:
    script.write_text(
        f"#!/bin/sh\nset -eu\n{command}\n", encoding="utf-8", newline="\n"
    )
    result = subprocess.run(
        ["wsl.exe", "-e", "sh", wsl_path(script)],
        capture_output=True,
        text=True,
        check=False,
    )
    if result.returncode:
        details = (
            log.read_text(encoding="utf-8", errors="replace")[-1200:]
            if log.exists()
            else result.stderr[-1200:]
        )
        raise RuntimeError(
            f"WSL command failed ({result.returncode}): {details.strip()}"
        )


def check_rebuild(
    source: Path,
    output: Path,
    retail: ELFImage,
    header: dict,
    symbol_map: dict[str, int],
    native: str,
    include: Path,
    gcc_home: str,
    pspdev: str,
) -> dict:
    output.mkdir(parents=True, exist_ok=True)
    obj = output / "object.o"
    compile_log = output / "compile.log"
    compile_job = compile_command(
        source, obj, compile_log, header, native, include, gcc_home
    )
    run_shell(compile_job, output / "compile.sh", compile_log)
    if not obj.is_file():
        raise RuntimeError("compiler returned without producing an object")
    link_job, linked_path = make_link_job(obj, output, header, symbol_map, pspdev)
    run_shell(link_job, output / "link.sh", output / "link.log")
    if not linked_path.is_file():
        raise RuntimeError("linker returned without producing linked.elf")
    linked = ELFImage(linked_path.read_bytes(), "independent rebuild linked.elf")
    return image_comparison(retail, linked, header)


def artifact_directory(source: Path, header: dict) -> Path:
    return (
        ROOT
        / "build"
        / str(header["module"])
        / build_key(source)
    )


def record_status(
    checks: dict[str, dict], mode: str
) -> tuple[str, bool, int, int | None, int]:
    active = [
        checks[name] for name in ("artifact", "independent_rebuild") if name in checks
    ]
    statuses = [check["status"] for check in active]
    order = ("ERROR", "REFUSED", "DIFFER", "MATCH")
    aggregate = next((status for status in order if status in statuses), "ERROR")
    disagreement = False
    if mode == "both" and len(active) == 2:
        left, right = active

        def image_fingerprint(check: dict) -> tuple:
            regions = tuple(
                (
                    region.get("label"),
                    region.get("actual_sha256"),
                    region.get("actual_bytes"),
                )
                for region in check.get("regions", [])
            )
            return check.get("reason"), regions

        disagreement = (
            left["status"] != right["status"]
            or left.get("first_differing_address")
            != right.get("first_differing_address")
            or left.get("compared_bytes") != right.get("compared_bytes")
            or image_fingerprint(left) != image_fingerprint(right)
        )
    compared = sum(int(check.get("compared_bytes", 0)) for check in active)
    differences = [
        check.get("first_differing_address")
        for check in active
        if check.get("first_differing_address") is not None
    ]
    start = None
    if active:
        start = active[0].get("span_start")
    first = min(differences) if differences else None
    offset = first - start if first is not None and start is not None else None
    differing = sum(int(check.get("differing_bytes", 0)) for check in active)
    return aggregate, disagreement, compared, offset, differing


def verify_one(
    source: Path,
    mode: str,
    run_root: Path,
    modules: dict[str, dict],
    retail_images: dict[str, ELFImage],
    range_cache: dict[str, list[tuple[int, int]]],
    symbol_cache: dict[str, dict[str, int]],
    config: dict[str, str],
) -> dict:
    record: dict[str, object] = {
        "file": source.relative_to(ROOT).as_posix()
        if source.is_relative_to(ROOT)
        else str(source),
        "status": "ERROR",
        "checks": {},
    }
    try:
        header = parse_header(source)
        module = str(header["module"])
        if module not in modules:
            raise VerificationRefusal(f"unsupported module={module}")
        ranges = range_cache.get(module)
        if ranges is None:
            ranges = load_ranges(ROOT / "data" / f"functions-{module}.txt")
            range_cache[module] = ranges
        covered = validate_reviewed_span(
            ranges, int(header["start"]), int(header["end"])
        )
        record.update(
            {
                "module": module,
                "start": f"0x{int(header['start']):08x}",
                "end": f"0x{int(header['end']):08x}",
                "symbol": header["symbol"],
                "profile": header["profile"],
                "reviewed_function_count": len(covered),
            }
        )
        retail = retail_images[module]
        symbol_map = symbol_cache.get(module)
        if symbol_map is None:
            symbol_map = parse_symbol_map(ROOT / "symbols" / f"{module}.txt")
            symbol_cache[module] = symbol_map
        checks: dict[str, dict] = {}
        output_dir = artifact_directory(source, header)
        if mode in ("artifact", "both"):
            try:
                result = check_artifact(output_dir, source, retail, header)
                result["span_start"] = int(header["start"])
                checks["artifact"] = result
            except VerificationRefusal as exc:
                checks["artifact"] = {
                    "status": "REFUSED",
                    "reason": str(exc),
                    "compared_bytes": 0,
                    "differing_bytes": 0,
                    "first_differing_address": None,
                    "span_start": int(header["start"]),
                }
            except (OSError, ELFFormatError) as exc:
                checks["artifact"] = {
                    "status": "ERROR",
                    "reason": str(exc),
                    "compared_bytes": 0,
                    "differing_bytes": 0,
                    "first_differing_address": None,
                    "span_start": int(header["start"]),
                }
        if mode in ("rebuild", "both"):
            key = f"{module}/{build_key(source)}"
            out = run_root / key
            try:
                result = check_rebuild(
                    source,
                    out,
                    retail,
                    header,
                    symbol_map,
                    config["native"],
                    ROOT / "include",
                    config["gcc_home"],
                    config["pspdev"],
                )
                result["span_start"] = int(header["start"])
                checks["independent_rebuild"] = result
            except VerificationRefusal as exc:
                checks["independent_rebuild"] = {
                    "status": "REFUSED",
                    "reason": str(exc),
                    "compared_bytes": 0,
                    "differing_bytes": 0,
                    "first_differing_address": None,
                    "span_start": int(header["start"]),
                }
            except (
                OSError,
                ELFFormatError,
                RuntimeError,
                subprocess.SubprocessError,
            ) as exc:
                checks["independent_rebuild"] = {
                    "status": "ERROR",
                    "reason": str(exc),
                    "compared_bytes": 0,
                    "differing_bytes": 0,
                    "first_differing_address": None,
                    "span_start": int(header["start"]),
                }
        record["checks"] = checks
        status, disagreement, compared, offset, differing = record_status(checks, mode)
        record.update(
            {
                "status": status,
                "verifier_disagreement": disagreement,
                "compared_bytes": compared,
                "differing_bytes": differing,
                "first_differing_offset": offset,
            }
        )
    except VerificationRefusal as exc:
        record.update(
            {
                "status": "REFUSED",
                "reason": str(exc),
                "compared_bytes": 0,
                "differing_bytes": 0,
                "first_differing_offset": None,
            }
        )
    except (OSError, json.JSONDecodeError, ELFFormatError, KeyError) as exc:
        record.update(
            {
                "status": "ERROR",
                "reason": str(exc),
                "compared_bytes": 0,
                "differing_bytes": 0,
                "first_differing_offset": None,
            }
        )
    return record


def parse_args(argv: list[str] | None = None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "paths", nargs="*", type=Path, help="source paths (default with --all)"
    )
    parser.add_argument(
        "--all", action="store_true", help="verify every C/C++ source under src/"
    )
    parser.add_argument(
        "--mode",
        choices=("artifact", "rebuild", "both"),
        default="both",
        help="check hstmatch artifacts, independently rebuild, or both (default)",
    )
    parser.add_argument(
        "--parallel",
        type=int,
        default=1,
        help="concurrent source rebuilds, from 1 to 4",
    )
    parser.add_argument(
        "--json", type=Path, help="write the complete JSON report to this path"
    )
    args = parser.parse_args(argv)
    if not 1 <= args.parallel <= 4:
        parser.error("--parallel must be between 1 and 4")
    if args.all and args.paths:
        parser.error("pass source paths or --all, not both")
    if not args.all and not args.paths:
        parser.error("pass source paths or --all")
    return args


def main(argv: list[str] | None = None) -> int:
    args = parse_args(argv)
    paths = (
        sorted((ROOT / "src").rglob("*"))
        if args.all
        else [p if p.is_absolute() else ROOT / p for p in args.paths]
    )
    paths = [p.resolve() for p in paths if p.is_file() and p.suffix in (".c", ".cpp")]
    if not paths:
        raise SystemExit("no C or C++ source paths found")
    run_id = f"{time.strftime('%Y%m%dT%H%M%SZ', time.gmtime())}-{os.getpid()}"
    run_root = ROOT / "build" / "hstxverify" / run_id
    expected_modules, retail_images = load_retail_modules()
    cache: dict[str, list[tuple[int, int]]] = {}
    symbol_cache: dict[str, dict[str, int]] = {}
    config = {
        "native": CFG.wsl_path("mwcc_native"),
        "gcc_home": CFG.wsl_path("gcc_home"),
        "pspdev": CFG.wsl_path("pspdev_bin"),
    }
    records: list[dict] = []
    if args.parallel == 1 or args.mode == "artifact":
        records = [
            verify_one(
                path,
                args.mode,
                run_root,
                expected_modules,
                retail_images,
                cache,
                symbol_cache,
                config,
            )
            for path in paths
        ]
    else:
        with concurrent.futures.ThreadPoolExecutor(max_workers=args.parallel) as pool:
            futures = [
                pool.submit(
                    verify_one,
                    path,
                    args.mode,
                    run_root,
                    expected_modules,
                    retail_images,
                    cache,
                    symbol_cache,
                    config,
                )
                for path in paths
            ]
            records = [future.result() for future in futures]
    counts = {
        status: sum(row["status"] == status for row in records)
        for status in ("MATCH", "DIFFER", "REFUSED", "ERROR")
    }
    report = {
        "schema": "hstxverify.report.v1",
        "mode": args.mode,
        "parallel": args.parallel,
        "run_id": run_id,
        "source_count": len(records),
        "summary": counts,
        "disagreements": sum(bool(row.get("verifier_disagreement")) for row in records),
        "records": records,
    }
    rendered = json.dumps(report, indent=2, sort_keys=True)
    if args.json:
        destination = args.json if args.json.is_absolute() else ROOT / args.json
        destination.parent.mkdir(parents=True, exist_ok=True)
        destination.write_text(rendered + "\n", encoding="utf-8")
        print(
            f"JSON={destination.relative_to(ROOT) if destination.is_relative_to(ROOT) else destination}"
        )
        print(
            f"SUMMARY={json.dumps(counts, sort_keys=True)} DISAGREEMENTS={report['disagreements']}"
        )
    else:
        print(rendered)
    return 0 if counts["DIFFER"] == counts["REFUSED"] == counts["ERROR"] == 0 else 1


if __name__ == "__main__":
    raise SystemExit(main())
