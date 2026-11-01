/* HSTMATCH module=hstp_trans_main.prx start=0x000b667c end=0x000b66a8 symbol=FUN_000b667c lang=c99 */
/* Clone of src/eboot/cmd_state_is_35_000b3464.c (hstclone.py tag dsn8b): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

int FUN_000b667c(int param_1)
{
  int *q = *(int **)(*(int *)(param_1 + 0x5bc) + 0x58);
  int iVar1 = q ? *(int *)((char *)q + 0x124) : -1;
  if (iVar1 == 0x35) {
    return 1;
  }
  return 0;
}
