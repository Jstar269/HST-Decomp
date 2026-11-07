/* HSTMATCH module=EBOOT.BIN start=0x001da168 end=0x001da180 symbol=FUN_001da168 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Sub { char pad[0xdc]; int value; };
struct Obj { char pad[0x58]; struct Sub *sub; };
int FUN_001da168(struct Obj *self) { return self->sub ? self->sub->value : 0; }
