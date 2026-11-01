/* HSTMATCH module=EBOOT.BIN start=0x001150d8 end=0x00115588 symbol=FUN_001150d8 lang=c++ */
#pragma exceptions off

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

struct Node0 {
    u8 pad[4];
    short x;        // +4
    short y;        // +6
};

struct Node2 {
    u8 pad[8];
    short x;        // +8
};

struct Node6 {
    u8 pad[10];
    short y;        // +10
};

struct Sub10 {
    Node0 *n0;      // 0
    u8 pad4[4];
    Node2 *n2;      // 8
    u8 padc[12];
    Node6 *n6;      // 24
};

struct Sub54Inner {
    u8 pad[14];
    u16 flags;      // 14
};

struct Sub54 {
    u8 pad[16];
    Sub10 *f10;     // 16
};

struct Sub58Obj {
    float pad[25];  // 100 / 4
    float f64;      // 100
};

struct Sub58 {
    int f0;
    void (**vtable)(); // 4
};

struct Sub5c {
    u8 pad[96];
    float f60;      // 96
    float f64;      // 100
};

struct Sub64Inner {
    u8 pad[96];
    float f60;      // 96
    float f64;      // 100
};

struct Sub04Obj {
    u8 pad[48];
    float f30;      // 48
    u8 pad34[16];
    float f44;      // 68
    u8 pad48[48];
    float f78;      // 120
};

struct Msg {
    u32 a, b, c, d;
};

struct Vector;
struct MsgIter {
    Msg *ptr;
};

extern "C" void FUN_0011619c(void *, MsgIter, signed char);

struct Vector {
    Msg *data;
    int size;

    MsgIter begin() {
        MsgIter it;
        it.ptr = data;
        return it;
    }

    void erase(MsgIter pos, signed char flag) {
        FUN_0011619c(this, pos, flag);
    }
};

struct Obj;

extern "C" {
extern int DAT_003415d4;
extern void *DAT_002cfc8c;
extern void *DAT_0034179c;

float fabsf(float);
int FUN_00190724(int);
void FUN_0011619c(void *, MsgIter, signed char);
void FUN_001142b0(Obj *, u32, u32, u32, u32);
void FUN_0001ebd4();
char FUN_0001efc0();
void FUN_0001efac(Obj *);
void FUN_00103128(Obj *);
void FUN_001b1c44(void *, int);
void FUN_0001dfe8(void *, int);
void FUN_0001e23c(void *, int, int);
void FUN_00103480(Obj *);
}

struct Obj {
    void *vtable;           // 0x00
    Sub04Obj **f04;         // 0x04
    u8 pad08[76];           // 0x08
    Sub54 *f54;             // 0x54 (84)
    Sub58 *f58;             // 0x58 (88)
    Sub5c *f5c;             // 0x5c (92)
    u8 pad60[4];            // 0x60
    Sub64Inner **f64;       // 0x64 (100)
    int f68;                // 0x68 (104)
    u32 f6c;                // 0x6c (108)
    float f70;              // 0x70 (112)
    float f74;              // 0x74 (116)
    float f78;              // 0x78 (120)
    u8 f7c;                 // 0x7c (124)
    u8 f7d;                 // 0x7d (125)
    short f7e;              // 0x7e (126)
    u8 pad80[4];            // 0x80
    u8 f84;                 // 0x84 (132)
    u8 pad85[3];            // 0x85
    int f88;                // 0x88 (136)
    Vector f8c;             // 0x8c (140)
};

