/* HSTMATCH module=EBOOT.BIN start=0x0004eac4 end=0x0004eaf8 symbol=FUN_0004eac4 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
/*
   Range test: returns 1 when param_1 lies in [DAT_0032b238, DAT_0032b23c), else 0.
   Matching idioms, both required for the 13-word two-branch shape (an `if (x >= lo) { if (x < hi) r = 1; }`
   without the label folds to an 11-word `beqzl` + `slt` select):
   (a) the result variable is set before a plain `goto out` label so MWCC keeps two separate
       branches instead of folding the test into one select;
   (b) the FIRST comparison is written constant-on-the-left (`if (DAT_0032b238 > param_1)`), which
       makes MWCC compare into the assembler temporary (`slt at`) as retail does; parameter-on-the-
       left gives `slt v1` and is 2 words off.
*/
extern int DAT_0032b238;
extern int DAT_0032b23c;

int FUN_0004eac4(int param_1)
{
    int r = 0;
    if (DAT_0032b238 > param_1) {
        goto out;
    }
    if (param_1 < DAT_0032b23c) {
        r = 1;
    }
out:
    return r;
}
