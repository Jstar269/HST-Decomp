/* HSTMATCH module=EBOOT.BIN start=0x00222134 end=0x002223fc symbol=FUN_00222134 lang=c++ */
/* Hand-written reconstruction from the disassembly; names and types await human review. */

typedef unsigned int uint;

struct Particle {
    int           pad[8];   /* 0x00 */
    unsigned char used;     /* 0x20 */
    int           f24;      /* 0x24 */
    int           f28;      /* 0x28 */
    float         f2c;      /* 0x2c */
    float         f30;      /* 0x30 */
    int           f34;      /* 0x34 */
    int           f38;      /* 0x38 */
    int           f3c;      /* 0x3c */
    float         f40;      /* 0x40 */
    float         f44;      /* 0x44 */
    int           f48;      /* 0x48 */
    int           f4c;      /* 0x4c */
};

extern uint  DAT_00310fc8;
extern float DAT_002f0e18;
extern float DAT_002f0e1c;
extern float DAT_002f0e20;

static inline int Rand(void)
{
    DAT_00310fc8 = DAT_00310fc8 * 0x343fd + 0x269ec3;
    return (DAT_00310fc8 >> 16) & 0x7fff;
}

static inline float RandUnit(void)
{
    return (float)(unsigned)Rand() * 3.0517578e-05f;
}

extern "C" void FUN_00222134(int *src, int count)
{
    int parity = Rand() % 2;
    float h;
    float s;
    float spread = DAT_002f0e20;
    float lo = DAT_002f0e18;
    float range = DAT_002f0e1c - lo;
    int i = 0;
    Particle *p = (Particle *)src;

    while (i < 16 && count > 0) {
        if (p->used == 0) {
            p->used = 1;
            h = 0.1f + (0.9f - 0.1f) * RandUnit();
            p->f24 = src[0];
            p->f28 = 0;
            p->f2c = 1.0f;
            p->f30 = 0.25f + 0.15f * RandUnit();
            p->f34 = 0;
            s = parity ? 0.9f : 0.1f;
            p->f38 = (int)((float)src[1] + s * (float)src[3]);
            p->f3c = (int)((float)src[2] + h * (float)src[4]);
            p->f40 = (!parity ? -1.0f : 1.0f) * (lo + range * RandUnit());
            p->f44 = spread * (h - 0.5f);
            p->f48 = 0;
            p->f4c = 0;
            parity = !parity;
            count--;
        }
        p = (Particle *)((char *)p + 0x30);
        i++;
    }
}
// RESIDUAL 2026-11-01: 78 differing words, 178/178 words (structure identical; all diffs are
// FP-register assignment). Retail: range=f6, lo=f7, spread=f8, h=f15, s=f14, 1.0f=f11, -1.0f=f10,
// 0.5f=f12, unsigned-conversion temp=f9. Ours: same f6/f7/f8, h reuses the f9 conversion temp,
// s=f15, 1.0f=f12, -1.0f=f14, 0.5f=f10 - so h never gets its own register and s takes the top f-reg.
// Also one delay-slot difference: retail stores p->f34=0 before the parity branch and puts
// `lw s0,12(a0)` in the slot; ours puts the f34 store in the slot (we are 1 word short).
// RULED OUT: h/s block-scope vs function-scope (93/102), const/initialised h (compile fail or worse),
// inline ternary without a named s (107), s assigned before h (139), global reads without locals (183),
// explicit hi local (107), named RandUnit temp (107), reordering p->f34 after the select (107/93).
// NEXT LEVER: force h into a dedicated FP live range (e.g. a helper that returns the value, or use of h
// before the second RandUnit), and re-check the delay slot once the f-regs shift.
// near miss from lane dsn8 (unreviewed)
