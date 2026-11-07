/* HSTMATCH module=hstp_trans_main.prx start=0x00137630 end=0x00137748 symbol=FUN_00137630 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern int FUN_00138c6c(int);
extern void FUN_00131f8c();
extern char DAT_00381ae4[];
extern char *DAT_003751d4;
extern int DAT_003e48b4, DAT_003e47e4;
#define F(s) ((((int)DAT_003751d4[0x8b]) << (s)) >> 0x1e)
int FUN_00137630(int m)
{
    switch (m) {
    case 0:
        return 0;
    case 1:
        return F(0x1a) != 1;
    case 2:
        return 0;
    case 3:
        if (DAT_003e48b4 != 0 && FUN_00138c6c(DAT_003e48b4)) return 0;
        return !F(0x1a) && DAT_003e47e4 != 0;
    case 4:
        return !F(0x1c);
    default:
        FUN_00131f8c(DAT_00381ae4);
        return 0;
    }
}
