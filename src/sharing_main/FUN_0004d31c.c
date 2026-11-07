/* HSTMATCH module=hstp_trans_main.prx start=0x0004d31c end=0x0004d3fc symbol=FUN_0004d31c lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern void FUN_0004cd60();
extern const char DAT_00376330[], DAT_0037635c[], DAT_00376364[], DAT_00376370[], DAT_003b3e6c[];
#define HST_ASSERT(cond, txt, line) do { if (!(cond)) { FUN_0004cd60(DAT_00376330, txt, DAT_00376364, line); for (;;) {} } } while (0)
extern int DAT_003ba498;
extern unsigned char DAT_003b8ec5;
extern int FUN_0001bd2c(const char *p);
extern void FUN_0001c134(const char *p, int v);
extern void FUN_002c7500(int a, int b, int c);
void FUN_0004d31c(int a, int b, int c)
{
    HST_ASSERT(b != 0, DAT_0037635c, 448);
    if (DAT_003ba498 == 0 && DAT_003b8ec5) {
        HST_ASSERT(c == FUN_0001bd2c(DAT_003b3e6c), DAT_00376370, 461);
        FUN_0001c134(DAT_003b3e6c, b);
        FUN_0001bd2c(DAT_003b3e6c);
    } else {
        FUN_002c7500(a, b, c);
    }
}
