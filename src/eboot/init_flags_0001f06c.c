/* HSTMATCH module=EBOOT.BIN start=0x0001f06c end=0x0001f0f0 symbol=FUN_0001f06c lang=c99 */
/* Hand-written reconstruction (external matching lane); exact match verified; names and types await human review. */
extern char DAT_00306f60[];
extern void FUN_0001f848(int *);

int *FUN_0001f06c(int *param_1)
{
  *param_1 = (int)DAT_00306f60;
  FUN_0001f848(param_1 + 2);
  param_1[1] = 0x1234567;
  param_1[5] = (int)(param_1 + 3);
  param_1[6] = (int)(param_1 + 3);
  param_1[7] = 0;
  *(char *)(param_1 + 8) = 1;
  *(char *)((int)param_1 + 0x21) = 1;
  *(char *)(param_1 + 9) = 1;
  *(char *)((int)param_1 + 0x25) = 1;
  param_1[10] = 0;
  *(char *)(param_1 + 0xb) = 1;
  *(char *)((int)param_1 + 0x2d) = 1;
  param_1[0xc] = 0;
  *(char *)(param_1 + 0xd) = 1;
  *(char *)((int)param_1 + 0x35) = 1;
  param_1[0xe] = 0;
  return param_1;
}
