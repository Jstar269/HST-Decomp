/* HSTMATCH module=EBOOT.BIN start=0x00170658 end=0x00170744 symbol=FUN_00170658 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

struct Elem {
  int id;
};

struct It {
  Elem **p;
  It() {}
  It(Elem **q) : p(q) {}
};

inline bool operator==(const It &a, const It &b) { return a.p == b.p; }
inline bool operator!=(const It &a, const It &b) { return !(a == b); }

struct Vec {
  Elem **data;
  int count;
  int size() const { return count; }
  It begin() const { return It(data); }
  It end() { return It(data + count); }
};

struct Obj {
  char pad0[0x1d4];
  Vec *vec;
  char pad1[0x548 - 0x1d8];
  int (*fn)(int);
  char pad2[0x560 - 0x54c];
  int f560;
  int f564;
  int f568;
};

extern "C" void FUN_00170658(Obj *self, int key)
{
  if (key == 0x7fff)
    return;
  if (key == 0x7ffe)
    return;
  int target = self->fn(key);
  Vec *v = self->vec;
  int n = v->size();
  int i = 0;
  for (It it = v->begin(); it != v->end(); ++it.p, ++i) {
    if ((*it.p)->id == target) {
      if (n < 5) {
        self->f560 = i;
        self->f568 = 0;
      } else if (i + 2 >= n) {
        self->f568 = n - 4;
        self->f560 = i - (n - 4);
      } else {
        if (i - 1 < 1) {
          self->f560 = i;
          self->f568 = 0;
        } else {
          self->f560 = 1;
          self->f568 = i - 1;
        }
      }
      self->f564 = i;
      break;
    }
  }
}
