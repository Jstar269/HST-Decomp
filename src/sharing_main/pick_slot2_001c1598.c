/* HSTMATCH module=hstp_trans_main.prx start=0x001c1598 end=0x001c15fc symbol=FUN_001c1598 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern void FUN_001bd5cc();
extern const char DAT_00390e3c[], DAT_00390ef4[], DAT_00390e78[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_001bd5cc(DAT_00390e3c, DAT_00390ef4, DAT_00390e78, line); for (;;) {} } } while (0)
struct O;
struct S { struct O *o; };
extern void FUN_001be964(struct O *o, int id, int a, int b, int c);
void FUN_001c1598(struct S *s, int i)
{
    struct O *o = s->o;
    int k = i - 1;
    HST_ASSERT(k >= 0 && k < 10, 2294);
    FUN_001be964(o, i + 39, 0, 0, 0);
}
