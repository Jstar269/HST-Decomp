/* HSTMATCH module=EBOOT.BIN start=0x000d57a8 end=0x000d589c symbol=FUN_000d57a8 lang=c++ */
/* Hand-written reconstruction from the disassembly (starting from a near-miss draft of lane dsn3), restructured by hand; exact match verified; matching idiom: the out-of-line fill call is sunk below the else block and the else path skips it through a goto (as vec_fill16_000d565c.cpp); names and types await human review. */
inline void *operator new(unsigned, void *p) { return p; }
struct Elem8 {
    int w[8];
    Elem8(const Elem8 &o) {
        w[0] = o.w[0];
        w[1] = o.w[1];
        w[2] = o.w[2];
        w[3] = o.w[3];
        w[4] = o.w[4];
        w[5] = o.w[5];
        w[6] = o.w[6];
        w[7] = o.w[7];
    }
};
struct False_000d57a8 {};
struct Vec_000d57a8 {
    Elem8 *data;
    unsigned size;
    unsigned capacity;
    void construct_at_end(unsigned n, const Elem8 &x, False_000d57a8) {
        Elem8 *p = data + size;
        for (; n != 0; --n) {
            new (p) Elem8(x);
            ++p;
            ++size;
        }
    }
};
extern "C" unsigned FUN_000d67c8(Vec_000d57a8 *v, unsigned need);
extern "C" void FUN_000d6534(Vec_000d57a8 *v, unsigned n, const Elem8 &x);
extern "C" void FUN_000d57a8(Vec_000d57a8 *v, unsigned n, const Elem8 &x)
{
    if (n > v->capacity || v->size > v->capacity - n) {
        FUN_000d67c8(v, v->size + n - v->capacity);
    } else {
        v->construct_at_end(n, x, False_000d57a8());
        goto done;
    }
    FUN_000d6534(v, n, x);
done:;
}
