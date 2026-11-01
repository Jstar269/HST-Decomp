/* HSTMATCH module=hstp_trans_main.prx start=0x002b16c0 end=0x002b16e4 symbol=FUN_002b16c0 lang=c99 */
/* Clone of src/eboot/set_short_delta_002a22b4.c (hstclone.py tag dsn8b): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

void FUN_002b16c0(int param_1, short param_2)
{
  short old = *(short *)(param_1 + 6);
  short delta = param_2 - old;
  *(short *)(param_1 + 6) = param_2;
  *(short *)(param_1 + 10) = *(short *)(param_1 + 10) + delta;
}
