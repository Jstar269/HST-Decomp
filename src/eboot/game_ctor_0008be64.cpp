/* HSTMATCH module=EBOOT.BIN start=0x0008be64 end=0x0008c230 symbol=FUN_0008be64 lang=c++ */
/* Hand-written reconstruction from the disassembly (structs from the far draft of lane ol2); exact match verified; matching idiom: the item loop keeps a byte offset local and spells the item address with (int)/(unsigned) casts on it in the 2nd/3rd calls so MWCC does not CSE self+off across the calls (retail recomputes it per call); names and types await human review. */
struct Item {
    virtual void i0();
    virtual void i1();
    virtual void Load(const char *, const char *, int, int, int, int);
    char pad[0x1b0 - 4];
};

struct Inner {
    int res;
};

struct Root {
    char pad[4];
    Inner *inner;
};

struct Del {
    virtual ~Del();
};

struct Menu {
    void *vt;
    char pad4[0xc];
    char listA[0x10];
    char listB[0x10];
};

struct Game {
    Root *root;
    char pad4[0x10 - 4];
    Item items[4];
    char model[0x8a0 - 0x6d0];
    float lightA[4];
    float lightB[4];
    char pad8c0[0x8dc - 0x8c0];
    int colB;
    int colA;
    char pad8e4[0x990 - 0x8e4];
    Menu menu;
    int mode;
    int mode2;
    unsigned char flag;
    char pad9c9[3];
    void *hud;
    void *heap;
    Item *Get(unsigned i) { return &items[i]; }
};

extern "C" {
extern char DAT_003070bc[];
extern char DAT_0030ab9c[];
extern void *DAT_00310fc4;
extern char DAT_0030ebf4[];
extern char DAT_0030fe20[];
extern char DAT_002d25a4[];
extern char DAT_002d25bc[];
extern char DAT_002d25e4[];
extern char DAT_002d2614[];
extern char DAT_002d262c[];
extern char DAT_002d2658[];
extern char DAT_002d266c[];
extern char DAT_002d259c[];
extern char DAT_002d2670[];
extern float DAT_002bc410[];
extern void *DAT_003313c4;
extern Del *DAT_003386c0;
extern void *DAT_0034a820;
extern int DAT_002d2568;
extern int DAT_003386bc;
extern char DAT_0034a678[];
void FUN_000007f4(void *, void *, void *, int, int);
void FUN_00056b98();
void FUN_00056cb0();
void FUN_0005b670(void *);
void FUN_000649c0(void *);
void FUN_0008c230(void *);
void *FUN_00010184(int, int);
void FUN_0001bfb4(void *, void *, int);
void FUN_00042698(void *, int, int);
void FUN_0001cad0(void *, int, const char *);
void *FUN_00000bcc(unsigned int);
void *FUN_0018ff74(void *);
void FUN_0001f21c(void *, void *);
Root *FUN_00102e38(void *);
void FUN_00104118(Root *, const char *, int);
void FUN_00102824(int);
void FUN_000573a0(Item *, int, const char *, int, int);
void FUN_00055044(Item *, const float *);
void FUN_0005ba78(void *, void *);
void FUN_0005bc58(void *);
void FUN_002800ac(void *);
void FUN_000651b8(void *, const char *, int, int, const char *);
void *FUN_00111044(void *, const char *, int);
void FUN_0001cb88(void *);
void FUN_0001ca58(void *, const char *);
void FUN_00291b8c(void *, int);
void FUN_00194030(int);
void *FUN_001b8bfc(void *);
void FUN_001b8cf8(void *);
void *FUN_0001cbfc(void);
int FUN_0001cc54(void);
void FUN_000104e0(void *);
void FUN_002345bc(void *, int);
void FUN_0008cb14(Game *, int, int);
}

extern "C" Game *FUN_0008be64(Game *self)
{
    FUN_000007f4(self->items, (void *)FUN_00056b98, (void *)FUN_00056cb0, 0x1b0, 4);
    FUN_0005b670(self->model);
    Menu *mn = &self->menu;
    FUN_000649c0(mn);
    mn->vt = DAT_003070bc;
    FUN_0008c230(mn->listA);
    FUN_0008c230(mn->listB);
    self->flag = 0;
    self->hud = 0;
    self->heap = FUN_00010184(64, 0x200000);
    FUN_0001bfb4(DAT_0030ab9c, self->heap, 0x200000);
    FUN_00042698(DAT_00310fc4, 512, 512);
    FUN_0001cad0(DAT_0030ebf4, 1, DAT_002d25a4);
    void *p = FUN_00000bcc(64);
    if (p != 0)
        p = FUN_0018ff74(p);
    FUN_0001f21c(DAT_0030fe20, p);
    void *q = FUN_00000bcc(84);
    if (q != 0)
        q = FUN_00102e38(q);
    self->root = (Root *)q;
    FUN_00104118(self->root, DAT_002d25bc, 0);
    FUN_00102824(self->root->inner->res);
    for (unsigned i = 0; i < 4; i++) {
        int off = i * sizeof(Item);
        ((Item *)((char *)self + off + 16))->Load(DAT_002d25e4, DAT_002d2614, 0, 0, 0, 0);
        FUN_000573a0((Item *)((char *)self + (int)off + 16), 0, DAT_002d262c, 0, 0);
        FUN_00055044((Item *)((char *)self + (unsigned)off + 16), DAT_002bc410);
    }
    FUN_0005ba78(self->model, DAT_003313c4);
    self->colA = 0xf100;
    self->colB = 0xff10;
    self->lightA[0] = 0.0f;
    self->lightA[1] = 0.0f;
    self->lightA[2] = 1.0f;
    self->lightA[3] = 1.0f;
    self->lightB[0] = 0.0f;
    self->lightB[1] = 0.0f;
    self->lightB[2] = -1.0f;
    self->lightB[3] = 0.0f;
    FUN_0005bc58(self->model);
    void *t = FUN_00000bcc(4);
    if (t != 0)
        FUN_002800ac(t);
    FUN_000651b8(&self->menu, DAT_002d2658, 1, 0, DAT_002d266c);
    if (DAT_003386c0 != 0) {
        delete DAT_003386c0;
        DAT_003386c0 = 0;
    }
    void *m = FUN_00000bcc(44);
    if (m != 0)
        m = FUN_00111044(m, DAT_002d259c, 2);
    DAT_003386c0 = (Del *)m;
    FUN_0001cb88(DAT_0030ebf4);
    FUN_0001ca58(DAT_0030ebf4, DAT_002d2670);
    if (DAT_0034a820 != 0)
        FUN_00291b8c(DAT_0034a820, 0);
    FUN_0001cb88(DAT_0030ebf4);
    FUN_00194030(1);
    void *h = FUN_00000bcc(20);
    if (h != 0)
        h = FUN_001b8bfc(h);
    self->hud = h;
    FUN_001b8cf8(h);
    void *base = FUN_0001cbfc();
    FUN_0001bfb4(DAT_0030ab9c, base, FUN_0001cc54());
    if (self->heap != 0) {
        FUN_000104e0(self->heap);
        self->heap = 0;
    }
    if (DAT_002d2568 != 26) {
        self->mode = DAT_002d2568;
        self->mode2 = DAT_002d2568;
        DAT_002d2568 = 26;
    } else if (DAT_003386bc == 0) {
        self->mode = 27;
        self->mode2 = 27;
    } else {
        self->mode = 29;
        self->mode2 = 29;
    }
    FUN_002345bc(DAT_0034a678, self->mode);
    FUN_0008cb14(self, self->mode, 1);
    return self;
}
