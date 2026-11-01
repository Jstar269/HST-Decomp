/* HSTMATCH module=EBOOT.BIN start=0x00032834 end=0x00032854 symbol=FUN_00032834 lang=c99 */
extern void FUN_00032608();
void FUN_00032834(int param_1, int param_2)
{
  *(unsigned int *)(param_2 + 0x30) = *(unsigned int *)(param_2 + 0x30) | 0x1000;
  FUN_00032608(param_2, 3, param_1 + 0x18);
}
// Ruling: PROGRESS.md VERDICTS (0x00032834); experiment: try/hsttry/set_flag_call_00032834-44364
// near miss from lane dsn8 (unreviewed)
