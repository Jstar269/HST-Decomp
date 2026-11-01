/* HSTMATCH module=EBOOT.BIN start=0x0006dfe4 end=0x0006e024 symbol=FUN_0006dfe4 lang=c99 */
/* Hand-written reconstruction (external matching lane); exact match verified; names and types await human review. */
extern int FUN_0006e8e0(int);
extern int DAT_00331c2c;

int *FUN_0006dfe4(int *param_1)
{
  int uVar1;

  *param_1 = 0;
  param_1[1] = 0;
  uVar1 = FUN_0006e8e0(DAT_00331c2c + 0xc);
  param_1[1] = uVar1;
  return param_1;
}
