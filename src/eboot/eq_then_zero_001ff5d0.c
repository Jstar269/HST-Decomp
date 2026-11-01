/* HSTMATCH module=EBOOT.BIN start=0x001ff5d0 end=0x001ff5f0 symbol=FUN_001ff5d0 lang=c99 */
/* Hand-written reconstruction (external matching lane); exact match verified; names and types await human review. */
int FUN_001ff5d0(int param_1, int param_2)
{
  if (param_2 == param_1) {
    if (param_2 == 0) {
      param_1 = 1;
    }
    else {
      param_1 = 0;
    }
  }
  return param_1;
}
