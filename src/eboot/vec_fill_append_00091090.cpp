/* HSTMATCH module=EBOOT.BIN start=0x00091090 end=0x0009117c symbol=FUN_00091090 lang=c++ */
/* Hand-written reconstruction from the disassembly (wip near miss as hint); exact match verified; matching idiom: `goto done` after the in-place fill loop (a plain return gives 40 words off); the unused trailing empty Tag parameter gives retail's 32-byte frame; names and types await human review. */
inline void *operator new(unsigned, void *p) { return p; }
struct V4 { float x, y, z, w; V4(const V4 &o) : x(o.x), y(o.y), z(o.z), w(o.w) {} } __attribute__((aligned(16)));
struct Elem_00091090 { int a; unsigned char b; int c; V4 v; };
struct Tag_00091090 {};
struct Vec_00091090 { Elem_00091090 *data; unsigned size; unsigned capacity; };
extern "C" unsigned FUN_0009156c(Vec_00091090 *v, unsigned n);
extern "C" void FUN_000912e8(Vec_00091090 *v, unsigned n, const Elem_00091090 *x);
extern "C" void FUN_00091090(Vec_00091090 *v, unsigned n, const Elem_00091090 *x, Tag_00091090)
{
    unsigned cap = v->capacity;
    if (n > cap || cap - n < v->size) {
        FUN_0009156c(v, v->size + n - cap);
    } else {
        Elem_00091090 *p = v->data + v->size;
        for (; n != 0; --n) {
            new (p) Elem_00091090(*x);
            ++p;
            ++v->size;
        }
        goto done;
    }
    FUN_000912e8(v, n, x);
done:;
}
