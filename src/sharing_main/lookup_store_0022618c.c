/* HSTMATCH module=hstp_trans_main.prx start=0x0022618c end=0x002261ec symbol=FUN_0022618c lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern char *FUN_00108124(int *, unsigned char *);
void FUN_0022618c(int *out, int **tbl, unsigned char *p) { int ok = 1; char *r = FUN_00108124(tbl[*p], p + 1); if (!r) ok = 0; if (ok) *out = *(int *)(r + 0x20); }
