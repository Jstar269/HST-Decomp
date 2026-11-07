/* HSTMATCH module=hstp_trans_main.prx start=0x00108cec end=0x00108da0 symbol=FUN_00108cec lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern void FUN_0010532c();
extern const char DAT_0037f75c[], DAT_0037f9f0[], DAT_0037f7a4[], DAT_0037f9b4[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_0010532c(DAT_0037f75c, DAT_0037f9f0, DAT_0037f7a4, line); for (;;) {} } } while (0)
extern void FUN_00051d38(void *v);
extern void FUN_00051d9c(void *v, int b);
extern void FUN_00051e30(void *v, int x, int y, int w, int h, int a, int b);
extern char DAT_003e39c0[];
extern int DAT_003e3a08;
struct S { char pad[64]; int a; int b; };
void FUN_00108cec(struct S *s, int b)
{
    if (!s->b) {
        FUN_0010532c(DAT_0037f9b4);
        HST_ASSERT(0, 1245);
    }
    FUN_00051d38(DAT_003e39c0);
    FUN_00051d9c(DAT_003e39c0, b);
    FUN_00051e30(DAT_003e39c0, 0, 0, 480, 272, s->b, s->a);
    DAT_003e3a08 = -1;
}
