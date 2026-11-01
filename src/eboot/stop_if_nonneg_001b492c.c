/* HSTMATCH module=EBOOT.BIN start=0x001b492c end=0x001b4958 symbol=FUN_001b492c lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

extern void FUN_000628e4(int, int);

void FUN_001b492c(int *param_1)
{
  if (*(int *)(*param_1 + 0x7c4) >= 0) {
    FUN_000628e4(*(int *)(*param_1 + 0x7c4), 1);
  }
}
