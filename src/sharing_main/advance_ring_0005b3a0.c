/* HSTMATCH module=hstp_trans_main.prx start=0x0005b3a0 end=0x0005b3fc symbol=FUN_0005b3a0 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern void FUN_0005aff8();
extern const char DAT_00376c9c[], DAT_00376ce8[], DAT_00376cd8[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_0005aff8(DAT_00376c9c, DAT_00376ce8, DAT_00376cd8, line); for (;;) {} } } while (0)
struct S { char pad[0x50]; int n; int pos; int x58; int z; };
void FUN_0005b3a0(struct S *s) { HST_ASSERT(s->n != 0, 319); s->pos = (s->pos + 1) % s->n; s->z = 0; }
