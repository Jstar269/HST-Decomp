/* HSTMATCH module=EBOOT.BIN start=0x00241164 end=0x00241194 symbol=FUN_00241164 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

struct Obj_00241164 {
    char pad0[0x3c];
    unsigned char flag;
    char pad1[3];
    int state;
};

int FUN_00241164(struct Obj_00241164 *self)
{
    if (self->state == 2 && self->flag != 0)
        return 1;
    return 0;
}
