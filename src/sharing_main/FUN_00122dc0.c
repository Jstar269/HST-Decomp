/* HSTMATCH module=hstp_trans_main.prx start=0x00122dc0 end=0x00122e94 symbol=FUN_00122dc0 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */ /* matching idiom: the walk is entered through one `i >= 0` test, then loops with breaks only (a `while (i >= 0)` re-tests at the bottom) */
extern void FUN_00121eb4();
extern const char DAT_0038024c[], DAT_00380360[], DAT_00380298[], DAT_00380370[];
#define HST_ASSERT(cond, txt, line) do { if (!(cond)) { FUN_00121eb4(DAT_0038024c, txt, DAT_00380298, line); for (;;) {} } } while (0)
void FUN_00122dc0(int *o, int a, int b, int flag)
{
    HST_ASSERT(a >= 0, DAT_00380360, 580);
    HST_ASSERT(a > 0 || b <= 0, DAT_00380370, 581);
    if (flag) {
        int i = o[23];
        if (i >= 0) {
            for (;;) {
                if (((unsigned char *)o)[i * 112 + 224]) break;
                if (i == o[24]) break;
                i = (i + 8) % 9;
                a++;
            }
        }
    }
    o[17] = a;
    o[18] = b;
}
