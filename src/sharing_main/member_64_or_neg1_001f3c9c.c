/* HSTMATCH module=hstp_trans_main.prx start=0x001f3c9c end=0x001f3cb4 symbol=FUN_001f3c9c lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Sub { char pad[0x64]; int value; };
struct Obj { char pad[0x1954]; struct Sub *sub; };
int FUN_001f3c9c(struct Obj *self) { return self->sub ? self->sub->value : -1; }
