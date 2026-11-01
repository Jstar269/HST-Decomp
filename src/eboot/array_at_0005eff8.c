/* HSTMATCH module=EBOOT.BIN start=0x0005eff8 end=0x0005f030 symbol=FUN_0005eff8 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
/*
   Bounds-checked element read: returns 0 for a negative index or an index past the count at +0xac,
   else the int at base(+0xa8)[index].
   Matching idiom: write the guards as NESTED POSITIVE tests with a single `return 0;` after them
   (`if (param_2 >= 0) { if ((unsigned)param_2 < count) return elem; } return 0;`).  The early-return
   spelling (`if (param_2 < 0) return 0;`) makes MWCC emit `bgezl` and hoist the count load into
   the delay slot; the nested form gives retail's `bltzl a1,<epilogue>` with `move v0,zero` in the
   delay and the array path inline.
*/
int FUN_0005eff8(int param_1, int param_2)
{
  if (param_2 >= 0) {
    if ((unsigned int)param_2 < *(unsigned int *)(param_1 + 0xac)) {
      return *(int *)(*(int *)(param_1 + 0xa8) + param_2 * 4);
    }
  }
  return 0;
}
