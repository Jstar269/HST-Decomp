/* HSTMATCH module=EBOOT.BIN start=0x001ec9f8 end=0x001eca14 symbol=FUN_001ec9f8 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0xc]; int state; };
int FUN_001ec9f8(struct Obj *self) { int r = 1; if (self->state != 1) r = 0; return r; }
