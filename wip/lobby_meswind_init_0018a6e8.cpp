/* HSTMATCH module=EBOOT.BIN start=0x0018a6e8 end=0x0018a950 symbol=FUN_0018a6e8 lang=c++ */

typedef struct Item Item;
typedef struct Node Node;
typedef struct Self Self;

struct Vec2 {
    float x, y;
};

struct Vec4 {
    float x, y, z, w;
};

struct Item {
    char pad[0x64];
    float f64;          // 100 = 0x64
    char pad68[0x108 - 0x68];
    struct Vec2 v108;   // 264 = 0x108
    char pad110[0x118 - 0x110];
    unsigned color;     // 280 = 0x118
    char pad11c[4];
    struct Vec4 colorVec;      // 288 = 0x120
    char pad130[0x13c - 0x130];
    struct Vec2 v13c;   // 316 = 0x13c
    char pad144[0x14c - 0x144];
    unsigned char f14c; // 332 = 0x14c
};

struct Node {
    char pad[0x20];
    Item *item;
};

struct Self {
    char pad[0x74];
    void *f74;          // 0x74 (116)
    Item *items[3];     // 0x78 (120), 0x7c (124), 0x80 (128)
    char pad84[4];      // 0x84
    float scores[3];    // 0x88 (136), 0x8c (140), 0x90 (144)
};

extern "C" {
extern float DAT_002e0ae8;
extern float DAT_002e0b08;
extern struct Vec2 DAT_002e0b0c;
extern float DAT_00341528;
extern int DAT_003417a4;
extern char DAT_002e0b13[];
extern char DAT_002e0b44[];
extern char DAT_002e0b50[];
extern char DAT_002e0b5c[];
extern char DAT_002e0b68[];

void FUN_00104118(void *self, const char *path, int flag);
void FUN_00103540(void *self, float f);
void *FUN_001039d8(void *self, const char *name, int flag);
Node *FUN_001026b8(void *parent, const char *name);
void FUN_000143b0(char *buf, const char *fmt, ...);
void FUN_00022498(float scale, struct Vec4 *out, const struct Vec4 *in);
void FUN_0010d4a8(Item *item);
}

inline struct Vec4 Scale(float s, const struct Vec4 *v)
{
    struct Vec4 r;
    FUN_00022498(s, &r, v);
    return r;
}

inline void SetFlag(Item *it, unsigned char v)
{
    it->f14c = v;
    if (it->f14c == 0) {
        FUN_0010d4a8(it);
    }
}

extern "C" void FUN_0018a6e8(Self *self)
{
    char buf[64];
    struct Vec4 in;
    struct Vec4 in2;
    Item *item;
    void *h;
    Node *node;
    int i;
    int j;

    FUN_00104118(self, DAT_002e0b13 + 1, 1);
    FUN_00103540(self, DAT_002e0ae8);

    h = FUN_001039d8(self, DAT_002e0b44, 0);
    self->f74 = FUN_001026b8(h, DAT_002e0b50);

    h = FUN_001039d8(self, DAT_002e0b44, 0);
    node = FUN_001026b8(h, DAT_002e0b5c);
    item = node->item;
    item->color = 0x80808080;
    in.x = 128.0f;
    in.y = 128.0f;
    in.z = 128.0f;
    in.w = 128.0f;
    item->colorVec = Scale(0.008f, &in);
    self->items[0] = item;
    self->scores[0] = item->f64;

    for (i = 0; i < 2; i++) {
        FUN_000143b0(buf, DAT_002e0b68, i);
        h = FUN_001039d8(self, DAT_002e0b44, 0);
        node = FUN_001026b8(h, buf);
        item = node->item;
        item->color = 0x80151515;
        in2.x = 21.0f;
        in2.y = 21.0f;
        in2.z = 21.0f;
        in2.w = 128.0f;
        item->colorVec = Scale(0.008f, &in2);
        self->items[i + 1] = item;
        self->scores[i + 1] = item->f64;
    }

    for (j = 0; j < 3; j++) {
        struct Vec2 t;
        t.x = DAT_002e0b08;
        t.y = DAT_00341528;
        self->items[j]->v13c = t;
        Item *it = self->items[j];
        it->v108 = DAT_002e0b0c;
        FUN_0010d4a8(it);
        if (DAT_003417a4 < 9) {
            SetFlag(self->items[j], 1);
        } else {
            SetFlag(self->items[j], 0);
        }
    }
}
// NEAR (lane w3f4, 11 words): everything matches except the order of the float loads in the 3-pass loop's
// first two statements. Retail: lwc1 f1,b08; lw v1,items[j]; lwc1 f0,341528; swc1 f1,316(v1); lwc1 f1,b0c;
// swc1 f0,320(v1); lw a0,items[j]; lwc1 f0,b10; swc1 f1,264(a0); jal; swc1 f0,268(a0). Ours loads 341528 first
// and keeps a0 for both pointers; pair 2 is not batched.
// Fixed on the way (from the ag3 draft, 67 words): C++ inline `Vec4 Scale(float,const Vec4*)` returning by value
// gives retail's scheduled colour copy; SetFlag inline gives the two flag branches; natural index loops.
// Ruled out: separate float statements (loads after stores), inline SetA/SetB with float params or as Item
// members (loads reversed), local Vec2 for pair 2 (worse), Vec2 initialiser list (frame grows), MakeVec2 inline,
// extern const floats (hoisted out of the loop).
// near miss from lane w3f4 (unreviewed)
