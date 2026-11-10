/* HSTMATCH module=EBOOT.BIN start=0x001b57f8 end=0x001b5c44 symbol=FUN_001b57f8 lang=c++ */
/* Hand-written reconstruction from the disassembly (lane ol3 near miss wip/lob_se_request_001b57f8.cpp, finished by lane pn4); exact match verified; matching idiom: the frames loop mixes direct `size` reads (CSE'd with the guard) and const getters Count()/Data() (reloaded per pass), written as `if (i < size) do {..} while (i < Count());` with an if-assigned clamp index in at(); names and types await human review. */
struct Vec4 {
    float x, y, z, w;
    Vec4() {}
    Vec4(const Vec4 &o) : x(o.x), y(o.y), z(o.z), w(o.w) {}
} __attribute__((aligned(16)));

struct Frame {
    char pad0[0x14];
    float f14;
    char pad18[8];
};
struct FrameVec {
    Frame *data;
    unsigned size;
    const Frame &at(unsigned i) { unsigned k = i; if (!(i < size)) k = Count() - 1; return Data()[k]; }
    unsigned Count() const { return size; }
    Frame *Data() const { return data; }
};
struct Track {
    char pad0[0x10];
    Vec4 pos;
    Vec4 vel;
    char pad30[0x26c - 0x30];
    FrameVec frames;
};
struct Ctx {
    char pad0[0x3c];
    int mode;
    char pad40[0x48];
    Track *track;
};
struct Smash {
    char pad0[0xc];
    float t;
};
struct Req {
    char pad0[0x68];
    int handle;
    int count;
    unsigned char active;
    unsigned char smash;
    char pad72[2];
    Track *track;
};

extern "C" {
extern Ctx *DAT_003386b0;
extern char DAT_0034179c[];
extern Smash DAT_00338760;
extern char DAT_002e997c[];
extern float DAT_002e995c;
extern float DAT_002e9968;
extern float DAT_002e996c;
extern float DAT_002e9970;
extern float DAT_002e9974;
extern float DAT_002e9978;
float FUN_000224d8(const Vec4 *, const Vec4 *);
float FUN_00022e44(float);
void FUN_001b1f84(void *, int *);
int FUN_001b1d10(void *, int, const Vec4 *, int);
int FUN_001b1c44(void *, int);
void FUN_001b210c(void *, int *, float, float);
void FUN_001b871c(Req *);
void FUN_001b5c44(char *);
}

static inline float Clamp(float value, const float &lo, const float &hi)
{
    return value < lo ? lo : (!(value > hi) ? value : hi);
}
static inline float Length(Vec4 v)
{
    return FUN_00022e44(FUN_000224d8(&v, &v));
}
static inline Smash *ActiveSmash()
{
    return DAT_00338760.t > 0.0f ? &DAT_00338760 : 0;
}

extern "C" void FUN_001b57f8(Req *self, int kind, int type, Track *track)
{
    FUN_001b1f84(DAT_0034179c, &self->handle);
    self->active = 0;
    self->smash = 0;
    self->count = 0;
    if (track != 0) {
        self->track = track;
    } else {
        track = DAT_003386b0->track;
        self->track = track;
    }
    Vec4 pos = track->pos;
    bool started = false;
    if (type == 0) {
        switch (kind) {
        case 1:
            if (DAT_003386b0->mode == 7) {
                self->handle = FUN_001b1d10(DAT_0034179c, 0x36, &pos, 0);
                started = true;
            }
            break;
        case 2:
            self->active = 1;
            break;
        default:
            if (DAT_003386b0->mode == 7) {
                self->handle = FUN_001b1d10(DAT_0034179c, 0x34, &pos, 0);
                started = true;
            }
            break;
        }
    } else if (type == 1) {
        if (DAT_003386b0->mode == 7) {
            float speed = Length(track->vel);
            if (speed >= DAT_002e9968 * 1000.0f / 3600.0f) {
                self->handle = FUN_001b1d10(DAT_0034179c, 0x35, &pos, 0);
                started = true;
            }
        }
    } else if (type == 2) {
        if (DAT_003386b0->mode == 7) {
            self->handle = FUN_001b1d10(DAT_0034179c, 0x35, &pos, 0);
            started = true;
        }
    } else if (type == 6) {
        FUN_001b1d10(DAT_0034179c, 0x39, &pos, 0);
        if (DAT_003386b0->mode == 7) {
            self->handle = FUN_001b1d10(DAT_0034179c, 0x38, &pos, 0);
            self->smash = 1;
        }
    }
    if (ActiveSmash() != 0) {
        self->active = 1;
        FUN_001b5c44(DAT_002e997c);
    }
    if (started) {
        Vec4 vel = DAT_003386b0->track->vel;
        float speed = FUN_00022e44(FUN_000224d8(&vel, &vel));
        float v = DAT_002e996c * speed;
        float vol = Clamp(v, DAT_002e9970, DAT_002e9974);
        self->count = (int)(vol * DAT_002e995c);
        if (self->count > 0)
            FUN_001b210c(DAT_0034179c, &self->handle, vol, vol);
        else
            FUN_001b871c(self);
    }
    if (self->active) {
        FrameVec &v = self->track->frames;
        float best = 1.0f;
        unsigned i = 0;
        if ((int)i < (int)v.size) {
            do {
                float f = v.at(i).f14;
                if (f > best)
                    break;
                best = f;
                i++;
            } while ((int)i < (int)v.Count());
        }
        if (best < DAT_002e9978)
            self->handle = FUN_001b1c44(DAT_0034179c, 0x37);
    }
}
