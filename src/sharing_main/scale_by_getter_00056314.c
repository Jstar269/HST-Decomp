/* HSTMATCH module=hstp_trans_main.prx start=0x00056314 end=0x00056344 symbol=FUN_00056314 lang=c99 */
/* Clone of src/eboot/scale_by_getter_00055768.c (hstclone.py tag dsn8b): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

extern float FUN_00054f10(int);

float FUN_00056314(int param_1)
{
  return *(float *)(param_1 + 0x88) * (float)FUN_00054f10(*(int *)(param_1 + 0x8c));
}
