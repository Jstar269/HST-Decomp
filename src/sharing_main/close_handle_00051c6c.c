/* HSTMATCH module=hstp_trans_main.prx start=0x00051c6c end=0x00051d0c symbol=FUN_00051c6c lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern void FUN_00051d0c();
extern const char DAT_003769d4[], DAT_00376a00[], DAT_00376a20[], DAT_00376a10[];
#define HST_ASSERT(cond, text, line) do { if (!(cond)) { FUN_00051d0c(DAT_003769d4, text, DAT_00376a10, line); for (;;) {} } } while (0)
extern void FUN_000506b8(void *mgr, int h);
extern void FUN_00051a2c(void *s);
extern char DAT_003ba4b0[];
struct S { int f0; int h; char pad[28]; void *a; void *b; };
void FUN_00051c6c(struct S *s)
{
    HST_ASSERT(s->a == 0, DAT_00376a00, 96);
    HST_ASSERT(s->b == 0, DAT_00376a20, 97);
    FUN_000506b8(DAT_003ba4b0, s->h);
    s->h = -1;
    FUN_00051a2c(s);
}
