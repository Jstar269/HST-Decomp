/* HSTMATCH module=hstp_trans_main.prx start=0x00049da8 end=0x00049e34 symbol=FUN_00049da8 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern "C" void FUN_000495f8(...);
extern "C" const char DAT_00375fa4[], DAT_00375ff4[], DAT_00375fe0[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_000495f8(DAT_00375fa4, DAT_00375ff4, DAT_00375fe0, line); for (;;) {} } } while (0)
struct M_00049da8 { float v[16]; };
struct O_00049da8 { virtual void f0(); virtual void f1(); virtual void f2(); virtual void f3(); virtual M_00049da8 *Get(); };
struct S_00049da8 { O_00049da8 *o; int f4; int f8; int fc; int f10; int idx; };
extern "C" float FUN_00049da8(S_00049da8 *s)
{
    if (s->f8 >= 0) {
        HST_ASSERT(s->idx >= 0, 307);
        return -s->o->Get()[s->idx].v[0];
    }
    return -1.0f;
}
