/* HSTMATCH module=EBOOT.BIN start=0x000b3464 end=0x000b3490 symbol=FUN_000b3464 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

int FUN_000b3464(int param_1)
{
  int *q = *(int **)(*(int *)(param_1 + 0x5bc) + 0x58);
  int iVar1 = q ? *(int *)((char *)q + 0x124) : -1;
  if (iVar1 == 0x35) {
    return 1;
  }
  return 0;
}
