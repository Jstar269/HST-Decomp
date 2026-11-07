/* HSTMATCH module=EBOOT.BIN start=0x000bd8e0 end=0x000bd8f8 symbol=FUN_000bd8e0 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0xc8]; int slots[1]; };
int FUN_000bd8e0(struct Obj *self, int i) { return self->slots[i] == 8; }
