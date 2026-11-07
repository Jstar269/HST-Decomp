/* HSTMATCH module=hstp_trans_main.prx start=0x00226298 end=0x002262f8 symbol=FUN_00226298 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern char *FUN_00108124(int *, unsigned char *);
void FUN_00226298(int *out, int **tbl, unsigned char *p) { int ok = 1; char *r = FUN_00108124(tbl[*p], p + 1); if (!r) ok = 0; if (ok) *out = *(int *)(r + 0x20); }
