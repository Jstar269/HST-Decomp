/* HSTMATCH module=EBOOT.BIN start=0x0001b6c4 end=0x0001b73c symbol=FUN_0001b6c4 lang=c99 */
extern int FUN_0001b73c(int *, int);

void FUN_0001b6c4(int *param_1, int *param_2)
{
  int iVar1;
  int *piVar2;
  int iVar3;

  iVar1 = FUN_0001b73c(param_1, *param_2);
  iVar3 = iVar1;
  while (1) {
    while (param_1[1] <= iVar3) {
      iVar3 = 0;
    }
    piVar2 = (int *)(*param_1 + iVar3 * 8);
    if (*piVar2 == -1) {
      break;
    }
    iVar3 = iVar3 + 1;
  }
  *piVar2 = iVar1;
  piVar2[1] = (int)param_2;
}
// Ruling: PROGRESS.md VERDICTS (0x0001b6c4); experiment: try/hsttry/hash_put_0001b6c4-17400
// near miss from lane dsn8 (unreviewed)
