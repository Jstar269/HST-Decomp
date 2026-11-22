/* HSTMATCH module=EBOOT.BIN start=0x001a673c end=0x001a680c symbol=FUN_001a673c lang=c++ */
/* Hand-written reconstruction from the disassembly (wip near miss vec_assign_001a673c as hint); exact match verified; matching idiom: `int n = bytes / 4;` for the count but `(long)bytes / (long)4` for the size update and `(last - first) * 4` for the copy length (MWCC CSEs all three otherwise); the empty by-value `False` tag is the grow call's 3rd argument; names and types await human review. */
struct False {};
struct IntVec {
    int *data;
    int count;
    int cap;
};
extern "C" void FUN_001a864c(IntVec *dst, int n, False);
extern "C" void FUN_000111c0(void *dst, const void *src, int bytes);

extern "C" IntVec *FUN_001a673c(IntVec *dst, IntVec *src)
{
    dst->data = 0;
    dst->count = 0;
    dst->cap = 0;
    int *first = src->data;
    int *last = first + src->count;
    int bytes = (char *)last - (char *)first;
    int n = bytes / 4;
    if (n != 0) {
        FUN_001a864c(dst, n, False());
        FUN_000111c0(dst->data + dst->count, first, (last - first) * 4);
        dst->count += (long)bytes / (long)4;
    }
    return dst;
}
