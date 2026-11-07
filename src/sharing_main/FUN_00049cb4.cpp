/* HSTMATCH module=hstp_trans_main.prx start=0x00049cb4 end=0x00049da8 symbol=FUN_00049cb4 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */ /* matching idiom: the product is computed in a local before the final multiply (a single expression swaps the mul operands) */
extern "C" {
extern void FUN_000495f8(...);
extern const char DAT_00375fa4[], DAT_00375ff4[], DAT_00375fe0[];
}
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_000495f8(DAT_00375fa4, DAT_00375ff4, DAT_00375fe0, line); for (;;) {} } } while (0)
struct Ent { char pad[48]; float x; float y; char pad2[8]; };
struct Src { virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual Ent *v4(); };
struct Self { Src *src; int m4; int m8; int pad[2]; int m14; int m18; };
extern "C" float FUN_00049cb4(Self *s)
{
  if (s->m8 >= 0) {
    HST_ASSERT(s->m14 >= 0, 281);
    if (s->m18 >= 0) {
      Ent *e = s->src->v4();
      float a = 0.017453292f * -*(float *)((char *)e + (s->m14 << 6) + 52);
      a *= 0.57f;
      return a;
    } else {
      Ent *e = s->src->v4();
      return 0.017453292f * -*(float *)((char *)e + (s->m14 << 6) + 48);
    }
  }
  return 0.7853982f;
}
