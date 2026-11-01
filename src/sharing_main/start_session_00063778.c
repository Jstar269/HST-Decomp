/* HSTMATCH module=hstp_trans_main.prx start=0x00063778 end=0x000637a8 symbol=FUN_00063778 lang=c99 */
/* Clone of src/eboot/start_session_000628b4.c (hstclone.py tag dsn8b): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

extern int FUN_0007787c(int, int, int, int);

int FUN_00063778(int param_1, int param_2, int param_3)
{
  if (*(int *)(param_1 + 4) < 0) {
    return -1;
  }
  return FUN_0007787c(*(int *)(param_1 + 4), param_2, param_3, 0);
}
