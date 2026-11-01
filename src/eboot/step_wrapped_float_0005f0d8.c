/* HSTMATCH module=EBOOT.BIN start=0x0005f0d8 end=0x0005f110 symbol=FUN_0005f0d8 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

typedef struct {
    char pad00[0xcc];
    float fcc;
    float fd0;
} Obj_0005f0d8;

extern void FUN_0005f110(Obj_0005f0d8 *self, float val);

void FUN_0005f0d8(Obj_0005f0d8 *self)
{
    FUN_0005f110(self, self->fcc);
    self->fcc += self->fd0;
}
