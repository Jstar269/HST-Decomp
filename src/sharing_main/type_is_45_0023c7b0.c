/* HSTMATCH module=hstp_trans_main.prx start=0x0023c7b0 end=0x0023c838 symbol=FUN_0023c7b0 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */ /* matching idiom: the range-checked lookup is a `static inline` helper that reads the global table pointer before its range test, and the type test is `(unsigned)(t - 4) < 2`. */
extern void FUN_00239f10();
extern const char DAT_0039af44[], DAT_0039b1bc[], DAT_0039afa8[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_00239f10(DAT_0039af44, DAT_0039b1bc, DAT_0039afa8, line); for (;;) {} } } while (0)
struct E { unsigned char type; char pad[55]; };
struct T { int n; struct E e[15]; };
extern struct T *DAT_003eb004;
static inline struct E *get(int i) { struct T *t = DAT_003eb004; if (i >= 0 && i < 15) return &t->e[i]; return 0; }
int FUN_0023c7b0(int i)
{
    HST_ASSERT(i >= 0 && i < 15, 809);
    return (unsigned)(get(i)->type - 4) < 2;
}
