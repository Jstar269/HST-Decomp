/* HSTMATCH module=EBOOT.BIN start=0x0019cb00 end=0x0019cec4 symbol=FUN_0019cb00 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. The base constructors are inlined as explicit calls with vtable stores; the slot-6 virtual goes through a cast to a polymorphic view. */

struct Sprite {                 /* 0x2c bytes */
    unsigned int color;
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
};

struct Vec {
    void *data;
    int count;
    int cap;
};

struct Panel {
    void *vtable;
    char pad4[0x10 - 4];
    Sprite **sprites;
    int count;
    char pad18[0x30 - 0x18];
    float f30;
    char pad34[0x44 - 0x34];
    float f44;
    char pad48[0x60 - 0x48];
    float f60;
    float f64;
    char pad68[0xd4 - 0x68];
    int fD4;
    int fD8;
    char padDC[0x114 - 0xdc];
    unsigned char f114;
    char pad115[0x120 - 0x115];
    Vec vec;
    char pad12c[0x134 - 0x12c];
    int f134;
    int f138;
    Sprite saved[3];
    int f1c0;
    float f1c4;
    float f1c8;
    float f1cc;
};

struct VPanel {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void SetDepth(float z);
};

extern char DAT_002e1b44[];
extern char DAT_002c2ad8[];
extern char DAT_00307f60[];
extern char DAT_003091e8[];

extern "C" {
void FUN_00103d80(Panel *, const char *);
void FUN_00103d6c(Vec *);
void FUN_0010b308(Panel *);
void FUN_00100de8(Panel *, void *);
void FUN_001016f4(Panel *, const char *, int);
void FUN_000ffb3c(Sprite *, int);
void FUN_0010b76c(Panel *, short, short, int, unsigned char, int);
}

static inline void MoveTo(Sprite *s, short x, short y)
{
    short dx = x - s->x0;
    s->x0 = x;
    s->x1 += dx;
    short dy = y - s->y0;
    s->y0 = y;
    s->y1 += dy;
}

extern "C" Panel *FUN_0019cb00(Panel *self, void *arg)
{
    FUN_00103d80(self, DAT_002e1b44);
    self->vtable = DAT_00307f60;
    FUN_00103d6c(&self->vec);
    FUN_0010b308(self);
    self->vtable = DAT_003091e8;
    self->f134 = 0;
    self->f138 = 0;
    self->f1c0 = 0;
    FUN_00100de8(self, arg);
    FUN_001016f4(self, DAT_002c2ad8, 6);
    ((VPanel *)self)->SetDepth(3100.0f);
    FUN_000ffb3c(self->sprites[0], 0);
    FUN_000ffb3c(self->sprites[1], 0);
    FUN_000ffb3c(self->sprites[2], 1);
    MoveTo(self->sprites[1], 0, self->sprites[0]->y1);
    MoveTo(self->sprites[2], 0, self->sprites[1]->y1);
    self->fD4 = 3;
    self->fD8 = 2;
    self->f114 = 1;
    Sprite *s = self->sprites[0];
    short w = s->x1 - s->x0;
    FUN_0010b76c(self, 4 - w / 4, s->y0 + 2, 0, 0, 3);
    s = self->sprites[1];
    w = s->x1 - s->x0;
    FUN_0010b76c(self, 4 - w / 4, s->y0 + 2, 0, 0, 3);
    self->f30 = 0.9f;
    self->f44 = 0.9f;
    self->f60 = 178.0f;
    self->f64 = 184.0f;
    self->saved[0] = *self->sprites[0];
    self->saved[1] = *self->sprites[1];
    self->saved[2] = *self->sprites[2];
    self->f1c4 = 1.0f;
    self->f1c8 = 1.0f;
    self->f1cc = 1.0f;
    return self;
}
