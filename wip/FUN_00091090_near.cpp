/* HSTMATCH module=EBOOT.BIN start=0x00091090 end=0x0009117c symbol=FUN_00091090 lang=c++ */

inline void *operator new(unsigned, void *p) { return p; }
struct V4 { float x, y, z, w; V4(const V4 &o) : x(o.x), y(o.y), z(o.z), w(o.w) {} } __attribute__((aligned(16)));
struct Elem { int a; unsigned char b; int c; V4 v; };
struct Alloc {
    void construct(Elem *p, const Elem &x) { new (p) Elem(x); }
};
struct Vec { Elem *data; unsigned size; unsigned capacity; };
struct Greater { bool operator()(const unsigned &a, const unsigned &b) const { return a > b; } };

extern "C" {
void FUN_0009156c(void *v, unsigned need);
void FUN_000912e8(void *v, unsigned n, const Elem &x);
}

extern "C" void FUN_00091090(Vec *v, unsigned n, const Elem &x)
{
    unsigned cap = v->capacity;
    if (Greater()(n, cap) || cap - n < v->size) {
        FUN_0009156c(v, v->size + n - cap);
    } else {
        Elem *p = v->data + v->size;
        for (unsigned i = n; i != 0; --i) {
            new (p) Elem(x);
            ++p;
        }
        v->size += n;
        return;
    }
    FUN_000912e8(v, n, x);
}

// 26 words differ in the in-place copy loop (retail increments v->size at 4(s2) and decrements n at s1 on each iteration; putting ++v->size inside the loop causes MWCC to fail to delete hoisted delay-slot instructions lw v1, 0(s2) and move a0, s2).
// Ruled out: hoisting data before the if, putting append call in if vs at end, loop-invariant pragmas, inline member helper.
// near miss from lane dsn4 (unreviewed)
