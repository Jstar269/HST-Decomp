/* HSTMATCH module=hstp_trans_main.prx start=0x000abd80 end=0x000ac150 symbol=FUN_000abd80 lang=c++ */
/* Hand-written reconstruction from the disassembly (restructured from the earlier far draft; first half is the clamp/limit code of src/sharing_main/check_motion_limits_000ac53c.cpp); NEAR MISS, not exact: 244 of 244 words, 19 differing; names and types await human review. */
extern "C" {
extern const float DAT_0037acac;
extern const float DAT_0037acb0;
extern const float DAT_0037acb4;
extern float DAT_003ea5dc;
extern float DAT_003ea5d4;
extern int DAT_003ea5c4;
extern int DAT_003ea5c8;
extern char DAT_003ea460[];
extern void FUN_0002245c(void *dst);
extern float FUN_0016e74c(void *, int);
extern float FUN_001709e8(void *, int);
extern float FUN_001709a0(void *, int);
extern float fabsf(float);
}

template <typename T>
static inline const T &max(const T &a, const T &b) {
    return (a < b) ? b : a;
}

template <typename T>
static inline const T &min(const T &a, const T &b) {
    return (b < a) ? b : a;
}

struct Vec4 {
    float x, y, z, w;
};

struct Ctx {
    char pad0[0x24];
    float f24;
    char pad28[4];
    float f2c;
    char pad30[0x184 - 0x30];
    float f184;
};

struct Obj {
    char pad[0x150];
    int f150;
    char pad154[0x440 - 0x154];
    int f440;
};

extern "C" Ctx *FUN_000b6e80(Obj *);

static inline bool check_range(float y, float lim)
{
    if (y > lim && y < 0.0f) return true;
    return false;
}

static inline float Lim2(Obj *self)
{
    float k = FUN_000b6e80(self)->f184 * 0.01f;
    return (1.0f - k) * -(FUN_000b6e80(self)->f24 * 0.01f);
}

static inline bool Stage23(Obj *self, float y)
{
    bool ok2;
    float lim = -(FUN_000b6e80(self)->f24 * 0.01f);
    if (y > lim && y < 0.0f) ok2 = true;
    else ok2 = false;
    bool ok3;
    if (ok2 && y <= Lim2(self)) ok3 = true;
    else ok3 = false;
    return ok3;
}

static inline int InBand(Obj *self, float x, float z)
{
    z = z - DAT_003ea5dc * 0.01f;
    float t = 0.0f;
    float az = fabsf(z);
    if (az > FUN_0016e74c(DAT_003ea460, 0) * 0.01f) {
        int sg = z < 0.0f ? -1 : 1;
        float e = FUN_0016e74c(DAT_003ea460, 0);
        float z2 = z - (float)sg * (e * 0.01f);
        float q;
        if (z2 > 0.0f) q = z2 / (DAT_003ea5d4 * 0.01f * (float)DAT_003ea5c4);
        else q = z2 / (DAT_003ea5d4 * 0.01f * (float)DAT_003ea5c8);
        t = (q > DAT_0037acb4) ? DAT_0037acb4 : q;
    }
    float h0, h1;
    h1 = FUN_001709e8(FUN_000b6e80(self), self->f440) * 0.01f;
    h0 = FUN_001709a0(FUN_000b6e80(self), self->f440) * 0.01f;
    Ctx *c = FUN_000b6e80(self);
    float thr = (1.0f - (h0 + t * (h1 - h0))) * (c->f2c * 0.01f);
    return fabsf(x) < thr;
}

extern "C" int FUN_000abd80(Obj *self, int a, int b, unsigned char flag)
{
    Vec4 local_10;
    Vec4 local_20;
    FUN_0002245c(&local_20);
    local_10 = local_20;
    if (flag == (self->f150 == 0)) {
        local_10.x = max(DAT_0037acac, local_10.x);
    } else {
        local_10.x = min(DAT_0037acb0, local_10.x);
    }
    float x, y, z;
    x = local_10.x;
    y = local_10.y;
    z = local_10.z;
    bool ok;
    if (fabsf(x) < FUN_000b6e80(self)->f2c * 0.01f && !InBand(self, x, z)) ok = true;
    else ok = false;
    int r;
    if (ok && Stage23(self, y)) r = 1;
    else r = 0;
    return r;
}
// NEAR MISS 19 words (lane fs1, 2026-11-10; was 77 in the far draft). Same length as retail; all branches, calls and selects match.
// Key structure found: the three stages are && chains, which is what keeps retail's UNFUSED selects
// (`bc1f P; nop; b Q; li v0,1; P: move v0,zero`): an else block with two predecessors cannot be folded into a likely slot.
// Stage 1 = `fabsf(x) < W && !InBand(self, x, z)` with InBand an inline returning INT (gives retail's andi 0xff);
// stages 2/3 = inline bool Stage23() whose third test is `ok2 && y <= Lim2(self)`; the result is `if (ok && Stage23()) r = 1; else r = 0;`.
// Still different: (1) FPR colours of x and z swapped (retail x f21, y f22, z/h1 f23; ours x f23, z/h1 f21): all 6 declaration
// orders x both InBand parameter orders, assignment order, init-declarations, a fresh `dz` local, and using local_10 fields directly
// (much worse) ruled out; (2) the thr expression `(1 - (h0 + t*(h1-h0))) * (f2c*0.01)`: retail materialises 1.0f in v1 before 0.01f
// in a0 and computes 0.01*f2c before 1-sum; ruled out: operand order, separate w/lerp/k locals, `*=`, inline Width()/Lerp() helpers.
// improved by lane fs1 (unreviewed): 77 -> 19 differing words
