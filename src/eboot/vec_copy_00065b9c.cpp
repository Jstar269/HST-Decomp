/* HSTMATCH module=EBOOT.BIN start=0x00065b9c end=0x00065c60 symbol=FUN_00065b9c lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */ /* matching idiom: an empty tag temporary passed by value (move v0,zero; seb a2) and the element count written as a byte difference divided by the element size, so the copy length is recomputed as in retail (cf. 0x146500). */
extern "C" void *FUN_000111c0(void *dst, const void *src, unsigned n);
struct Tag_00065b9c {};
typedef void *Elem_00065b9c;
struct Vec_00065b9c { Elem_00065b9c *data; int size; unsigned capacity; };
extern "C" void FUN_00066c44(Vec_00065b9c *v, unsigned n, Tag_00065b9c);
extern "C" Vec_00065b9c *FUN_00065b9c(Vec_00065b9c *v, const Vec_00065b9c *o)
{
    v->data = 0;
    v->size = 0;
    v->capacity = 0;
    Elem_00065b9c *first = o->data;
    Elem_00065b9c *last = first + o->size;
    int n = ((char *)last - (char *)first) / (int)sizeof(Elem_00065b9c);
    if (n != 0) {
        FUN_00066c44(v, n, Tag_00065b9c());
        FUN_000111c0(v->data + v->size, first, (last - first) * sizeof(Elem_00065b9c));
        v->size += n;
    }
    return v;
}
