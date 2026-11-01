/* HSTMATCH module=EBOOT.BIN start=0x001cd164 end=0x001cd190 symbol=FUN_001cd164 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

int FUN_001cd164(int param_1)
{
  int *p = *(int **)(param_1 + 0x15c);
  int b = 0 <= p[0x1b];
  if (!b) {
    b = p[1] == 2;
  }
  return b;
}
