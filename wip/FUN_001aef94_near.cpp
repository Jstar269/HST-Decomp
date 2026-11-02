/* HSTMATCH module=EBOOT.BIN start=0x001aef94 end=0x001af0d8 symbol=FUN_001aef94 lang=c++ */
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

struct Self { char pad0[4]; int f4; void *f8; void *fc; int *f10; };

extern "C" void FUN_001aef94(struct Self *self)
{
    FUN_001a9e34(self->f4);
    int *q = (int *)self->f8;
    FUN_0019d474();
    FUN_00103628(*q, 0);
    FUN_001abe98(q);
    FUN_00103628(*(int *)self->fc, 1);
    int *p = self->f10;
    FUN_00103628(*p, 0);
    int a = FUN_0019d474();
    int b = FUN_0019d474();
    int idx = FUN_001a49dc(b, *(int *)(a + 0x2c4)) - 1;
    int sel = Clamp(idx, DAT_002e2138, DAT_002e213c);
    int *node = (int *)p[8];
    int i = 0;
    if (0 < node[5]) {
        do {
            FUN_000ffb3c(((int *)node[4])[i], i != sel);
            i++;
        } while (i < node[5]);
    }
    *(int *)((int *)node[4])[sel] = 0x80808080;
}
// near miss: 79/81 words, 30 differ, first +0xa0. Reconstructed from the address-labelled disassembly:
// five-call walk, two FUN_0019d474 into FUN_001a49dc(b, a->0x2c4), inline Clamp(idx, DAT_002e2138,
// DAT_002e213c) (double global load), item loop FUN_000ffb3c(items[i], i!=sel), then
// *(int*)items[sel]=0x80808080. Residual is two related shapes: (1) ours strength-reduces i*4 into a
// callee-saved byte accumulator (s1) where retail keeps s1 = p+0x1c and recomputes sll v1,s3,2 each
// pass; (2) retail reloads p[8] at the final store through s1+4, ours reuses the loop local s0.
// Ruled out: #pragma opt_strength_reduction off (frame drops to -16, 81 differ, worse).
// near miss from lane dsn6 (unreviewed)
