/* HSTMATCH module=hstp_trans_main.prx start=0x001bc3cc end=0x001bc4a0 symbol=FUN_001bc3cc lang=c++ */
/* Clone of src/eboot/holder_v2_counter_001aecf4.cpp (hstclone.py tag c211507): addresses substituted; exact match verified; names and types await human review. */
/* Ghidra pseudocode (hstgen.py tag a1/v0) corrected by hand: fixed branch condition to use r->f4 > v->c400 for at-reg comparison, adjusted inner counter logic; exact match verified; names and types await human review. */
struct Base { virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4(); };
struct Counter { char pad0[36]; int f24; int f28; };
struct Holder { Base *p; char pad1[688]; Counter *f2b4; char pad2[328]; int c400; };
struct Result { char pad0[4]; int f4; };
struct Self { char pad0[4]; Holder *f4; Holder *f8; Holder *fc; Holder *f10; };
extern "C" Result *FUN_001aa928(void);
extern "C" void FUN_001bc3cc(Self *self)
{
  Self *s = self;
  Holder *v = s->f4;
  Result *r;
  Counter *c;
  int n;
  v->p->v2();
  v->c400 = v->c400 + 1;
  r = FUN_001aa928();
  if (r->f4 > v->c400) {
  } else {
    v->c400 = 0;
    c = v->f2b4;
    n = c->f24 + 1;
    if (n >= 32) n = 0;
    c->f24 = n;
    c->f28 = n;
  }
  s->f8->p->v2();
  s->fc->p->v2();
  s->f10->p->v2();
}