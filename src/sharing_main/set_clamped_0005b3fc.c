/* HSTMATCH module=hstp_trans_main.prx start=0x0005b3fc end=0x0005b448 symbol=FUN_0005b3fc lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern void FUN_0005aff8();
extern const char DAT_00376c9c[], DAT_00376cf4[], DAT_00376cd8[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_0005aff8(DAT_00376c9c, DAT_00376cf4, DAT_00376cd8, line); for (;;) {} } } while (0)
struct S { char pad[0x58]; int a58; int a5c; };
void FUN_0005b3fc(struct S *s, int v) { HST_ASSERT(s->a58 >= v, 327); s->a5c = v; }
