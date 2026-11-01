/* HSTMATCH module=EBOOT.BIN start=0x0008d0e4 end=0x0008d114 symbol=FUN_0008d0e4 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

typedef struct Blob16 {
  char b[16];
} Blob16;

Blob16 *FUN_0008d0e4(Blob16 *param_1, const Blob16 *param_2)
{
  *param_1 = *param_2;
  return param_1;
}
