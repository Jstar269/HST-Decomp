/* HSTMATCH module=EBOOT.BIN start=0x000b21cc end=0x000b21f4 symbol=FUN_000b21cc lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

struct Obj {
    char pad0[0x480];
    unsigned char f480;
    char pad1[3];
    int f484;
    char pad2[0x4ce - 0x488];
    unsigned char f4ce;
};

int FUN_000b21cc(struct Obj *self, int *out, unsigned char *outb)
{
    int b;
    if (self->f480 != 0) {
        *out = self->f484;
        *outb = self->f4ce;
        b = 1;
    } else {
        b = 0;
    }
    return b;
}
