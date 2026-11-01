/* HSTMATCH module=EBOOT.BIN start=0x00048d5c end=0x00048dd0 symbol=FUN_00048d5c lang=c99 */
extern int FUN_00000a00();
extern void FUN_00048e6c(int *, int, int);

int *FUN_00048d5c(int *param_1, short param_2)
{
  if (param_1 != 0) {
    param_1[1] = 0;
    if ((param_1 != 0) && (FUN_00048e6c(param_1, param_1[1], 0), *param_1 != 0)) {
      FUN_00000a00();
    }
    if (0 < param_2) {
      FUN_00000a00(param_1);
    }
  }
  return param_1;
}
// Ruling: PROGRESS.md VERDICTS (0x00048d5c); experiment: try/hsttry/free_obj_00048d5c-6476
// near miss from lane dsn8 (unreviewed)
