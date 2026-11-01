/* HSTMATCH module=hstp_trans_main.prx start=0x001e4030 end=0x001e4054 symbol=FUN_001e4030 lang=c99 */
/* Clone of src/eboot/mid_short_001d6240.c (hstclone.py tag dsn8b): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

int FUN_001e4030(int param_1)
{
  int sum = (int)*(short *)(param_1 + 4) + (int)*(short *)(param_1 + 8);
  return (short)(sum / 2);
}
