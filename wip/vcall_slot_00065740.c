/* HSTMATCH module=EBOOT.BIN start=0x00065740 end=0x000657e4 symbol=FUN_00065740 lang=c99 */
extern void FUN_00065c60(int *, int *, int *);

int FUN_00065740(int param_1, int param_2, int param_3)
{
  int local_c[2];
  int local_4;
  int iVar2;

  local_4 = param_2;
  FUN_00065c60(local_c, (int *)(param_1 + 0x10), &local_4);
  if (local_c[0] == param_1 + 0x14) {
    iVar2 = -1;
  }
  else {
    iVar2 = *(int *)(local_c[0] + 0x10);
  }
  if ((iVar2 < 0) || (param_3 < 0)) {
    return 0;
  }
  return (*(int (**)(void))(**(int **)(*(int *)(*(int *)(param_1 + 4) + iVar2 * 0xc) + param_3 * 4) + 0xc))();
}
// Ruling: PROGRESS.md VERDICTS (0x00065740); experiment: PROGRESS.md VERDICTS
// near miss from lane dsn8 (unreviewed)
