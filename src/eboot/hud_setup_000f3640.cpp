/* HSTMATCH module=EBOOT.BIN start=0x000f3640 end=0x000f3948 symbol=FUN_000f3640 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; the per-player HUD fields are parallel two-entry arrays (retail keeps one induction pointer per stride); names and types await human review. */

struct Item {
    int color;
    short x;
    short y;
    short w;
    short h;
    char pad0c[0x24 - 0x0c];
    int c24;
    int c28;
};

struct Quad {
    Item *left;
    Item *right;
    Item *top;
    Item *bottom;
};

struct Mid {
    short a;
    short b;
};

struct Flag2 {
    unsigned char a;
    unsigned char b;
};

struct Pair {
    int a;
    int b;
};

struct Row48 {
    int a;
    int pad4[2];
    int b;
    int pad10[8];
};

struct Row12 {
    int a;
    int pad[2];
};

struct HudTable {
    char pad0[0x110];
    Row12 tabs[4];
    char pad140[0x248 - 0x140];
    Row48 rows[2];
    char pad2a8[0x2f0 - 0x2a8];
    Row12 colors[2][2];
};

struct Cfg {
    char pad[0x45];
    unsigned char f45;
};

struct Entry {
    char pad[0x104];
    unsigned char f104;
    char pad105[0x120 - 0x105];
};

struct Game {
    char pad[0x198];
    int sel;
};

struct Hud {
    virtual void v0();
    virtual void Refresh();
    char pad4[0x68 - 4];
    Item *frame[2];
    char pad70[0xc0 - 0x70];
    Item *tabs[4];
    char padd0[0xe0 - 0xd0];
    Item *iconA[2];
    Item *iconB[2];
    char padf0[0x120 - 0xf0];
    Quad quad[2];
    char pad140[0x150 - 0x140];
    Item *barA[2];
    Item *barB[2];
    Item *textA[2];
    Item *textB[2];
    char pad170[0x1b8 - 0x170];
    int f1b8;
    unsigned char f1bc;
    char pad1bd[0x1c4 - 0x1bd];
    int f1c4;
    int f1c8;
    Mid midA[2];
    Mid midB[2];
    Flag2 flags[2];
    Pair range[2];
    unsigned char f1f0[2];
    unsigned char f1f2[2];
    unsigned char f1f4[2];
    unsigned char f1f6[2];
    char pad1f8[0x204 - 0x1f8];
    unsigned char f204[2];
    char pad206[2];
    int f208[2];
    unsigned char f210;
    char pad211[3];
    int f214;
    unsigned char f218;
};

extern HudTable DAT_002d7cf4;
extern Cfg *DAT_00338b44;
extern Game *DAT_00338ac4;
extern unsigned char DAT_003386b8;
extern Entry DAT_003417b8[];

extern "C" {
void FUN_000f0644(int);
void FUN_000f5778(Hud *);
void FUN_000f3948(Hud *);
void FUN_000ffb3c(Item *, int);
void FUN_000ffc3c(Item *, int);
}

static inline void SetColor(Item *it, int c)
{
    it->c24 = c;
    it->c28 = c;
}

extern "C" void FUN_000f3640(Hud *h)
{
    FUN_000f0644(h->f1b8);
    FUN_000f5778(h);
    h->f1c4 = 0;
    h->f1c8 = 0;
    h->f210 = 0;
    h->f214 = -1;
    h->f218 = 1;
    h->f1bc = 0;
    for (int i = 0; i < 2; i++) {
        h->flags[i].a = 0;
        h->flags[i].b = 0;
        h->range[i].a = DAT_002d7cf4.rows[i].a;
        h->range[i].b = DAT_002d7cf4.rows[i].b;
        h->f204[i] = 0;
        h->f208[i] = DAT_002d7cf4.colors[1][i].a;
        h->f1f0[i] = 0;
        h->f1f2[i] = 0;
        h->f1f4[i] = 0;
        h->f1f6[i] = 0;
        SetColor(h->textA[i], DAT_002d7cf4.colors[0][i].a);
        SetColor(h->textB[i], DAT_002d7cf4.colors[1][i].a);
        FUN_000ffb3c(h->iconA[i], 1);
        if ((DAT_00338b44->f45 != 0) ^ 1)
            FUN_000ffb3c(h->iconB[i], 1);
        FUN_000ffc3c(h->barA[i], -256);
        if ((DAT_00338b44->f45 != 0) ^ 1)
            FUN_000ffc3c(h->barB[i], -256);
        FUN_000ffb3c(h->frame[i], 1);
        h->frame[i]->color = 0x808080;
        h->midA[i].a = (h->quad[i].left->x + h->quad[i].left->w) / 2;
        h->midB[i].a = (h->quad[i].top->y + h->quad[i].top->h) / 2;
        h->midA[i].b = (h->quad[i].right->x + h->quad[i].right->w) / 2;
        h->midB[i].b = (h->quad[i].bottom->y + h->quad[i].bottom->h) / 2;
    }
    for (int j = 0; j < (DAT_00338b44->f45 == 0 ? 4 : 2); j++) {
        int sel;
        unsigned char lit;
        if (DAT_003386b8 != 0 && DAT_00338ac4 != 0) {
            sel = DAT_00338ac4->sel;
            lit = DAT_003417b8[sel].f104 != 0;
        } else {
            sel = 0;
            lit = 0;
        }
        FUN_000ffb3c(h->tabs[j], j != sel);
        SetColor(h->tabs[j], DAT_002d7cf4.tabs[j].a + lit);
    }
    FUN_000f3948(h);
    h->Refresh();
}
