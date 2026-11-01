/* HSTMATCH module=EBOOT.BIN start=0x0004577c end=0x000457b4 symbol=FUN_0004577c lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

extern void FUN_000457b4(int);

int FUN_0004577c(int param_1)
{
  FUN_000457b4(param_1);
  if (*(unsigned int *)(param_1 + 0xc) & 1) {
    return *(int *)(param_1 + 0x14);
  }
  return param_1 + 0xe;
}
