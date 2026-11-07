/* HSTMATCH module=EBOOT.BIN start=0x00146b88 end=0x00146c4c symbol=FUN_00146b88 lang=c++ */
/* Hand-written reconstruction from the disassembly, adapted from the twin src/eboot/vec_copy_00065b9c.cpp in the same module: addresses substituted; exact match verified; names and types await human review. */ /* matching idiom: an empty tag temporary passed by value and the element count written as a byte difference divided by the element size (cf. 0x146500). */
extern "C" void *FUN_000111c0(void *dst, const void *src, unsigned n);
struct Tag_00065b9c {};
typedef void *Elem_00065b9c;
struct Vec_00065b9c { Elem_00065b9c *data; int size; unsigned capacity; };
extern "C" void FUN_0014a86c(Vec_00065b9c *v, unsigned n, Tag_00065b9c);
extern "C" Vec_00065b9c *FUN_00146b88(Vec_00065b9c *v, const Vec_00065b9c *o)
{
    v->data = 0;
    v->size = 0;
    v->capacity = 0;
    Elem_00065b9c *first = o->data;
    Elem_00065b9c *last = first + o->size;
    int n = ((char *)last - (char *)first) / (int)sizeof(Elem_00065b9c);
    if (n != 0) {
        FUN_0014a86c(v, n, Tag_00065b9c());
        FUN_000111c0(v->data + v->size, first, (last - first) * sizeof(Elem_00065b9c));
        v->size += n;
    }
    return v;
}
