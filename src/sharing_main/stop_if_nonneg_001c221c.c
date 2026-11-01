/* HSTMATCH module=hstp_trans_main.prx start=0x001c221c end=0x001c2248 symbol=FUN_001c221c lang=c99 */
/* Clone of src/eboot/stop_if_nonneg_001b40d8.c (hstclone.py tag dsn8b): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

extern void FUN_000637a8(int, int);

void FUN_001c221c(int *param_1)
{
  if (*(int *)(*param_1 + 0x7ac) >= 0) {
    FUN_000637a8(*(int *)(*param_1 + 0x7ac), 1);
  }
}
