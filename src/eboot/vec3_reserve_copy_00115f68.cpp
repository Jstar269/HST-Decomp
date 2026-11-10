/* HSTMATCH module=EBOOT.BIN start=0x00115f68 end=0x0011605c symbol=FUN_00115f68 lang=c++ */
/* Hand-written reconstruction (external matching lane); exact match verified; names and types await human review. */
/* Ghidra pseudocode (wip/vec3_reserve_copy_00115f68.cpp) corrected by hand: fixed tail swap/clear order and tmp.size reset as c - (int)c; exact match verified; names and types await human review. */
#pragma opt_strength_reduction off

struct Vec3 { float axis[3]; };

struct Vec {
    Vec3 *data;
    unsigned int size;
    unsigned int cap;
};

struct Tag { char c; };
struct Tag2 {};

extern "C" void FUN_0011676c(Vec *v, unsigned int n, Tag tag);
extern "C" void FUN_00000a00(void *p);
static inline void Reserve(Vec *v, unsigned int n, Tag2 tag)
{
    FUN_0011676c(v, n, Tag());
}

extern "C" void FUN_00115f68(Vec *self, unsigned int n)
{
    Vec tmp;
    Vec3 *src;
    Vec3 *end;
    Vec3 *dst;
    unsigned int c;
    Vec3 *d;

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
