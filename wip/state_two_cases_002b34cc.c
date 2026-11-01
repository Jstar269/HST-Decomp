/* HSTMATCH module=EBOOT.BIN start=0x002b34cc end=0x002b34fc symbol=FUN_002b34cc lang=c99 */

int FUN_002b34cc(int param_1)
{
  switch (*(int *)(param_1 + 0x400)) {
    case 1:
      return 0;
    case 2:
      return 0;
  }
  return 1;
}
// NEAR 7 words short (11/12), first diff +0x8: retail's ==2 test jumps to the shared
// `jr ra` with `move v0,zero` in the delay slot, and its ==1 test is a plain `beq`
// (not beql); our switch emits beql for both. Ruled out: nested if/else (bnel, wrong
// registers), else-if chain, int u=1 + switch with u=0/break.
// near miss from lane dsn8 (unreviewed)
