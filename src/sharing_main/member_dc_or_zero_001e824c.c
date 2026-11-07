/* HSTMATCH module=hstp_trans_main.prx start=0x001e824c end=0x001e8264 symbol=FUN_001e824c lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Sub { char pad[0xdc]; int value; };
struct Obj { char pad[0x58]; struct Sub *sub; };
int FUN_001e824c(struct Obj *self) { return self->sub ? self->sub->value : 0; }
