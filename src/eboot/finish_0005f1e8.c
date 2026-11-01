/* HSTMATCH module=EBOOT.BIN start=0x0005f1e8 end=0x0005f230 symbol=FUN_0005f1e8 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

extern void FUN_0005d98c(int, float);
extern void FUN_00055798(int, int);

void FUN_0005f1e8(int param_1, int param_2)
{
  if (*(int *)(param_1 + 0xa4) != 0) {
    FUN_0005d98c(*(int *)(param_1 + 0xa4), *(float *)(param_1 + 0xc8));
  }
  FUN_00055798(param_1, param_2);
}
