/* HSTMATCH module=hstp_trans_main.prx start=0x0006f75c end=0x0006f80c symbol=FUN_0006f75c lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern void FUN_0006f3e0();
extern const char DAT_00377320[], DAT_00377368[], DAT_00377354[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_0006f3e0(DAT_00377320, DAT_00377368, DAT_00377354, line); for (;;) {} } } while (0)
struct V { int *data; unsigned n; };
extern struct V *DAT_003daf6c;
extern float DAT_003daf74;
struct S { int cur; int on; };
extern void FUN_0006f80c(struct S *s);
extern void FUN_0006f988(struct S *s, float f);
void FUN_0006f75c(struct S *s, unsigned i, float a, float b, float f)
{
    if (s->on) {
        HST_ASSERT(i < DAT_003daf6c->n, 141);
        s->cur = DAT_003daf6c->data[i];
        FUN_0006f80c(s);
        if (f < 0.0f)
            f = DAT_003daf74;
        FUN_0006f988(s, f);
    }
}
