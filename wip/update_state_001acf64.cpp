/* HSTMATCH module=EBOOT.BIN start=0x001acf64 end=0x001ad350 symbol=FUN_001acf64 lang=c++ */
/* Hand-written reconstruction from the disassembly; near match (24/251 words differ, 227 words identical); names and types await human review. */
#pragma opt_strength_reduction off

static inline int Clamp(int v, const int &lo, const int &hi) {
    return v < lo ? lo : (hi < v ? hi : v);
}

struct State {
    char pad0[0x2c4];
    int f2c4;
    int f2c8;
    int pad1;
    int f2d0;
    char pad2[0x37c - 0x2d4];
    int f37c;
};

struct TableEntry {
    char pad0[10];
    short f0a;
    short f0c;
    short f0e;
};

struct Vec90c {
    char pad0[16];
    void **data;
    int size;
};

struct SubWidget {
    char pad0[0x24];
    int f24;
    int f28;
};

struct Holder {
    void *w;
};

struct Widget {
    virtual void s0();
    virtual void s1(int);
};

struct Self_001acf64 {
    char pad[0x90c];
    Vec90c *vec90c;
    int pad_910;
    Widget *w914;
};

extern "C" {
State *FUN_0019d474();
int FUN_001a49dc(State *, int);
TableEntry *FUN_0027edbc(void *, int);
void FUN_000ffb3c(void *, int);
void FUN_0010b980(void *, int, int);

extern int DAT_002e2100;
extern int DAT_002e2104;
extern int DAT_002e2108;
extern int DAT_002e210c;
extern void *DAT_0034a7ec;

void FUN_001acf64(Self_001acf64 *self, bool flag)
{
    int a = FUN_0019d474()->f2c4;
    int s8 = FUN_0019d474()->f2c8;
    int c = FUN_0019d474()->f2d0;
    FUN_0019d474();
    int idx = FUN_001a49dc(FUN_0019d474(), a);
    int clamped = Clamp(idx - 1, DAT_002e2100, DAT_002e2104);

    TableEntry *entry = FUN_0027edbc(DAT_0034a7ec, idx);
    int f0c = entry->f0c;
    int f0e = entry->f0e;
    int f0a = entry->f0a;

    Vec90c *vec = self->vec90c;
    for (int i = 0; i < vec->size; i++) {
        FUN_000ffb3c(vec->data[i], i != clamped);
    }

    int i2, off2;
    for (i2 = 0, off2 = 0; i2 < 5; i2++, off2 += 20) {
        int *h = (int *)((char *)self + (off2 + 520)) + 9;
        if (i2 < f0c) {
            FUN_000ffb3c((void *)*h, 0);
        } else {
            FUN_000ffb3c((void *)*h, 1);
        }
    }

    int i3, off3;
    for (i3 = 0, off3 = 0; i3 < 5; i3++, off3 += 20) {
        int *h = (int *)((char *)self + (off3 + 620)) + 9;
        if (i3 < f0e) {
            FUN_000ffb3c((void *)*h, 0);
        } else {
            FUN_000ffb3c((void *)*h, 1);
        }
    }

    FUN_000ffb3c(*(void **)((char *)self + 776), 0);
    SubWidget *sub776 = *(SubWidget **)((char *)self + 776);
    sub776->f24 = sub776->f28 + f0a;

    int i4, off4;
    for (i4 = 0, off4 = 0; i4 < 6; i4++, off4 += 20) {
        int cur = s8 + i4;
        int *h_s1 = (int *)((char *)self + (off4 + 760)) + 9;
        int *h_s0 = (int *)((char *)self + (off4 + 880)) + 9;
        int *h_s2 = (int *)((char *)self + (off4 + 1000)) + 9;
        if (cur < FUN_0019d474()->f37c) {
            int idx2 = FUN_001a49dc(FUN_0019d474(), cur);
            int clamped2 = Clamp(idx2 - 1, DAT_002e2108, DAT_002e210c);
            FUN_000ffb3c((void *)*h_s2, 0);
            SubWidget *sw = *(SubWidget **)h_s2;
            sw->f24 = sw->f28 + clamped2;
            if (cur == c) {
                FUN_000ffb3c((void *)*h_s1, 0);
                FUN_000ffb3c((void *)*h_s0, 1);
            } else {
                FUN_000ffb3c((void *)*h_s1, 1);
                FUN_000ffb3c((void *)*h_s0, 0);
            }
        } else {
            FUN_000ffb3c((void *)*h_s2, 1);
            FUN_000ffb3c((void *)*h_s1, 1);
            FUN_000ffb3c((void *)*h_s0, 1);
        }
    }

    if (flag) {
        self->w914->s1(0);
        FUN_0010b980(self->w914, a - s8, 0);
    }
    Holder *h_v1 = (Holder *)((char *)self + 1156);
    Holder *h_end_s0 = (Holder *)((char *)self + 1176);
    if (flag) {
        if (s8 > 0) {
            FUN_000ffb3c(h_v1->w, 0);
        } else {
            FUN_000ffb3c(h_v1->w, 1);
        }
        bool ok = FUN_0019d474()->f37c > s8 + 6;
        if (!ok) {
            FUN_000ffb3c(h_end_s0->w, 1);
        } else {
            FUN_000ffb3c(h_end_s0->w, 0);
        }
    } else {
        FUN_000ffb3c(h_v1->w, 1);
        FUN_000ffb3c(h_end_s0->w, 1);
    }
}
}
// Near miss: 24/251 words differ (227 words match exactly). First difference at +0x1d8 (0x001ad13c).
// Differs: loop 4 register allocation assigns (h_s2, h_s1, h_s0, cur, clamped2) to (s1, s0, s6, s2, s7)
// instead of retail's (s2, s1, s0, s7, s6).
// Ruled out:
// 1. Taking pointer addresses of variables forces local frame to 80 bytes; natural types keep retail's 64-byte frame.
// 2. Declaring variables at the function level disrupts i4/off4 register allocation from s4/s3 to s3/s2.
// 3. Reusing variables explicitly between loops 2/3 and 4 caused type cast overheads.
// 4. Post-loop flag condition requires `bool ok = FUN_0019d474()->f37c > s8 + 6; if (!ok)` to match retail's `slt v0, v1, v0; bnezl v0, 0x1ad2f8` sequence with fallthrough 1 and branch 0.
// near miss re-tested by lane w3f3: declaration order h_s1,h_s0,h_s2,cur inside the
// last loop lowers the residual from 24 to 21 words; residual is the loop-4 saved-register
// colouring only (retail h_s2=s2,h_s1=s1,h_s0=s0,cur=s7,clamped2=s6; ours h_s1=s1,h_s0=s0,
// h_s2=s6,cur=s2,clamped2=s7). Ruled out: all 24 orderings of the three pointers+cur
// (best 21), moving cur/clamped2 to function scope (33), declaring cur before the pointers
// (23-24). Root cause is the allocator keeping s2 for cur instead of h_s2.
// improved by lane w3f3 (unreviewed): 24 -> 21 differing words
