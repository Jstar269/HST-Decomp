/* HSTMATCH module=EBOOT.BIN start=0x00055768 end=0x00055798 symbol=FUN_00055768 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

extern float FUN_000543c4(int);

float FUN_00055768(int param_1)
{
  return *(float *)(param_1 + 0x88) * (float)FUN_000543c4(*(int *)(param_1 + 0x8c));
}
