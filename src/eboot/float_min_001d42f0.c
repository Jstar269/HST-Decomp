/* HSTMATCH module=EBOOT.BIN start=0x001d42f0 end=0x001d4310 symbol=FUN_001d42f0 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

float FUN_001d42f0(float *param_1, float *param_2)
{
  if (!(*param_1 > *param_2)) {
    return *param_1;
  }
  return *param_2;
}