extern "C" void FUN_001150d8(Obj *self) {
    if ((*(u16 *)(**(int **)((char *)self->f54 + 16) + 14) & 3) != 0) {
        if (self->f8c.size == 0) {
            goto check_flags;
        }
        if (DAT_003415d4 == 0 || FUN_00190724(DAT_003415d4) != 0) {
            Msg msg = *self->f8c.data;
            MsgIter it = self->f8c.begin();
            MsgIter it_copy = it;
            signed char zero_val = 0;
            FUN_0011619c(&self->f8c, it_copy, zero_val);
            FUN_001142b0(self, msg.a, msg.b, msg.c, msg.d);
            return;
        }
    }

check_flags:
    if (self->f6c & 8) {
        if (self->f88 > 0) {
            self->f88++;
            if (self->f6c & 4) {
                FUN_0001ebd4();
            }
            if (self->f88 < 4) {
                return;
            }
            self->f88 = 0;
            return;
        }
    }

    self->f7c = 0;
    self->f7d = 0;
    self->f84 = 0;

    if ((*(u16 *)(**(int **)((char *)self->f54 + 16) + 14) & 3) != 0) {
        ((void (*)(Sub58 *))self->f58->vtable[4])(self->f58);
        return;
    }

    if (self->f6c & 4) {
        FUN_0001ebd4();
    }
    char s0 = FUN_0001efc0();
    if (s0 != 0) {
        FUN_0001efac(self);
    }
    FUN_00103128(self);

    if (self->f68 == 2 || self->f68 == 5) {
        if (((int)*(char *)((char *)DAT_002cfc8c + 139) << 28) >> 30 == 0) {
            self->f7c = 1;
            self->f88 = 1;
            FUN_001b1c44(DAT_0034179c, 3);
            FUN_0001dfe8(DAT_002cfc8c, 0x2000);
            FUN_0001e23c(DAT_002cfc8c, 0x2000, 1);
        }
    } else if (self->f68 == 6) {
        if (((int)*(char *)((char *)DAT_002cfc8c + 139) << 28) >> 30 == 0) {
            self->f84 = 1;
            self->f88 = 1;
            FUN_001b1c44(DAT_0034179c, 3);
            FUN_0001dfe8(DAT_002cfc8c, 0x2000);
            FUN_0001e23c(DAT_002cfc8c, 0x2000, 1);
        }
    } else if ((unsigned)(self->f68 - 3) < 2) {
        if (((int)*(char *)((char *)DAT_002cfc8c + 139) << 26) >> 30 == 0) {
            self->f7d = 1;
            self->f88 = 1;
            FUN_001b1c44(DAT_0034179c, 4);
            FUN_0001dfe8(DAT_002cfc8c, 0x4000);
            FUN_0001e23c(DAT_002cfc8c, 0x4000, 1);
        }
    }

    if (s0 != 0) {
        FUN_0001ebd4();
    }

    Sub10 *pi = *(Sub10 **)((char *)self->f54 + 16);
    int x_diff = (short)(pi->n2->x - pi->n0->x);
    int y_diff = (short)(pi->n6->y - pi->n0->y);

    int y_half = y_diff / 2;
    float f_y_half = (float)y_half;

    (*(Sub58Obj **)self->f58)->f64 = f_y_half - 44.0f;

    self->f5c->f60 = (float)((-self->f7e) / 2);
    self->f5c->f64 = f_y_half - 36.0f;

    (*self->f64)->f60 = ((float)x_diff * 0.9f) / -2.0f;
    (*self->f64)->f64 = f_y_half - 72.0f;

    float f3 = self->f70;
    float diff = 1.0f - f3;
    if (!(fabsf(diff) >= 0.025f)) {
        self->f70 = f3;
    } else {
        self->f70 = f3 + diff * 0.75f;
    }

    (*self->f04)->f30 = self->f70 * self->f74;
    (*self->f04)->f44 = self->f70 * self->f78;
    (*self->f04)->f78 = self->f70;

    FUN_00103480(self);
}

// Differing words: ~274/300 due to ~13 instruction shift.
// Frame size: 48 bytes (addiu sp, sp, -48, ra at 12, s1 at 8, s0 at 4) - exact match.
// Vector copy and erase idiom identified: self->f8c is Vector<Msg>, Msg is 16 bytes passed by value to FUN_001142b0.
// Delay slots, branches, and register usage matched for most blocks.
// Needs fine-tuning on reload of DAT_002cfc8c in switch cases and exact iterator temporary stack layout.
// near miss from lane dsn4 (unreviewed)
