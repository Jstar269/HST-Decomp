/* HSTMATCH module=hstp_trans_main.prx start=0x002beb38 end=0x002beba0 symbol=FUN_002beb38 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern void FUN_002be7c0();
extern const char DAT_003abef4[], DAT_003abf58[], DAT_003abf44[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_002be7c0(DAT_003abef4, DAT_003abf58, DAT_003abf44, line); for (;;) {} } } while (0)
struct E { char b[176]; };
struct S { char pad[19316]; int n; struct E e[1]; };
struct E *FUN_002beb38(struct S *s, int i)
{
    HST_ASSERT(i >= 0 && i < s->n, 802);
    return &s->e[i];
}
