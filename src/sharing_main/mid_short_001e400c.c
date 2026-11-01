/* HSTMATCH module=hstp_trans_main.prx start=0x001e400c end=0x001e4030 symbol=FUN_001e400c lang=c99 */
/* Clone of src/eboot/mid_short_001d621c.c (hstclone.py tag dsn8b): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

int FUN_001e400c(int param_1)
{
  int sum = (int)*(short *)(param_1 + 6) + (int)*(short *)(param_1 + 10);
  return (short)(sum / 2);
}
