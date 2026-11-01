/* HSTMATCH module=EBOOT.BIN start=0x00289cd4 end=0x00289cf4 symbol=FUN_00289cd4 lang=c99 */
int *FUN_00289cd4(int *param_1)
{
  param_1[0] = 0;
  param_1[1] = 0;
  param_1[2] = 0;
  param_1[1] = param_1[1] - param_1[1];
  return param_1;
}
// Ruling: PROGRESS.md VERDICTS (0x00289cd4); experiment: try/hsttry/vec_zero_00289cd4-8760
// near miss from lane dsn8 (unreviewed)
