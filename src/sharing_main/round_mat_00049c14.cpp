/* HSTMATCH module=hstp_trans_main.prx start=0x00049c14 end=0x00049cb4 symbol=FUN_00049c14 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern "C" void FUN_000495f8(...);
extern "C" const char DAT_00375fa4[], DAT_00375ff4[], DAT_00375fe0[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_000495f8(DAT_00375fa4, DAT_00375ff4, DAT_00375fe0, line); for (;;) {} } } while (0)
struct M_00049c14 { float v[16]; };
struct O_00049c14 { virtual void f0(); virtual void f1(); virtual void f2(); virtual void f3(); virtual M_00049c14 *Get(); };
struct S_00049c14 { O_00049c14 *o; int f4; int f8; int fc; int f10; int idx; int f18; };
extern "C" int FUN_00049c14(S_00049c14 *s)
{
    int r = 0;
    if (s->f8 >= 0) {
        HST_ASSERT(s->idx >= 0, 265);
        if (s->f18 >= 0)
            r = (int)(0.5f - s->o->Get()[s->idx].v[12]);
    }
    return r;
}
