/* HSTMATCH module=EBOOT.BIN start=0x0005c810 end=0x0005c854 symbol=FUN_0005c810 lang=c99 */
void FUN_0005c810(int param_1, int param_2, int *param_3)
{
  unsigned int uVar1;

  uVar1 = *(unsigned int *)(param_1 + 0x10);
  if (*(unsigned int *)(param_1 + 0x10) <= param_2 + 1U) {
    uVar1 = param_2 + 1U;
  }
  *(unsigned int *)(param_1 + 0x10) = uVar1;
  param_1 = param_2 * 0x20 + param_1;
  *(int *)(param_1 + 0xa0) = param_3[0];
  *(int *)(param_1 + 0xa4) = param_3[1];
  *(int *)(param_1 + 0xa8) = param_3[2];
  *(int *)(param_1 + 0xac) = param_3[3];
}
// Ruling: PROGRESS.md VERDICTS (0x0005c810); experiment: PROGRESS.md VERDICTS
// near miss from lane dsn8 (unreviewed)
