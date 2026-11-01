/* HSTMATCH module=EBOOT.BIN start=0x0005c7a4 end=0x0005c810 symbol=FUN_0005c7a4 lang=c99 */
void FUN_0005c7a4(int param_1, int param_2, float *param_3)
{
  unsigned int uVar1;
  float fVar2;
  float fVar3;
  float fVar4;

  uVar1 = *(unsigned int *)(param_1 + 0x10);
  if (*(unsigned int *)(param_1 + 0x10) <= param_2 + 1U) {
    uVar1 = param_2 + 1U;
  }
  *(unsigned int *)(param_1 + 0x10) = uVar1;
  fVar4 = param_3[3];
  fVar2 = param_3[2];
  fVar3 = param_3[1];
  param_1 = param_2 * 0x20 + param_1;
  *(float *)(param_1 + 0x90) = -*param_3;
  *(float *)(param_1 + 0x94) = -fVar3;
  *(float *)(param_1 + 0x98) = -fVar2;
  *(float *)(param_1 + 0x9c) = -fVar4;
}
// Ruling: PROGRESS.md VERDICTS (0x0005c7a4); experiment: PROGRESS.md VERDICTS
// near miss from lane dsn8 (unreviewed)
