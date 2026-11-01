/* HSTMATCH module=EBOOT.BIN start=0x002add6c end=0x002adda0 symbol=FUN_002add6c lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

extern int FUN_002b7d28(void *);
extern void FUN_002adc74(const char *, int);
extern char DAT_00302cec[];

void FUN_002add6c(int param_1)
{
  int r = FUN_002b7d28((void *)(param_1 + 0x38));
  if (r != 0) {
    FUN_002adc74(DAT_00302cec, r);
  }
}
