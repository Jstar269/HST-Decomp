/* HSTMATCH module=hstp_trans_main.prx start=0x001205f0 end=0x00120618 symbol=FUN_001205f0 lang=c99 */
/* Clone of src/eboot/field_dc_001156d4.c (hstclone.py tag dsn8b): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

int FUN_001205f0(int param_1)
{
  if (*(int *)(param_1 + 0x68) != 1) {
    return 0;
  }
  return *(int *)(**(int **)(param_1 + 0x58) + 0xdc);
}
