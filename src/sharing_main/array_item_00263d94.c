/* HSTMATCH module=hstp_trans_main.prx start=0x00263d94 end=0x00263dec symbol=FUN_00263d94 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern void FUN_0026396c();
extern const char DAT_003a5740[], DAT_003a57e4[], DAT_003a5798[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_0026396c(DAT_003a5740, DAT_003a57e4, DAT_003a5798, line); for (;;) {} } } while (0)
struct S { char pad[0x18]; int n; int items[1]; };
int FUN_00263d94(struct S *s, int i) { HST_ASSERT(i >= 0 && i < s->n, 238); return s->items[i]; }
