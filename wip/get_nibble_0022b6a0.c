/* HSTMATCH module=EBOOT.BIN start=0x0022b6a0 end=0x0022baec symbol=FUN_0022b6a0 lang=c99 rodata=0x002c9adc */
typedef unsigned char u8;
typedef unsigned int u32;

extern u32 DAT_003415a8;

struct ByteNibbles {
    u8 lo:4;
    u8 hi:4;
};

#define CASE_NIBBLE(byte_idx, nibble) do { \
    u8 *base = (u8 *)DAT_003415a8; \
    base += param_1 * 12; \
    struct ByteNibbles *bn = (struct ByteNibbles *)base; \
    return bn[0x8d00 + (byte_idx)].nibble; \
} while (0)

int FUN_0022b6a0(int param_1, int param_2)
{
    switch (param_2) {
    case 0: CASE_NIBBLE(0, lo);
    case 1: CASE_NIBBLE(0, hi);
    case 2: CASE_NIBBLE(1, lo);
    case 3: CASE_NIBBLE(1, hi);
    case 4: CASE_NIBBLE(2, lo);
    case 5: CASE_NIBBLE(2, hi);
    case 6: CASE_NIBBLE(3, lo);
    case 7: CASE_NIBBLE(3, hi);
    case 8: CASE_NIBBLE(4, lo);
    case 9: CASE_NIBBLE(4, hi);
    case 10: CASE_NIBBLE(5, lo);
    case 11: CASE_NIBBLE(5, hi);
    case 12: CASE_NIBBLE(6, lo);
    case 13: CASE_NIBBLE(6, hi);
    case 14: CASE_NIBBLE(7, lo);
    case 15: CASE_NIBBLE(7, hi);
    case 16: CASE_NIBBLE(8, lo);
    case 17: CASE_NIBBLE(8, hi);
    case 18: CASE_NIBBLE(9, lo);
    case 19: CASE_NIBBLE(9, hi);
    case 20: CASE_NIBBLE(10, lo);
    case 21: CASE_NIBBLE(10, hi);
    case 22: CASE_NIBBLE(11, lo);
    default: return 0;
    }
}
// Near miss: 57 words differ out of 275 words.
// Jump table in rodata at 0x002c9adc with 23 switch cases extracting 4-bit bitfields (lo and hi) from 12-byte structs at DAT_003415a8 + 0x8d00 + param_1 * 12.
// Body of each case is verified: lui/lw of global, index multiply by 12, struct offset addition, lbu from -29440, and bitfield extraction (andi for lo, sll 24/srl 28 for hi).
// Residual differs: jump table dispatch register allocation (v1/a1 vs v0/v1), operand order in addu v1,v1,v0 vs addu v1,v0,v1, and branch displacements.
// near miss from lane dsn5 (unreviewed)
