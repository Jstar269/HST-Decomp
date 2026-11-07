/* HSTMATCH module=hstp_trans_main.prx start=0x00237b74 end=0x00237f18 symbol=FUN_00237b74 lang=c++ data=0x0039ae84 */
/* Hand-written reconstruction: clone of the source of court_ambient_sound_00228600.cpp in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. Data: its string literals (11 strings, 0x002f2ab8-0x002f2b4c) are defined in place and placed with data= (tools/hstdatapool.py); before, the source named them through address externs. */
/* Prop kinds, named after the sound each one starts (the string it passes). */
enum {
    PROP_KIND_ROCK = 1,
    PROP_KIND_PET = 2,
    PROP_KIND_GRASS = 4,
    PROP_KIND_BRANCH = 5,
    PROP_KIND_BAMBOO_SHOOT = 7,
    PROP_KIND_CAN = 9,
    PROP_KIND_IRREGULAR = 11,
    PROP_KIND_WATER = 13,
    PROP_KIND_GIMMICK = 15,
    PROP_KIND_18 = 18,
    PROP_KIND_ROCK_23 = 23,
    PROP_KIND_ROCK_24 = 24,
    PROP_KIND_ROCK_25 = 25
};

/* A player counts as close when the distance to the prop's centre is below this. */
#define NEAR_DISTANCE 1.5f

struct Vec4 {
    float x, y, z, w;
    Vec4() {}
    Vec4(const Vec4 &o) : x(o.x), y(o.y), z(o.z), w(o.w) {}
} __attribute__((aligned(16)));

struct Player {
    char pad[0x5bc];
    void *body;
};

struct Ctx {
    char pad0[0x70];
    Player *players[4];
};

/* Court prop: centre point, kind, a counter, an active flag and the current sound handle. */
struct Court {
    char pad0[0x110];
    Vec4 center;
    char pad120[0x428 - 0x120];
    int kind;
    int pad42c;
    int count;
    unsigned char active;
    char pad435[0x450 - 0x435];
    int sound;
};

extern "C" {
extern Ctx *DAT_003e19f0;
extern char DAT_003eaad8[];
extern char DAT_0039af18[];
float FUN_00022500(const Vec4 *, const Vec4 *);
float FUN_00022e6c(float);
Vec4 *FUN_0002245c(Vec4 *, const Vec4 *, const Vec4 *);
Vec4 *FUN_0009575c(void *);
int FUN_001c0010(void *, int);
int FUN_001c0984(void *, char *);
}

static inline Vec4 Sub(const Vec4 &a, const Vec4 &b)
{
    Vec4 r;
    FUN_0002245c(&r, &a, &b);
    return r;
}

/* Starts the ambient sound that belongs to the prop's kind (when active, or when the held sound handle has ended). */
extern "C" void FUN_00237b74(Court *self, int quiet)
{
    switch (self->kind) {
    case PROP_KIND_ROCK:
    case PROP_KIND_ROCK_23:
    case PROP_KIND_ROCK_24:
    case PROP_KIND_ROCK_25:
        if (self->active)
            FUN_001c0984(DAT_003eaad8, "court_rock1");
        break;
    case PROP_KIND_PET:
        if (self->active)
            FUN_001c0984(DAT_003eaad8, "court_pet1");
        if (self->count > 0 && !FUN_001c0010(DAT_003eaad8, self->sound))
            self->sound = FUN_001c0984(DAT_003eaad8, "court_pet2");
        break;
    case PROP_KIND_GRASS:
        if (self->active)
            FUN_001c0984(DAT_003eaad8, "court_grass");
        break;
    case PROP_KIND_IRREGULAR:
        if (self->active) {
            Vec4 pos = self->center;
            int variant = 1;
            for (int i = 0; i < 4; i++) {
                Player *p = DAT_003e19f0->players[i];
                if (p) {
                    Vec4 other = *FUN_0009575c(p->body);
                    Vec4 d;
                    d = Sub(pos, other);
                    if (FUN_00022e6c(FUN_00022500(&d, &d)) < NEAR_DISTANCE)
                        variant = 2;
                }
            }
            if (variant == 1)
                FUN_001c0984(DAT_003eaad8, "court_irregular");
            else
                FUN_001c0984(DAT_003eaad8, "court_slip");
        }
        break;
    case PROP_KIND_CAN:
        if (self->active)
            FUN_001c0984(DAT_003eaad8, "court_can2");
        break;
    case PROP_KIND_BRANCH:
        if (self->active)
            FUN_001c0984(DAT_003eaad8, "court_branch");
        break;
    case PROP_KIND_BAMBOO_SHOOT:
        if (self->active)
            FUN_001c0984(DAT_003eaad8, "court_takenoko2");
        break;
    case PROP_KIND_GIMMICK:
        if (self->active)
            FUN_001c0984(DAT_003eaad8, "court_gimmic");
        break;
    case PROP_KIND_WATER:
        if (!FUN_001c0010(DAT_003eaad8, self->sound))
            self->sound = FUN_001c0984(DAT_003eaad8, "court_water");
        break;
    case PROP_KIND_18:
        if (quiet == 0 && !FUN_001c0010(DAT_003eaad8, self->sound))
            self->sound = FUN_001c0984(DAT_003eaad8, DAT_0039af18);
        break;
    }
}
