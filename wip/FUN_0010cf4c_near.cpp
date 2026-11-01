/* HSTMATCH module=EBOOT.BIN start=0x0010cf4c end=0x0010d340 symbol=FUN_0010cf4c lang=c++ */

struct Rec {
    int f0;
    int f4;
    short f8;
    short f10;
    short f12;
    short pad14;
    int f16;
};

struct Param2 {
    char pad0[8];
    int f8;             // 8
    int fC;             // 12 (0xc)
    short f10;          // 16 (0x10)
    short f12;          // 18 (0x12)
    short f14;          // 20 (0x14)
    char pad22[2];
    short f18;          // 24 (0x18)
    char pad26[2];
    int f1C;            // 28 (0x1c)
};

struct Node {
    virtual void _0();
    virtual void _1();
    virtual void _2();
    virtual void _3();
    virtual short func4(int arg);
};

struct Model {
    char pad0[4];
    Node **nodes;       // 4
    char pad8[88];      // 8..96
    float f60;          // 96 (0x60)
    float f64;          // 100 (0x64)
    char pad104[104];   // 104..208
    int fD0;            // 208 (0xd0)
};

extern "C" {
void FUN_00101590(Model *model, Rec *records, int count);
}

extern "C" void FUN_0010cf4c(Model *self, Param2 *p2, int count) {
    short f10 = p2->f10;
    short f12 = p2->f12;
    short f14 = p2->f14;
    float f60 = (float)f10;
    int f8 = p2->f8;
    int fC = p2->fC;
    int f1C = p2->f1C;
    short f18 = p2->f18;
    short diff = (short)(f14 - f10);
    float f64 = (float)f12;

    self->f60 = f60;
    self->f64 = f64;

    short s6 = 0;
    int s4 = 0;
    short step = self->nodes[f8]->func4(fC);

    diff = (short)(diff - step * (short)count);
    if (diff <= 0) {
        if (count > 1) {
            step += diff / (count - 1);
        }
    } else {
        short div = diff / (short)count;
        s6 += div / 2;
        step += div;
    }

    Rec r[128];
    for (int i = 0; i < count; i++) {
        r[s4].f0 = f8;
        r[s4].f4 = fC;
        r[s4].f8 = s6 + i * step;
        r[s4].f10 = 0;
        r[s4].f12 = f18;
        r[s4].f16 = f1C;
        s4++;
    }

    self->fD0 = fC;
    FUN_00101590(self, r, s4);
}

// NEAR: 251/253 words; 205 differ; first at +0x34; frame -2608 matches retail exactly.
// Frame layout: save block ra, s8..s0 (10 regs, 40 bytes) + Rec r[128] (2560 bytes) at sp+48.
// Differs: retail unrolls 8-way loop with dual-accumulator induction (move t5, zero; move t4, zero; ... addu t5, t5, t1; addu t4, t4, t1) spending 2 extra words.
// Register coloring: retail keeps self in s8 and diff in s7, freeing s7 to be used as scratch in the unrolled loop; our compile places self in s7 and diff in s8.
// near miss from lane dsn6 (unreviewed)
