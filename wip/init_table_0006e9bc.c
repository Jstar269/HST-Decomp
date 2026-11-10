/* HSTMATCH module=EBOOT.BIN start=0x0006e9bc end=0x0006ea5c symbol=FUN_0006e9bc lang=c99 */
/* NEAR MISS: 40/40 words, 6 differ (lane pn4; was 7 in lane om1's wip, whose notes follow the code).
   An `int n` parameter with an unsigned-typed count prototype lets the multiply read a1 directly like retail;
   the `p->count = n` store still reads the s1 copy (retail: a1) and the copy sits before the store instead of
   just before the first jal. */
extern void *FUN_00000a90(unsigned int n);
extern void *FUN_000008d8(void *mem, void (*ctor)(void *), void (*dtor)(void *), unsigned int size, unsigned int count);
extern void FUN_0005a8ec(void *);
extern void FUN_0005aeac(void *);

struct Rec {
  unsigned int count;
  void *items;
  unsigned char *buf;
};

void FUN_0006e9bc(struct Rec *p, int n)
{
  p->count = n;
  p->items = FUN_000008d8(FUN_00000a90(n * 0x70 + 0x10), FUN_0005a8ec, FUN_0005aeac, 0x70, n);
  p->buf = (unsigned char *)FUN_00000a90(p->count);
  for (unsigned int i = 0; i < p->count; i++) {
    p->buf[i] = 0;
  }
}
// Ruled out for the late parameter copy:
//  - plain `unsigned *p` + `((unsigned char *)p[2])[i] = 0`: the loop address becomes
//    `addu v1,a0,v1` (index first) instead of retail's `addu v1,v1,a0` (base first);
//    the struct member form fixes that but not the copy.
//  - the count stored after the array creation (17 words off), the count read back
//    from memory as the 5th argument (`lw t0,0(s0)` instead of `move t0,s1`).
//  - a local copy of n used for the multiply/store/t0 argument, `n` as `int`,
//    `p->count` as the multiply operand (no store-to-load forwarding), the
//    declaration order of the local, a separate `unsigned size = n * 0x70`,
//    `0x70 * n` / `n * 112` / `(n << 4) * 7` spellings, a C++ member function,
//    `#pragma opt_propagation off` / `#pragma opt_peephole off`, and an unused
//    third parameter: all keep the copy in the prologue.
//  - MWCC splits the parameter live range only when the parameter is NOT used
//    after the first call (then no copy is emitted at all), so the 5th argument
//    has to be a register copy of n that survives the call to FUN_00000a90.
// near miss from lane om1 (unreviewed)
// lane pn4 (2026-11-10), still 7 words: also ruled out an inline NewItems(n)/NewArr(size,n)/Alloc(bytes) wrapper,
// a `cnt` copy used only for the 5th argument (assigned at declaration, after the store, or with a `self = p` copy),
// `(int)n` casts on the argument or the multiply, `short`/`unsigned long` count prototypes, and real C++
// `new C[n]` (MWCC's own new[] adds a null test and EH: 48 words; symbols not kept).
// lane pn4 (2026-11-10, later): int param + unsigned prototype = 6 words. Also ruled out with the int param: an
// int count field (with an unsigned loop cast), explicit (unsigned) casts on the store / the 5th argument / the
// multiply, an unsigned `cnt` local for the 5th argument (6-7 words).
// improved by lane pn4 (unreviewed): 7 -> 6 differing words
