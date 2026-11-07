/* HSTMATCH module=hstp_trans_main.prx start=0x001a3b2c end=0x001a3c20 symbol=FUN_001a3b2c lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern "C" {
extern void FUN_001a33a4(...);
extern const char DAT_00388f14[], DAT_00388f40[], DAT_00388f48[], DAT_00388eec[], DAT_00388f0c[];
extern const char *DAT_00388e74[];
extern void *FUN_00109a44(void *o, const char *s, int z);
extern void *FUN_00109304(void *o, const char *s, int z);
struct Rec { char pad[36]; int f; };
extern Rec *FUN_00108124(void *p, const char *s);
}
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_001a33a4(DAT_00388f14, DAT_00388f40, DAT_00388f48, line); for (;;) {} } } while (0)
struct Obj { virtual void v0(); virtual void v1(); virtual void v2(); };
struct Self { Obj *o; int m[31]; };
extern Self *DAT_003eaaa8;
extern "C" void FUN_001a3b2c(void)
{
  Self *s = DAT_003eaaa8;
  if (s != 0) {
    void *h;
    int i;
    FUN_00109a44(s->o, DAT_00388eec, 0);
    h = FUN_00109304(s->o, DAT_00388f0c, 0);
    for (i = 0; i < 30; i++) {
      Rec *r = FUN_00108124(h, DAT_00388e74[i]);
      HST_ASSERT(r != 0, 233);
      s->m[i] = (int)r;
      s->m[i + 30] = r->f;
    }
    s->o->v2();
  }
}
