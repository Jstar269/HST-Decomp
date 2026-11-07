/* HSTMATCH module=hstp_trans_main.prx start=0x001781f0 end=0x001782a8 symbol=FUN_001781f0 lang=c99 */
/* Hand-written reconstruction: clone of the source of mode_retry_check_0016bf78.c in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
/* Matching idiom: the two switch(v) bodies share one trailing return 1 (return 1 inside each arm gives a trampoline). */
extern int DAT_003f39a8;
extern int FUN_002447e0(int *);
extern int FUN_0024481c(int *);

int FUN_001781f0(int param_1)
{
  int v;
  if (DAT_003f39a8 == 0x24) return 0;
  if (DAT_003f39a8 == 0x55) return param_1;
  if (param_1 != 0) {
    v = FUN_0024481c(&DAT_003f39a8);
    switch (v) { case 0x29: case 0x30: return 0; }
  } else {
    v = FUN_002447e0(&DAT_003f39a8);
    switch (v) { case 0x29: case 0x30: return 0; }
  }
  return 1;
}
