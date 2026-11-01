/* HSTMATCH module=EBOOT.BIN start=0x00296b3c end=0x00296b5c symbol=FUN_00296b3c lang=c99 */
void FUN_00296b3c(unsigned char *param_1, unsigned char param_2, int param_3)
{
  for (; param_3 != 0; param_3 = param_3 + -1) {
    *param_1 = param_2;
    param_1 = param_1 + 1;
  }
}
// Ruling: PROGRESS.md VERDICTS (0x00296b3c); experiment: try/hsttry/fill_bytes_00296b3c-7992
// near miss from lane dsn8 (unreviewed)
