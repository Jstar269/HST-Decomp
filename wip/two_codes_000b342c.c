/* HSTMATCH module=EBOOT.BIN start=0x000b342c end=0x000b3464 symbol=FUN_000b342c lang=c99 */

int FUN_000b342c(int param_1)
{
  int *q = *(int **)(*(int *)(param_1 + 0x5bc) + 0x58);
  int iVar1 = q ? *(int *)((char *)q + 0x124) : -1;
  if (iVar1 == 0x91) {
    return 1;
  }
  if (iVar1 != 0x93) {
    return 0;
  }
  return 1;
}
// NEAR 13/14 words, 8 differ, first +0x18: retail tests 0x91 with `beql` (1 in delay)
// and 0x93 with `bnel` (0 in delay); ours folds the second test to xori/sltiu or
// branches `bnel` with an extra `li 1`. Ruled out: two returns, !=0x93/return 1,
// result variable else-if, `||` of two equals.
// near miss from lane dsn8 (unreviewed)
