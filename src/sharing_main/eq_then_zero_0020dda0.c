/* HSTMATCH module=hstp_trans_main.prx start=0x0020dda0 end=0x0020ddc0 symbol=FUN_0020dda0 lang=c99 */
/* Clone of src/eboot/eq_then_zero_001ff5d0.c (hstclone.py tag dsn8b): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction (external matching lane); exact match verified; names and types await human review. */
int FUN_0020dda0(int param_1, int param_2)
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
