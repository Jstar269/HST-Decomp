/* HSTMATCH module=hstp_trans_main.prx start=0x0020b804 end=0x0020b8b4 symbol=FUN_0020b804 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern void FUN_0020b684();
extern const char DAT_0039708c[], DAT_003970b8[], DAT_003970c8[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_0020b684(DAT_0039708c, DAT_003970b8, DAT_003970c8, line); for (;;) {} } } while (0)
extern void *FUN_00000bcc(unsigned n);
extern void FUN_0020b950(void *p, int a);
extern void FUN_0020bc3c(void *p, int b, int c);
extern void FUN_0020bfb8(void *p, int b, int c, int d);
extern void *DAT_003f3474;
extern float DAT_003daf70;
void FUN_0020b804(int a, int b)
{
    void *p;
    DAT_003daf70 = -0.015f;
    p = FUN_00000bcc(4944);
    if (p)
        FUN_0020b950(p, a);
    HST_ASSERT(DAT_003f3474 != 0, 31);
    FUN_0020bc3c(DAT_003f3474, 1, 1);
    FUN_0020bfb8(DAT_003f3474, b, 0, 0);
}
