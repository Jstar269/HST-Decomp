/* HSTMATCH module=hstp_trans_main.prx start=0x00063608 end=0x00063644 symbol=FUN_00063608 lang=c99 */
/* Clone of src/eboot/close_handle_00062780.c (hstclone.py tag dsn8b): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

extern void FUN_00073e1c(int);

void FUN_00063608(int param_1)
{
  if (*(int *)(param_1 + 4) >= 0) {
    FUN_00073e1c(*(int *)(param_1 + 4));
    *(int *)(param_1 + 4) = -1;
  }
}
