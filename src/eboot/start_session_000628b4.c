/* HSTMATCH module=EBOOT.BIN start=0x000628b4 end=0x000628e4 symbol=FUN_000628b4 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

extern int FUN_000760bc(int, int, int, int);

int FUN_000628b4(int param_1, int param_2, int param_3)
{
  if (*(int *)(param_1 + 4) < 0) {
    return -1;
  }
  return FUN_000760bc(*(int *)(param_1 + 4), param_2, param_3, 0);
}
