/* HSTMATCH module=EBOOT.BIN start=0x001a673c end=0x001a680c symbol=FUN_001a673c lang=c99 */
extern void FUN_001a864c(int *, int, char);
extern void FUN_000111c0(int *, int *, int);

int *FUN_001a673c(int *param_1, int *param_2)
{
  int s;
  int e;
  int n;

  param_1[0] = 0;
  param_1[1] = 0;
  param_1[2] = 0;
  s = param_2[0];
  e = s + param_2[1] * 4;
  n = (e - s) / 4;
  if (n == 0) {
    return param_1;
  }
  FUN_001a864c(param_1, n, 0);
  FUN_000111c0((int *)(param_1[0] + param_1[1] * 4), (int *)s, n * 4);
  param_1[1] = param_1[1] + n;
  return param_1;
}
// Ruling: PROGRESS.md VERDICTS (0x001a673c); experiment: PROGRESS.md VERDICTS
// near miss from lane dsn8 (unreviewed)
