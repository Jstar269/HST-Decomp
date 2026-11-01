/* HSTMATCH module=EBOOT.BIN start=0x001156d4 end=0x001156fc symbol=FUN_001156d4 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

int FUN_001156d4(int param_1)
{
  if (*(int *)(param_1 + 0x68) != 1) {
    return 0;
  }
  return *(int *)(**(int **)(param_1 + 0x58) + 0xdc);
}
