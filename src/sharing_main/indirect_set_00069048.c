/* HSTMATCH module=hstp_trans_main.prx start=0x00069048 end=0x00069080 symbol=FUN_00069048 lang=c99 */
/* Clone of src/eboot/indirect_set_00067bf4.c (hstclone.py tag dsn8b): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

void FUN_00069048(int param_1, float (*param_2)(float, float, float, int), float param_3)
{
  *(float *)(param_1 + 0xc) = param_2(128.0f, 0.0f, param_3, param_1);
}
