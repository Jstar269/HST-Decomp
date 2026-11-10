/* HSTMATCH module=EBOOT.BIN start=0x0024ba68 end=0x0024bd80 symbol=FUN_0024ba68 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Vec4 {
    float x, y, z, w;
};

struct Rng {
    char pad[0x1360];
    unsigned int seed;
};

struct Shot {
    char pad00[0x50];
    char model[0x210 - 0x50];
    Vec4 from;
    Vec4 to;
    Vec4 cur;
};

extern "C" {
extern Rng *DAT_003402a0;
void FUN_000550c8(void *, Vec4 *);
}

static inline int Rand()
{
    Rng *g = DAT_003402a0;
    unsigned int s = g->seed * 0x343fd + 0x269ec3;
    g->seed = s;
    return (s >> 16) & 0x7fff;
}

static inline float RandF(float lo, float hi)
{
    unsigned int r = Rand();
    float range = hi - lo;
    return lo + range * ((1.0f / 32768.0f) * r);
}

extern "C" void FUN_0024ba68(Shot *self, Vec4 t1, Vec4 t2)
{
    self->from.x = t1.x;
    self->from.y = t1.y;
    self->from.z = t1.z;
    self->from.w = t1.w;
    self->to.x = t2.x;
    self->to.y = t2.y;
    self->to.z = t2.z;
    self->to.w = t2.w;
    if (Rand() % 2 != 0) {
        self->from.z = -RandF(0.0f, 11.887199f);
        self->to.z = -RandF(0.0f, 11.887199f);
    } else {
        self->from.z = RandF(0.0f, 11.887199f);
        self->to.z = RandF(0.0f, 11.887199f);
    }
    self->cur.x = self->from.x;
    self->cur.y = self->from.y;
    self->cur.z = self->from.z;
    self->cur.w = self->from.w;
    FUN_000550c8(self->model, &self->from);
}
