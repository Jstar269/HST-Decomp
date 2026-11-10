/* HSTMATCH module=hstp_trans_main.prx start=0x001a1754 end=0x001a2288 symbol=FUN_001a1754 lang=c++ rodata=0x002d2d6c */
/* clg_database.cpp (from the Sharing build's asserts): fills the NPC/info panel from a record: copies the
   record into the global at 0x341c54, resolves the two NPC entries (0x1931b8) and the keyed entry, and
   draws them through 0x1c1c50. Names provisional. */
struct Tri { unsigned char a, b, c; };
struct Sh3 { short a, b, c; };
struct Rec {
    Rec();
    char name[16];          /* 0x00 */
    unsigned short x10;     /* 0x10 */
    char name2[16];         /* 0x12 */
    short x22;              /* 0x22 */
    short x24;              /* 0x24 */
    unsigned char x26, x27, x28, x29, x2a, x2b;
    char x2c, x2d;
    unsigned char x2e;
    char x2f;
    unsigned char x30, x31;
    char x32;
    unsigned char x33, x34;
    short x36, x38, x3a, x3c;
    char name3[16];         /* 0x3e */
    char name4[16];         /* 0x4e */
    Tri x5e;
    Sh3 x62;
    short x68, x6a;
};

struct Sub68 { char pad[0x10]; };
struct Entry {
    short id;               /* 0x00 */
    short x2, x4, x6, x8, xa;
    char pad0c[2];
    unsigned char xe;       /* 0x0e */
    char pad0f[0x20 - 0xf];
    unsigned short x20;     /* 0x20 */
    char pad22[0x68 - 0x22];
    Sub68 sub;              /* 0x68 */
    short x78;              /* 0x78 */
    char pad7a[2];
    unsigned char x7c;      /* 0x7c */
    Tri tri[3];             /* 0x7d */
    char pad86[0x9c - 0x86];
};
struct Table { unsigned n; Entry *data; };
struct Node { char pad[0x10]; int value; };
struct It { Node *node; };
struct Map { char pad[4]; Node header; };
struct Owner { char pad[0x30]; Map map; };
struct Base { char pad[0x12]; char x12[1]; };
struct Obj1038 { char pad[0x1038]; short x1038; };
struct Obj138 { char pad[0x138]; int x138; };
struct Info { unsigned short h0, h1, h2, h3; unsigned char x8, x9, xa; };
struct FObj { char pad[4]; float f4; };
struct Flags {
    unsigned char arr[0x1025 + 15];
    int Get(int i) { if (i >= 0 && i < 15) return arr[0x1025 + i]; return 1; }
};
struct Game { char pad[0x80]; Flags flags; };

#define HST_ASSERT(cond, text, line)     if (!(cond)) {         FUN_001a2288(ASSERT_FMT, text, ASSERT_FILE, line);         for (;;) {}     }
#define ASSERT_FMT DAT_00388c84
#define ASSERT_FILE DAT_00388cb8
#define ASSERT_STR1 DAT_00388cb0
#define ASSERT_STR2 DAT_00388ccc

extern "C" {
extern const char DAT_00388c84[];   /* "assertion \"%s\" failed: file \"%s\", line %d
" */
extern const char DAT_00388cb8[];   /* "clg_database.cpp" */
extern const char DAT_00388cb0[];   /* "pNpc1" */
extern const char DAT_00388ccc[];   /* "pNpc2" */
void FUN_001a2288(...);
extern Rec DAT_003eaf8c;
extern char DAT_003eb004[];
extern int DAT_003eb024;
extern void *DAT_003f3b1c;
extern int DAT_00388ac8, DAT_00388acc;
extern Table *DAT_003eaa70;
extern Owner *DAT_003eaa88;
extern Table *DAT_003eaa84;
extern Game *DAT_003ea8e8;
extern int DAT_003f3aac;
extern int DAT_003ba31c;

Base *FUN_0023bc78(void);
int FUN_00290228(void *);
int FUN_00290230(void *, int);
int FUN_001776b4(void);
void FUN_001cf7b0(void *, int, int, int, int, int, int, int);
Obj1038 *FUN_00177d4c(void);
Obj138 *FUN_0023ce60(void);
void FUN_0019f9c8(Entry **, Entry **, Rec *);
int FUN_002bf370(int);
void FUN_002bf498(int *, int, int);
void FUN_001a2d70(It *, Map *, const int *);
int FUN_0020d1e0(int, int, int);
int FUN_00177d88(int, Sub68 *);
void FUN_001cfe64(int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, Sub68 *, int,
                  int, int, int, int, int, int, int, Entry *);
int FUN_00177328(void);
void FUN_0017788c(int);
int FUN_001772dc(void);
int FUN_0020dda0(int, int);
int FUN_001773a0(int);
Info *FUN_0017759c(int, int);
int FUN_00177360(int);
FObj *FUN_001773e0(int);
int FUN_00177d20(void);
int FUN_0025df78(float, int);
int FUN_0020d294(int, int);
void FUN_001cfbf8(int, int, int, bool, int);
void FUN_001cff20(int, int, int, int, int);
}

