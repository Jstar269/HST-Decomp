/* HSTMATCH module=EBOOT.BIN start=0x0005d638 end=0x0005d818 symbol=FUN_0005d638 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. Matching idiom: the inline push helper takes an unused by-value Tag (MSL-style) that reserves the extra frame slot. */
struct Pair {
  float *data;
  unsigned size;
  unsigned cap;
};

struct Rec {
  unsigned char flag;
  char pad0[3];
  int f4;
  Pair v;
  int count;
  float *tab;
  int f1c;
  int f20;
  Pair w;
};

struct Self {
  void *handle;
  Rec **data;
  unsigned size;
  unsigned cap;
  float limit;
};

struct Tag { char c; };

extern "C" {
void FUN_0005d8c4(Self *);
void *FUN_0004cad0(void *, int *, int, int);
void *FUN_00000bcc(unsigned);
void *FUN_0005cfb4(Rec *, void *);
void FUN_0005dc60(Rec ***, Rec **, struct Tag);
void FUN_0005d8d0(Self *, void *, void *, void *, void *, void *, void *);
}

inline float Max(const float &a, const float &b) { return a < b ? b : a; }

static inline void Push(Self *s, Rec *&x, Tag)
{
  if (s->size < s->cap) {
    s->size = s->size + 1;
    s->data[s->size - 1] = x;
  } else {
    FUN_0005dc60(&s->data, &x, Tag());
  }
}
extern "C" void *FUN_0005d638(Self *self, void *src, void *a2, void *a3, void *a4, void *a5, void *a6, void *a7)
{
  int out;
  Rec *slot;
  Rec *hdr;
  Rec *item;
  void *cur;
  int n;
  int i;
  float last;
  float v;

  self->data = 0;
  self->size = 0;
  self->cap = 0;
  FUN_0005d8c4(self);
  hdr = (Rec *)FUN_0004cad0(src, &out, 16, 1);
  self->handle = hdr;
  if (hdr != 0) {
  n = *(int *)hdr;
  cur = (char *)hdr + 4;
  for (i = 0; i < n; i++) {
    item = (Rec *)FUN_00000bcc(48);
    if (item != 0) {
      item->v.data = 0;
      item->v.size = 0;
      item->v.cap = 0;
      item->w.data = 0;
      item->w.size = 0;
      item->w.cap = 0;
      item->flag = 0;
      item->f4 = 0;
      item->count = 0;
      item->tab = 0;
      item->f1c = 0;
      item->f20 = 0;
    }
    slot = item;
    cur = FUN_0005cfb4(item, cur);
    Push(self, slot, Tag());
    last = slot->count > 0 ? slot->tab[slot->count - 1] : 0.0f;
    v = 30.0f * last;
    const float &m = self->limit < v ? v : self->limit;
    self->limit = m;
  }
  if (a2 != 0 && a3 != 0)
    FUN_0005d8d0(self, a2, a3, a4, a5, a6, a7);
  }
  return self;
}
