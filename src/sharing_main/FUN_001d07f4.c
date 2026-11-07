/* HSTMATCH module=hstp_trans_main.prx start=0x001d07f4 end=0x001d08d8 symbol=FUN_001d07f4 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern void FUN_001d0468();
extern const char DAT_00391f58[], DAT_00391fc8[], DAT_00391fac[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_001d0468(DAT_00391f58, DAT_00391fc8, DAT_00391fac, line); for (;;) {} } } while (0)
extern char DAT_003eb0b4;
extern int DAT_003eb0bc;
extern unsigned FUN_00014a40(const char *s);
extern void FUN_000149a8(char *d, const char *s);
extern void FUN_001dbf68(void *p, int a, int b);
extern void FUN_001d06dc(void *p);
extern void FUN_001d1c4c(void *p, int a);
void FUN_001d07f4(char *self, const char *str, int n, int flag)
{
    unsigned len;
    DAT_003eb0b4 = 0;
    self[144] = 0;
    *(int *)(self + 148) = n + 1;
    len = FUN_00014a40(str);
    HST_ASSERT(len < 16, 1039);
    FUN_000149a8(self + 152, str);
    self[152 + len] = 0;
    if (flag)
        DAT_003eb0bc = 16;
    else
        DAT_003eb0bc = 0;
    FUN_001dbf68(self, 3, 1);
    FUN_001d06dc(self);
    FUN_001d1c4c(self, 1);
}
