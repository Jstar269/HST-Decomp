/* HSTMATCH module=EBOOT.BIN start=0x001aef94 end=0x001af0d8 symbol=FUN_001aef94 lang=c++ */
/* Hand-written reconstruction from the disassembly (starting from a near-miss draft of lane dsn6), restructured by hand: p+28 sub-record, Node items accessed through an inline unsigned operator[] (keeps the loop index un-strength-reduced); exact match verified; names and types await human review. */
extern int DAT_002e2138;
extern int DAT_002e213c;
extern "C" void FUN_001a9e34(int);
extern "C" int FUN_0019d474(void);
extern "C" void FUN_00103628(int, int);
extern "C" void FUN_001abe98(void *);
extern "C" int FUN_001a49dc(int, int);
extern "C" void FUN_000ffb3c(int, int);

static inline int Clamp(const int &v, const int &lo, const int &hi)
{
    return v < lo ? lo : (hi < v ? hi : v);
}

struct Node { char pad[16]; int **items; int count; int *&operator[](unsigned i) { return items[i]; } };
struct Sub { int x; Node *node; };
struct P { char pad[28]; Sub sub; };
struct Self { char pad0[4]; int f4; void *f8; void *fc; P *f10; };

extern "C" void FUN_001aef94(struct Self *self)
{
    FUN_001a9e34(self->f4);
    int *q = (int *)self->f8;
    FUN_0019d474();
    FUN_00103628(*q, 0);
    FUN_001abe98(q);
    FUN_00103628(*(int *)self->fc, 1);
    P *p = self->f10;
    FUN_00103628(*(int *)p, 0);
    int a = FUN_0019d474();
    int b = FUN_0019d474();
    int idx = FUN_001a49dc(b, *(int *)(a + 0x2c4)) - 1;
    int sel = Clamp(idx, DAT_002e2138, DAT_002e213c);
    Sub *sub = &p->sub;
    Node *node = sub->node;
    int i = 0;
    if (0 < node->count) {
        do {
            FUN_000ffb3c((int)(*node)[i], i != sel);
            i++;
        } while (i < node->count);
    }
    *(*sub->node)[sel] = 0x80808080;
}
