/* HSTMATCH module=EBOOT.BIN start=0x001dcdbc end=0x001dd0a4 symbol=FUN_001dcdbc lang=c99 */
/* Ghidra pseudocode (advisory) corrected by hand: the message test is a switch (compare chain
   with out-of-line bodies) and the 0x14 arm walks a record based at +0x140; names and types await
   human review. */

typedef struct Vec4 {
    float x, y, z, w;
} Vec4;

typedef struct Mat4 {
    float m[16];
} Mat4;

typedef struct Vec2i {
    float x, y;
    int   z, w;
} Vec2i;

typedef struct G2 {
    char          pad[0x521];
    unsigned char f521;
} G2;

typedef struct G1 {
    char pad[0x88];
    char *f88;
    char pad2[0xa0 - 0x8c];
    G2  *fa0;
} G1;

typedef struct G3 {
    char          pad[0x5d];
    unsigned char f5d;
} G3;

typedef struct Obj {
    char          pad0[0x40];
    int           f64;      /* 0x40 */
    char          pad1[0x7c];
    int           state;    /* 0xc0 */
    unsigned char f196;     /* 0xc4 */
    char          pad2[3];
    int           f200;     /* 0xc8 */
    char          pad3[1];
    unsigned char f205;     /* 0xcd */
} Obj;

extern G1 *DAT_003386b0;
extern G3 *DAT_00338b44;

extern void FUN_0004c644(void *, void *);
extern void FUN_0004c5cc(void *, void *);
extern int  FUN_000df000(G3 *);

int FUN_001dcdbc(Obj *o, int msg)
{
    switch (msg) {
    case 0x11:
        o->f200 = 0;
        if (o->state != 1) {
            o->state = 1;
            o->f196 = 1;
        }
        break;
    case 0x10:
        o->f200 = 0;
        if (o->state != 2) {
            o->state = 2;
            o->f196 = 1;
        }
        break;
    case 0x12:
        if (DAT_003386b0->fa0->f521 == 0) {
            if (o->state != 2) {
                o->state = 2;
                o->f196 = 1;
            }
        } else if (o->state != 7) {
            o->state = 7;
            o->f196 = 1;
        }
        break;
    case 0x15:
        o->f205 = 1;
        break;
    case 0x0c:
        if (o->state != 6 && FUN_000df000(DAT_00338b44) == 0 &&
            DAT_00338b44->f5d == 0) {
            o->f200 = 0;
            if (o->state != 4) {
                o->state = 4;
                o->f196 = 1;
            }
        }
        break;
    case 0x14: {
        char *r = DAT_003386b0->f88 + 0x140;
        Vec4  a;
        Vec4  b;
        Vec4  c;
        Vec2i d;
        int   n;
        Mat4  m;

        FUN_0004c644(&a, r);
        FUN_0004c644(&b, r + 16);
        FUN_0004c644(&c, r + 32);
        d = *(Vec2i *)(r + 48);
        n = *(int *)(r + 64);
        FUN_0004c5cc(&m, r + 80);
        if (n == o->f64) {
            o->f200 = 0;
            if (o->state != 6) {
                o->state = 6;
                o->f196 = 1;
            }
        } else if (o->state != 6 && FUN_000df000(DAT_00338b44) == 0 &&
                   DAT_00338b44->f5d == 0) {
            o->f200 = 0;
            if (o->state != 4) {
                o->state = 4;
                o->f196 = 1;
            }
        }
        break;
    }
    case 0x0b:
    case 0x0a:
    case 0x09:
        break;
    case 0x08:
        o->f200 = 0;
        if (o->state != 1) {
            o->state = 1;
            o->f196 = 1;
        }
        break;
    case 0x0e:
    case 0x16:
        if (o->state != 1) {
            o->state = 1;
            o->f196 = 1;
        }
        break;
    case 0x01:
    case 0x00:
        break;
    }
    return 0;
}
// NEAR 170 words (retail 186). Differs from the first word on:
//   prologue frame 128 vs 160 (retail homes the 0x14 arm's locals m,d; ours folds them away),
//   and the switch is lowered as a different compare ORDER: retail's chain is 0,1,22,14,16,...
//   (a MWCC binary-decision tree over the case values), ours is 17,16,18,21,... (parse order).
// Ruled out: if/else chain in source order (same order), folded base pointer (same body win),
//   signed vs unsigned switch expression.
// NEXT LEVER: give the switch the exact case set with distinct bodies (no shared 18/19/20 or
//   8/9/10 fallthrough) so MWCC's BSEL tree has the same shape, and keep m/d alive by using
//   their addresses (e.g. pass &m to the callee).
// CORRECTION 2026-11-01: hstdiff prints "ours | retail". The compare chain 0,1,22,14,... is OURS;
// retail's is 17,16,18,21,... Re-read the first diff with the columns the right way round before
// trying another switch shape.
// near miss from lane dsn8 (unreviewed)
