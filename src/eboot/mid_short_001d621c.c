/* HSTMATCH module=EBOOT.BIN start=0x001d621c end=0x001d6240 symbol=FUN_001d621c lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

int FUN_001d621c(int param_1)
{
  int sum = (int)*(short *)(param_1 + 6) + (int)*(short *)(param_1 + 10);
  return (short)(sum / 2);
}
