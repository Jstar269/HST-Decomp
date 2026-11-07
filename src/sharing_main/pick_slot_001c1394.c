/* HSTMATCH module=hstp_trans_main.prx start=0x001c1394 end=0x001c142c symbol=FUN_001c1394 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern void FUN_001bd5cc();
extern const char DAT_00390e3c[], DAT_00390eb0[], DAT_00390e78[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_001bd5cc(DAT_00390e3c, DAT_00390eb0, DAT_00390e78, line); for (;;) {} } } while (0)
struct T { int v; int w; };
struct O { char pad[1448]; struct T tab[8]; char pad2[1928 - 1448 - 64]; int cur; char pad3[2008 - 1932]; int idx; unsigned char on; };
struct S { struct O *o; };
extern void FUN_001be964(struct O *o, int id, int a, int b, int c);
void FUN_001c1394(struct S *s, int i)
{
    struct O *o = s->o;
    HST_ASSERT(i >= 0 && i < 8, 2248);
    FUN_001be964(o, i * 2 + 362, 0, 0, 0);
    o->cur = o->tab[i].v;
    o->idx = i;
    o->on = 1;
}
