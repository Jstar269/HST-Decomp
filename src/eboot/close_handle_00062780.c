/* HSTMATCH module=EBOOT.BIN start=0x00062780 end=0x000627bc symbol=FUN_00062780 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

extern void FUN_0007265c(int);

void FUN_00062780(int param_1)
{
  if (*(int *)(param_1 + 4) >= 0) {
    FUN_0007265c(*(int *)(param_1 + 4));
    *(int *)(param_1 + 4) = -1;
  }
}
