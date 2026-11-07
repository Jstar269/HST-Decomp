/* HSTMATCH module=EBOOT.BIN start=0x00094438 end=0x0009444c symbol=FUN_00094438 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0x74]; signed char flags; };
void FUN_00094438(struct Obj *self) { self->flags &= ~0x18; }
