/* HSTMATCH module=EBOOT.BIN start=0x000f4c9c end=0x000f4cac symbol=FUN_000f4c9c lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0x68]; int slots[1]; };
int FUN_000f4c9c(struct Obj *self, int i) { return self->slots[i]; }
