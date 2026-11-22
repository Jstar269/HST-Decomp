/* HSTMATCH module=EBOOT.BIN start=0x0027dfac end=0x0027e0e8 symbol=FUN_0027dfac lang=c++ */
/* Hand-written reconstruction from the disassembly (starting from a near-miss draft of lane dsn6; template src/eboot/vec3_reserve_copy_00115f68.cpp): 104-byte element = char[100] + 4-uchar sub-struct with a user default ctor (implicit memberwise copy: 2-per-pass lb/sb loop, then the sub-struct loaded whole), reserve callee 0x27e4c0, trailing unused empty `False` parameter gives the 48-byte frame; exact match verified; names and types await human review. */
struct Tag { char c; };
struct False {};
inline void *operator new(unsigned, void *p) { return p; }
struct Quad { unsigned char b0, b1, b2, b3; };
struct Elem { char a[100]; Quad q; Elem() {} };
struct Vec { Elem *begin; int n; int cap; };
extern "C" void FUN_0027e4c0(Vec *v, int a, Tag t);
extern "C" void FUN_00000a00(void *p);

extern "C" void FUN_0027dfac(Vec *self, int n, False)
{
  Vec tmp;
  Elem *p;
  Elem *end;
  Elem *d;
  tmp.begin = 0;
  tmp.n = 0;
  tmp.cap = 0;
  FUN_0027e4c0(&tmp, n, Tag());
  p = self->begin;
  end = self->begin + self->n;
  d = tmp.begin + tmp.n;
  for (; p < end; p++, tmp.n++, d++) {
    new (d) Elem(*p);
  }
  unsigned c = self->cap;
  self->cap = tmp.cap;
  tmp.cap = c;
  Elem *oldbegin = self->begin;
  self->begin = tmp.begin;
  tmp.begin = oldbegin;
  c = self->n;
  self->n = tmp.n;
  tmp.n = c - (int)c;
  if (tmp.begin != 0)
    FUN_00000a00(tmp.begin);
}
