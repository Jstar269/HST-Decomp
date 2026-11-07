/* HSTMATCH module=hstp_trans_main.prx start=0x002bec6c end=0x002becd8 symbol=FUN_002bec6c lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern void FUN_002be7c0();
extern const char DAT_003abef4[], DAT_003abf84[], DAT_003abf44[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_002be7c0(DAT_003abef4, DAT_003abf84, DAT_003abf44, line); for (;;) {} } } while (0)
struct E { char b[308]; };
struct S { char pad[36920]; int n; struct E e[1]; };
struct E *FUN_002bec6c(struct S *s, int i)
{
    HST_ASSERT(i >= 0 && i < s->n, 827);
    return &s->e[i];
}
