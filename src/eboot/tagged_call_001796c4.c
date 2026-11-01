/* HSTMATCH module=EBOOT.BIN start=0x001796c4 end=0x00179700 symbol=FUN_001796c4 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

int FUN_001796c4(int param_1, int param_2, int param_3, int param_4)
{
  if (*(unsigned char *)(param_1 + 0x59c) != 0) {
    if (param_2 == param_3) {
      return param_1 + 0x59e;
    }
  }
  return (*(int (**)(int))(param_1 + 0x3ec))(param_4);
}
