/* HSTMATCH module=hstp_trans_main.prx start=0x00177ec4 end=0x00177f38 symbol=FUN_00177ec4 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern void FUN_00177f38();
extern const char DAT_00387a88[], DAT_00387ab4[], DAT_00387ac4[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_00177f38(DAT_00387a88, DAT_00387ab4, DAT_00387ac4, line); for (;;) {} } } while (0)
extern void *FUN_00000bcc(unsigned n);
extern void FUN_0017940c(void *p);
extern void *DAT_003ea858;
extern float DAT_003daf70;
int FUN_00177ec4(void)
{
    void *p;
    DAT_003daf70 = -0.015f;
    p = FUN_00000bcc(3516);
    if (p)
        FUN_0017940c(p);
    HST_ASSERT(DAT_003ea858 != 0, 126);
    return DAT_003ea858 != 0;
}
