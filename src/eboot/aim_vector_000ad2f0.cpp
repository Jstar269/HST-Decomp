/* HSTMATCH module=EBOOT.BIN start=0x000ad2f0 end=0x000ad674 symbol=FUN_000ad2f0 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. Vec4 is aligned(16) so the result sits below the address-taken float locals; Min/Max take const float & (the -lim temporary slot). */

struct Vec4 {
    float x, y, z, w;
    Vec4() {}
    Vec4(float a, float b, float c, float d) : x(a), y(b), z(c), w(d) {}
} __attribute__((aligned(16)));

struct Stats {
    char pad[0x18];
    float f18;
    float f1c;
    float f20;
};

struct Obj {
    char pad0[0x580];
    float f580;
    float f584;
    float f588;
    char pad58c[0x5a0 - 0x58c];
    Vec4 m5a0;
};

struct Cfg {
    char pad[0x45];
    unsigned char f45;
};

struct Cam {
    char pad[0x110];
    float f110;
};

struct View {
    char pad[0x88];
    Cam *cam;
};

extern Cfg *DAT_00338b44;
extern View *DAT_003386b0;
extern float DAT_0034118c;
extern float DAT_00341188;
extern float DAT_00341174;
extern float DAT_0034117c;
extern float DAT_003412a0;
extern float DAT_003412a4;
extern float DAT_003412a8;
extern float DAT_003412ac;
extern float DAT_003412b0;
extern unsigned char DAT_00341304;

extern "C" {
int FUN_000ad674(Obj *, int);
Stats *FUN_000b3c68(Obj *);
void FUN_00022450(Vec4 *, const Vec4 *, const Vec4 *);
}

static inline const float &Min(const float &a, const float &b) { return b < a ? b : a; }
static inline const float &Max(const float &a, const float &b) { return a < b ? b : a; }
static inline float Clamp(float v, float lo, float hi) { return v < lo ? lo : (!(v > hi) ? v : hi); }

extern "C" float fabsf(float);

extern "C" Vec4 FUN_000ad2f0(Obj *o, int mode, float x, float y)
{
    float lim;
    float t;
    Vec4 r(0.0f, 0.0f, 0.0f, 0.0f);
    float k;
    switch (FUN_000ad674(o, mode)) {
    case 0:
        k = FUN_000b3c68(o)->f18 * 0.01f;
        break;
    case 1:
        k = FUN_000b3c68(o)->f1c * 0.01f;
        break;
    case 2:
        k = FUN_000b3c68(o)->f20 * 0.01f;
        break;
    }
    lim = DAT_0034118c * 0.01f;
    if ((DAT_00338b44->f45 != 0) ^ 1) {
        float e = DAT_00341188 * 0.01f;
        lim = lim + e;
        k = k + e;
    }
    r.x = k * x;
    r.z = DAT_00341174 * 0.01f + DAT_0034117c * 0.01f * y;
    r.y = -DAT_003386b0->cam->f110;
    r.w = 1.0f;
    float v = o->f580;
    float d = DAT_003412a0 * v;
    float add = DAT_003412ac * (o->f588 + DAT_003412b0 * 0.01f);
    if (DAT_00341304 != 0) {
        if (DAT_0034118c != 0.0f) {
            if (x == 0.0f) {
                t = r.x + d;
                if (t > 0.0f)
                    t = Min(t, lim);
                else
                    t = Max(t, -lim);
                d = t - r.x;
            } else {
                float m = lim - k;
                if (x * v >= 0.0f)
                    d *= DAT_003412a4;
                else
                    d *= DAT_003412a8;
                d = Clamp(d, -m, m);
            }
        } else if (x != 0.0f && k != 0.0f) {
            if (x * v >= 0.0f) {
                d *= DAT_003412a4;
                if (fabsf(r.x + d) < fabsf(v))
                    d = v - r.x;
            } else {
                d *= DAT_003412a8;
            }
        }
    }
    r.x += d;
    r.z += add;
    FUN_00022450(&r, &r, &o->m5a0);
    return r;
}
