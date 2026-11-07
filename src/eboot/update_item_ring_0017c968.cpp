/* HSTMATCH module=EBOOT.BIN start=0x0017c968 end=0x0017ce84 symbol=FUN_0017c968 lang=c++ */
/* Hand-written reconstruction: clone of the source of update_item_ring_0018905c.cpp in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
/* Per-frame update of the item ring owner (same TU as FUN_0017c39c/FUN_0017c6d4/FUN_00189578): while the
   current item is in state 2/3 the ring angle ry advances by speed*20 degrees until it crosses zero (then the
   item gets 5 or 4); the yaw/pitch/roll matrix built from the identity DAT_002bc410 plus the position row is
   handed to every item, and from n >= 3 on each item is stepped through FUN_0017beb4. Vec4 is 16-aligned
   (the matrix sits at sp+32 under the 16-byte top pad) with a memberwise operator= (the extern const source
   gives the one-ahead lwc1 $f1/$f0 copy, the position row the single-$f0 pairs); the item loops use the
   const begin() / non-const end() iterator with a free operator!= (xor/sltiu/xori, end() recomputed per pass).
   EBOOT twin 0x0017c968 (same shape). Names provisional. */
struct Vec4 {
    float x, y, z, w;
    Vec4 &operator=(const Vec4 &o) { x = o.x; y = o.y; z = o.z; w = o.w; return *this; }
} __attribute__((aligned(16)));
struct Mat4 {
    Vec4 r0, r1, r2, r3;
};

struct Sub {
    virtual void v0();
    virtual void v1();
};

struct Link {
    char pad[0x1b4];
    int f1b4;
};

struct Item {
    int type;
    int state;
    char pad8[8];
    Sub sub;
    char pad14[0x60 - 0x14];
    Link *link;
};

template <class T> struct It {
    T *p;
    It(T *q) : p(q) {}
    T &operator*() const { return *p; }
    It &operator++() { ++p; return *this; }
};
template <class T> inline bool operator==(const It<T> &a, const It<T> &b) { return a.p == b.p; }
template <class T> inline bool operator!=(const It<T> &a, const It<T> &b) { return !(a == b); }

template <class T> struct Vec {
    T *data;
    unsigned size;
    It<T> begin() const { return It<T>(data); }
    It<T> end() { return It<T>(data + size); }
};

struct Ctx {
    char pad[0xc];
    float fc;
    char pad10[4];
    int f14;
};

struct Obj {
    int pad0;
    int idx;
    Vec<Item *> items;
    char pad10[0x20 - 0x10];
    Vec4 pos;
    float rx, ry, rz;
    char pad3c[4];
    unsigned char dirty;
    unsigned char f41;
    char pad42[2];
    int counter;
    float speed;
    char pad4c[4];
    int n;
    int pending;
    unsigned char f58;
    char pad59[0x2e4 - 0x59];
    char sel[4];
};

extern "C" {
Ctx *FUN_0016bc7c(void);
int FUN_0016bc70(void);
void FUN_0017cfb4(Obj *, int);
void FUN_0017de30(Obj *, float, int);
void FUN_0017dcfc(Obj *);
void FUN_0004d2cc(float, Mat4 *, const Mat4 *);
void FUN_0004d278(float, Mat4 *, const Mat4 *);
void FUN_0004d320(float, Mat4 *, const Mat4 *);
void FUN_00204400(int);
float FUN_0017dab8(Obj *, int);
void FUN_00093484(float, Sub *);
void FUN_00093434(Sub *, Mat4 *);
int FUN_0017beb4(Item *, int, int, Item *, void *, Obj *);
void FUN_0017df44(Obj *);
void FUN_0017df34(Obj *);
void FUN_0017c39c(Obj *, int);
void FUN_0017c6d4(Obj *, unsigned);
extern const Mat4 DAT_002bc410;
}

extern "C" void FUN_0017c968(Obj *self)
{
    if (self->f58 != 0)
        return;
    Item *cur = self->items.data[self->idx];
    if (self->pending != 0) {
        self->n = self->pending;
        self->pending = 0;
    }
    switch (cur->type) {
    case 2:
    case 3:
        self->ry += self->speed * 20.0f * 0.017453292f;
        if (self->speed > 0.0f) {
            if (self->ry >= 0.0f) {
                self->ry = 0.0f;
                if (cur->type == 3)
                    FUN_0017cfb4(self, 5);
                else if (cur->type == 2)
                    FUN_0017cfb4(self, 4);
            }
        } else if (self->ry <= 0.0f) {
            self->ry = 0.0f;
            if (cur->type == 3)
                FUN_0017cfb4(self, 5);
            else if (cur->type == 2)
                FUN_0017cfb4(self, 4);
        }
        break;
    }
    Mat4 m;
    m = DAT_002bc410;
    Ctx *g = FUN_0016bc7c();
    if (self->n >= 3 && self->dirty == 0 && self->f41 != 0) {
        int c = self->counter + 1;
        self->counter = c;
        if (c > 0x60000000)
            self->counter = c >> 4;
        FUN_0016bc70();
        if (self->counter > g->f14)
            FUN_0017de30(self, g->fc * 0.017453292f, 0);
    }
    FUN_0017dcfc(self);
    FUN_0004d2cc(self->ry, &m, &m);
    FUN_0004d278(self->rx, &m, &m);
    FUN_0004d320(self->rz, &m, &m);
    m.r3 = self->pos;
    int link = 0;
    if (cur->link != 0)
        link = cur->link->f1b4;
    FUN_00204400(link);
    FUN_0016bc7c();
    int i = 0;
    for (It<Item *> it = self->items.begin(); it != self->items.end(); ++it, ++i) {
        Item *e = *it;
        FUN_00093484(FUN_0017dab8(self, i), &e->sub);
        FUN_00093434(&e->sub, &m);
    }
    if (self->n >= 3) {
        self->items.data[self->idx]->sub.v1();
        int j = 0;
        for (It<Item *> it = self->items.begin(); it != self->items.end(); ++it, ++j) {
            Item *e = *it;
            int prev = e->type;
            int r = FUN_0017beb4(e, j, self->idx, self->items.data[self->idx], self->sel, self);
            if (j == self->idx) {
                switch (r) {
                case 2:
                case 3:
                    FUN_0017df44(self);
                    if (self->ry >= 0.0f)
                        self->speed = -1.0f;
                    else
                        self->speed = 1.0f;
                    break;
                case 5:
                case 4:
                    FUN_0017df44(self);
                    break;
                case 0:
                    break;
                default:
                    if (prev != r)
                        FUN_0017df34(self);
                    break;
                }
            }
        }
    }
    FUN_0017c39c(self, 0);
    FUN_0017c6d4(self, 0);
}
