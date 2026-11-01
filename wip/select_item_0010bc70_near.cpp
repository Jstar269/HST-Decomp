/* HSTMATCH module=EBOOT.BIN start=0x0010bc70 end=0x0010c17c symbol=FUN_0010bc70 lang=c++ */
#pragma exceptions off
#pragma opt_strength_reduction off

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

struct Rec {
    short s0;
    short s2;
    u8 pad[4];
    int key;
};

struct QuadItem {
    u8 pad[14];
    u16 flags;
};

struct GlobalObj {
    u8 pad0[84];
    struct SubGlobal *sub;
};

struct SubGlobal {
    u8 pad[16];
    QuadItem **items;
};

extern "C" GlobalObj *DAT_003415ac;
extern "C" void *DAT_002cfc8c;

extern "C" int FUN_0001ea64(void *, int *, int, int, int);
extern "C" void FUN_0001e23c(void *, int, int);
extern "C" void FUN_0001dfe8(void *, int);

class Base {
public:
    virtual void v0();
    virtual void v1();
    virtual void vfunc_10(int arg);
};

class Obj : public Base {
public:
    u8 pad4[12];            // 4
    QuadItem **quads_10;    // 16 (0x10)
    u8 pad14[188];          // 20 -> 208 (0xd0)
    int f0d0;               // 208 (0xd0)
    int f0d4;               // 212 (0xd4)
    int f0d8;               // 216 (0xd8)
    int f0dc;               // 220 (0xdc)
    int f0e0;               // 224 (0xe0)
    int f0e4;               // 228 (0xe4)
    u8 f0e8;                // 232 (0xe8)
    u8 f0e9;                // 233 (0xe9)
    u8 f0ea;                // 234 (0xea)
    u8 pad235;              // 235
    short f0ec;             // 236 (0xec)
    short f0ee;             // 238 (0xee)
    int f0f0[2];            // 240 (0xf0)
    int f0f8[2];            // 248 (0xf8)
    u8 pad256[20];          // 256
    u8 f114;                // 276 (0x114)
    u8 pad277[3];           // 277
    int f118;               // 280 (0x118)
    short f11c;             // 284 (0x11c)
    short f11e;             // 286 (0x11e)
    Rec *f120;              // 288 (0x120)
    u8 pad292[8];           // 292
    u8 f12c;                // 300 (0x12c)
    u8 f12d;                // 301 (0x12d)
};

extern "C" void FUN_0010b3cc(Obj *);

extern "C" void FUN_0010bc70(Obj *self) {
    if (self->f12d != 0) {
        if ((DAT_003415ac->sub->items[0]->flags & 3) != 0) {
            self->vfunc_10(0);
        } else {
            self->vfunc_10(1);
            return;
        }
    }
    self->f0ea = 0;
    self->f0e9 = 0;

    QuadItem *q;
    int d4 = self->f0d4;
    switch (self->f118) {
    case 1:
        q = self->quads_10[d4];
        break;
    case 2:
        q = self->quads_10[d4 + 2];
        break;
    case 0:
        q = self->quads_10[d4 + 1];
        break;
    default:
        q = self->quads_10[d4 + 1];
        break;
    }

    if ((q->flags & 3) != 0) {
        self->f114 = 1;
        return;
    }

    int s1 = self->f0e0;
    int s0 = (self->f0e4 - s1) + 1;
    if (self->f0d8 != 0) {
        volatile int mask[4];
        if (self->f0d8 == 1) {
            mask[2] = 128;
            mask[3] = 0x80000;
            mask[0] = 32;
            mask[1] = 0x20000;
        } else {
            mask[2] = 16;
            mask[3] = 0x10000;
            mask[0] = 64;
            mask[1] = 0x40000;
        }

        if (FUN_0001ea64(DAT_002cfc8c, self->f0f0, mask[2], 2, 9) != 0 && !(s0 < 2)) {
            if (self->f12c != 0) {
                self->f0dc = (self->f0dc - 1 + s0) % s0;
                self->f0ea = 1;
            } else {
                if (self->f0dc > 0) {
                    self->f0dc--;
                    self->f0ea = 1;
                }
            }
            if (self->f0dc == 0) {
                FUN_0001e23c(DAT_002cfc8c, mask[2], 1);
                FUN_0001e23c(DAT_002cfc8c, mask[3], 1);
            }
            if (self->f0ea != 0) {
                self->f0d0 = (self->f0d8 == 1) ? 3 : 0;
            }
        } else if (FUN_0001ea64(DAT_002cfc8c, self->f0f8, mask[0], 2, 9) != 0 && !(s0 < 2)) {
            if (self->f12c != 0) {
                self->f0dc = (self->f0dc + 1) % s0;
                self->f0ea = 1;
            } else {
                if (self->f0dc < s0 - 1) {
                    self->f0dc++;
                    self->f0ea = 1;
                }
            }
            if (self->f0dc == s0 - 1) {
                FUN_0001e23c(DAT_002cfc8c, mask[0], 1);
                FUN_0001e23c(DAT_002cfc8c, mask[1], 1);
            }
            if (self->f0ea != 0) {
                self->f0d0 = (self->f0d8 == 1) ? 3 : 0;
            }
        } else {
            if (!((((int)*(signed char *)((char *)DAT_002cfc8c + 0x8b) << 28) >> 30) > 0)) {
                if (self->f120[self->f0dc + s1].pad[4] == 0) {
                    self->f0d0 = (self->f0d8 == 1) ? 3 : 0;
                    self->f0e8 = 1;
                    FUN_0001dfe8(DAT_002cfc8c, 0x2000);
                    FUN_0001e23c(DAT_002cfc8c, 0x2000, 1);
                } else {
                    self->f0e9 = 1;
                }
            }
        }
    }

    if (self->f120[self->f0dc + s1].key != 3) {
        self->f118 = self->f120[self->f0dc + s1].key;
    }
    if (self->f114 != 0) {
        self->f114 = 0;
        self->f0ec = self->f11c + self->f120[self->f0dc + s1].s0;
        self->f0ee = self->f11e + self->f120[self->f0dc + s1].s2;
    } else {
        self->f0ec = (self->f0ec / 2) + ((self->f11c + self->f120[self->f0dc + s1].s0) / 2);
        self->f0ee = (self->f0ee / 2) + ((self->f11e + self->f120[self->f0dc + s1].s2) / 2);
    }
    FUN_0010b3cc(self);
    self->f0d0++;
}

// Differing words: 324/323 words (1 word extra due to switch codegen).
// All logic, frame size (32 bytes), loops, divisions, mask stack slots 16(sp)..28(sp), calls,
// and arithmetic after the switch match instruction-for-instruction.
// Ruled out:
// - if/else chain for the switch: emits bnezl/bnel instead of beqz/beql.
// - index variable across switch: causes register allocation spill / order shift.
// - inlined helper method: didn't collapse the CSE displacement +8/+4.
// near miss from lane dsn4 (unreviewed)
