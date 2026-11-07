/* HSTMATCH module=EBOOT.BIN start=0x00094424 end=0x00094438 symbol=FUN_00094424 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0x74]; signed char flags; };
void FUN_00094424(struct Obj *self) { self->flags &= ~0x20; }
