/* HSTMATCH module=hstp_trans_main.prx start=0x001e1fa4 end=0x001e1fcc symbol=FUN_001e1fa4 lang=c99 */
/* Clone of src/eboot/float_int_min_001d41a0.c (hstclone.py tag dsn8b): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

int FUN_001e1fa4(int *param_1, float *param_2)
{
  float b = *param_2;
  float f = (float)*param_1;
  return (int)(f > b ? b : f);
}
