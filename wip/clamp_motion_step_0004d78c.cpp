/* HSTMATCH module=hstp_trans_main.prx start=0x0004d78c end=0x0004da18 symbol=FUN_0004d78c lang=c++ */
struct Vec4 {
    float x, y, z, w;
    float &operator[](int i) { return (&x)[i]; }
    const float &operator[](int i) const { return (&x)[i]; }
} __attribute__((aligned(16)));

extern "C" {
void FUN_00022440(Vec4 *out, const Vec4 *a, const Vec4 *b);
void FUN_0002245c(Vec4 *out, const Vec4 *a, const Vec4 *b);
void FUN_000224c0(float s, Vec4 *out, const Vec4 *v);
float FUN_00022500(const Vec4 *a, const Vec4 *b);
float FUN_00022e6c(float x);
}

inline Vec4 operator*(const Vec4 &v, float s)
{
    Vec4 r;
    FUN_000224c0(s, &r, &v);
    return r;
}

inline Vec4 operator+(const Vec4 &a, const Vec4 &b)
{
    Vec4 r;
    FUN_00022440(&r, &a, &b);
    return r;
}
inline Vec4 operator-(const Vec4 &a, const Vec4 &b)
{
    Vec4 r;
    FUN_0002245c(&r, &a, &b);
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

extern "C" void FUN_0004d78c(Vec4 *out, Params *p, Body *b)
{
    Vec4 r;
    if (p->limit != 3.4028235e+38f) {
        if (p->push < 0.0f) {
            r = b->pos - p->dir * (p->push * b->pos.w);
        } else {
            float d = FUN_00022e6c(FUN_00022500(&b->vel, &b->vel));
            float f = p->limit;
            if (f == 0.0f || d == 0.0f) {
                r = b->pos;
            } else {
                float s = f - 0.005f * b->pos.w / d;
                s = s > 0.95f * f ? s : 0.95f * f;
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
// NEAR 13 words (hstmatch 163/163). Remaining: (1) the two operator* return temporaries take swapped stack
// slots (retail: branch-2 scale at 16(sp), branch-3 scale at 32(sp); ours the reverse); (2) in branch 3 the
// 0.005f constant and pos.w take f2/f1 in ours vs f1/f2 in retail (same mul operand order).
// Key findings: POD Vec4 (no user ctors), no named temporaries, explicit memberwise store to *out after r.w = 1;
// the clamp is the ternary `s = s > m ? s : m` (c.ngt + bc1tl); branch 3 scales vel by the limit f, not by s.
// Ruled out: definition order of the operators, member operators, float*Vec4 operator, inverted push test,
// named k / float locals hoisted, w*0.005f, p->limit reread, named temps (user copy ctor or POD).
// near miss from lane fs1 (unreviewed)
