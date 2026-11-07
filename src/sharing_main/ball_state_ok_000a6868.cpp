/* HSTMATCH module=hstp_trans_main.prx start=0x000a6868 end=0x000a69f0 symbol=FUN_000a6868 lang=c++ */
/* Hand-written reconstruction: clone of the source of ball_state_ok_000a4098.cpp in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Ghidra pseudocode (hstgen.py tag a1) corrected by hand, starting from the sn7 near miss: each of the two limit tests reads the limit through its own local Sub pointer (sub1 before the first ladder, sub2 before the GetF230 check), with the operands written f268 < limit; matching idiom: the two Sub pointer locals; exact match verified; names and types await human review. */
typedef unsigned char u8;

struct Sub {
  int a;
  int limit;
};
struct Hd {
  char pad0[0x18];
  float f18;
  char pad1[0x230 - 0x1c];
  int f230;
  int GetF230() const { return f230; }
  char pad2[0x268 - 0x234];
  int f268;
  Sub sub;
};
struct Ctl {
  char pad0[0x88];
  Hd *h;
};
struct Gs {
  char pad0[0x5c];
  u8 f5c;
  char pad1[1];
  u8 f5e;
  u8 f5f;
  char pad2[0x84 - 0x60];
  u8 f84;
};
struct Obj {
  char pad0[0x144];
  int f144;
  char pad1[4];
  u8 f14c;
  char pad2[0x4f0 - 0x14d];
  int f4f0;
  char pad3[0x5a8 - 0x4f4];
  float f5a8;
  char pad4[0x800 - 0x5ac];
  int f800;
};

extern "C" Ctl *DAT_003e19f0;
extern "C" Gs *DAT_003e1e84;
extern "C" u8 DAT_003e19f8;

extern "C" unsigned int FUN_000a6868(Obj *o, int param_2)
{
  Hd *h;
  int b;

  if (o->f14c != 0 && o->f800 >= 0) {
    return 0;
  }
  if (param_2 != 0 && 0 < o->f4f0) {
    return 1;
  }
  if (o->f14c == 0) {
    return 1;
  }
  h = DAT_003e19f0->h;
  Sub *sub1 = &h->sub;
  if ((o->f144 ^ 1) == h->f230 && (h->f268 < sub1->limit) &&
      o->f5a8 * h->f18 < 0.0f &&
      (DAT_003e1e84->f5c == 0 ||
       (DAT_003e19f8 == 0 &&
        (DAT_003e1e84->f84 != 0 || DAT_003e1e84->f5e != 0 ||
         DAT_003e1e84->f5f != 0)))) {
    b = 1;
  } else {
    b = 0;
  }
  if (b) {
    return 1;
  }
  Sub *sub2 = &h->sub;
  if (h->GetF230() < 0) {
    return 1;
  }
  if ((h->f268 < sub2->limit) &&
      (DAT_003e1e84->f5c == 0 ||
       (DAT_003e19f8 == 0 &&
        (DAT_003e1e84->f84 != 0 || DAT_003e1e84->f5e != 0 ||
         DAT_003e1e84->f5f != 0)))) {
    return 1;
  }
  return 0;
}
