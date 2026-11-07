/* HSTMATCH module=EBOOT.BIN start=0x001fb8c4 end=0x001fb8d8 symbol=FUN_001fb8c4 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0x8c]; int slots[(0xe0 - 0x8c) / 4]; int cur; };
int FUN_001fb8c4(struct Obj *self) { return self->slots[self->cur]; }
