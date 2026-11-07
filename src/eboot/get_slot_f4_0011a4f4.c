/* HSTMATCH module=EBOOT.BIN start=0x0011a4f4 end=0x0011a504 symbol=FUN_0011a4f4 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0xf4]; int slots[1]; };
int FUN_0011a4f4(struct Obj *self, int i) { return self->slots[i]; }
