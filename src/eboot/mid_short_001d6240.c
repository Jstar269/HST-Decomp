/* HSTMATCH module=EBOOT.BIN start=0x001d6240 end=0x001d6264 symbol=FUN_001d6240 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

int FUN_001d6240(int param_1)
{
  int sum = (int)*(short *)(param_1 + 4) + (int)*(short *)(param_1 + 8);
  return (short)(sum / 2);
}
