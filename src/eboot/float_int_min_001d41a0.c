/* HSTMATCH module=EBOOT.BIN start=0x001d41a0 end=0x001d41c8 symbol=FUN_001d41a0 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

int FUN_001d41a0(int *param_1, float *param_2)
{
  float b = *param_2;
  float f = (float)*param_1;
  return (int)(f > b ? b : f);
}
