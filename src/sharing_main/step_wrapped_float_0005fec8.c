/* HSTMATCH module=hstp_trans_main.prx start=0x0005fec8 end=0x0005ff00 symbol=FUN_0005fec8 lang=c99 */
/* Clone of src/eboot/step_wrapped_float_0005f0d8.c (hstclone.py tag dsn67): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

typedef struct {
    char pad00[0xcc];
    float fcc;
    float fd0;
} Obj_0005fec8;

extern void FUN_0005ff00(Obj_0005fec8 *self, float val);

void FUN_0005fec8(Obj_0005fec8 *self)
{
    FUN_0005ff00(self, self->fcc);
    self->fcc += self->fd0;
}
