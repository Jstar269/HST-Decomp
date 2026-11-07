/* HSTMATCH module=EBOOT.BIN start=0x0023a830 end=0x0023a848 symbol=FUN_0023a830 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0x1814]; int mode; char pad1[0x1838 - 0x1818]; char a[12]; char b[4]; };
void *FUN_0023a830(struct Obj *self) { return self->mode ? self->b : self->a; }
