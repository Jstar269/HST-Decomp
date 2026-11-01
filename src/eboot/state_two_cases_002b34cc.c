/* HSTMATCH module=EBOOT.BIN start=0x002b34cc end=0x002b34fc symbol=FUN_002b34cc lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
/*
   Two-state predicate over the int at +0x400: returns 0 for state 2 or state 1, else 1.
   Matching idiom: the guards must be NESTED NEGATIVE tests with a single trailing `return 0;`
   (`if (x != 2) { if (x != 1) return 1; } return 0;`).  The switch over cases 1/2 and the
   positive early-return form both lose words (11/12 and 9/12); the nested negative form gives
   retail's `beql` on the ==2 test with `move v0,zero` in the delay, a plain `beq` on ==1, and
   `li v0,1` before it as the fall-through value.
*/
int FUN_002b34cc(int param_1)
{
  int x = *(int *)(param_1 + 0x400);
  if (x != 2) {
    if (x != 1) {
      return 1;
    }
  }
  return 0;
}
