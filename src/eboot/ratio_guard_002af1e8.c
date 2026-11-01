/* HSTMATCH module=EBOOT.BIN start=0x002af1e8 end=0x002af214 symbol=FUN_002af1e8 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

float FUN_002af1e8(int param_1)
{
  float num = (float)*(int *)(param_1 + 0x14);
  float den = (float)*(int *)(param_1 + 0x18);
  if (*(int *)(param_1 + 0x18) == 0) {
    return 0.0f;
  }
  return num / den;
}
