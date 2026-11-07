/* HSTMATCH module=EBOOT.BIN start=0x001b5578 end=0x001b5588 symbol=FUN_001b5578 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0x64]; int value; };
int FUN_001b5578(struct Obj *self) { return self->value >= 0; }
