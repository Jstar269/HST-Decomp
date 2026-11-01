/* HSTMATCH module=EBOOT.BIN start=0x0001eb44 end=0x0001ebd4 symbol=FUN_0001eb44 lang=c99 */
unsigned char FUN_0001eb44(int param_1, unsigned int *param_2, unsigned int param_3)
{
  unsigned int uVar3;
  unsigned int uVar4;

  if ((*(unsigned int *)(param_1 + 0x80) & param_3) == 0) {
    *param_2 = 0;
    return 0;
  }
  uVar3 = *param_2;
  uVar4 = uVar3 + 1;
  *param_2 = uVar4;
  if (uVar4 == 0) {
    *param_2 = 0xffffffff;
    return 0;
  }
  if (uVar4 == 1) {
    return 1;
  }
  if (uVar4 < 4) {
    return 0;
  }
  return ((int)(uVar3 - 3) % 2) == 0;
}
// Ruling: PROGRESS.md VERDICTS (0x0001eb44); experiment: PROGRESS.md VERDICTS
// near miss from lane dsn8 (unreviewed)
