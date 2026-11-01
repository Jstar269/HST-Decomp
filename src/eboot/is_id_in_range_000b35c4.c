/* HSTMATCH module=EBOOT.BIN start=0x000b35c4 end=0x000b35f4 symbol=FUN_000b35c4 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

struct Inner {
    char pad[0xf0];
    int val;
};

struct Mid {
    char pad[0x58];
    struct Inner *inner;
};

struct Outer {
    char pad[0x5bc];
    struct Mid *mid;
};

int FUN_000b35c4(struct Outer *self)
{
    struct Inner *p = self->mid->inner;
    int v = p ? p->val : -1;
    return v >= 198 && v < 224;
}
