/* HSTMATCH module=hstp_trans_main.prx start=0x002bea6c end=0x002bea98 symbol=FUN_002bea6c lang=c99 */
/* Clone of src/eboot/ratio_guard_002af1e8.c (hstclone.py tag dsn8b): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

float FUN_002bea6c(int param_1)
{
  float num = (float)*(int *)(param_1 + 0x14);
  float den = (float)*(int *)(param_1 + 0x18);
  if (*(int *)(param_1 + 0x18) == 0) {
    return 0.0f;
  }
  return num / den;
}
