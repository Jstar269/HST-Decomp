/* HSTMATCH module=EBOOT.BIN start=0x0004cc18 end=0x0004ccb8 symbol=FUN_0004cc18 lang=c99 */
extern int FUN_0004ccb8(int, int, int);
extern int FUN_0004d084(int);
extern void FUN_0004d028(int, int, int);
extern void FUN_0004cfd0(int);

int FUN_0004cc18(int param_1, int param_2, int *param_3)
{
  int iVar1;
  int iVar2;

  iVar2 = 0;
  iVar1 = FUN_0004ccb8(param_2, 1, 1);
  if ((-1 < iVar1) && (iVar2 = FUN_0004d084(iVar1), -1 < iVar2)) {
    FUN_0004d028(iVar1, param_1, iVar2);
  }
  if (-1 < iVar1) {
    FUN_0004cfd0(iVar1);
  }
  if (param_3 != 0) {
    *param_3 = iVar2;
  }
  return param_1;
}
// Ruling: PROGRESS.md VERDICTS (0x0004cc18); experiment: PROGRESS.md VERDICTS
// near miss from lane dsn8 (unreviewed)
