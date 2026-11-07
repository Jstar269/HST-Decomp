/* HSTMATCH module=hstp_trans_main.prx start=0x000fa390 end=0x000fa3a0 symbol=FUN_000fa390 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0x68]; int slots[1]; };
int FUN_000fa390(struct Obj *self, int i) { return self->slots[i]; }
