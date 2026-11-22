/* HSTMATCH module=EBOOT.BIN start=0x00237330 end=0x00237444 symbol=FUN_00237330 lang=c++ */
/* Hand-written reconstruction from the disassembly (adapted from the sibling src/eboot/vec3_reserve_copy_00115f68.cpp: 28-byte element with a user copy ctor copied by placement new, reserve callee 0x238608); exact match verified; names and types await human review. */
#pragma opt_strength_reduction off

struct Elem {
    int f[7];
    Elem() {}
    Elem(const Elem &o) {
        f[0] = o.f[0]; f[1] = o.f[1]; f[2] = o.f[2]; f[3] = o.f[3];
        f[4] = o.f[4]; f[5] = o.f[5]; f[6] = o.f[6];
    }
};
inline void *operator new(unsigned, void *p) { return p; }

struct Vec {
    Elem *data;
    unsigned int size;
    unsigned int cap;
};

struct Tag { char c; };
struct Tag2 {};

extern "C" void FUN_00238608(Vec *v, unsigned int n, Tag tag);
extern "C" void FUN_00000a00(void *p);
static inline void Reserve(Vec *v, unsigned int n, Tag2 tag)
{
    FUN_00238608(v, n, Tag());
}

extern "C" void FUN_00237330(Vec *self, unsigned int n)
{
    Vec tmp;
    Elem *src;
    Elem *end;
    Elem *dst;
    unsigned int c;
    Elem *d;

    tmp.data = 0;
    tmp.size = 0;
    tmp.cap = 0;
    Reserve(&tmp, n, Tag2());
    src = self->data;
    end = src + self->size;
    dst = tmp.data + tmp.size;
    for (; src < end; src++, tmp.size++, dst++) {
        new (dst) Elem(*src);
    }
    c = self->cap;
    self->cap = tmp.cap;
    tmp.cap = c;
    d = self->data;
    self->data = tmp.data;
    tmp.data = d;
    c = self->size;
    self->size = tmp.size;
    tmp.size = c - (int)c;
    if (tmp.data)
        FUN_00000a00(tmp.data);
}
