/* HSTMATCH module=hstp_trans_main.prx start=0x001f8d98 end=0x001f8dac symbol=FUN_001f8d98 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Sub { char pad[0x3d4]; int slots[1]; };
struct Obj { struct Sub *sub; };
int FUN_001f8d98(struct Obj *self, int i) { return self->sub->slots[i]; }
