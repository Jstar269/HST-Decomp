/* HSTMATCH module=EBOOT.BIN start=0x000b7e7c end=0x000b801c symbol=FUN_000b7e7c lang=c++ */
/* Hand-written reconstruction from the disassembly (restructured from the near miss wip/draft_000b7e7c.c); exact match verified; matching idiom: the raw value is stored to the field first and the clamped value (nested ternary) stored again, as retail does; names and types await human review. */
extern "C" {
extern int DAT_002d496c;
extern int DAT_002d4970;
extern char DAT_003072b8;
extern char *DAT_00341ccc;
extern char DAT_002d4974[];
extern char DAT_0030ebf4;
extern void *FUN_00000bcc(unsigned);
extern void FUN_0001ca58(...);
extern void FUN_0001cb88(...);
extern void FUN_0001f06c(...);
extern void FUN_000b793c(...);
extern void FUN_000b9450(...);
extern void *FUN_0015e558(...);
}
struct Obj {
  void *vptr;          /* 0 */
  char pad04[0x38];
  unsigned char b3c;
  unsigned char b3d;
  char pad3e[2];
  int f40;
  char pad44[0xc];
  int f50, f54, f58;
  float f5c;
  int f60;
  int f64, f68, f6c;
  int f70;
  float f74;
  int f78;
  int f7c;
  float f80, f84, f88;
  char pad8c[8];
  int f94;
  float f98, f9c, fa0;
  char pada4[8];
  float fac, fb0, fb4, fb8;
  char padbc[8];
  int fc4, fc8, fcc, fd0;
  float fd4;
  int fd8;
  char paddc[4];
  int fe0, fe4, fe8;
  float fec;
  float ff0;
  int ff4, ff8;
};

extern "C" Obj *FUN_000b7e7c(Obj *self)
{
  FUN_0001f06c(self);
  self->vptr = &DAT_003072b8;
  self->f70 = 0;
  self->f74 = 1.0f;
  self->f78 = 0;
  self->fc4 = 0;
  self->fc8 = 0;
  self->fcc = 0;
  self->fd0 = 0;
  self->fd4 = -2.0f;
  self->fd8 = 0;
  self->ff0 = 30.0f;
  self->ff4 = 0;
  self->ff8 = 0;
  int v = *(char *)(DAT_00341ccc + 0x1052);
  self->f64 = v;
  int w = v < DAT_002d496c ? DAT_002d496c : (DAT_002d4970 < v ? DAT_002d4970 : v);
  self->f64 = w;
  self->f68 = w;
  self->f6c = self->f64;
  self->f7c = 0;
  self->f80 = 16.0f;
  self->f84 = 39.0f;
  self->f88 = 26.0f;
  self->f94 = 0;
  self->f98 = 16.0f;
  self->f9c = 39.0f;
  self->fa0 = 26.0f;
  self->fac = -1.3f;
  self->fb0 = 5.6f;
  self->fb4 = 6.0f;
  self->fb8 = 33.0f;
  self->fe0 = 0;
  self->fe4 = 0;
  self->fe8 = 0;
  self->fec = 1.0f;
  FUN_0001ca58(&DAT_0030ebf4, DAT_002d4974);
  FUN_000b9450(self, 0);
  FUN_0001cb88(&DAT_0030ebf4);
  void *u = FUN_00000bcc(0xc0);
  if (u != 0) u = FUN_0015e558(u);
  self->ff8 = (int)u;
  FUN_000b793c(u);
  self->b3c = 0;
  self->b3d = 0;
  self->f40 = 0;
  self->f50 = 0;
  self->f54 = 0;
  self->f58 = 0;
  self->f5c = 1.0f;
  self->f60 = 0;
  return self;
}
