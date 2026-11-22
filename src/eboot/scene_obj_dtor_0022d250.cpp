/* HSTMATCH module=EBOOT.BIN start=0x0022d250 end=0x0022d3f0 symbol=FUN_0022d250 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern "C" {
extern char DAT_0030983c;
extern int *DAT_0034b350;
extern void FUN_00000788(...);
extern void FUN_00000a00(...);
extern int FUN_00004138(...);
extern void FUN_000462a0(...);
extern void FUN_00102ea4(...);
extern int FUN_0015dd9c(...);
extern void FUN_0022ffe4(...);
extern void FUN_00053618(...);
}
struct Alloc {};
struct Vec { int *data; unsigned size; int cap; void shrink(int n, Alloc) { size -= n; } };
struct VC_0 { virtual ~VC_0(); };
struct VC_1 { virtual ~VC_1(); };

struct Sub { int a[3]; int b[3]; };
struct Obj {
  void *vtbl;
  char pad04[0x54];
  Sub s58;
  Sub s70;
  Sub s88;
  char pad[0x21c - 0x88 - 0x18];
  int f21c;
  int f220;
  char pad224[0x290 - 0x224];
  Vec a[15];
  Vec b[15];
};

extern "C" Obj *FUN_0022d250(Obj *self, short flag)
{
  if (self != 0) {
    self->vtbl = &DAT_0030983c;
    FUN_0022ffe4(self);
    for (int i = 0; i < 15; i++) {
      int n = self->a[i].size;
      self->a[i].shrink(n, Alloc());
      int m = self->b[i].size;
      self->b[i].shrink(m, Alloc());
    }
    if (self->f21c) {
      FUN_0015dd9c(self->f21c, 1);
      self->f21c = 0;
    }
    if (self->f220) {
      delete (VC_0 *)self->f220;
      self->f220 = 0;
    }
    if (DAT_0034b350) {
      delete (VC_1 *)DAT_0034b350;
      DAT_0034b350 = 0;
    }
    FUN_00000788((char *)self + 0x344, FUN_00053618, 12, 15);
    FUN_00000788((char *)self + 0x290, FUN_00053618, 12, 15);
    if (&self->s88 != 0) {
      FUN_000462a0(&self->s88.b, -1);
      FUN_00004138(&self->s88, 0);
    }
    if (&self->s70 != 0) {
      FUN_000462a0(&self->s70.b, -1);
      FUN_00004138(&self->s70, 0);
    }
    if (&self->s58 != 0) {
      FUN_000462a0(&self->s58.b, -1);
      FUN_00004138(&self->s58, 0);
    }
    FUN_00102ea4(self, 0);
    if (flag > 0) FUN_00000a00(self);
  }
  return self;
}
