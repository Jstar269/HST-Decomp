/* HSTMATCH module=EBOOT.BIN start=0x00002688 end=0x000026d8 symbol=FUN_00002688 lang=c99 */
/* Hand-written reconstruction: clone of the source of skip_three_ints_00002688.c in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern char *FUN_00001040(char *, int *);
char *FUN_00002688(char *p) { int t; int f = *p & 0x40; char *q = FUN_00001040(p + 1, &t); q = FUN_00001040(q, &t); if (f) q = FUN_00001040(q, &t); return q; }
