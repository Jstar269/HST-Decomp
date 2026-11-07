/* HSTMATCH module=hstp_trans_main.prx start=0x00211238 end=0x002112f8 symbol=FUN_00211238 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern void FUN_002112f8();
extern const char DAT_00397260[], DAT_0039728c[], DAT_003972a4[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_002112f8(DAT_00397260, DAT_0039728c, DAT_003972a4, line); for (;;) {} } } while (0)
struct Ent { float f; int type; float cnt; int v; };
struct Obj { char pad[20]; int n; int last; struct Ent ents[12]; };
int FUN_00211238(struct Obj *self, int type, int cnt, int *pv, float f)
{
    int r = self->n;
    int ok = r < 12;
    HST_ASSERT(ok, 391);
    if (ok != 0) goto body;
    return r;
body:
    self->ents[self->n].type = type;
    self->ents[self->n].f = f;
    self->ents[self->n].cnt = (float)cnt;
    { struct Ent *e = &self->ents[self->n]; e->v = *pv; }
    if (type == 6) self->last = self->n;
    { int t = self->n + 1; self->n = t; return t - 1; }
}
