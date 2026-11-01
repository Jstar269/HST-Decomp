/* HSTMATCH module=EBOOT.BIN start=0x0027c8d0 end=0x0027c908 symbol=FUN_0027c8d0 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
/*
   Index lookup with a negative-result escape: the signed int at +0x78 is returned as-is when it is
   not negative, otherwise it indexes a table of 64-int rows (the row index is the int at +0x54) at
   +0x34 inside the row, selected by param_2.
   Matching idioms: (a) the POSITIVE test is written first (`if (x >= 0) return x;`), which is what
   makes MWCC branch on the negative (`bltzl`) into the out-of-line table block with the +0x54 load
   in the delay slot, instead of inlining the table into the fall-through; (b) the index arithmetic
   is spelled with SHIFTS (`<< 6`, `<< 2`), not `* 0x40` / `* 4`: the shift form makes MWCC encode
   the commutative `addu`s as `addu v0,a0,v0` / `addu v0,v1,v0` (index first, accumulated base
   second) exactly as retail does, where the multiply form gives `addu v0,v0,a0` (2 words off).
*/
extern int DAT_0034a7e4;

int FUN_0027c8d0(int param_1, int param_2)
{
    int x = *(int *)(param_1 + 0x78);
    if (x >= 0) {
        return x;
    }
        return *(int *)((*(int *)(param_1 + 0x54) << 6) + DAT_0034a7e4 + (param_2 << 2) + 0x34);
}
