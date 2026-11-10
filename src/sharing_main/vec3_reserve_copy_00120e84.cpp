/* HSTMATCH module=hstp_trans_main.prx start=0x00120e84 end=0x00120f78 symbol=FUN_00120e84 lang=c++ */
/* Clone of src/eboot/vec3_reserve_copy_00115f68.cpp (hstclone.py tag c211507): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction (external matching lane); exact match verified; names and types await human review. */
/* Ghidra pseudocode (wip/vec3_reserve_copy_00120e84.cpp) corrected by hand: fixed tail swap/clear order and tmp.size reset as c - (int)c; exact match verified; names and types await human review. */
#pragma opt_strength_reduction off

struct Vec3 { float axis[3]; };

struct Vec {
    Vec3 *data;
    unsigned int size;
    unsigned int cap;
};

struct Tag { char c; };
struct Tag2 {};

extern "C" void FUN_00121688(Vec *v, unsigned int n, Tag tag);
extern "C" void FUN_00000a00(void *p);
static inline void Reserve(Vec *v, unsigned int n, Tag2 tag)
{
    FUN_00121688(v, n, Tag());
}

extern "C" void FUN_00120e84(Vec *self, unsigned int n)
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
