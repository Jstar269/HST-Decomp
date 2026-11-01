/* HSTMATCH module=EBOOT.BIN start=0x000be7b4 end=0x000be7f0 symbol=FUN_000be7b4 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

int FUN_000be7b4(int param_1)
{
  switch (*(int *)(param_1 + 0x28)) {
    case 4:
    case 8:
    case 11:
      return 1;
  }
  return 0;
}
