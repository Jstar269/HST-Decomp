/* HSTMATCH module=EBOOT.BIN start=0x00103898 end=0x001039d8 symbol=FUN_00103898 lang=c++ */
/* Hand-written reconstruction from the disassembly (restructured from the near miss wip/FUN_00103898_near.c); exact match verified; matching idioms: the push helper takes the item by const reference plus an unused empty Tag (frame slots), grows with `size += 1; data[size - 1] = e`, and the final sort call takes a Tag(); names and types await human review. */
struct Tag { char c; };
struct Entry { char pad[0x20]; float key; };
struct Item { char pad[0x20]; Entry *entry; };
struct Group {
  char pad[0x10];
  Item **items;
  int count;
  char pad18[8];
  float key;
  Item *At(unsigned i) { return items[i]; }
};
struct Vec {
  Entry **data;
  unsigned size;
  unsigned cap;
};
extern "C" void FUN_001073ac(Vec *, Entry **, Tag);
extern "C" void FUN_00106edc(Entry **, Entry **, Tag);

inline void Push(Vec *v, Entry *const &e, Tag) {
  if (v->size < v->cap) {
    v->size += 1;
    v->data[v->size - 1] = e;
  } else {
    FUN_001073ac(v, (Entry **)&e, Tag());
  }
}

struct It {
  Group **p;
  It() {}
  It(Group **q) : p(q) {}
  Group *operator*() const { return *p; }
  It &operator++() { ++p; return *this; }
};
inline bool operator==(const It &a, const It &b) { return a.p == b.p; }
inline bool operator!=(const It &a, const It &b) { return !(a == b); }

struct Self {
  char pad[4];
  Group **begin;
  int n;
  char padc[4];
  Vec arr;
  It Begin() const { return It(begin); }
  It End() const { return It(begin + n); }
};

extern "C" void FUN_00103898(Self *self)
{
  for (It it = self->Begin(); it != self->End(); ++it) {
    Group *g = *it;
    float key = g->key;
    int i = 0;
    if (g->count > 0) {
      do {
        Entry *e = g->At(i)->entry;
        if (e != 0 && key != e->key) {
          Push(&self->arr, e, Tag());
        }
        i++;
      } while (i < g->count);
    }
  }
  FUN_00106edc(self->arr.data, self->arr.data + self->arr.size, Tag());
}
