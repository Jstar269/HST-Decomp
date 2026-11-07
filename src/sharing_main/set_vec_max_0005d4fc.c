/* HSTMATCH module=hstp_trans_main.prx start=0x0005d4fc end=0x0005d580 symbol=FUN_0005d4fc lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern void FUN_0005d4d0();
extern const char DAT_00376d3c[], DAT_00376d68[], DAT_00376d84[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_0005d4d0(DAT_00376d3c, DAT_00376d68, DAT_00376d84, line); for (;;) {} } } while (0)
struct V4 { float x, y, z, w; };
struct E { struct V4 p; char pad[16]; };
struct S { char pad[16]; unsigned n; char pad2[160 - 20]; struct E e[2]; };
void FUN_0005d4fc(struct S *s, unsigned i, const struct V4 *v)
{
    unsigned k;
    HST_ASSERT(i < 2, 409);
    k = i + 1;
    s->n = k < s->n ? s->n : k;
    s->e[i].p.x = v->x;
    s->e[i].p.y = v->y;
    s->e[i].p.z = v->z;
    s->e[i].p.w = v->w;
}
