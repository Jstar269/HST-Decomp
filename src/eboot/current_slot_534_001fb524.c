/* HSTMATCH module=EBOOT.BIN start=0x001fb524 end=0x001fb538 symbol=FUN_001fb524 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0x3dc]; int cur; char pad1[0x534 - 0x3e0]; int slots[1]; };
int FUN_001fb524(struct Obj *self) { return self->slots[self->cur]; }
