/* HSTMATCH module=EBOOT.BIN start=0x00060bb8 end=0x00060e2c symbol=FUN_00060bb8 lang=c++ */
/* Clone of src/sharing_main/rotate_basis_up_00061a3c.cpp (hstclone.py tag c145620): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
/* Builds a basis from dir (up = Y unless dir is near-parallel to Y, then Z), rotates it by angle about dir (quaternion 0x1cd04 -> matrix 0x1cdb0, multiply 0x22598) and returns row 1. Mat4 is assigned (implicit trivial operator=: block copy with retail's element-8 lw/sw), not copy-constructed; matching idiom: the r3 self-assignment (m.r3 = m.r3) reproduces retail's reload/store of row 3. Names provisional. */
struct Vec4 {
    float x, y, z, w;
    Vec4() {}
    Vec4(const Vec4 &o) : x(o.x), y(o.y), z(o.z), w(o.w) {}
} __attribute__((aligned(16)));
struct Mat4 {
    Vec4 r0, r1, r2, r3;
};
struct Quat {
    float x, y, z, w;
    Quat() {}
    Quat(const Quat &o) : x(o.x), y(o.y), z(o.z), w(o.w) {}
} __attribute__((aligned(16)));

extern "C" {
float fabsf(float);
extern Mat4 DAT_002bc410;
extern Vec4 DAT_002bc420;
extern Vec4 DAT_002bc430;
Vec4 *FUN_00022518(Vec4 *out, const Vec4 *v);
Vec4 *FUN_000224f8(Vec4 *out, const Vec4 *a, const Vec4 *b);
float FUN_000224d8(const Vec4 *a, const Vec4 *b);
Mat4 *FUN_00022570(Mat4 *out, const Mat4 *a, const Mat4 *b);
Quat FUN_0001d0b8(const Vec4 *axis, float angle);
Mat4 FUN_0001d164(const Quat *q);
}

inline float Dot(const Vec4 &a, const Vec4 &b)
{
    return FUN_000224d8(&a, &b);
}

inline Vec4 Normalize(const Vec4 &v)
{
    Vec4 r;
    FUN_00022518(&r, &v);
    r.w = 0.0f;
    return r;
}

extern "C" Vec4 FUN_00060bb8(const Vec4 *dir, float angle)
{
    Mat4 m;
    m = DAT_002bc410;
    m.r2 = *dir;
    if (fabsf(Dot(Normalize(m.r2), DAT_002bc420)) <= 0.99999988f)
        m.r1 = DAT_002bc420;
    else
        m.r1 = DAT_002bc430;
    FUN_00022518(&m.r2, &m.r2);
    m.r2.w = 0.0f;
    FUN_000224f8(&m.r0, &m.r1, &m.r2);
    FUN_00022518(&m.r0, &m.r0);
    m.r0.w = 0.0f;
    FUN_000224f8(&m.r1, &m.r2, &m.r0);
    m.r3 = m.r3;
    Quat q = FUN_0001d0b8(&m.r2, angle);
    Mat4 rot = FUN_0001d164(&q);
    FUN_00022570(&m, &rot, &m);
    return m.r1;
}
