/* HSTMATCH module=EBOOT.BIN start=0x002af1bc end=0x002af1e8 symbol=FUN_002af1bc lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
float FUN_002af1bc(int param_1)
{
  float num = (float)*(int *)(param_1 + 0xc);
  float den = (float)*(int *)(param_1 + 0x10);
  if (*(int *)(param_1 + 0x10) == 0) {
    return 0.0f;
  }
  return num / den;
}
