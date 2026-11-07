/* HSTMATCH module=hstp_trans_main.prx start=0x0024a9c8 end=0x0024a9e0 symbol=FUN_0024a9c8 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0x1814]; int mode; char pad1[0x1838 - 0x1818]; char a[12]; char b[4]; };
void *FUN_0024a9c8(struct Obj *self) { return self->mode ? self->b : self->a; }
