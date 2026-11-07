/* HSTMATCH module=EBOOT.BIN start=0x001eaaf4 end=0x001eab08 symbol=FUN_001eaaf4 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Sub { char pad[0x3d4]; int slots[1]; };
struct Obj { struct Sub *sub; };
int FUN_001eaaf4(struct Obj *self, int i) { return self->sub->slots[i]; }
