/* HSTMATCH module=EBOOT.BIN start=0x002af274 end=0x002af27c symbol=FUN_002af274 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0x4b74]; int value; };
int FUN_002af274(struct Obj *self) { return self->value; }
