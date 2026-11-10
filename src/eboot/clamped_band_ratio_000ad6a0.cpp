/* HSTMATCH module=EBOOT.BIN start=0x000ad6a0 end=0x000ada80 symbol=FUN_000ad6a0 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern "C" {
extern float DAT_002d46f8;
extern float DAT_002d46fc;
extern float DAT_002d4700;
extern float DAT_002d4704;
extern float DAT_002d4708;
extern float DAT_003411fc;
extern float DAT_00341200;
extern float DAT_0034126c;
extern float DAT_00341270;
extern float DAT_00341274;
extern float DAT_00341278;
extern float DAT_0034127c;
extern float DAT_00341280;
extern float DAT_003414e8;
extern float DAT_003414ec;
extern float DAT_003414f0;
struct Ctx {
    char pad00[0x24];
    float f24;
    char pad28[0x14];
    float f3c;
};
struct Obj {
    char pad00[0x154];
    int f154;
};
struct Pt {
    float x;
    float y;
};
extern Ctx *FUN_000b3c68(void *obj);
extern float FUN_00163f58(Ctx *, int);
}

static inline float Clamp(const float &v, const float &lo, const float &hi)
{
    return v < lo ? lo : (v > hi ? hi : v);
}

extern "C" float FUN_000ad6a0(Obj *self, float *a, float *b, float *lo, float *hi, int kind, int mode, Pt *p)
{
    float scale = 1.0f;
    if (mode == 1 || mode == 2 || mode == 10) {
        *a = -(0.01f * FUN_000b3c68(self)->f24);
        *b = -(0.01f * FUN_000b3c68(self)->f3c);
    } else {
        *a = -0.0f;
        *b = -(0.01f * FUN_000b3c68(self)->f24);
    }
    if (mode == 1) {
        float w = 0.0f;
        float r = FUN_00163f58(FUN_000b3c68(self), self->f154);
        switch (kind) {
        case 0:
            w = (DAT_0034126c + DAT_00341278 * r) * 0.01f;
            scale = DAT_003414e8 * 0.01f;
            break;
        case 1:
            w = (DAT_00341270 + DAT_0034127c * r) * 0.01f;
            scale = DAT_003414ec * 0.01f;
            break;
        case 2:
            w = (DAT_00341274 + DAT_00341280 * r) * 0.01f;
            scale = DAT_003414f0 * 0.01f;
            break;
        }
        float m = *a + DAT_002d46f8 * (*b - *a);
        *lo = m + 0.5f * w;
        *hi = m - 0.5f * w;
        *lo = *lo > *a ? *a : *lo;
        *hi = *hi < *b ? *b : *hi;
    } else {
        float k = DAT_00341200 * 0.01f;
        *lo = *a + DAT_003411fc * 0.01f * (*b - *a);
        *hi = *b + k * (*a - *b);
    }
    float v = p->y;
    if (*lo >= v && v >= *hi)
        return 0.0f;
    if (v <= *hi)
        return Clamp((v - *hi) / (scale * (*b - *hi)), DAT_002d46fc, DAT_002d4700);
    float r = Clamp((v - *lo) / (scale * (*a - *lo)), DAT_002d4704, DAT_002d4708);
    r *= -1.0f;
    return r;
}