inline int Clamp(int v, const int &lo, const int &hi)
{
    return v < lo ? lo : (hi < v ? hi : v);
}

static inline Entry *Lookup(int key)
{
    if (DAT_003eaa70) {
        It it;
        FUN_001a2d70(&it, &DAT_003eaa88->map, &key);
        if (it.node != &DAT_003eaa88->map.header)
            return &DAT_003eaa70->data[it.node->value];
    }
    return 0;
}

static inline Entry *Get(int i)
{
    if (DAT_003eaa84 && i >= 0 && i < DAT_003eaa84->n)
        return &DAT_003eaa84->data[i];
    return 0;
}

extern "C" void FUN_001a1754(Rec *p, int key)
{
    if (p == 0)
        return;
    int lvl = p->x24;
    int c29;
    int idx = p->x22;
    bool b27 = p->x27;
    switch (p->x29) {
    case 1:
    case 3:
    case 5:
        c29 = p->x29 & 0xff;
        break;
    default:
        c29 = 1;
        break;
    }
    int c2a;
    switch (p->x2a) {
    case 2:
    case 4:
    case 6:
        c2a = p->x2a & 0xff;
        break;
    default:
        c2a = 4;
        break;
    }
    bool found = false;
    if ((short)idx == 0) {
        Base *base = FUN_0023bc78();
        for (int i = 0; i < FUN_00290228(DAT_003f3b1c); i++) {
            int id = FUN_00290230(DAT_003f3b1c, i);
            if (id && base->x12[id] == 2) {
                idx = i;
                found = true;
                break;
            }
        }
        if (!found)
            idx = 1;
    }
    if (p->x24 == -1)
        lvl = FUN_001776b4();
    FUN_001cf7b0(DAT_003eb004, 0, 0, p->x2c, p->x2d, p->x2b, c29, c2a);
    Obj1038 *o = FUN_00177d4c();
    o->x1038 = Clamp((unsigned short)idx, DAT_00388ac8, DAT_00388acc);
    DAT_003eaf8c = *p;
    int mode;
    switch (FUN_0023ce60()->x138) {
    case 0:
        mode = 0;
        break;
    case 2:
        mode = 2;
        break;
    default:
        mode = 1;
        break;
    }
    Entry *pNpc1 = 0;
    Entry *pNpc2 = 0;
    FUN_0019f9c8(&pNpc1, &pNpc2, p);
    switch (p->x28) {
    case 0:
    case 4:
        DAT_003eb024 = 0;
        break;
    default:
        DAT_003eb024 = 1;
        break;
    }
    int h;
    int t = FUN_002bf370(0);
    h = 0;
    FUN_002bf498(&h, 0, t);
    switch (p->x28) {
    case 1:
    case 2:
    case 5:
        if (key) {
            Entry *e = Lookup(key);
            int id = e->id;
            if (id >= 0 && id < 15)
                FUN_001cfe64(2, FUN_0020d1e0(12, 0, id), id, e->x2, e->x4, e->x6, e->x8, e->xa, 0, 0, 0,
                             e->tri[mode].c, e->tri[mode].a, FUN_00177d88(id, &e->sub), e->x78, &e->sub, e->x7c,
                             0, 0, e->tri[mode].b, 1, 0, 0, 0, e);
        } else {
            int n = 0;
            int c0 = 0, c1 = 0, c2 = 0, c3 = 0;
            int v = 0;
            if (FUN_00177328() != 0x7ffe) {
                if (FUN_00177328() == 0x7fff) {
                    FUN_0017788c(0);
                    FUN_0017788c(FUN_0020dda0(FUN_00177328(), FUN_001772dc()));
                    n = FUN_00177328();
                    Info *in = FUN_0017759c(n, FUN_001773a0(n));
                    c0 = in->h0;
                    c1 = in->h1;
                    c2 = in->h2;
                    c3 = in->h3;
                    v = FUN_00177360(n);
                } else {
                    FUN_0017788c(FUN_0020dda0(FUN_00177328(), FUN_001772dc()));
                    n = FUN_00177328();
                    Info *in = FUN_0017759c(n, FUN_001773a0(n));
                    c0 = in->h0;
                    c1 = in->h1;
                    c2 = in->h2;
                    c3 = in->h3;
                    v = FUN_00177360(n);
                }
            }
            Entry *e = Get(n);
            Info *in = FUN_0017759c(n, FUN_001773a0(n));
            FObj *fo = FUN_001773e0(n);
            int fl = DAT_003ea8e8->flags.Get(n);
            FUN_001cfe64(2, FUN_0020d1e0(12, 0, n), n, c0, c1, c2, c3, v, in->x9 != 0, in->xa != 0, in->x8 != 0, h,
                         FUN_00177d20(), FUN_00177d88(n, &e->sub), n, &e->sub, fl, 0, 0,
                         FUN_0025df78(fo->f4, DAT_003f3aac), 1, 0, 0, 0, e);
        }
        break;
    }
    switch (p->x28) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        HST_ASSERT(pNpc1, ASSERT_STR1, 2129);
        FUN_001cfe64(1, FUN_0020d294(pNpc1->xe, pNpc1->x20), pNpc1->id, pNpc1->x2, pNpc1->x4, pNpc1->x6, pNpc1->x8, pNpc1->xa, 0, 0, 0,
                     pNpc1->tri[mode].c, pNpc1->tri[mode].a, FUN_00177d88(pNpc1->id, &pNpc1->sub), pNpc1->x78, &pNpc1->sub, pNpc1->x7c,
                     0, 0, pNpc1->tri[mode].b, 1, 0, 0, 0, *(Entry *volatile *)&pNpc1);
        break;
    }
    switch (p->x28) {
    case 1:
    case 3:
    case 5:
        HST_ASSERT(pNpc2, ASSERT_STR2, 2171);
        FUN_001cfe64(3, FUN_0020d294(pNpc2->xe, pNpc2->x20), pNpc2->id, pNpc2->x2, pNpc2->x4, pNpc2->x6, pNpc2->x8, pNpc2->xa, 0, 0, 0,
                     pNpc2->tri[mode].c, pNpc2->tri[mode].a, FUN_00177d88(pNpc2->id, &pNpc2->sub), pNpc2->x78, &pNpc2->sub, pNpc2->x7c,
                     0, 0, pNpc2->tri[mode].b, 1, 0, 0, 0, *(Entry *volatile *)&pNpc2);
        break;
    }
    FUN_001cfbf8(FUN_001772dc(), h, lvl, b27, 0);
    int g = DAT_003ba31c;
    FUN_001cff20(g, g + 1, g + 2, g + 3, g + 4);
}
// NEAR MISS (4 words): only the branch-likely choice differs at the default-index search.
// Retail: outer `if ((short)idx == 0)` branch is `bnez v0,join; nop` and the inner `if (!found) idx = 1;`
// branch is `bnezl s7,join+4; lh v1,36(s4)`; ours converts the OUTER branch to the likely form instead
// (one likely copy per target block). Ruled out (all still 4 words): `!(short)idx`, `(short)idx != 0 {} else`,
// int/char found, found declared outside the if, found = true/idx = i order, while/continue loop forms,
// `if (found) {} else`, `found == false` (worse), goto/label/do-while(0)/switch wrappers, empty else,
// the search as an inline (bool Find(int&), int Search(), void Search(int&), whole-if Pick/Pick2),
// && form `(short)idx == 0 && !Find(idx)`, x24 test as if/else, inline normalisation helpers.
// The `*(Entry *volatile *)&pNpc` last arguments reproduce retail's reload of the local after the nested
// calls (a plain pNpc1/pNpc2 is CSE'd into s0; tried local copy, reference, Ref wrapper, array, struct
// by value (lwc1 copy), split statements (moves mode*3 after the call), inline name helper, real asserts).
// lane pn3 (2026-11-10, ~30 compiles): 4 -> 2 words. `bool found = false;` declared BEFORE the outer
// `if ((short)idx == 0)` (the EBOOT twin 0x194f44's best form) makes the outer branch plain and the inner
// `bnezl s7,join+4; lh v1,36(s4)` match retail; the residual 2 words are `move s7,zero` hoisted into the outer
// branch's delay slot (retail: nop there, `move s7,zero` in the `jal FUN_0023bc78` slot). Rule seen: the outer
// branch takes the likely copy whenever its slot is not filled from above. Ruled out here: found=true before
// the if + found=false inside + `if (!found)` after the if (8: outer not threaded through the known true),
// `bool found = (short)idx != 0;` (3 / 671), else { found = true; } (2, same), redundant found=false inside
// (4 / 666), `for (..; !found && i < n; ..)` / while(!found) / `idx = found ? idx : 1` (600+), short /
// unsigned short idx (5/7), empty statements after the if (empty inline call, if(0), do{}while(0), dead local:
// all 4), while/for(;cond;){..break;} wrappers (671), found before c2a (9) or at function top (696).
// improved by lane pn3 (unreviewed): 4 -> 2 differing words
