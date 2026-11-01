/* HSTMATCH module=EBOOT.BIN start=0x00067bf4 end=0x00067c2c symbol=FUN_00067bf4 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

void FUN_00067bf4(int param_1, float (*param_2)(float, float, float, int), float param_3)
{
  *(float *)(param_1 + 0xc) = param_2(128.0f, 0.0f, param_3, param_1);
}
