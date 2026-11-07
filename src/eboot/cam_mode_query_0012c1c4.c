/* HSTMATCH module=EBOOT.BIN start=0x0012c1c4 end=0x0012c2dc symbol=FUN_0012c1c4 lang=c99 */
/* Hand-written reconstruction: clone of the source of cam_mode_query_00137630.c in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern int FUN_0012d76c(int);
extern void FUN_00126d1c();
extern char DAT_002da9f4[];
extern char *DAT_002cfc8c;
extern int DAT_0033b574, DAT_0033b4a4;
#define F(s) ((((int)DAT_002cfc8c[0x8b]) << (s)) >> 0x1e)
int FUN_0012c1c4(int m)
{
    switch (m) {
    case 0:
        return 0;
    case 1:
        return F(0x1a) != 1;
    case 2:
        return 0;
    case 3:
        if (DAT_0033b574 != 0 && FUN_0012d76c(DAT_0033b574)) return 0;
        return !F(0x1a) && DAT_0033b4a4 != 0;
    case 4:
        return !F(0x1c);
    default:
        FUN_00126d1c(DAT_002da9f4);
        return 0;
    }
}
