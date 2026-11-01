/* HSTMATCH module=EBOOT.BIN start=0x001b40d8 end=0x001b4104 symbol=FUN_001b40d8 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

extern void FUN_000628e4(int, int);

void FUN_001b40d8(int *param_1)
{
  if (*(int *)(*param_1 + 0x7ac) >= 0) {
    FUN_000628e4(*(int *)(*param_1 + 0x7ac), 1);
  }
}
