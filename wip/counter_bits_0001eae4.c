/* HSTMATCH module=EBOOT.BIN start=0x0001eae4 end=0x0001eb44 symbol=FUN_0001eae4 lang=c99 */
unsigned char FUN_0001eae4(int param_1, unsigned int *param_2, unsigned int param_3)
{
  unsigned char uVar1;
  unsigned int uVar2;

  if ((*(unsigned int *)(param_1 + 0x80) & param_3) == 0) {
    *param_2 = 0;
    uVar1 = 0;
  }
  else {
    uVar2 = *param_2 + 1;
    *param_2 = uVar2;
    if (uVar2 == 0) {
      *param_2 = 0xffffffff;
      uVar1 = 0;
    }
    else if (uVar2 == 1) {
      uVar1 = 1;
    }
    else if (uVar2 < 8) {
      uVar1 = 0;
    }
    else {
      uVar1 = 1;
    }
  }
  return uVar1;
}
// Ruling: PROGRESS.md VERDICTS (0x0001eae4); experiment: try/hsttry/counter_bits_0001eae4-45632
// near miss from lane dsn8 (unreviewed)
