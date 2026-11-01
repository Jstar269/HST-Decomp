/* HSTMATCH module=EBOOT.BIN start=0x0011574c end=0x00115788 symbol=FUN_0011574c lang=c99 */

int FUN_0011574c(int param_1)
{
  int st = *(int *)(param_1 + 0x68);
  int r;
  if (st == 4) {
    r = *(unsigned char *)(param_1 + 0x7d);
  }
  else if (st == 3) {
    r = *(unsigned char *)(param_1 + 0x7d);
  }
  else {
    r = *(unsigned char *)(*(int *)(param_1 + 0x58) + 0x6d);
  }
  return r;
}
// NEAR 13/15 words, 12 differ, first +0x8: retail emits `beql x==4` with the lbu in the
// delay and a PLAIN `beq x==3` whose body is out of line (then `b` to the epilogue);
// every spellings tried puts `bnel` for the first test. Ruled out: two early returns,
// shared switch cases 3/4, else-if result variable.
// near miss from lane dsn8 (unreviewed)
