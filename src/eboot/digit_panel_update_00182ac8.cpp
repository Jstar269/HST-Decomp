/* HSTMATCH module=EBOOT.BIN start=0x00182ac8 end=0x00182ea4 symbol=FUN_00182ac8 lang=c++ */
/* Hand-written reconstruction from the disassembly (starting from a near-miss draft of lane ol3), restructured by hand; exact match verified; matching idiom: the first reset loop keeps byte-offset induction variables (`(char *)self + (off20 + 0xd98)`) so the group bases are not folded into displacements; names and types await human review. */
struct Widget {
    virtual void m0();
    virtual void SetVisible(int);
};

struct Num {
    char pad[0x24];
    int value;
    int base;
    void Set(int v)
    {
        value = v;
        base = v;
    }
};

struct Entry {
    char pad[8];
    unsigned char f8;
    unsigned char f9;
    unsigned char fa;
    unsigned char fb;
    unsigned char fc;
    bool IsFree() const
    {
        bool r;
        if (f8 != 0 || fb != 0 || fc != 0)
            r = false;
        else
            r = true;
        return r;
    }
};

struct Row {
    Entry **items;
    int a, b;
};

struct List {
    int f0;
    int sel;
    int cur;
    char padc[0x18 - 0xc];
    Row *rows;
};

struct Game {
    char pad0[0x8c];
    int f8c;
    char pad90[1000 - 0x90];
    void (*Select)(int);
    unsigned short *(*GetRect)(int, int);
    char pad3f0[0x538 - 0x3f0];
    List *list;
    char pad53c[0x548 - 0x53c];
    int (*GetA)(int);
    char pad54c[4];
    int (*GetB)(int);
};

struct Item {
    int a, b;
    Num *n;
    int base;
    int c;
};

struct Group {
    Item it[5];
    void Reset(int i)
    {
        Item &x = it[i + 3];
        x.n->base = x.n->value = x.base + 6;
    }
};

struct Cell {
    int v, u;
};

struct WRow {
    Widget *w;
    int a, b;
};

struct Self {
    char pad0[0x34];
    Widget *w34;
    Widget *w38;
    char pad3c[0xd98 - 0x3c];
    Group grp[8];
    char pad10b8[0x1154 - 0x10b8];
    WRow rows[4];
    char pad1184[0x118c - 0x1184];
    Cell cells[4][5];
};

extern "C" {
Game *FUN_0016bc70();
void FUN_000ffb3c(Num *, int);
void FUN_001826b8(Self *, int, unsigned short, unsigned short, unsigned short, unsigned short, int);
}

#define LOOKUP(fn) \
    Game *g = FUN_0016bc70(); \
    int r; \
    if (id != 0x7fff && id != 0x7ffe) \
        r = g->fn(id); \
    else \
        r = 0; \
    return r;

static inline int GetA(int id)
{
    LOOKUP(GetA)
}

static inline int GetB(int id)
{
    LOOKUP(GetB)
}

extern "C" void FUN_00182ac8(Self *self, int id)
{
    List *l = FUN_0016bc70()->list;
    Entry *e = l->rows[l->cur].items[l->sel];
    if (id == 0x7ffe || id == 0x7fff) {
        self->w34->SetVisible(1);
        self->w38->SetVisible(1);
    } else if (!e->IsFree()) {
        self->w34->SetVisible(0);
        self->w38->SetVisible(0);
        int i = 0;
        int off20 = 0;
        char *cp = (char *)self;
        for (; i < 5; i++, off20 += 20, cp += 8) {
            ((Cell *)(cp + 4492))->v = 0;
            ((Cell *)(cp + 4532))->v = 0;
            ((Cell *)(cp + 4572))->v = 0;
            ((Cell *)(cp + 4612))->v = 0;
            Item *x = (Item *)((char *)self + (off20 + 0xd98));
            Item *y = (Item *)((char *)self + (off20 + 0xdfc));
            int v = x[3].base + 6;
            Num *n = x[3].n;
            n->value = v;
            n->base = v;
            int w = y[3].base + 6;
            Num *m = y[3].n;
            m->value = w;
            m->base = w;
        }
        for (int i = 0; i < 4; i++) {
            self->rows[i].w->SetVisible(1);
            FUN_000ffb3c(self->grp[7].it[i].n, 1);
        }
    } else {
        FUN_0016bc70()->Select(id);
        int a = GetA(id);
        int b = GetB(id);
        unsigned short *r = FUN_0016bc70()->GetRect(id, b);
        self->w34->SetVisible(0);
        self->w38->SetVisible(0);
        FUN_001826b8(self, id, r[0], r[1], r[2], r[3], a);
    }
    Num **d0 = &self->grp[0].it[1].n;
    Num **d1 = &self->grp[0].it[2].n;
    int n = FUN_0016bc70()->f8c + 1;
    int hi = n / 10;
    int lo = n % 10;
    if (e->f9) {
        FUN_000ffb3c(*d0, 1);
        FUN_000ffb3c(*d1, 1);
    } else if (!e->IsFree()) {
        FUN_000ffb3c(*d0, 0);
        (*d0)->value = (*d0)->base + 10;
        FUN_000ffb3c(*d1, 0);
        (*d1)->value = (*d1)->base + 11;
    } else if (hi != 0) {
        FUN_000ffb3c(*d0, 0);
        (*d0)->value = (*d0)->base + hi;
        FUN_000ffb3c(*d1, 0);
        (*d1)->value = (*d1)->base + lo;
    } else {
        FUN_000ffb3c(*d0, 0);
        (*d0)->value = (*d0)->base + lo;
        FUN_000ffb3c(*d1, 1);
    }
}

