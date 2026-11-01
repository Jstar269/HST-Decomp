/* HSTMATCH module=EBOOT.BIN start=0x001a673c end=0x001a680c symbol=FUN_001a673c lang=c99 */

typedef struct IntVec {
    int *data;
    int count;
    int cap;
} IntVec;

extern void FUN_001a864c(IntVec *dst, int n, int copy);
extern void FUN_000111c0(void *dst, const void *src, int bytes);

/* NEAR MISS 42/52 words. Residual: retail keeps first/last AND the byte length live in s1/s3/s0
   (48-byte frame) and recomputes the element count at each use; the closest form is a byte
   length divided by 4 per use, 42 words. The 3rd argument of FUN_001a864c is a signed char
   (retail does move v0,zero; seb a2,v0). Ruled out: int* difference with a count local,
   repeated last-first expressions, n*sizeof(int) size argument, and the plain pointer-diff
   forms (all 39-41 words, MWCC CSEs the scaling away). */
IntVec *FUN_001a673c(IntVec *dst, IntVec *src)
{
    int *first = src->data;
    int *last = first + src->count;
    int bytes = (char *)last - (char *)first;

    dst->data = 0;
    dst->count = 0;
    dst->cap = 0;
    if (bytes != 0) {
        FUN_001a864c(dst, bytes / 4, 0);
        FUN_000111c0(dst->data + dst->count, first, bytes);
        dst->count += bytes / 4;
    }
    return dst;
}
// near miss from lane dsn8 (unreviewed)
