/* HSTMATCH module=hstp_trans_main.prx start=0x0028dc40 end=0x0028dc78 symbol=FUN_0028dc40 lang=c99 */
/* Clone of src/eboot/table_lookup_0027c8d0.c (hstclone.py tag dsn8b): addresses substituted; exact match verified; names and types await human review. */
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
extern int DAT_003f3b14;

int FUN_0028dc40(int param_1, int param_2)
{
    int x = *(int *)(param_1 + 0x78);
    if (x >= 0) {
        return x;
    }
        return *(int *)((*(int *)(param_1 + 0x54) << 6) + DAT_003f3b14 + (param_2 << 2) + 0x34);
}
