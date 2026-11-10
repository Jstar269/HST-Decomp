/* HSTMATCH module=EBOOT.BIN start=0x00203dcc end=0x00204110 symbol=FUN_00203dcc lang=c++ */
struct Vec4 {
    float x, y, z, w;
};

typedef struct {
    float m[16];
} M4;

union Mat4 {
    Vec4 r[4];
    float m[16];
};

struct Gauge {
    float scale;
    int kind;
    float size;
    int pad;
};

struct Hud {
    char pad00[0x1c];
    Gauge gauges[(0x388 - 0x1c) / 16];
    char pad37c[0x388 - 0x1c - 16 * ((0x388 - 0x1c) / 16)];
    float f388;
    char pad38c[0x4f0 - 0x38c];
    Vec4 offset;
    Vec4 origin;
    float fade;
    char pad514[4];
    int a;
    int b;
};

extern "C" {
extern Mat4 DAT_002bc410;
extern const M4 DAT_0032b1d0;
extern int DAT_002ef1d8;
void FUN_000224b8(float, Vec4 *, Vec4 *);
void FUN_00022418(Vec4 *, Vec4 *, Vec4 *);
void FUN_00022570(Mat4 *, M4 *, Mat4 *);
void FUN_00022548(Vec4 *, Mat4 *, Vec4 *);
float FUN_0000ce3c(float, float);
void FUN_00202cb8(float, Hud *, int, int, Vec4 *);
void FUN_002033a8(float, Hud *, int, int, Vec4 *);
void FUN_00203680(float, Hud *, int, int, Vec4 *);
}

static inline const int &Min(const int &a, const int &b)
{
    return a < b ? a : b;
}

extern "C" void FUN_00203dcc(Hud *self, int idx, int alt)
{
    Mat4 m1;
    m1.m[0] = DAT_002bc410.m[0];
    m1.m[1] = DAT_002bc410.m[1];
    m1.m[2] = DAT_002bc410.m[2];
    m1.m[3] = DAT_002bc410.m[3];
    m1.m[4] = DAT_002bc410.m[4];
    m1.m[5] = DAT_002bc410.m[5];
    m1.m[6] = DAT_002bc410.m[6];
    m1.m[7] = DAT_002bc410.m[7];
    m1.m[8] = DAT_002bc410.m[8];
    m1.m[9] = DAT_002bc410.m[9];
    m1.m[10] = DAT_002bc410.m[10];
    m1.m[11] = DAT_002bc410.m[11];
    m1.m[12] = DAT_002bc410.m[12];
    m1.m[13] = DAT_002bc410.m[13];
    m1.m[14] = DAT_002bc410.m[14];
    m1.m[15] = DAT_002bc410.m[15];
    FUN_000224b8(self->gauges[idx].scale, &m1.r[3], &self->offset);
    FUN_00022418(&m1.r[3], &m1.r[3], &self->origin);
    Vec4 v;
    v.x = 0.0f;
    v.y = 0.0f;
    v.z = 0.0f;
    m1.r[3].w = 1.0f;
    v.w = 1.0f;
    float f = self->gauges[idx].size * (1.0f - (1.0f - self->fade) * self->gauges[idx].scale);
    M4 m2 = DAT_0032b1d0;
    Mat4 t;
    FUN_00022570(&t, &m2, &m1);
    Vec4 res;
    FUN_00022548(&res, &t, &v);
    res.x = res.x / res.w;
    res.y = res.y / res.w;
    if (self->gauges[idx].kind == 6) {
        if (alt != 0)
            FUN_002033a8(f, self, idx, (self->a + self->b) / 2, &res);
        else
            FUN_00202cb8(f * self->f388 / self->gauges[idx].size / 2.0f, self, idx, (self->a + self->b) / 2, &res);
    } else {
        int n = Min(DAT_002ef1d8, self->b);
        float p = FUN_0000ce3c((float)n / 68.0f, 0.5f);
        FUN_00203680(f, self, idx, (int)(p * 128.0f), &res);
    }
}
// NEAR (lane w4f1): 132 words differ by position, 187/209 words. Everything up to the second matrix
// matches (identity copied element-wise with one lui per float, FUN_000224b8/FUN_00022418 on &m1.r[3],
// v = {0,0,0,1} sharing the 1.0f with m1[15]).
// Remaining: retail copies DAT_0032b1d0 (64 bytes) with ONE base register (`lui v0,0x33; addiu v0,-20016`)
// and 16 `lwc1 k(v0)/swc1`; every spelling tried gives either a lw/sw word loop (struct assignment of
// struct{float m[16]}, float m[4][4], Vec4 r[4], const or not, C or C++) or one lui per element
// (element-wise, Vec4 row copies, a `const float *` alias, a 4-row loop, Vec4::operator=, 16 named
// members). Once that copy is right the stack layout (m2 at sp+48, t 112, res 176, v 192, m1 208) and the
// FP temporaries ($f5 for 1.0f) probably follow; the tail (three calls, Min via const int&) looked right.
// near miss from lane w4f1 (unreviewed)
