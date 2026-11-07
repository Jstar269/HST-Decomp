/* HSTMATCH module=hstp_trans_main.prx start=0x0023c648 end=0x0023c6d4 symbol=FUN_0023c648 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */ /* matching idiom: the flag test is a `static inline` helper returning `unsigned char` (retail keeps the sltu to a byte flag) and the loop breaks out with a result variable. */
extern void FUN_00239f10();
extern const char DAT_0039af44[], DAT_0039b2c4[], DAT_0039afa8[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_00239f10(DAT_0039af44, DAT_0039b2c4, DAT_0039afa8, line); for (;;) {} } } while (0)
struct G { char pad[0x3500]; unsigned char b[1]; };
extern struct G *DAT_003ea8e8;
static inline unsigned char flag2(int i) { HST_ASSERT(i >= 0 && (unsigned)i < 16, 40); return (DAT_003ea8e8->b[i + 0x8d00] & 2) != 0; }
int FUN_0023c648(void)
{
    int r = 1;
    int i;
    for (i = 9; i >= 0; i--) {
        if (!flag2(i)) {
            r = 0;
            break;
        }
    }
    return r;
}
