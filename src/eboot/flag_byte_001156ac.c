/* HSTMATCH module=EBOOT.BIN start=0x001156ac end=0x001156d4 symbol=FUN_001156ac lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

int FUN_001156ac(int param_1)
{
  if (*(int *)(param_1 + 0x68) != 1) {
    return 0;
  }
  return *(unsigned char *)(**(int **)(param_1 + 0x58) + 0xea);
}
