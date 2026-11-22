/* HSTMATCH module=EBOOT.BIN start=0x000e8810 end=0x000e88e4 symbol=FUN_000e8810 lang=c++ */
/* Hand-written reconstruction (external matching lane); exact match verified; names and types await human review. */
/* Ghidra-free reconstruction from the disassembly (starting from a near-miss draft of lane lnm2), restructured by hand; exact match verified; matching idiom: the out-of-line fill call is sunk below the else block and the else path skips it through a goto (same as vec_fill16_000d565c.cpp); names and types await human review. */
inline void *operator new(unsigned, void *p) { return p; }
struct Tag_000ed8e4 {};
struct Elem_000ed8e4 {
    float x, y, z, w;
    Elem_000ed8e4(const Elem_000ed8e4 &o) : x(o.x), y(o.y), z(o.z), w(o.w) {}
};
struct Vec_000ed8e4 {
    Elem_000ed8e4 *data;
    unsigned size;
    unsigned cap;
};

extern "C" void FUN_000e92c8(Vec_000ed8e4 *, unsigned);
extern "C" void FUN_000e9074(Vec_000ed8e4 *, unsigned, const Elem_000ed8e4 *);

inline void Fill(Elem_000ed8e4 *p, unsigned m, const Elem_000ed8e4 *x, Vec_000ed8e4 *v, Tag_000ed8e4)
{
    for (; m != 0; --m) {
        if (p) {
            p->x = x->x;
            p->y = x->y;
            p->z = x->z;
            p->w = x->w;
        }
        ++p;
        ++v->size;
    }
}

extern "C" void FUN_000e8810(Vec_000ed8e4 *v, unsigned n, const Elem_000ed8e4 *x)
{
    if (n > v->cap || v->size > v->cap - n) {
        FUN_000e92c8(v, (v->size + n) - v->cap);
    } else {
        Fill(v->data + v->size, n, x, v, Tag_000ed8e4());
        goto done;
    }
    FUN_000e9074(v, n, x);
done:;
}
