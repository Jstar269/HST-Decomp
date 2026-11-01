/* HSTMATCH module=EBOOT.BIN start=0x00044eac end=0x00044f4c symbol=FUN_00044eac lang=c99 */
extern int DAT_0030ff80;
extern int DAT_0030ff7c;
extern char DAT_002d0110[];
extern char DAT_002d0164[];
extern int FUN_000df8e8(int);
extern int FUN_0000fe3c(void);
extern void FUN_00042dd8(int, char *, int);

void FUN_00044eac(int param_1, int param_2)
{
  int iVar1;

  DAT_0030ff80 = DAT_0030ff80 - *(int *)(param_2 - 4);
  iVar1 = FUN_000df8e8(DAT_0030ff7c);
  if (iVar1 == -0x7fffff00) {
    iVar1 = FUN_0000fe3c();
    FUN_00042dd8(*(int *)(iVar1 + 0xc), DAT_002d0110, DAT_0030ff7c);
  }
  else if (iVar1 == -0x7ffffefd) {
    iVar1 = FUN_0000fe3c();
    FUN_00042dd8(*(int *)(iVar1 + 0xc), DAT_002d0164, param_2);
  }
}
// Ruling: PROGRESS.md VERDICTS (0x00044eac); experiment: try/hsttry/shutdown_err_00044eac-39216
// near miss from lane dsn8 (unreviewed)
