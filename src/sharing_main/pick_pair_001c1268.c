/* HSTMATCH module=hstp_trans_main.prx start=0x001c1268 end=0x001c1338 symbol=FUN_001c1268 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern void FUN_001bd5cc();
extern const char DAT_00390e3c[], DAT_00390e8c[], DAT_00390e78[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_001bd5cc(DAT_00390e3c, DAT_00390e8c, DAT_00390e78, line); for (;;) {} } } while (0)
struct T { int v; int w; };
struct O { char pad[780]; struct T a[15]; struct T b[15]; char pad2[1896 - 1020]; int cur_a; int cur_b; char pad3[2008 - 1904]; int idx; unsigned char on; };
struct S { struct O *o; };
extern void FUN_001be964(struct O *o, int id, int a, int b, int c);
extern void FUN_001bdbf8(struct O *o, int a, int i, int b);
void FUN_001c1268(struct S *s, int i)
{
    struct O *o = s->o;
    HST_ASSERT(i >= 0 && i < 15, 2222);
    FUN_001be964(o, i * 2 + 195, 0, 0, 0);
    FUN_001be964(o, i * 2 + 225, 0, 0, 0);
    FUN_001bdbf8(o, 0, i, 0);
    o->cur_a = o->a[i].v;
    o->cur_b = o->b[i].v;
    o->idx = i;
    o->on = 0;
}
