/* HSTMATCH module=EBOOT.BIN start=0x001e5a84 end=0x001e5a9c symbol=FUN_001e5a84 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Sub { char pad[0x64]; int value; };
struct Obj { char pad[0x1954]; struct Sub *sub; };
int FUN_001e5a84(struct Obj *self) { return self->sub ? self->sub->value : -1; }
