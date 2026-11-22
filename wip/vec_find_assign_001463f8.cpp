/* HSTMATCH module=EBOOT.BIN start=0x001463f8 end=0x0014648c symbol=FUN_001463f8 lang=c++ */
#pragma opt_strength_reduction off
struct False {};
struct Obj { int pad[4]; int id; };
struct Vec { Obj **data; unsigned size; unsigned cap; };
struct Holder {
    char pad[0x7bc];
    Vec *items;
    volatile unsigned count;
};
extern "C" void FUN_001465ac(Vec *, Obj **, Obj **, False);

extern "C" int FUN_001463f8(Holder *h, int id, Vec *dst)
{
    for (unsigned i = 0; i < h->count; i++) {
        Vec *e = &h->items[i];
        if (id == (*e->data)->id) {
            if (dst != e) {
                FUN_001465ac(dst, e->data, e->data + e->size, False());
            }
            return 1;
        }
    }
    return 0;
}
// NEAR, 13 words differ (matcher: 37/37 words; 13 differ; first at +0x10), lane api-hste2-20261121 (was 35). Found: C++ with `struct Vec {Obj **data; unsigned size, cap;}` elements, callee typed
// (Vec *, Obj **, Obj **, False) (the empty by-value `False` tag gives `move v0,zero; seb a3,v0` and the 32-byte frame), `#pragma opt_strength_reduction off` (i*12 recomputed), and
// `volatile unsigned count` in the holder (retail reloads the bound in the bnel slot every pass; a const getter does not, a plain member is hoisted). Left: retail loads e->data a second time
// for the call (`lw a1,0(v1)` after the dst==e test) and uses t0 for i; ours CSEs the test's load into t0 (casts to an identical struct, inline Front()/Begin() getters did not split it).
// near miss from lane api-hste2-20261121 (unreviewed)
