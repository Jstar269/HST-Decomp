/* HSTMATCH module=EBOOT.BIN start=0x0001ea64 end=0x0001eae4 symbol=FUN_0001ea64 lang=c99 */
int FUN_0001ea64(int param_1, unsigned int *param_2, unsigned int param_3, int param_4, unsigned int param_5)
{
  int bVar1;
  unsigned int uVar2;

  if ((*(unsigned int *)(param_1 + 0x80) & param_3) == 0) {
    *param_2 = 0;
    bVar1 = 0;
  }
  else {
    uVar2 = *param_2 + 1;
    *param_2 = uVar2;
    if (uVar2 == 0) {
      *param_2 = 0xffffffff;
      bVar1 = 0;
    }
    else {
      if (uVar2 == 1) {
        bVar1 = 1;
      }
      else if (uVar2 < param_5) {
        bVar1 = 0;
      }
      else {
        bVar1 = (uVar2 - param_5 == 0) || ((int)(uVar2 - param_5) % param_4 == 0);
      }
    }
  }
  return bVar1;
}
// Ruling: PROGRESS.md VERDICTS (0x0001ea64); experiment: try/hsttry/counter_mod_0001ea64-45340
// near miss from lane dsn8 (unreviewed)
