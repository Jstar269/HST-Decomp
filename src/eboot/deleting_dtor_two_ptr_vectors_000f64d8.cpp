/* HSTMATCH module=EBOOT.BIN start=0x000f64d8 end=0x000f6690 symbol=FUN_000f64d8 lang=c++ */
/* Hand-written reconstruction: clone of the source of deleting_dtor_two_ptr_vectors_000fbbcc.cpp in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
/* Modelled on src/eboot/FUN_000e6d4c.cpp and erase_entry_by_key_*.cpp: free !(a == b) iterator operator!=, const begin() / non-const end(), `int n = size; shrink(n, Tag())` clear, the out-of-line member destructors called as FUN_ addresses behind duplicated null tests. */
struct Tag {};
struct Base {
  virtual ~Base();
};
struct Item {
  int x;
};
template <class T> struct Iter {
  T **p;
  Iter(T **q) : p(q) {}
  T *&operator*() const { return *p; }
};
template <class T> inline bool operator==(const Iter<T> &a, const Iter<T> &b) { return a.p == b.p; }
template <class T> inline bool operator!=(const Iter<T> &a, const Iter<T> &b) { return !(a == b); }
template <class T> struct Vec {
  T **data;
  int size;
  Iter<T> begin() const { return Iter<T>(data); }
  Iter<T> end() { return Iter<T>(data + size); }
  void shrink(int n, Tag) { size -= n; }
};
struct Self {
  Base *m0;
  Vec<Base> vec1;
  char pad[0x830 - 12];
  Vec<Item> vec2;
};
extern "C" {
extern void FUN_00000a00(void *);
extern void FUN_0006eb04(Vec<Base> *, int);
extern Item **FUN_000fc19c(Vec<Item> *, Item **, Tag);
extern Base **FUN_000fc210(Vec<Base> *, Base **, Tag);
extern void FUN_000fc398(Vec<Item> *, int);
}

extern "C" Self *FUN_000f64d8(Self *self, short flag)
{
  if (self != 0) {
    if (self->m0 != 0) {
      delete self->m0;
      self->m0 = 0;
    }
    Iter<Base> it = self->vec1.begin();
    if (it != self->vec1.end()) {
      do {
        if (*it != 0) {
          delete *it;
          *it = 0;
        }
        it = Iter<Base>(FUN_000fc210(&self->vec1, it.p, Tag()));
      } while (it != self->vec1.end());
    }
    Iter<Item> it2 = self->vec2.begin();
    if (it2 != self->vec2.end()) {
      do {
        if (*it2 != 0) {
          FUN_00000a00(*it2);
          *it2 = 0;
        }
        it2 = Iter<Item>(FUN_000fc19c(&self->vec2, it2.p, Tag()));
      } while (it2 != self->vec2.end());
    }
    int n = self->vec1.size;
    self->vec1.shrink(n, Tag());
    Vec<Item> *v2 = &self->vec2;
    if (v2 != 0 && v2 != 0) {
      FUN_000fc398(v2, 0);
    }
    Vec<Base> *v1 = &self->vec1;
    if (v1 != 0 && v1 != 0) {
      FUN_0006eb04(v1, 0);
    }
    if (0 < flag) {
      FUN_00000a00(self);
    }
  }
  return self;
}
