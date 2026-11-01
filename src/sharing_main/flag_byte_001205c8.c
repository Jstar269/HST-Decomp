/* HSTMATCH module=hstp_trans_main.prx start=0x001205c8 end=0x001205f0 symbol=FUN_001205c8 lang=c99 */
/* Clone of src/eboot/flag_byte_001156ac.c (hstclone.py tag dsn8b): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

int FUN_001205c8(int param_1)
{
  if (*(int *)(param_1 + 0x68) != 1) {
    return 0;
  }
  return *(unsigned char *)(**(int **)(param_1 + 0x58) + 0xea);
}
