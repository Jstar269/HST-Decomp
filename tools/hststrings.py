#!/usr/bin/env python3
"""Print the string literals a retail function references (lui/lo address pairs that point at NUL-terminated text).

    python tools/hststrings.py [--module EBOOT.BIN] 0xADDR [0xADDR ...]

Evidence for naming: format strings, file names in asserts, resource paths. The scan follows each `lui` into the
next instruction that uses its register as a base (addiu/ori/loads/stores) and decodes the low half the way the CPU
does (sign-extended, `ori` zero-extended). A target counts as a string when it lies in the load segment and starts
with at least 4 printable characters followed by a NUL.
"""
import argparse
import json
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hstmatch as M  # noqa: E402

LO_OPS = {0x09, 0x0D, 0x20, 0x21, 0x23, 0x24, 0x25, 0x28, 0x29, 0x2B, 0x31, 0x39}   # addiu ori lb lh lw lbu lhu sb sh sw lwc1 swc1
# SPECIAL functions that write rd on Allegrex (shifts, jalr, movz/movn, mfhi/mflo, clz/clo, ALU, slt/sltu, max/min);
# syscall/break/sync, jr, mthi/mtlo and the HI/LO multiply-divide family write no GPR (their rd bits are other fields)
SPECIAL_RD_WRITES = {0x00, 0x02, 0x03, 0x04, 0x06, 0x07, 0x09, 0x0A, 0x0B, 0x10, 0x12, 0x16, 0x17,
                     0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x2A, 0x2B, 0x2C, 0x2D}
# I-type opcodes that write rt: addi..xori, the integer loads lb..lwr, ll, and sc (which writes its success flag)
RT_WRITES = set(range(0x08, 0x0F)) | set(range(0x20, 0x27)) | {0x30, 0x38}


def gpr_written(w: int) -> int | None:
    """The general register instruction `w` writes, if any (lui is handled by the caller)."""
    op, rs, rt, rd = w >> 26, (w >> 21) & 31, (w >> 16) & 31, (w >> 11) & 31
    if op == 0:
        return rd if (w & 0x3F) in SPECIAL_RD_WRITES else None
    if op in RT_WRITES:
        return rt
    if op == 0x03 or (op == 0x01 and rt in (0x10, 0x11, 0x12, 0x13)):  # jal, bltzal/bgezal(l): the link register
        return 31
    if op == 0x11 and rs in (0, 2):                     # mfc1, cfc1
        return rt
    if op == 0x12 and rs == 3:                          # mfv/mfvc (VFPU to GPR)
        return rt
    if op == 0x1F:                                      # SPECIAL3: seb/seh/wsbh/bitrev write rd, ext/ins write rt
        return rd if (w & 0x3F) == 0x20 else rt if (w & 0x3F) in (0x00, 0x04) else None
    return None


def functions(module: str) -> dict[int, int]:
    out = {}
    for ln in (M.ROOT / "data" / f"functions-{module}.txt").read_text(encoding="utf-8").splitlines():
        if ln and not ln.startswith("#"):
            s, e = ln.split()[:2]
            out[int(s, 16)] = int(e, 16)
    return out


def text_at(data: bytes, seg: dict, addr: int) -> str | None:
    off = seg["file_offset"] + addr - seg["start"]
    if addr < seg["start"] or off >= seg["file_end"]:
        return None
    end = data.find(b"\0", off, min(off + 256, seg["file_end"]))
    if end < 0:
        return None
    raw = data[off:end]
    if len(raw) < 4 or any(not (0x20 <= b < 0x7F or b in (9, 10)) for b in raw):
        return None
    return raw.decode("ascii")


def strings_in(data: bytes, seg: dict, start: int, end: int) -> list[tuple[int, str]]:
    hi: dict[int, int] = {}
    found = []
    for pc in range(start, end, 4):
        off = seg["file_offset"] + pc - seg["start"]
        w = int.from_bytes(data[off:off + 4], "little")
        op, rs, rt, imm = w >> 26, (w >> 21) & 31, (w >> 16) & 31, w & 0xFFFF
        if op == 0x0F:                                  # lui
            if rt != 0:                                 # $zero is immutable
                hi[rt] = imm << 16
            continue
        if op in LO_OPS and rs in hi:
            lo = imm if op == 0x0D else (imm - 0x10000 if imm & 0x8000 else imm)
            t = text_at(data, seg, (hi[rs] + lo) & 0xFFFFFFFF)
            if t is not None and (pc, t) not in found:
                found.append((pc, t))
        written = gpr_written(w)                        # a register written here no longer holds the high half
        if written is not None:
            hi.pop(written, None)
    return found


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--module", default="EBOOT.BIN")
    ap.add_argument("addresses", nargs="+")
    a = ap.parse_args()
    data, _ = M.module_bytes({}, a.module)
    campaign = M.campaign_root()
    mod = next(m for m in json.loads((campaign / "ledger" / "campaign.json").read_text(encoding="utf-8-sig"))["modules"]
               if m["name"] == a.module)
    segs = mod["load_segments"]
    ends = functions(a.module)
    for s in a.addresses:
        start = int(s, 16)
        end = ends.get(start)
        if end is None:
            print(f"0x{start:08x}: not a function start in data/functions-{a.module}.txt")
            continue
        seg = next(g for g in segs if g["start"] <= start < g["start"] + g["file_end"] - g["file_offset"])
        hits = strings_in(data, seg, start, end)
        print(f"0x{start:08x}: {len(hits)} string(s)")
        for pc, t in hits:
            print(f"  +0x{pc - start:04x} {json.dumps(t)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
