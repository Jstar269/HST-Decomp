/* HSTMATCH module=hstp_trans_main.prx start=0x001e20f4 end=0x001e2114 symbol=FUN_001e20f4 lang=c99 */
/* Clone of src/eboot/float_min_001d42f0.c (hstclone.py tag dsn8b): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

float FUN_001e20f4(float *param_1, float *param_2)
{
  if (!(*param_1 > *param_2)) {
    return *param_1;
  }
  return *param_2;
}
