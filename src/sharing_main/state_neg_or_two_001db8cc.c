/* HSTMATCH module=hstp_trans_main.prx start=0x001db8cc end=0x001db8f8 symbol=FUN_001db8cc lang=c99 */
/* Clone of src/eboot/state_neg_or_two_001cd164.c (hstclone.py tag dsn8b): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

int FUN_001db8cc(int param_1)
{
  int *p = *(int **)(param_1 + 0x15c);
  int b = 0 <= p[0x1b];
  if (!b) {
    b = p[1] == 2;
  }
  return b;
}
