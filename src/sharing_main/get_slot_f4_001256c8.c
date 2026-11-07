/* HSTMATCH module=hstp_trans_main.prx start=0x001256c8 end=0x001256d8 symbol=FUN_001256c8 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0xf4]; int slots[1]; };
int FUN_001256c8(struct Obj *self, int i) { return self->slots[i]; }
