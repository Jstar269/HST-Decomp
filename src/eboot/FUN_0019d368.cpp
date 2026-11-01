/* HSTMATCH module=EBOOT.BIN start=0x0019d368 end=0x0019d474 symbol=FUN_0019d368 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

extern "C" unsigned int FUN_0001342c(void);

struct Tag {};

extern "C" unsigned int FUN_0019d368(void *self, unsigned int n, Tag, Tag, Tag, Tag, Tag)
{
    switch (n) {
    case 0: {
        unsigned int hi = FUN_0001342c() >> 30;
        unsigned int lo = FUN_0001342c() << 1;
        return lo | hi;
    }
    case 1:
        return 0;
    default:
        break;
    }

    if (n < 0x80000001U) {
        unsigned int q = 0x80000000U / n;
        unsigned int lim = q * n;
        unsigned int r;
        do {
            r = FUN_0001342c();
        } while (r >= lim);
        return r / q;
    } else {
        unsigned int q;
        if (n & 1) {
            q = 0xffffffffU / n;
        } else {
            q = 0x80000000U / (n >> 1);
        }
        unsigned int lim = q * n;
        unsigned int r;
        do {
            unsigned int hi = FUN_0001342c() >> 30;
            unsigned int lo = FUN_0001342c() << 1;
            r = lo | hi;
        } while (r >= lim);
        return r / q;
    }
}
