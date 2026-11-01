/* HSTMATCH module=EBOOT.BIN start=0x000007f4 end=0x000008d8 symbol=FUN_000007f4 lang=c99 */

typedef void (*ElemFn)(char *elem, int arg);

/* NEAR MISS 50/57 words. Residual: retail spills p/ctor/dtor/stride/count/i to fixed stack
   slots (44..60(sp), 64-byte frame, 4 s-regs) and reloads them across the two indirect
   calls; this form keeps them in registers (32-byte frame, 6 s-regs). The two loops are a
   construct loop calling ctor(elem, 1) and a destroy loop calling dtor(elem, -1) whose guard
   is i < count (unreachable from the construct loop, so the source has some other counter
   relationship). Ruled out: do-while over an advancing pointer (43 words), the for + pointer
   destroy form (50 words), no dtor test. */
void FUN_000007f4(char *p, ElemFn ctor, ElemFn dtor, int stride, int count)
{
    int i;
    char *e;

    for (i = 0; i < count; i++) {
        ctor(p + i * stride, 1);
    }
    if (i < count) {
        if (dtor != 0) {
            e = p + i * stride;
            while (i != 0) {
                e -= stride;
                dtor(e, -1);
                i--;
            }
        }
    }
}
// near miss from lane dsn8 (unreviewed)
