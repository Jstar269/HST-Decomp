/* HSTMATCH module=EBOOT.BIN start=0x00288cf4 end=0x00288dec symbol=FUN_00288cf4 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

extern "C" {
void FUN_0012a254(void *, int, int, int);
void FUN_0014c214(void *, int, int, void *);
void FUN_0014c914(int, void *);
void FUN_0014c820(void *, int);
}

struct Derived;

struct Obj {
  virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
  virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7();
  virtual void v8(); virtual void v9(); virtual void v10();
  virtual int state();
  int f4;
  int f8;
  int fc;
  char pad10[0x2c];
  int f3c;

  bool IsActive() { return state() == 2 || state() == 3; }
  Derived *AsDerived();
};

struct Base0 {
  char pad[0xe0];
};

struct Derived : Base0, Obj {};

inline Derived *Obj::AsDerived()
{
  return IsActive() ? static_cast<Derived *>(this) : 0;
}

extern "C" void FUN_00288cf4(Obj *self)
{
  if (self->IsActive()) {
    FUN_0012a254(self->AsDerived(), 0, -1, 0);
  }
  if (self->f4 != 0) {
    char local[36];
    FUN_0014c214(local, self->f4, self->f3c, self);
    FUN_0014c914(self->fc, local);
    FUN_0014c820(local, -1);
  }
}
