/* HSTMATCH module=hstp_trans_main.prx start=0x00065fac end=0x0006600c symbol=FUN_00065fac lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern void FUN_0006600c();
extern const char DAT_00377108[], DAT_00377134[], DAT_00377148[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_0006600c(DAT_00377108, DAT_00377134, DAT_00377148, line); for (;;) {} } } while (0)
struct E { int a; int f4; int c; };
struct S { int a; struct E *e; int n; };
int FUN_00065fac(struct S *s, int i) { HST_ASSERT(i < s->n, 204); struct E *p = &s->e[i]; return p->f4; }
