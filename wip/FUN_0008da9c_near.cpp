/* HSTMATCH module=EBOOT.BIN start=0x0008da9c end=0x0008dba8 symbol=FUN_0008da9c lang=c++ */
struct Tag {};

struct Vec4 {
  float x, y, z, w;
  Vec4() {}
} __attribute__((aligned(16)));

struct Item {
  int a;
  unsigned char b;
  int c;
  Vec4 v;
  Item() {}
  Item(const Item &o) : a(o.a), b(o.b), c(o.c), v(o.v) {}
} __attribute__((aligned(16)));

struct ItemA {
  char data[48];
} __attribute__((aligned(16)));

struct ItemVec {
  Item *data;
  int size;
  unsigned cap;
};

struct Self {
  ItemVec a;
  ItemVec b;
};

extern "C" {
void FUN_00091080(void *v, unsigned n, Tag);
void FUN_00091090(void *v, unsigned n, const void *x, Tag);

void FUN_0008da9c(Self *self, unsigned s0)
{
  ItemA item_a;
  Item item_b;

  if (self->a.size < s0) {
    FUN_00091090(&self->a, s0 - self->a.size, &item_a, Tag());
  } else if (s0 < self->a.size) {
    FUN_00091080(&self->a, self->a.size - s0, Tag());
  }

  int count = self->b.size;
  int t0 = 0;
  if (count <= 0)
    return;

  Item *data = self->b.data;
  int last = count - 1;
  for (; t0 < count; t0++) {
    unsigned a3 = (unsigned)t0 < (unsigned)count ? t0 : last;
    if (data[a3].a >= (int)s0) {
      if (self->b.size < (unsigned)t0) {
        FUN_00091090(&self->b, t0 - self->b.size, &item_b, Tag());
      } else if ((unsigned)t0 < self->b.size) {
        FUN_00091080(&self->b, self->b.size - t0, Tag());
      }
      break;
    }
  }
}
}
// Near miss: 46 words differ out of 67 words (first at +0x3c).
// Frame size 112 matches (addiu sp, sp, -112), prologue matches,
// item_a at 48(sp) and item_b at 16(sp) match, first 23 instructions (up to +0x38) match retail exactly.
// Difference starts at +0x3c where retail uses a2 for count, a1 for data, a0 for last, t0 for loop index,
// and a3 for selected index, with bnez v1, 0x8db1c taking move a3, t0 in the branch delay slot.
// near miss from lane dsn5 (unreviewed)
