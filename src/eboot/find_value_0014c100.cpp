/* HSTMATCH module=EBOOT.BIN start=0x0014c100 end=0x0014c214 symbol=FUN_0014c100 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

struct Rec {
  char *key;
  void *value;
};

struct Iter {
  Rec *p;
  Iter(Rec *q) : p(q) {}
  Iter &operator++() { ++p; return *this; }
  Rec &operator*() const { return *p; }
};

inline bool operator==(const Iter &a, const Iter &b) { return a.p == b.p; }
inline bool operator!=(const Iter &a, const Iter &b) { return a.p != b.p; }

struct Cont {
  Rec *data;
  int count;
  Iter begin() { return Iter(data); }
  Iter end() { return Iter(data + count); }
};

struct ModuleCtx {
  int f0;
  int f4;
  int owned;
  int pad[3];
  int nodes[3];
};

extern "C" {
int FUN_00014934(const char *a, const char *b);
int FUN_0014c794(ModuleCtx *, Cont *, void *, int);
void FUN_0014f528(void *);
ModuleCtx *FUN_0014c914(ModuleCtx *, ModuleCtx *);
int FUN_0014c820(ModuleCtx *, int);
}

struct Pred {
  const char *key;
  Pred(const char *k) : key(k) {}
  bool operator()(const Rec &r) const { return FUN_00014934(r.key, key) == 0; }
};

template <class It, class P> inline It find_if(It first, It last, P pred)
{
  while (first != last && !pred(*first))
    ++first;
  return first;
}

extern "C" int FUN_0014c100(ModuleCtx *dest, Cont *c, const char *key, int arg)
{
  Iter it = find_if(c->begin(), c->end(), Pred(key));
  if (it == c->end())
    return FUN_0014c794(dest, 0, 0, 0);
  ModuleCtx temp;
  FUN_0014c794(&temp, c, it.p->value, arg);
  dest->owned = 0;
  FUN_0014f528(&dest->nodes);
  FUN_0014c914(dest, &temp);
  FUN_0014c820(&temp, -1);
  return 0;
}
