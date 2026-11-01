/* HSTMATCH module=hstp_trans_main.prx start=0x002bea40 end=0x002bea6c symbol=FUN_002bea40 lang=c99 */
/* Clone of src/eboot/ratio_guard_002af1bc.c (hstclone.py tag dsn8b): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
float FUN_002bea40(int param_1)
{
  float num = (float)*(int *)(param_1 + 0xc);
  float den = (float)*(int *)(param_1 + 0x10);
  if (*(int *)(param_1 + 0x10) == 0) {
    return 0.0f;
  }
  return num / den;
}
