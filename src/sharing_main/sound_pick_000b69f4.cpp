/* HSTMATCH module=hstp_trans_main.prx start=0x000b69f4 end=0x000b6da8 symbol=FUN_000b69f4 lang=c++ */
/* Hand-written reconstruction: clone of the source of sound_pick_000b37dc.cpp in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. Matching idioms: the sound id lives in a one-int struct (SoundId) so MWCC keeps the default-case `id == 0xd0` test unfolded (`move v0,s0; bne s0,v0`), and the two clamp operands are named floats declared first so their const& temporaries take the top frame slots. */
enum { SOUND_NONE = 0xd0 };  /* no sound selected */

struct CVec {
    float x, y, z, w;
    CVec() {}

} __attribute__((aligned(16)));

struct Sample { char pad0[0x10]; CVec pos; };
struct SampleList { char pad0[0xc]; Sample *data; int size;
    int Count() { return size; }
    Sample &at(unsigned i) { if (i >= (unsigned)size) i = size - 1; return data[i]; }
};

struct Ball {
    char pad0[0x20];
    CVec vel;
    char pad30[0x26c - 0x30];
    SampleList samples;
};

struct Player {
    char pad0[0x168];
    unsigned seed;
    char pad16c[0x5a0 - 0x16c];
    CVec m5a0;
    float Rand()
    {
        seed = seed * 0x343fd + 0x269ec3;
        return (float)((seed >> 16) & 0x7fff) * (1.0f / 32768.0f);
    }
};

struct Game {
    char pad0[0x88];
    Ball *ball;
};

template <class T> inline const T &Max(const T &a, const T &b) { return a < b ? b : a; }

extern "C" {
extern Game *DAT_003e19f0;
extern void *DAT_003e1e84;
extern float DAT_0037afcc;
extern float DAT_0037afd0;
float FUN_00022500(const CVec *, const CVec *);
float FUN_00022e6c(float);
CVec *FUN_00022478(CVec *, const CVec *, const CVec *);
int FUN_000e1f50(void *, const CVec *, int);
}

static inline CVec Sub(const CVec &a, const CVec &b) { CVec r; FUN_00022478(&r, &a, &b); return r; }

struct SoundId { int id; };

/* Pick a sound id for the event `kind` (1, 2, 11, 12 or other): ball speed and landing spot checks, with a random fallback. */
extern "C" int FUN_000b69f4(Player *pl, int unused, int kind, int flag)
{
    float za, zc;
    SoundId r;
    r.id = SOUND_NONE;
    CVec v = DAT_003e19f0->ball->vel;
    float spd = FUN_00022e6c(FUN_00022500(&v, &v)); /* ball speed in m/s */
    switch (kind) {
    case 1:
        r.id = 0x5c;
        break;
    case 2:
        if (spd * 3.6f >= 130.0f)  /* km/h */
            r.id = 0x5c;
        break;
    case 11:
        r.id = 0x5f;
        break;
    case 12:
        r.id = 0x5e;
        break;
    default: {
        SampleList &samples = DAT_003e19f0->ball->samples;
        if (r.id == SOUND_NONE) {
            if (samples.Count() > 0) {
                CVec d = Sub(samples.at(0).pos, pl->m5a0);
                CVec a = d;
                float k = -0.199999988f;
                float s = a.x < 0.0f ? -1 : 1;
                a.x -= k * s;
                za = a.z - k;
                a.z = Max(DAT_0037afcc, za);
                if (!FUN_000e1f50(DAT_003e1e84, &a, 0)) {
                    CVec c = d;
                    float k2 = 1.5f;
                    float s2 = c.x < 0.0f ? -1 : 1;
                    c.x -= k2 * s2;
                    zc = c.z - k2;
                    c.z = Max(DAT_0037afd0, zc);
                    float p = FUN_000e1f50(DAT_003e1e84, &c, 0) ? 0.0f : 0.5f;
                    if (pl->Rand() < p)
                        r.id = 0x5e;
                }
            }
        }
        break;
    }
    }
    if (r.id == SOUND_NONE) {
        if (flag || pl->Rand() < 1.0f / 3.0f)
            r.id = 0x5d;
    }
    return r.id;
}
