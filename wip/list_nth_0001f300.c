/* HSTMATCH module=EBOOT.BIN start=0x0001f300 end=0x0001f394 symbol=FUN_0001f300 lang=c99 */
int FUN_0001f300(int param_1, int param_2)
{
  int iVar2;
  int iVar3;
  int bVar1;

  iVar2 = *(int *)(param_1 + 0x10);
  iVar3 = 0;
  if (param_2 < 1) goto LAB_0001f388;
  if (param_2 < 9) {
LAB_0001f36c:
    bVar1 = iVar3 < param_2;
  }
  else {
    bVar1 = 0;
    if ((-1 < param_2) && (param_2 < 0x7fffffff)) {
      bVar1 = 1;
    }
    if (bVar1) {
      do {
        iVar3 = iVar3 + 8;
        iVar2 = *(int *)(*(int *)(*(int *)(*(int *)(*(int *)(*(int *)(*(int *)(*(int *)(iVar2 + 4) + 4) + 4) + 4) + 4) + 4) + 4) + 4);
      } while (iVar3 < param_2 - 8);
      goto LAB_0001f36c;
    }
    bVar1 = 0 < param_2;
  }
  if (!bVar1) {
    return *(int *)(iVar2 + 8);
  }
  do {
    iVar3 = iVar3 + 1;
    iVar2 = *(int *)(iVar2 + 4);
  } while (iVar3 < param_2);
LAB_0001f388:
  return *(int *)(iVar2 + 8);
}
// Ruling: PROGRESS.md VERDICTS (0x0001f300); experiment: PROGRESS.md VERDICTS
// near miss from lane dsn8 (unreviewed)
