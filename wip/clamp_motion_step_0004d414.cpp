/* HSTMATCH module=EBOOT.BIN start=0x0004d414 end=0x0004d6a0 symbol=FUN_0004d414 lang=c++ */
struct Vec4 {
    float x, y, z, w;
    float &operator[](int i) { return (&x)[i]; }
    const float &operator[](int i) const { return (&x)[i]; }
} __attribute__((aligned(16)));

extern "C" {
    void FUN_00022418(Vec4 *out, const Vec4 *a, const Vec4 *b);
    void FUN_00022498(float s, Vec4 *out, const Vec4 *v);
    void FUN_00022434(Vec4 *out, const Vec4 *a, const Vec4 *b);
    float FUN_000224d8(const Vec4 *a, const Vec4 *b);
    float FUN_00022e44(float x);
}

inline Vec4 operator+(const Vec4 &a, const Vec4 &b) {
    Vec4 r;
    FUN_00022418(&r, &a, &b);
    return r;
}

inline Vec4 operator*(const Vec4 &v, float s) {
    Vec4 r;
    FUN_00022498(s, &r, &v);
    return r;
}

struct Params {
    char pad[0x20];
    Vec4 dir;
    float limit;
    float push;
};

struct Body {
    Vec4 pos;
    Vec4 vel;
};

extern "C" void FUN_0004d414(Vec4 *out, Params *p, Body *b)
{
    Vec4 r;
    if (p->limit != 3.4028235e+38f) {
        if (p->push < 0.0f) {
            r = b->pos + p->dir * (p->push * b->pos.w);
        } else {
            float d = FUN_00022e44(FUN_000224d8(&b->vel, &b->vel));
            float f = p->limit;
            if (f == 0.0f || d == 0.0f) {
                r = b->pos;
            } else {
                float t = 0.005f * b->pos.w;
                float s = f - t / d;
                float m = 0.95f * f;
                s = s > m ? s : m;
                if (s <= 0.0f) {
                    r = b->pos;
                } else {
                    r = b->pos + b->vel * f;
                    for (int i = 0; i < 3; i++) {
                        if (b->vel[i] == 0.0f)
                            r[i] = b->pos[i];
                    }
                }
            }
        }
    } else {
        r = b->pos + b->vel;
    }
    r.w = 1.0f;
    out->x = r.x;
    out->y = r.y;
    out->z = r.z;
    out->w = r.w;
}
// NEAR 14 words. Remaining: (1) branch-2 scale temp at 32(sp) vs retail 16(sp), branch-3 scale temp at 16(sp) vs retail 32(sp) (swapped); (2) 0.005f constant in f2 vs retail f1, pos.w in f1 vs retail f2 (same mul operand order). Ruled out: operator definition order, w*0.005f, named temps, separate t=0.005f*w stmt.
// near miss from lane w3n1 (unreviewed)
