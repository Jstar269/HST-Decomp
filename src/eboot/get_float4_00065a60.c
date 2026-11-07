/* HSTMATCH module=EBOOT.BIN start=0x00065a60 end=0x00065a68 symbol=FUN_00065a60 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { int pad; float value; };
float FUN_00065a60(struct Obj *self) { return self->value; }
