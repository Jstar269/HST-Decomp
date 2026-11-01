/* HSTMATCH module=EBOOT.BIN start=0x00109a20 end=0x00109ca8 symbol=FUN_00109a20 lang=c99 */
/* Entry block only: the sixth argument arrives in t1 and is a pointer to two floats. */
void FUN_00109a20(int a0, int a1, int a2, int a3, int a4, float *p6)
{
  float f1;
  float f2;

  f1 = p6[0];
  f2 = p6[1];
  if (f2 < f1) {
    a0 = a0 + 1;
  }
}
// Ruling: PROGRESS.md VERDICTS (0x00109a20); experiment: PROGRESS.md VERDICTS
// near miss from lane dsn8 (unreviewed)
