/* HSTMATCH module=EBOOT.BIN start=0x000d5948 end=0x000d5a84 symbol=FUN_000d5948 lang=c++ */
/* Hand-written reconstruction from the disassembly (wip near miss FUN_000d5948 as hint); exact match verified; matching idiom: Vec3 as `float v[3]` so the struct copy is retail's batched lwc1 x3 / swc1 x3, a named `False tag;` for the first callee (`lb t1,44(sp)`), and the vector-copy recipe of 0x001a673c; names and types await human review. */
struct False {};
struct IntVec { int *data; int size; int cap; };
struct Vec3 { float v[3]; };
struct Buf {
    Vec3 p;
    IntVec v;
};
extern "C" void FUN_00003ed4(Buf *, int);
extern "C" void FUN_00003370(Buf *, int, int, int, int, False);
extern "C" void FUN_000c4450(IntVec *, int, False);
extern "C" void FUN_000111c0(int *, int *, int);

extern "C" Buf *FUN_000d5948(Buf *self, Buf *src)
{
    False tag;
    if ((*(int *)&src->p.v[0] & 1) == 0) {
        self->p = src->p;
    } else {
        self->p.v[0] = 0.0f;
        self->p.v[1] = 0.0f;
        self->p.v[2] = 0.0f;
        FUN_00003ed4(self, *(int *)&src->p.v[1]);
        FUN_00003370(self, 0, 0, *(int *)&src->p.v[2], *(int *)&src->p.v[2] + *(int *)&src->p.v[1], tag);
    }
    IntVec *dst = &self->v;
    dst->data = 0;
    dst->size = 0;
    dst->cap = 0;
    int *first = src->v.data;
    int *last = first + src->v.size;
    int bytes = (char *)last - (char *)first;
    int n = bytes / 4;
    if (n != 0) {
        FUN_000c4450(dst, n, False());
        FUN_000111c0(dst->data + dst->size, first, (last - first) * 4);
        dst->size += (long)bytes / (long)4;
    }
    return self;
}
