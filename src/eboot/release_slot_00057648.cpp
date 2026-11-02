/* HSTMATCH module=EBOOT.BIN start=0x00057648 end=0x00057730 symbol=FUN_00057648 lang=c++ */
/* Hand-written reconstruction (external matching lane); exact match verified; names and types await human review. */
/* Found with hstnearperm.py (tag c2): the seed differed only in the tail; the one change that makes it exact is the (int) on the array index of the load, which makes MWCC compute idx*4 again after the call as retail does. Exact match verified; names and types await human review. */
struct Node {
  char pad[0xf0];
  unsigned a;
  char pad2[0x1c];
  unsigned b;
};

struct Obj {
  char pad[0xe0];
  void **items;
  unsigned count;
};

extern "C" void FUN_00057b58(float f, void *obj, int idx, int flag);
extern "C" void FUN_00057bc0(float f1, void *obj, int idx, int flag, float f2, float f3);
extern "C" void FUN_000561d4(void *p, int flag);

extern "C" void FUN_00057648(Obj *self, unsigned idx)
{
  if ((int)idx < 0) {
    return;
  }
  if (idx >= self->count) {
    return;
  } else {
    unsigned i = 0;
    Node *n = (Node *)self;
    do {
      if (n->a == idx) {
        FUN_00057b58(0.0f, self, i, -1);
      }
      if (n->b == idx) {
        FUN_00057bc0(0.0f, self, i, -1, 0.0f, 0.0f);
      }
      i++;
      n = (Node *)((char *)n + 0x34);
    } while (i < 2);
    void *item = self->items[(int)idx];
    if (item != 0 && item != 0) {
      FUN_000561d4(item, 1);
      self->items[idx] = 0;
    }
  }
}
