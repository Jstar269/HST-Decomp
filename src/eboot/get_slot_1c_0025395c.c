/* HSTMATCH module=EBOOT.BIN start=0x0025395c end=0x0025396c symbol=FUN_0025395c lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0x1c]; int slots[1]; };
int FUN_0025395c(struct Obj *self, int i) { return self->slots[i]; }
