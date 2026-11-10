/* HSTMATCH module=EBOOT.BIN start=0x002a45d8 end=0x002a4938 symbol=FUN_002a45d8 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. The two mode tables are declared const so their loads schedule ahead of the o->prev store. */

struct Slot {
    unsigned char state;
    char pad[0x37];
};

struct Party {
    char pad[0x2a4];
    struct Slot slot[3];
};

struct Game {
    char pad0[0x80];
    struct Party party;
    char pad[0x86d24 - 0x80 - sizeof(struct Party)];
    unsigned char seen0;
    unsigned char seen1;
    unsigned char seen2;
};

struct Ref {
    int v;
};

struct Obj {
    char pad0[0x1b0];
    int kind;
    char pad1b4[4];
    int mode;
    int sub;
    int prev;
    char pad1c4[0x1d0 - 0x1c4];
    struct Ref *ref;
};

extern struct Game *DAT_003415a8;
extern const int DAT_002cc0a4[];
extern const int DAT_002cc0d0;
extern unsigned short DAT_0034a0e2;

extern int FUN_0022c194(void);
extern int FUN_0022c928(int);
extern void FUN_0022c8d8(int, int);
extern void FUN_0023165c(int);
extern int FUN_0022c988(int);
extern void FUN_002a1a64(struct Obj *, int);
extern void FUN_002a2488(struct Obj *, int);
extern int FUN_001c062c(struct Party *);

static inline int Busy(struct Party *p, int i)
{
    return (unsigned)(p->slot[i].state - 4) < 2;
}

static inline int Once(unsigned char *f)
{
    if (*f != 0)
        return 0;
    *f = 1;
    return 1;
}

static inline int Ready(struct Obj *o)
{
    return o->kind == 5 && o->ref->v == 2;
}

static inline void SetMode(struct Obj *o, int m, int s)
{
    o->prev = o->mode;
    o->mode = m;
    o->sub = s;
}

static inline void Mark0(void)
{
    struct Game *g = DAT_003415a8;
    if (Busy(&g->party, 0)) {
        unsigned char *f = &g->seen0;
        if (*f == 0)
            *f = 1;
    }
}

void FUN_002a45d8(struct Obj *o)
{
    int r = FUN_0022c194();
    if (r < 0)
        return;
    if (FUN_0022c928(r) != 0) {
        if (r != 7)
            FUN_0022c8d8(r, 0);
        FUN_0023165c(r);
        if (FUN_0022c988(8) != 0) {
            FUN_002a1a64(o, 32);
            SetMode(o, DAT_002cc0a4[r], DAT_002cc0d0);
        } else {
            FUN_002a1a64(o, 33);
            SetMode(o, DAT_002cc0a4[r], 0);
        }
        if (r == 9) {
            struct Game *g = DAT_003415a8;
            if (Busy(&g->party, 0)) {
                unsigned char *f = &g->seen0;
                if (*f == 0) {
                    *f = 1;
                    o->mode = 53;
                }
            }
        }
    } else {
        struct Game *g = DAT_003415a8;
        struct Party *p = &g->party;
        unsigned char *f;
        if (Busy(p, 2) && *(f = &g->seen1) == 0) {
            {
                *f = 1;
                if (o->kind != 5) {
                    SetMode(o, 55, 0);
                    FUN_002a2488(o, 1);
                }
                Mark0();
            }
        }
        else if (Busy(p, 1) && *(f = &g->seen2) == 0) {
            {
                *f = 1;
                {
                    int same = DAT_0034a0e2 == FUN_001c062c(&DAT_003415a8->party);
                    if (o->kind != 5) {
                        SetMode(o, same ? 51 : 52, 0);
                        FUN_002a2488(o, 1);
                    }
                }
                Mark0();
            }
        }
        else if (Busy(p, 0) && *(f = &g->seen0) == 0) {
            int ok = 1;
            if (2 < (unsigned)(o->kind - 1)) {
                if (!Ready(o))
                    ok = 0;
            }
            if (ok && o->mode == 53)
                return;
            *f = 1;
            if (o->kind != 5) {
                SetMode(o, 54, 0);
                FUN_002a2488(o, 1);
            }
        }
    }
}
