/* HSTMATCH module=EBOOT.BIN start=0x001f483c end=0x001f4900 symbol=FUN_001f483c lang=c++ */
extern int DAT_003313c4;
extern "C" void FUN_00049774(void);
extern "C" void FUN_00064194(int, void *);

struct Inner {
    char pad0[8];
    float val;
};

struct Elem {
    Inner **inner;
    char pad[12];
    float val;
    char pad2[32];
};

struct Container {
    char pad0[344];
    int start;
    int end;
    int End() const { return end; }
};

struct Obj {
    char pad0[960];
    Container **container_ptr;
};

static inline bool AllOk(Container *c)
{
    int i = c->start;
    if (i <= c->End()) {
        Elem *e = (Elem *)((char *)c + 244) + i;
        do {
            float limit = 0.0f;
            if (e->inner != 0)
                limit = (*e->inner)->val;
            if (e->val < limit)
                return false;
            i++;
            e++;
        } while (i <= c->End());
    }
    return true;
}

extern "C" void FUN_001f483c(Obj *self)
{
    if (self->container_ptr != 0) {
        if (!AllOk(*self->container_ptr))
            FUN_00049774();
        FUN_00064194(DAT_003313c4, self->container_ptr);
    }
}
// NEAR 22 differ (api-hste1): retail keeps the container ptr in a0 across the null test (move s0,a0; lw a0,960(a0); beqzl a0 with lw ra in the slot) and
// colours loop counter a2 / element a1; ours v1 / a1,a0. Element address is c+i*52 with 244/260 displacements NOT folded (use (char*)c + i*52 cast: 48 words, folds right, but outer branch loses likely slot).
// Tried: Container** helper param, local cp, const getter Ptr(), open-coded loop, early return: all 45-46 differ.
// improved by lane api-hste1-20261121 (unreviewed): 36 -> 22 differing words
