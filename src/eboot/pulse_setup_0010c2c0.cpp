/* HSTMATCH module=EBOOT.BIN start=0x0010c2c0 end=0x0010c69c symbol=FUN_0010c2c0 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. Shares the Pulse/Sprite layout of sprite_pulse_0010c69c.cpp; the sprite-set updates go through a re-read local as retail reloads the pointer per group. */

struct Tag {};

struct Sprite {                 /* 0x2c bytes */
    unsigned int color;         /* 0xAABBGGRR */
    short x0;
    short y0;
    short x1;
    short y1;
    short z;
    unsigned short flags;
    int m10;
    int m14;
    int m18;
    int m1c;
    int m20;
    int m24;
    int m28;
    Sprite();
    short Width() const { return x1 - x0; }
};

struct Elem {
    short x;
    short y;
    unsigned char f4;
    unsigned char f5;
    unsigned char f6;
    unsigned char f7;
    int f8;
};

struct Vec {
    Elem *data;
    int count;
    int cap;
};

struct SpriteSet {
    char pad0[0x10];
    Sprite **sprites;
    int count;
    char pad18[0xd0 - 0x18];
    int fD0;
    int fD4;
    int fD8;
    int fDC;
    int fE0;
    int fE4;
    char padE8[2];
    unsigned char fEA;
    char padEB;
    short fEC;
    short fEE;
    char padF0[0x110 - 0xf0];
    int f110;
    unsigned char f114;
    char pad115[0x120 - 0x115];
    Vec vec;
    Sprite *GetSprite(int i) { return sprites[i]; }
};

struct Pulse {
    SpriteSet *set;
    int pad4;
    int counter;
    int index;
    Sprite normal;              /* +0x10 */
    Sprite active;              /* +0x3c */
    int mode;                   /* +0x68 */
};

extern int DAT_003417a4;
extern int DAT_002d9218;

extern "C" {
void FUN_000ffca4(Sprite *);
void FUN_000ffb3c(Sprite *, int);
void FUN_001087d0(Vec *, int, Tag);
void FUN_0010b76c(SpriteSet *, short, short, int, unsigned char, int);
void FUN_0010b614(SpriteSet *);
}

static inline void MoveX(Sprite *s, short x)
{
    short d = x - s->x0;
    s->x0 = x;
    s->x1 += d;
}

static inline int Pick(const int &g, int d) { return g > 1 ? g : (d <= 0 ? d : 1); }

extern "C" void FUN_0010c2c0(Pulse *self)
{
    self->counter = 0;
    self->mode = 0;
    self->counter = 3;
    Sprite *a = self->set->GetSprite(self->index);
    Sprite *b = self->set->GetSprite(self->index + 1);
    FUN_000ffca4(a);
    FUN_000ffca4(b);
    FUN_000ffb3c(a, 0);
    FUN_000ffb3c(b, 0);
    a->color = 0x80203570;
    b->color = 0x807f4020;
    short wa = a->Width();
    short xa = wa / -2 - 50;
    short wb = b->Width();
    short xb = wb / -2 + 50;
    MoveX(a, xa);
    MoveX(b, xb);
    self->normal = *a;
    self->active = *b;
    SpriteSet *set = self->set;
    set->fD4 = 2;
    set = self->set;
    set->fD8 = 1;
    set->f114 = 1;
    set = self->set;
    set->f110 = 0;
    set->fDC = 0;
    set->fEC = 0;
    set->fEE = 0;
    set->fD0 = 0;
    FUN_001087d0(&set->vec, set->vec.count, Tag());
    FUN_0010b76c(self->set, xa - wa / 4 + 5, 4, 0, 0, 3);
    if (DAT_003417a4 == 9)
        FUN_0010b76c(self->set, xb - wb / 4 - 8, 4, 0, 0, 3);
    else
        FUN_0010b76c(self->set, xb - wb / 4 + 5, 4, 0, 0, 3);
    set = self->set;
    int d = set->fE4 - set->fE0;
    int n = Pick(DAT_002d9218, d);
    if (set->fDC != n) {
        set->fD0 = set->vec.data[set->fDC].x != set->vec.data[n].x ? 3 : 0;
        set->fEA = 1;
    }
    set->fDC = n;
    set->f114 = 1;
    FUN_0010b614(self->set);
}
