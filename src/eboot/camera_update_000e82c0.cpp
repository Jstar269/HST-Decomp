/* HSTMATCH module=EBOOT.BIN start=0x000e82c0 end=0x000e8458 symbol=FUN_000e82c0 lang=c++ */
/* Hand-written reconstruction from the disassembly (restructured from the near miss wip/draft_000e82c0.c); exact match verified; matching idiom: the 64-byte copy into the outgoing matrix is written as sub-struct copies in the order a,b,c,d,f,e (retail loads the last three-float group before the lone int word); names and types await human review. */
struct V3 { float x, y, z; };
struct M16 { float f[16]; };
struct W3 { float x, y, z; W3() {} };
struct M2 { W3 a, b, c, d; int e; W3 f; M2() {} };
struct V4 { float x, y, z, w; };
struct Inner { char pad[4]; };
struct Obj {
  char pad00[0x3c];
  void *f3c;
  float f40;
  char pad44[0xc];
  Inner inner;
  char pad54[0x320 - 0x54];
  V4 v320;
  char pad330[0x50];
  V3 v380;
  float f38c;
  float f390;
};
extern "C" {
extern int DAT_003313c4;
extern void FUN_00055044(void *, void *);
extern void FUN_00055798(void *, void *);
extern void FUN_0005ba78(void *, int);
extern void FUN_0005c76c(void *, float, float, float, float);
extern void FUN_0005c854(void *, int, void *, void *);
extern void FUN_0005cbac(void *, float *, float *, float *, float *);

void FUN_000e82c0(Obj *self, M16 *src)
{
  float t1, t2, t3, t4;
  M16 m;
  M2 mm;
  V4 up0;
  V4 up1;
  m = *src;
  FUN_0005ba78(&self->inner, DAT_003313c4);
  FUN_0005cbac(&self->inner, &t1, &t2, &t3, &t4);
  FUN_0005c76c(&self->inner, self->v380.x, self->v380.y, self->v380.z, self->f40 * self->f390);
  up0.x = 0.0f; up0.y = 0.0f; up0.z = -1.0f; up0.w = 0.0f;
  FUN_0005c854(&self->inner, 0, &up0, &self->v380);
  up1.x = 0.0f; up1.y = 0.0f; up1.z = 1.0f; up1.w = 0.0f;
  FUN_0005c854(&self->inner, 1, &up1, &self->v380);
  {
    M2 *ms = (M2 *)&m;
    mm.a = ms->a;
    mm.b = ms->b;
    mm.c = ms->c;
    mm.d = ms->d;
    mm.f = ms->f;
    mm.e = ms->e;
  }
  *(float *)&mm.e = self->v320.x;
  mm.f.x = self->v320.y;
  mm.f.y = self->v320.z;
  mm.f.z = self->v320.w;
  FUN_00055044(self->f3c, &mm);
  FUN_00055798(self->f3c, &self->inner);
}
}
