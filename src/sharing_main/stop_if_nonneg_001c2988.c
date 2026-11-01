/* HSTMATCH module=hstp_trans_main.prx start=0x001c2988 end=0x001c29b4 symbol=FUN_001c2988 lang=c99 */
/* Clone of src/eboot/stop_if_nonneg_001b492c.c (hstclone.py tag dsn8b): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

extern void FUN_000637a8(int, int);

void FUN_001c2988(int *param_1)
{
  if (*(int *)(*param_1 + 0x7c4) >= 0) {
    FUN_000637a8(*(int *)(*param_1 + 0x7c4), 1);
  }
}
