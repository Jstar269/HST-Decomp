/* HSTMATCH module=EBOOT.BIN start=0x00065078 end=0x000650e0 symbol=FUN_00065078 lang=c99 */
/* Hand-written reconstruction (external matching lane); exact match verified; names and types await human review. */
char *FUN_00065078(char *param_1, char *param_2, char **param_3)
{
  char target;
  char *p;
  char ch;

  if (param_1 == 0) {
    return 0;
  }
  if (param_2 == 0) {
    return param_1;
  }
  target = *param_2;
  p = param_1;
  while (*p != target && *p != 0) {
    p++;
  }
  ch = *p;
  if (param_3 != 0) {
    if (ch == 0) {
      *param_3 = 0;
    } else {
      *param_3 = p + 1;
    }
  }
  *p = 0;
  return param_1;
}
