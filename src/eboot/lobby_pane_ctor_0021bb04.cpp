/* HSTMATCH module=EBOOT.BIN start=0x0021bb04 end=0x0021be48 symbol=__ct__10C_0021bb04FUc lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. The selected item is read as *(Item **)(base + *ps * 4 + 0x70) (retail keeps &sel in a register and adds this + idx*4), and the tint is set through a by-value Vec4 inline. */

#define PAGE_COUNT 2
#define ITEM_COUNT 18

struct Vec4 {
    float x, y, z, w;
};

struct Pane {
    char pad[0x118];
    unsigned color;
    char pad11c[4];
    Vec4 tint;
    void SetTint(Vec4 v) { tint = v; }
};

struct Obj;

struct Item {
    unsigned color;
    short x;
    short y;
    short w;
    short h;
    char pad0c[0x20 - 0x0c];
    Pane *pane;
    int f24;
};

struct Lookup {
    int page;
    char *name;
    int out;
};

struct K_0021d7c4 {
    char pad[0x210];
};

struct B_00102e38 {
    B_00102e38();
    virtual void v0();
    char pad4[0x50];
};

struct C_0021bb04 : B_00102e38 {
    char pad54[0x5c - 0x54];
    int f5c;
    int f60;
    int f64;
    Obj *pages[PAGE_COUNT];
    Item *items[ITEM_COUNT];
    int fb8;
    K_0021d7c4 *list;
    Pane *sel;
    Pane *p1;
    Pane *p2;
    Pane *p3;
    int fd0;
    int fd4;
    int fd8;
    unsigned char fdc;
    unsigned char mode;
    char padde[2];
    int fe0;
    int fe4;
    C_0021bb04(unsigned char mode);
    virtual void v1();
};

void *operator new(unsigned int size);

extern float DAT_002eff88;
extern char DAT_002f0368[];
extern char DAT_002f0394[];
extern char *DAT_002f01b4[];
struct Table {
    Lookup e[ITEM_COUNT];
    int sel;
};

extern Table DAT_002f0288;

extern "C" {
void FUN_00104118(C_0021bb04 *m, char *path, int f);
void FUN_00103540(float t, C_0021bb04 *m);
Obj *FUN_001039d8(C_0021bb04 *m, char *name, int f);
Item *FUN_001026b8(Obj *page, char *name);
K_0021d7c4 *FUN_0021d7c4(void *mem);
void FUN_0021e2d4(K_0021d7c4 *k, int a);
void FUN_00022498(Vec4 *out, const Vec4 *in, float s);
void FUN_0021ca68(C_0021bb04 *m);
}

C_0021bb04::C_0021bb04(unsigned char m)
{
    fb8 = 0;
    list = 0;
    fd0 = 0;
    mode = m;
    if (mode != 0) {
        FUN_00104118(this, DAT_002f0368, 0);
        K_0021d7c4 *k = (K_0021d7c4 *)operator new(sizeof(K_0021d7c4));
        if (k != 0)
            k = FUN_0021d7c4(k);
        list = k;
        FUN_0021e2d4(k, 0);
    } else
        FUN_00104118(this, DAT_002f0394, 0);
    FUN_00103540(DAT_002eff88, this);
    int base = (int)this;
    {
        int i = 0;
        char **n = DAT_002f01b4;
        int off = 0;
        do {
            *(Obj **)(base + off + 0x68) = FUN_001039d8(this, *n, 0);
            i++;
            n++;
            off += 4;
        } while (i < PAGE_COUNT);
    }
    {
        int i = 0;
        Lookup *l = DAT_002f0288.e;
        int off = 0;
        do {
            if (mode != 0 || i < 3 || 14 < i) {
                Item *it = FUN_001026b8(this->pages[l->page], l->name);
                *(Item **)(base + off + 0x70) = it;
                l->out = it->f24;
            } else
                *(Item **)(base + off + 0x70) = 0;
            i++;
            l++;
            off += 4;
        } while (i < ITEM_COUNT);
    }
    int *ps = &DAT_002f0288.sel;
    Pane *p = (*(Item **)(base + *ps * 4 + 0x70))->pane;
    sel = p;
    unsigned c = (*(Item **)(base + *ps * 4 + 0x70))->color;
    p->color = c;
    Vec4 in;
    Vec4 out;
    in.x = c & 0xff;
    in.y = (c >> 8) & 0xff;
    in.z = (c >> 16) & 0xff;
    in.w = (c >> 24) & 0xff;
    FUN_00022498(&out, &in, 0.008f);
    p->SetTint(out);
    if (mode != 0) {
        p1 = items[5]->pane;
        p2 = items[6]->pane;
        p3 = items[7]->pane;
    } else {
        p1 = 0;
        p2 = 0;
        p3 = 0;
    }
    if (mode == 0) {
        Item *r = items[2];
        r->w = r->x + 64;
        r->h = r->y + 64;
    }
    f5c = 0;
    f60 = 0;
    f64 = -1;
    fd4 = 0;
    fd8 = 0;
    fdc = 0;
    fe0 = 0;
    fe4 = 0;
    FUN_0021ca68(this);
}
