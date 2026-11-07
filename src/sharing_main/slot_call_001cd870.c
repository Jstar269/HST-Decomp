/* HSTMATCH module=hstp_trans_main.prx start=0x001cd870 end=0x001cd8fc symbol=FUN_001cd870 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */ /* matching idiom: the range-checked lookup is a `static inline` helper (a plain `static` function is emitted out of line by MWCC in C). */
extern void FUN_001c9440();
extern const char DAT_00391910[], DAT_00391c24[], DAT_00391964[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_001c9440(DAT_00391910, DAT_00391c24, DAT_00391964, line); for (;;) {} } } while (0)
struct E { char b[20]; };
struct S { char pad[4184]; struct E e[40]; };
extern void FUN_001c9b18(struct E *e, int k, int z);
static inline struct E *get(struct S *s, int i) { if (i >= 0 && i < 40) return &s->e[i]; return 0; }
void FUN_001cd870(struct S *s, int i, int k)
{
    struct E *e = get(s, i);
    if (e) {
        HST_ASSERT(k >= 0 && k < 7, 2080);
        FUN_001c9b18(e, k, 0);
    }
}
