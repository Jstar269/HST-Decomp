/* HSTMATCH module=hstp_trans_main.prx start=0x0008e830 end=0x0008e860 symbol=FUN_0008e830 lang=c99 */
/* Clone of src/eboot/blob16_copy_0008d0e4.c (hstclone.py tag dsn8b): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

typedef struct Blob16 {
  char b[16];
} Blob16;

Blob16 *FUN_0008e830(Blob16 *param_1, const Blob16 *param_2)
{
  *param_1 = *param_2;
  return param_1;
}
