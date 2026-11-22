/* HSTMATCH module=EBOOT.BIN start=0x000ec0ec end=0x000ec204 symbol=FUN_000ec0ec lang=c++ */
/* Hand-written reconstruction from the disassembly (starting from a near-miss draft of lane km2); exact match verified; names and types await human review. */
/* Virtual destructor of a class that owns an array of 16 slots (96 bytes apart, managed pointer at +0x40): vtable reset, base dtor 0xec664, each slot released through virtual slot 0 and cleared, then the sub-objects at +0x68 and +0x58 are destroyed and the base dtor 0x1f0f0 runs.
   matching idiom: the member-address tests are written as nested `if (&self->m)` pairs (inlined sub-object dtors each test their own this), which keeps the test in a0; the slot walk needs opt_strength_reduction off. */
#pragma opt_strength_reduction off

struct VC_0 { virtual ~VC_0(); };
struct Tag {};

struct V {
  void *data;
  int size;
};

struct C {
  void *p;
  int n;
};

struct Obj {
  void *vptr;
  char pad[0x54];
  V v58;
  char pad2[8];
  C c68;
};

extern "C" {
extern char DAT_00307728[];
void FUN_000ec664(int p);
void FUN_000ed8c0(C *p, int v, Tag t);
void FUN_000ed8d0(V *p, int v);
void FUN_000ed810(Obj *p, int v);
void FUN_0001f0f0(Obj *p, int v);
void FUN_00000a00(void *p);
}

extern "C" Obj *FUN_000ec0ec(Obj *self, short flag)
{
  if (self != 0) {
    self->vptr = &DAT_00307728;
    FUN_000ec664((int)self);
    int i = 0;
    do {
      VC_0 *p = *(VC_0 **)((char *)self->v58.data + i * 96 + 0x40);
      if (p != 0) {
        delete p;
        *(VC_0 **)((char *)self->v58.data + i * 96 + 0x40) = 0;
      }
      i++;
    } while (i < 16);
    FUN_000ed8c0(&self->c68, self->c68.n, Tag());
    if (&self->c68) {
      if (&self->c68)
        FUN_000ed8d0((V *)&self->c68, 0);
    }
    if (&self->v58) {
      if (&self->v58)
        FUN_000ed810((Obj *)&self->v58, 0);
    }
    FUN_0001f0f0(self, 0);
    if (flag > 0) {
      FUN_00000a00(self);
    }
  }
  return self;
}
// near miss from lane km2 (unreviewed)
