/* HSTMATCH module=EBOOT.BIN start=0x001fb8b0 end=0x001fb8c4 symbol=FUN_001fb8b0 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0x3c]; int slots[(0xe0 - 0x3c) / 4]; int cur; };
int FUN_001fb8b0(struct Obj *self) { return self->slots[self->cur]; }
