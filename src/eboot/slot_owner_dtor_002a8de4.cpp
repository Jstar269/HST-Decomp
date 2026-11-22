/* HSTMATCH module=EBOOT.BIN start=0x002a8de4 end=0x002a8fbc symbol=FUN_002a8de4 lang=c++ */
/* Hand-written reconstruction from the disassembly (restructured from the near miss wip/draft_002a8de4.cpp); exact match verified; matching idioms: null tests of the inlined member destructors written twice, the vector size read for shrink() through a cast; names and types await human review. */
extern "C" {
extern char DAT_00309f18;
extern void FUN_00000788(...);
extern void FUN_00000a00(...);
extern int FUN_00049094(...);
extern void FUN_00055e48(...);
extern void FUN_0005ba00(...);
extern void FUN_000b42f8(...);
extern void FUN_00102ea4(...);
extern void FUN_00053618(...);
}
struct VC_0 { virtual ~VC_0(); };
struct Alloc {};
struct Vec {
  int **data;
  unsigned size;
  void shrink(int n, Alloc) { size -= n; }
};
struct Item {
  VC_0 *f0;
  Vec v;
};
struct Vec3 { int **data; unsigned size; int cap; void shrink(int n, Alloc) { size -= n; } };

struct Obj {
  void *vtbl;
  char pad04[0x30ec - 4];
  Vec3 recs[2];
  Item *items[4];
  int pad3114[4];
  int *f3124;
  int *f3128[4];
};

extern "C" Obj *FUN_002a8de4(Obj *self, short flag)
{
  int i;
  unsigned u;
  Obj *pp;
  int off;
  Item *it;
  if (self != 0) {
    self->vtbl = &DAT_00309f18;
    i = 0;
    pp = self;
    do {
      it = pp->items[0];
      if (it != 0) {
        if (it != 0) {
          FUN_00049094(1);
          if (it->f0 != 0) {
            delete it->f0;
            it->f0 = 0;
          }
          u = 0;
          if (it->v.size != 0) {
            off = 0;
            do {
              int *e = *(int **)((char *)it->v.data + off);
              if (e != 0) {
                FUN_00055e48(e, 1);
                *(int **)((char *)it->v.data + off) = 0;
              }
              u++;
              off += 4;
            } while (u < it->v.size);
          }
          {
            int n = ((Vec *)&it->v)->size;
            it->v.shrink(n, Alloc());
          }
          if (&it->v != 0 && &it->v != 0) {
            FUN_000b42f8(&it->v, 0);
          }
          FUN_00000a00(it);
        }
        pp->items[0] = 0;
      }
      if (pp->f3128[0] != 0) {
        FUN_0005ba00(pp->f3128[0], 1);
        pp->f3128[0] = 0;
      }
      i++;
      pp = (Obj *)((char *)pp + 4);
    } while (i < 4);
    if (self->f3124 != 0 && self->f3124 != 0) {
      FUN_0005ba00(self->f3124, 1);
      self->f3124 = 0;
    }
    for (int j = 0; j < 2; j++) {
      int n = self->recs[j].size;
      self->recs[j].shrink(n, Alloc());
    }
    FUN_00000788((char *)self + 0x30ec, FUN_00053618, 12, 2);
    FUN_00102ea4(self, 0);
    if (flag > 0) FUN_00000a00(self);
  }
  return self;
}
