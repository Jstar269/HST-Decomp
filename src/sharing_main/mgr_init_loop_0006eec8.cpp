/* HSTMATCH module=hstp_trans_main.prx start=0x0006eec8 end=0x0006f098 symbol=FUN_0006eec8 lang=c++ */
/* Clone of src/eboot/mgr_init_loop_0006d8e0.cpp (hstclone.py tag c085007): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Mgr;
struct Alloc {};
struct Elem { char b[0x70]; };
extern "C" void FUN_00070260(Mgr *p, int b);

struct Mgr {
  void **items;
  unsigned count;
  unsigned cap;
  unsigned n3;
  Elem *data4;
  int f5;
  Mgr(const Alloc & = Alloc()) {
    items = 0;
    count = 0;
    cap = 0;
    FUN_00070260(this, 0);
    n3 = 0;
    data4 = 0;
    f5 = 0;
  }
  void shrink(int n, Alloc) { count -= n; }
};

extern "C" void FUN_0006f4cc();
extern "C" void *FUN_00000bcc(unsigned);
extern "C" void FUN_000143b0(char *buf, const char *fmt, int a, int b);
extern "C" void FUN_000149a8(char *dst, const char *src);
extern "C" void FUN_00014788(char *dst, const char *src);
extern "C" int FUN_0004cfac(const char *path, int a, int b);
extern "C" void FUN_0004d2c4(int h);
extern "C" void *FUN_00051908(void *p, const char *name, int flag);
extern "C" void FUN_00051d38(float v, void *p);
extern "C" void FUN_00051d9c(void *p, int b);
struct Tag { char c; };
extern "C" void FUN_0007021c(Mgr *m, void **item, Tag t);
extern "C" void FUN_0007017c(void *sub, unsigned n);
/* FUN_0006f098 is an out-of-line for_each(first, last, Op) returning the functor (sret in a0). */
struct Op { float s; Op(float v) : s(v) {} };
extern "C" Op FUN_0006f098(Elem *first, Elem *last, Op op);

extern Mgr *DAT_003daf6c;
extern float DAT_003daf74;
extern char DAT_00377308[];
extern char DAT_00377318[];

extern "C" void FUN_0006eec8(int p2, int p3, float scale)
{
  FUN_0006f4cc();
  int i;
  Mgr *p = new Mgr;
  DAT_003daf6c = p;
  i = 0;
  {
    int n = p->count;
    p->shrink(n, Alloc());
  }
  char name[256];
  char name2[256];
  void *item;
  do {
    FUN_000143b0(name, DAT_00377308, p2, i);
    FUN_000149a8(name2, name);
    FUN_00014788(name2, DAT_00377318);
    int r = FUN_0004cfac(name2, 1, 1);
    if (r < 0) {
      break;
    }
    FUN_0004d2c4(r);
    void *q = FUN_00000bcc(0x48);
    if (q != 0) {
      q = FUN_00051908(q, name, 1);
    }
    item = q;
    FUN_00051d38(2000.0f, item);
    FUN_00051d9c(item, 1);
    Mgr *m = DAT_003daf6c;
    if (m->count < m->cap) {
      m->count = m->count + 1;
      m->items[m->count - 1] = item;
    } else {
      FUN_0007021c(m, &item, Tag());
    }
    i++;
  } while (i < 100);
  DAT_003daf74 = scale;
  FUN_0007017c((int *)DAT_003daf6c + 3, p3);
  Op op(DAT_003daf74);
  FUN_0006f098(DAT_003daf6c->data4, DAT_003daf6c->data4 + DAT_003daf6c->n3, op);
}
