/* HSTMATCH module=hstp_trans_main.prx start=0x00185db8 end=0x00185df4 symbol=FUN_00185db8 lang=c99 */
/* Clone of src/eboot/tagged_call_001796c4.c (hstclone.py tag dsn8b): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

int FUN_00185db8(int param_1, int param_2, int param_3, int param_4)
{
  if (*(unsigned char *)(param_1 + 0x59c) != 0) {
    if (param_2 == param_3) {
      return param_1 + 0x59e;
    }
  }
  return (*(int (**)(int))(param_1 + 0x3ec))(param_4);
}
