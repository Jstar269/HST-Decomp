/* HSTMATCH module=EBOOT.BIN start=0x000e863c end=0x000e8744 symbol=FUN_000e863c lang=c++ */

inline void *operator new(unsigned, void *p) { return p; }
extern "C" void FUN_0005b9d0(void *dst, void *src);
struct Elem {
    int id;
    char gap0[12];
    float v[5];
    char gap1[12];
    char tail[704];
    Elem(const Elem &o) : id(o.id)
    {
        v[0] = o.v[0]; v[1] = o.v[1]; v[2] = o.v[2]; v[3] = o.v[3]; v[4] = o.v[4];
        FUN_0005b9d0(tail, (void *)(o.tail));
    }
};
struct Vec { Elem *data; unsigned size; unsigned capacity; };
struct Greater { bool operator()(const unsigned &a, const unsigned &b) const { return a > b; } };

extern "C" {
void FUN_000e8f90(void *v, unsigned need);
void FUN_000e8cc4(Vec *v, unsigned n, const Elem &x);
}

extern "C" void FUN_000e863c(Vec *v, unsigned n, const Elem &x)
{
    unsigned cap = v->capacity;
    if (Greater()(n, cap) || cap - n < v->size) {
        FUN_000e8f90(v, v->size + n - cap);
    } else {
        Elem *p = v->data + v->size;
        for (; n != 0; --n) {
            new (p) Elem(x);
            ++p;
            ++v->size;
        }
        return;
    }
    FUN_000e8cc4(v, n, x);
}

// 68/66 words; 46 differ (first difference at +0x3c).
// Entire loop body and prologue/epilogue match retail byte-for-byte; 2 words extra due to MWCC duplicating the delay slot instructions sll v1, a0, 1 (at 0xe869c) and move a0, s2 (at 0xe8720) when filling beqzl and b delay slots across the in-place copy loop with incremental ++v->size.
// Same pattern as FUN_00091090.
// near miss from lane dsn4 (unreviewed)
