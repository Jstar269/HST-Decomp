/* HSTMATCH module=hstp_trans_main.prx start=0x00248eac end=0x00248f1c symbol=FUN_00248eac lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern void FUN_00248f1c();
extern const char DAT_0039dc3c[], DAT_0039dc68[], DAT_0039dc78[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_00248f1c(DAT_0039dc3c, DAT_0039dc68, DAT_0039dc78, line); for (;;) {} } } while (0)
extern void *FUN_00000bcc(unsigned n);
extern void FUN_00248f48(void *p, int a);
extern void *DAT_003f39ec;
void FUN_00248eac(int a)
{
    void *p = FUN_00000bcc(6456);
    if (p)
        FUN_00248f48(p, a);
    HST_ASSERT(DAT_003f39ec != 0, 104);
}
