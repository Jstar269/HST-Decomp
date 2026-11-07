/* HSTMATCH module=hstp_trans_main.prx start=0x0008f108 end=0x0008f170 symbol=FUN_0008f108 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern void FUN_0008d950();
extern const char DAT_0037872c[], DAT_003787b4[], DAT_0037875c[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_0008d950(DAT_0037872c, DAT_003787b4, DAT_0037875c, line); for (;;) {} } } while (0)
struct E { char b[432]; };
struct T { char pad[16]; struct E e[4]; };
struct S { char pad[60]; struct T *t; };
struct E *FUN_0008f108(struct S *s, int i)
{
    HST_ASSERT(i >= 0 && i < 4, 1419);
    return &s->t->e[i];
}
