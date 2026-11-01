/* HSTMATCH module=EBOOT.BIN start=0x002a22b4 end=0x002a22d8 symbol=FUN_002a22b4 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

void FUN_002a22b4(int param_1, short param_2)
{
  short old = *(short *)(param_1 + 6);
  short delta = param_2 - old;
  *(short *)(param_1 + 6) = param_2;
  *(short *)(param_1 + 10) = *(short *)(param_1 + 10) + delta;
}
