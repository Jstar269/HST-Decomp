/* HSTMATCH module=hstp_trans_main.prx start=0x00179388 end=0x0017940c symbol=FUN_00179388 lang=c99 */
/* Hand-written reconstruction: clone of the source of get_object_state_0016d110.c in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction (external matching lane); exact match verified; names and types await human review. */
/* Ghidra pseudocode (hstgen.py candidate FUN_00179388) corrected by hand: integer-first callee prototype and inverted ( != 0 ) conditions; readability pass; exact match verified; names and types await human review. */
extern char *DAT_003ea858;
extern int DAT_003f3aac;
extern int FUN_0025df78(int, float);
extern int FUN_0025e0c0(int, int, int);

/* Returns 4, 3 or 2 depending on whether the lookup succeeds for kind 0xf, then for kind 4, then for neither. */
int FUN_00179388(void)
{
  int info;
  int index;
  int u;

  info = (*(int (**)(void))(DAT_003ea858 + 1000))();
  index = FUN_0025df78(DAT_003f3aac, *(float *)(info + 4));
  if (FUN_0025e0c0(DAT_003f3aac, 0xf, index) != 0) {
    u = 4;
  }
  else {
    if (FUN_0025e0c0(DAT_003f3aac, 4, index) != 0) u = 3;
    else u = 2;
  }
  return u;
}
