/* HSTMATCH module=hstp_trans_main.prx start=0x0005dec8 end=0x0005dfc0 symbol=FUN_0005dec8 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern void FUN_0005dfc0();
extern const char DAT_00376d90[], DAT_00376dbc[], DAT_00376dcc[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_0005dfc0(DAT_00376d90, DAT_00376dbc, DAT_00376dcc, line); for (;;) {} } } while (0)
extern void FUN_000149a8(char *d, const char *s);
extern char *FUN_00014d64(const char *s, const char *p);
extern void FUN_00014b74(char *d, const char *s, int n);
extern int FUN_00014a40(const char *s);
extern void FUN_00014788(char *d, const char *s);
void FUN_0005dec8(char *dst, const char *src, const char *pat, const char *rep)
{
    HST_ASSERT(dst != 0 && src != 0, 105);
    FUN_000149a8(dst, src);
    if (pat != 0 && rep != 0) {
        char *p = FUN_00014d64(src, pat);
        if (p != 0) {
            int n = p - src;
            const char *tail;
            FUN_00014b74(dst, src, n);
            dst[n] = 0;
            tail = p + FUN_00014a40(pat);
            FUN_00014788(dst, rep);
            FUN_00014788(dst, tail);
        }
    }
}
