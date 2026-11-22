/* HSTMATCH module=EBOOT.BIN start=0x001c7160 end=0x001c725c symbol=FUN_001c7160 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified (matching idiom: distinct index types (int)idx / (long)idx / unsigned operator[] to prevent CSE of idx<<2 across delete[]); names and types await human review. */

struct Tag {};
struct Alloc {};

struct Vec;
extern "C" {
void FUN_001ce7b0(Vec *, unsigned int, char *const *, Tag);
void FUN_000009e4(void *);
int FUN_00014a40(const char *);
char *FUN_00000a90(int);
void FUN_000149a8(char *, const char *);
}

struct Vec {
  char **data;
  unsigned int size;
  unsigned int cap;
  char *&operator[](unsigned int i) { return data[i]; }
  void shrink(unsigned int n, Alloc) { size -= n; }
  void resize(unsigned int sz, char *val) {
    if (sz > size)
      FUN_001ce7b0(this, sz - size, &val, Tag());
    else if (sz < size)
      shrink(size - sz, Alloc());
  }
};

struct Obj {
  char pad[0x64];
  Vec vec;
};

extern "C" void FUN_001c7160(Obj *self, unsigned int idx, const char *s)
{
  if (idx >= self->vec.size)
    self->vec.resize(idx + 1, 0);
  if (self->vec.data[(int)idx] != 0) {
    FUN_000009e4(self->vec.data[(int)idx]);
    self->vec.data[(long)idx] = 0;
  }
  char *&slot = self->vec[idx];
  slot = FUN_00000a90(FUN_00014a40(s) + 1);
  FUN_000149a8(self->vec[idx], s);
}
