/* HSTMATCH module=EBOOT.BIN start=0x0012ca4c end=0x0012cb38 symbol=FUN_0012ca4c lang=c++ */
/* Hand-written reconstruction from the disassembly (adapted from the sibling src/eboot/vec3_reserve_copy_00115f68.cpp: Vec4 element, reserve callee 0x12cb38); exact match verified; names and types await human review. */
#pragma opt_strength_reduction off

struct Vec4 { float x, y, z, w; };

struct Vec {
    Vec4 *data;
    unsigned int size;
    unsigned int cap;
};

struct Tag { char c; };
struct Tag2 {};

extern "C" void FUN_0012cb38(Vec *v, unsigned int n, Tag tag);
extern "C" void FUN_00000a00(void *p);
static inline void Reserve(Vec *v, unsigned int n, Tag2 tag)
{
    FUN_0012cb38(v, n, Tag());
}

extern "C" void FUN_0012ca4c(Vec *self, unsigned int n)
{
    Vec tmp;
    Vec4 *src;
    Vec4 *end;
    Vec4 *dst;
    unsigned int c;
    Vec4 *d;

    tmp.data = 0;
    tmp.size = 0;
    tmp.cap = 0;
    Reserve(&tmp, n, Tag2());
    src = self->data;
    end = src + self->size;
    dst = tmp.data + tmp.size;
    for (; src < end; src++, tmp.size++, dst++) {
        if (dst) {
            *dst = *src;
        }
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
