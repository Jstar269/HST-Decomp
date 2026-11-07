/* HSTMATCH module=hstp_trans_main.prx start=0x0023a54c end=0x0023a620 symbol=FUN_0023a54c lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */ /* matching idiom: the offset index of the then-branch is held in a local (int j = k + 10) before the subscript. */
extern void FUN_00239f10();
extern const char DAT_0039af44[], DAT_0039aff4[], DAT_0039b058[], DAT_0039afa8[];
#define HST_ASSERT(cond, text, line) do { if (!(cond)) { FUN_00239f10(DAT_0039af44, text, DAT_0039afa8, line); for (;;) {} } } while (0)
struct E { unsigned char b[20]; };
struct G { char pad[0x3d00]; struct E e[1025]; };
extern struct G *DAT_003ea8e8;
unsigned char FUN_0023a54c(int i, int k, int which)
{
    HST_ASSERT(i >= 0 && i <= 1024, DAT_0039aff4, 196);
    HST_ASSERT(k >= 0 && k < 5, DAT_0039b058, 197);
    if (which == 1) {
        int j = k + 10;
        return DAT_003ea8e8->e[i].b[j];
    }
    return DAT_003ea8e8->e[i].b[k];
}
