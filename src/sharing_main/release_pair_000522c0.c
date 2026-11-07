/* HSTMATCH module=hstp_trans_main.prx start=0x000522c0 end=0x00052354 symbol=FUN_000522c0 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern void FUN_00051d0c();
extern const char DAT_003769d4[], DAT_00376a30[], DAT_00376a40[], DAT_00376a10[];
#define HST_ASSERT(cond, text, line) do { if (!(cond)) { FUN_00051d0c(DAT_003769d4, text, DAT_00376a10, line); for (;;) {} } } while (0)
extern void FUN_0003cb10(void *heap, void *p);
extern void *DAT_003d44bc;
struct S { char pad[36]; void *a; void *b; };
void FUN_000522c0(struct S *s)
{
    HST_ASSERT(s->a != 0, DAT_00376a30, 322);
    HST_ASSERT(s->b != 0, DAT_00376a40, 323);
    FUN_0003cb10(DAT_003d44bc, s->b);
    s->b = 0;
    s->a = 0;
}
