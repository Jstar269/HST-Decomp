/* HSTMATCH module=EBOOT.BIN start=0x0005c810 end=0x0005c854 symbol=FUN_0005c810 lang=c99 */
/* Hand-written reconstruction (external matching lane); exact match verified; names and types await human review. */
/* HSTMATCH module=EBOOT.BIN start=0x0005c810 end=0x0005c854 symbol=FUN_0005c810 lang=c99 */
void FUN_0005c810(int param_1, int param_2, float *param_3)
{
  unsigned int uVar1;
  unsigned int uVar2;
  uVar2 = param_2 + 1U;
  uVar1 = *(unsigned int *)(param_1 + 0x10);
  *(unsigned int *)(param_1 + 0x10) = (uVar1 > uVar2) ? uVar1 : uVar2;
  {
    int off = param_2 * 0x20;
    float *p = (float *)(off + param_1 + 0xa0);
    p[0] = param_3[0];
    p[1] = param_3[1];
    p[2] = param_3[2];
    p[3] = param_3[3];
  }
}
