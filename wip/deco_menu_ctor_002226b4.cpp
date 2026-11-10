/* HSTMATCH module=EBOOT.BIN start=0x002226b4 end=0x00222a00 symbol=__ct__10C_002226b4FUc lang=c++ */

#define PART_COUNT 16
#define PAGE_COUNT 2
#define ITEM_COUNT 11

struct B_00050fc0 {
    char pad[0x48];
};

struct Part {
    unsigned char on;
    char pad[0x2f];
};

struct Kira {
    B_00050fc0 *spr;
    char pad4[0x1c];
    Part parts[PART_COUNT];
    Kira(char *name);
};

struct Obj;

struct Item {
    int color;
    short x;
    short y;
    short w;
    short h;
    char pad0c[0x20 - 0x0c];
    int f20;
    int f24;
};

struct Lookup {
    int page;
    char *name;
    int out;
};

struct B_00102e38 {
    B_00102e38();
    virtual void v0();
    char pad4[0x50];
};

struct C_002226b4 : B_00102e38 {
    int f54;
    char pad58[8];
    int f60;
    int f64;
    Obj *pages[PAGE_COUNT];
    Item *items[ITEM_COUNT];
    int f9c;
    int fa0;
    int fa4;
    int fa8;
    int fac;
    unsigned char fb0;
    unsigned char mode;
    unsigned char fb2;
    int fb4;
    int fb8;
    float fbc;
    float fc0;
    float fc4;
    float fc8;
    C_002226b4(unsigned char mode);
    virtual void v1();
};

void *operator new(unsigned int size);

extern C_002226b4 *DAT_0034a264;
extern Kira *DAT_0034a268;
extern Kira *DAT_0034a26c;
extern float DAT_002eff88;
extern char DAT_002f0f4c[];
extern char DAT_002f0f78[];
extern char DAT_002f0dd0[];
extern char DAT_002f0dfc[];
extern char *DAT_002f0e34[];
extern Lookup DAT_002f0ec0[];

extern "C" {
void FUN_00104118(C_002226b4 *m, char *path, int f);
void FUN_00103540(float t, C_002226b4 *m);
Obj *FUN_001039d8(C_002226b4 *m, char *name, int f);
Item *FUN_001026b8(Obj *page, char *name);
void FUN_000ffb3c(Item *it, int on);
B_00050fc0 *FUN_00050fc0(void *mem);
void FUN_000511c4(B_00050fc0 *s, char *name, int f);
void FUN_00051398(B_00050fc0 *s, float z);
void FUN_0022305c(C_002226b4 *m);
}

inline Kira::Kira(char *name) : spr(0)
{
    B_00050fc0 *p = (B_00050fc0 *)operator new(sizeof(B_00050fc0));
    if (p != 0)
        p = FUN_00050fc0(p);
    spr = p;
    FUN_000511c4(spr, name, 0);
    FUN_00051398(spr, DAT_002eff88 + 100.0f);
    for (int i = 0; i < PART_COUNT; i++)
        parts[i].on = 0;
}

C_002226b4::C_002226b4(unsigned char m)
{
    f9c = 0;
    DAT_0034a264 = this;
    mode = m;
    if (mode != 0)
        FUN_00104118(this, DAT_002f0f4c, 0);
    else
        FUN_00104118(this, DAT_002f0f78, 0);
    FUN_00103540(DAT_002eff88, this);
    DAT_0034a268 = new Kira(DAT_002f0dd0);
    DAT_0034a26c = new Kira(DAT_002f0dfc);
    int base = (int)this;
    {
        int i = 0;
        char **n = DAT_002f0e34;
        int off = 0;
        do {
            *(Obj **)(base + off + 0x68) = FUN_001039d8(this, *n, 0);
            i++;
            n++;
            off += 4;
        } while (i < PAGE_COUNT);
    }
    for (int i = 0; i < ITEM_COUNT; i++) {
        items[i] = 0;
        if (pages[DAT_002f0ec0[i].page] != 0)
            items[i] = FUN_001026b8(pages[DAT_002f0ec0[i].page], DAT_002f0ec0[i].name);
        if (items[i] != 0)
            DAT_002f0ec0[i].out = items[i]->f24;
    }
    fa0 = items[6]->f20;
    if (mode != 0)
        fa4 = items[1]->f20;
    f60 = -1;
    f54 = -999;
    fb0 = 0;
    fb2 = 0;
    fb4 = 0;
    fb8 = 0;
    f64 = -1;
    fa8 = 0;
    fac = 0;
    Item *r = items[2];
    fbc = r->x;
    fc0 = r->w;
    fc4 = r->y;
    fc8 = r->h;
    FUN_000ffb3c(items[3], 1);
    items[3]->color = 0x00ffffff;
    FUN_0022305c(this);
}
// NEAR 72 words (lane w4f2, 2026-11-10); 207/211 words. Needs symbol __vt__10C_002226b4 (appended).
// Matches: prologue, vptr, the two Kira allocations (explicit operator new + FUN_00050fc0 returning
//   the object, since retail stores the ctor's return value), the first name loop (base+off form).
// Remaining: the 11-entry lookup loop. Retail addresses the slot as base+off (off in s1, +4/iter),
//   stores 0 via (s3+s1)+112, keeps &slot in v1 and recomputes it after FUN_001026b8 (no callee-saved
//   copy). Indexed items[i] (this file) strength-reduces to a pointer IV (4 words short); base+off
//   spellings (plain, pointer local, reassigned pointer, inline with Item** or Item*&, char* base)
//   all keep &slot in s1 and spill the counter into s4 (206 differ by register shift).
// near miss from lane w4f2 (unreviewed)
