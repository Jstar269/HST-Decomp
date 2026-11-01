/* HSTMATCH module=EBOOT.BIN start=0x001b4958 end=0x001b4984 symbol=FUN_001b4958 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

extern void FUN_000628e4(int, int);

void FUN_001b4958(int *param_1)
{
  if (*(int *)(*param_1 + 0x7c4) >= 0) {
    FUN_000628e4(*(int *)(*param_1 + 0x7c4), 0);
  }
}
