/* HSTMATCH module=EBOOT.BIN start=0x000e8468 end=0x000e855c symbol=FUN_000e8468 lang=c++ */
/* Hand-written reconstruction from the disassembly, by the recipe of 0x00091090; exact match verified; matching idiom: `goto done` after the in-place fill loop; names and types await human review. */
inline void *operator new(unsigned, void *p) { return p; }
struct Elem_000e8468 {
    float a, b, c, d, e, f, g, h;
    Elem_000e8468(const Elem_000e8468 &o) : a(o.a), b(o.b), c(o.c), d(o.d), e(o.e), f(o.f), g(o.g), h(o.h) {}
};
struct Tag_000e8468 {};
struct Vec_000e8468 { Elem_000e8468 *data; unsigned size; unsigned capacity; };
extern "C" unsigned FUN_000e8be0(Vec_000e8468 *v, unsigned n);
extern "C" void FUN_000e894c(Vec_000e8468 *v, unsigned n, const Elem_000e8468 *x);
extern "C" void FUN_000e8468(Vec_000e8468 *v, unsigned n, const Elem_000e8468 *x, Tag_000e8468)
{
    unsigned cap = v->capacity;
    if (n > cap || cap - n < v->size) {
        FUN_000e8be0(v, v->size + n - cap);
    } else {
        Elem_000e8468 *p = v->data + v->size;
        for (; n != 0; --n) {
            new (p) Elem_000e8468(*x);
            ++p;
            ++v->size;
        }
        goto done;
    }
    FUN_000e894c(v, n, x);
done:;
}
