/* HSTMATCH module=hstp_trans_main.prx start=0x0004587c end=0x000458b4 symbol=FUN_0004587c lang=c99 */
/* Clone of src/eboot/flag_union_0004577c.c (hstclone.py tag dsn8b): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

extern void FUN_000458b4(int);

int FUN_0004587c(int param_1)
{
  FUN_000458b4(param_1);
  if (*(unsigned int *)(param_1 + 0xc) & 1) {
    return *(int *)(param_1 + 0x14);
  }
  return param_1 + 0xe;
}
