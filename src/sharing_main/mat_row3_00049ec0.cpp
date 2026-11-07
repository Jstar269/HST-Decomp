/* HSTMATCH module=hstp_trans_main.prx start=0x00049ec0 end=0x00049f8c symbol=FUN_00049ec0 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern "C" void FUN_000495f8(...);
extern "C" const char DAT_00375fa4[], DAT_00376004[], DAT_00375fe0[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_000495f8(DAT_00375fa4, DAT_00376004, DAT_00375fe0, line); for (;;) {} } } while (0)
struct V4_00049ec0 { float x, y, z, w; };
struct M_00049ec0 { float v[12]; V4_00049ec0 t; };
struct O_00049ec0 { virtual void f0(); virtual void f1(); virtual void f2(); virtual void f3(); virtual M_00049ec0 *Get(); char pad[132]; float scale; };
struct S_00049ec0 { O_00049ec0 *o; int f4; int f8; int fc; int idx; };
extern "C" void FUN_000224c0(V4_00049ec0 *d, const V4_00049ec0 *s, float k);
extern "C" int FUN_00049ec0(S_00049ec0 *s, V4_00049ec0 *out)
{
    if (s->f8 >= 0) {
        HST_ASSERT(s->idx >= 0, 354);
        M_00049ec0 *m = &s->o->Get()[s->idx];
        out->x = m->t.x;
        out->y = m->t.y;
        out->z = m->t.z;
        out->w = m->t.w;
        FUN_000224c0(out, out, s->o->scale);
        out->w = 1.0f;
        return 1;
    }
    return 0;
}
