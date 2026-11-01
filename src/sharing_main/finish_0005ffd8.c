/* HSTMATCH module=hstp_trans_main.prx start=0x0005ffd8 end=0x00060020 symbol=FUN_0005ffd8 lang=c99 */
/* Clone of src/eboot/finish_0005f1e8.c (hstclone.py tag dsn8b): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

extern void FUN_0005e7a8(int, float);
extern void FUN_00056344(int, int);

void FUN_0005ffd8(int param_1, int param_2)
{
  if (*(int *)(param_1 + 0xa4) != 0) {
    FUN_0005e7a8(*(int *)(param_1 + 0xa4), *(float *)(param_1 + 0xc8));
  }
  FUN_00056344(param_1, param_2);
}
