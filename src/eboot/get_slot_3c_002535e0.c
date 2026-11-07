/* HSTMATCH module=EBOOT.BIN start=0x002535e0 end=0x002535f0 symbol=FUN_002535e0 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0x3c]; int slots[1]; };
int FUN_002535e0(struct Obj *self, int i) { return self->slots[i]; }
