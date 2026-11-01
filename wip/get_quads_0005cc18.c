/* HSTMATCH module=EBOOT.BIN start=0x0005cc18 end=0x0005ccac symbol=FUN_0005cc18 lang=c99 */
void FUN_0005cc18(int param_1, int param_2, float *param_3, int *param_4)
{
  int iVar1;
  float fVar2;

  if (param_3 != 0) {
    iVar1 = param_2 * 0x20 + param_1;
    *param_3 = *(float *)(iVar1 + 0x90);
    param_3[1] = *(float *)(iVar1 + 0x94);
    param_3[2] = *(float *)(iVar1 + 0x98);
    fVar2 = *(float *)(iVar1 + 0x9c);
    param_3[3] = fVar2;
    *param_3 = -*param_3;
    param_3[1] = -param_3[1];
    param_3[2] = -param_3[2];
    param_3[3] = -fVar2;
  }
  if (param_4 != 0) {
    param_1 = param_2 * 0x20 + param_1;
    *param_4 = *(int *)(param_1 + 0xa0);
    param_4[1] = *(int *)(param_1 + 0xa4);
    param_4[2] = *(int *)(param_1 + 0xa8);
    param_4[3] = *(int *)(param_1 + 0xac);
  }
}
// Ruling: PROGRESS.md VERDICTS (0x0005cc18); experiment: try/hsttry/get_quads_0005cc18-15496
// near miss from lane dsn8 (unreviewed)
