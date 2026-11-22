/* HSTMATCH module=EBOOT.BIN start=0x00266d94 end=0x00266fd4 symbol=FUN_00266d94 lang=c++ */
#pragma opt_strength_reduction off

extern "C" {
extern void *DAT_00309b58;

void FUN_0005d818(void *, int);
void FUN_00000a00(void *);
void FUN_00053718(void *, int);
void FUN_0022909c(void *, void *, void *, ...);
void FUN_001466dc(void *, int);
void FUN_00269310(void *, int);
void FUN_0001f0f0(void *, int);
}

struct Alloc {};

struct Tag {
    char dummy;
    char pad[3];
};

template <class T> inline void Swap(T &a, T &b) { T t = a; a = b; b = t; }

struct Sub1 {
    virtual ~Sub1();
};

struct SubElem {
    void *vptr;
    char pad[32];      // 4..35
    Sub1 *sub1;        // 36 (0x24)
    void *sub2;        // 40 (0x28)
};

struct Vec {
    SubElem **data;
    unsigned int size;
    unsigned int cap;

    void shrink(int n, Alloc) { size -= n; }
    void clear(Alloc a = Alloc()) { shrink(size, a); }
};

struct VecVoid {
    void **data;
    unsigned int size;
    unsigned int cap;

    void shrink(int n, Alloc) { size -= n; }
    void clear(Alloc a = Alloc()) { shrink(size, a); }
};

struct Obj {
    void *vptr;
    char pad[76]; // 4..79
    Vec m_vec;    // 80 (0x50)
};

static inline void SwapVec(VecVoid &a, VecVoid &b)
{
    Swap(a.cap, b.cap);
    Swap(a.data, b.data);
    Swap(a.size, b.size);
}
extern "C" {
extern VecVoid DAT_0034a7d4;
}

extern "C" void *FUN_00266d94(Obj *self, short param_2) {
    if (self != 0) {
        self->vptr = &DAT_00309b58;

        unsigned int i = 0;
        if (self->m_vec.size != 0) {
            do {
                if (self->m_vec.data[i]->sub1 != 0) {
                    delete self->m_vec.data[i]->sub1;
                    self->m_vec.data[i]->sub1 = 0;
                }
                if (self->m_vec.data[i]->sub2 != 0) {
                    FUN_0005d818(self->m_vec.data[i]->sub2, 1);
                    self->m_vec.data[i]->sub2 = 0;
                }
                if (self->m_vec.data[i] != 0) {
                    delete self->m_vec.data[i];
                    self->m_vec.data[i] = 0;
                }
                i++;
            } while (i < self->m_vec.size);
        }

        self->m_vec.clear();

        i = 0;
        if (DAT_0034a7d4.size != 0) {
            do {
                if (DAT_0034a7d4.data[i] != 0) {
                    FUN_00053718(DAT_0034a7d4.data[i], 1);
                    DAT_0034a7d4.data[i] = 0;
                }
                i++;
            } while (i < DAT_0034a7d4.size);
        }

        DAT_0034a7d4.clear();

        VecVoid temp;
        Tag tag;
        temp.data = 0;
        temp.size = 0;
        temp.cap = 0;
        FUN_0022909c(&temp, DAT_0034a7d4.data, DAT_0034a7d4.data + DAT_0034a7d4.size, tag.dummy);
        Swap(temp.cap, DAT_0034a7d4.cap);
        Swap(temp.data, DAT_0034a7d4.data);
        Swap(temp.size, DAT_0034a7d4.size);
        FUN_001466dc(&temp, 0);

        Vec *v = &self->m_vec;
        if (v != 0) {
            if (v != 0) {
                FUN_00269310(v, 0);
            }
        }

        FUN_0001f0f0(self, 0);

        if (param_2 > 0) {
            FUN_00000a00(self);
        }
    }
    return self;
}

// Differing words: 25 / 144 (119/144 match).
// Size: 576 bytes / 144 words (exact size match).
// Frame: 64 bytes, saved registers s0..s3, ra match bit-for-bit.
// First loop (self->m_vec element destruction): 100% exact match.
// Epilogue & deletion: 100% exact match.
// Remaining 25 words differ in the shrink-to-fit swap sequence for DAT_0034a7d4.
// near miss from lane ag4 (unreviewed)
// NEAR, 10 words differ (matcher: 144/144 words; 10 differ; first at +0x158), lane api-hste2-20261121 (was 25). Found: an inline `SwapVec(a, b)`-style swap written as
// Swap(temp.cap, DAT.cap); Swap(temp.data, DAT.data); Swap(temp.size, DAT.size) (TEMP FIRST) fixes the swap block (25 -> 11); the inlined dtor's second null test is on the member
// pointer (`if (v != 0) { if (v != 0) FUN_00269310(v, 0); }`, retail `beqz a0` twice) -> 10. Left: the DAT clear block (retail: size loaded, tag `lb a3`, `addiu a0,sp,52`, `subu v1,v1,v1; sw`,
// THEN `lui v0; lw a1,data` after the store; ours hoists the data load above the store). Tried: pointer-local clear(), shrink(size, Alloc()) direct, `int n` local (15).
// near miss from lane api-hste2-20261121 (unreviewed)
