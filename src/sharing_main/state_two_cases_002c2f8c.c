/* HSTMATCH module=hstp_trans_main.prx start=0x002c2f8c end=0x002c2fbc symbol=FUN_002c2f8c lang=c99 */
/* Clone of src/eboot/state_two_cases_002b34cc.c (hstclone.py tag dsn8b): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
/*
   Two-state predicate over the int at +0x400: returns 0 for state 2 or state 1, else 1.
   Matching idiom: the guards must be NESTED NEGATIVE tests with a single trailing `return 0;`
   (`if (x != 2) { if (x != 1) return 1; } return 0;`).  The switch over cases 1/2 and the
   positive early-return form both lose words (11/12 and 9/12); the nested negative form gives
   retail's `beql` on the ==2 test with `move v0,zero` in the delay, a plain `beq` on ==1, and
   `li v0,1` before it as the fall-through value.
*/
int FUN_002c2f8c(int param_1)
{
  int x = *(int *)(param_1 + 0x400);
  if (x != 2) {
    if (x != 1) {
      return 1;
    }
  }
  return 0;
}
