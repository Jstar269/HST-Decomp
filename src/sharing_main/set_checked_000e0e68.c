/* HSTMATCH module=hstp_trans_main.prx start=0x000e0e68 end=0x000e0eb4 symbol=FUN_000e0e68 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern void FUN_000df718();
extern const char DAT_0037d738[], DAT_0037d810[], DAT_0037d774[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_000df718(DAT_0037d738, DAT_0037d810, DAT_0037d774, line); for (;;) {} } } while (0)
extern unsigned char DAT_003e19f8;
struct S { char pad[0xac]; int aac; };
void FUN_000e0e68(struct S *s, int v) { HST_ASSERT(DAT_003e19f8, 958); s->aac = v; }
