/* HSTMATCH module=hstp_trans_main.prx start=0x000c1e54 end=0x000c1e90 symbol=FUN_000c1e54 lang=c99 */
/* Clone of src/eboot/state_one_of_3_000be7b4.c (hstclone.py tag dsn8b): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

int FUN_000c1e54(int param_1)
{
  switch (*(int *)(param_1 + 0x28)) {
    case 4:
    case 8:
    case 11:
      return 1;
  }
  return 0;
}
