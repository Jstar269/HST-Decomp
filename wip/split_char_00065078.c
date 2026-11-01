/* HSTMATCH module=EBOOT.BIN start=0x00065078 end=0x000650e0 symbol=FUN_00065078 lang=c99 */

char *FUN_00065078(char *param_1, char *param_2, char **param_3)
{
  if (param_1 == 0) {
    return 0;
  }
  if (param_2 == 0) {
    return param_1;
  }
  char target = *param_2;
  char *p = param_1;
  char ch;
  while ((ch = *p) != target && ch != 0) {
    p++;
  }
  if (param_3 == 0) {
    *p = 0;
  }
  else {
    if (ch == 0) {
      *param_3 = 0;
    }
    else {
      *param_3 = p + 1;
    }
    *p = 0;
  }
  return param_1;
}
// NEAR 29/26 words, 22 differ: target char must live in v1 and the cursor in a1 (retail
// reuses param_2's register), with no `seb` before the compare; our `char` locals give
// the target a1 and emit seb. Ruled out: one attempt with char locals and this shape.
// near miss from lane dsn8 (unreviewed)
