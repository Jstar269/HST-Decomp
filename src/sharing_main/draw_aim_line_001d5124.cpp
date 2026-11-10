/* HSTMATCH module=hstp_trans_main.prx start=0x001d5124 end=0x001d5398 symbol=FUN_001d5124 lang=c++ */
/* Clone of src/eboot/draw_aim_line_001c6cfc.cpp (hstclone.py tag c211507): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly (starting from the lane ol1 near miss in wip/); exact match verified; matching idiom: FUN_00022440 and FUN_00048bfc declared both as sret and as pointer-out functions; names and types await human review. */
struct Tag {};

struct Vec4 {
    float x, y, z, w;
} __attribute__((aligned(16)));

extern "C" {
Vec4 *FUN_0002245c(Vec4 *out, const Vec4 *a, const Vec4 *b);
Vec4 *FUN_000224c0(Vec4 *out, const Vec4 *v, float s);
Vec4 FUN_00048bfc(const Vec4 &v);
}

/* FUN_00022440 returns its sum by value (hidden result pointer in a0); FUN_00048bfc is used in both forms. */
namespace sret {
extern "C" Vec4 FUN_00022440(const Vec4 &a, const Vec4 &b);
}
inline Vec4 operator-(const Vec4 &a, const Vec4 &b)
{
    Vec4 r;
    FUN_0002245c(&r, &a, &b);
    return r;
}
inline Vec4 operator+(const Vec4 &a, const Vec4 &b)
{
    return Vec4(sret::FUN_00022440(a, b));
}
inline Vec4 operator*(float s, const Vec4 &v)
{
    Vec4 r;
    FUN_000224c0(&r, &v, s);
    return r;
}

namespace raw {
extern "C" void FUN_00048bfc(Vec4 *out, const Vec4 *v);
}

struct Body {
    char pad[0x20];
    Vec4 dir;
    Vec4 pos;
};

struct Drawer {
    virtual void m0();
    virtual void m1();
    virtual void m2();
    virtual void m3();
    virtual void m4();
    virtual void m5();
    virtual void m6();
    virtual void m7();
    virtual void m8();
    virtual void m9();
    virtual void m10();
    virtual void m11();
    virtual void m12();
    virtual void m13();
    virtual void m14();
    virtual void m15();
    virtual void m16();
    virtual void m17();
    virtual void m18();
    virtual void m19();
    virtual void m20();
    virtual void m21();
    virtual void m22();
    virtual void m23();
    virtual void m24();
    virtual void m25();
    virtual void m26();
    virtual void m27();
    virtual void m28();
    virtual void m29();
    virtual void m30();
    virtual void m31();
    virtual void m32();
    virtual void m33();
    virtual void m34();
    virtual void m35();
    virtual void m36();
    virtual void m37();
    virtual void m38();
    virtual void m39();
    virtual void DrawLines(Vec4 *data, int count, int flag); // offset 0xa8
};

extern "C" {
extern Drawer *DAT_003f3450;
Body *FUN_0009573c(Drawer *);
int FUN_001d94ec(void *g, int p, Vec4 *out, Vec4 *buf);
void FUN_00137e74(void *vec, const Vec4 *val, Tag);
void FUN_000ed9b8(void *vec, int flag);
}

struct VectorVec4 {
    Vec4 *data;
    unsigned int size;
    unsigned int capacity;

    void push_back(const Vec4 &val)
    {
        if (size < capacity) {
            Vec4 *dst = &data[size];
            if (dst != 0)
                *dst = val;
            size++;
        } else {
            FUN_00137e74(this, &val, Tag());
        }
    }
};

extern "C" void FUN_001d5124(void *g, float len, int aim)
{
    if (DAT_003f3450 != 0) {
        Body *b = FUN_0009573c(DAT_003f3450);
        Vec4 dir, hit, aux;
        raw::FUN_00048bfc(&dir, &b->dir);
        if (aim != 0) {
            if (FUN_001d94ec(g, 0, &hit, &aux) != 0) {
                dir = FUN_00048bfc(hit - b->pos);
            } else {
                dir.w = -dir.w;
                dir.z = -dir.z;
                dir.y = -dir.y;
                dir.x = -dir.x;
            }
        }
        VectorVec4 vec;
        vec.data = 0;
        vec.size = 0;
        vec.capacity = 0;
        vec.push_back(b->pos);
        vec.push_back(b->pos + len * dir);
        DAT_003f3450->DrawLines(vec.data, 2, 0);
        FUN_000ed9b8(&vec, 0);
    }
}
