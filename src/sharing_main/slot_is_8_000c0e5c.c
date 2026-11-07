/* HSTMATCH module=hstp_trans_main.prx start=0x000c0e5c end=0x000c0e74 symbol=FUN_000c0e5c lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0xc8]; int slots[1]; };
int FUN_000c0e5c(struct Obj *self, int i) { return self->slots[i] == 8; }
