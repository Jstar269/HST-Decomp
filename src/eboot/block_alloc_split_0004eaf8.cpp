/* HSTMATCH module=EBOOT.BIN start=0x0004eaf8 end=0x0004ecb0 symbol=FUN_0004eaf8 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Block;
extern Block *DAT_0032b234;
extern "C" unsigned FUN_00010184(int, unsigned);
void *operator new(unsigned);
struct Block {
    Block *prev;
    Block *next;
    unsigned char avail;
    unsigned start;
    unsigned end;
    Block(Block *p, unsigned at, unsigned char f) {
        unsigned e = p->end;
        Block *n = p->next;
        prev = p;
        next = n;
        if (prev) {
            prev->next = this;
            prev->end = at;
        } else {
            DAT_0032b234 = this;
        }
        if (next) {
            next->prev = this;
            next->start = e;
        }
        avail = f;
        start = at;
        end = e;
    }
};

extern "C" unsigned FUN_0004eaf8(unsigned size, unsigned align)
{
    unsigned r = 0;
    if (size) {
        size = (size + 15) & ~15;
        for (Block *b = DAT_0032b234; b; b = b->next) {
            if (b->avail) {
                unsigned off = size & (align - 1);
                unsigned at = (~(align - 1) & (align + (b->start + off) - 1)) - off;
                unsigned e = at + size;
                if (e <= b->end) {
                    if (b->start < at)
                        b = new Block(b, at, 0);
                    else
                        b->avail = 0;
                    if (e < b->end)
                        new Block(b, e, 1);
                    r = b->start;
                    break;
                }
            }
        }
        if (r == 0)
            r = FUN_00010184(0x40, size);
    }
    return r;
}
