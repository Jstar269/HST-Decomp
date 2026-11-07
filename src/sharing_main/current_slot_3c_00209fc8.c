/* HSTMATCH module=hstp_trans_main.prx start=0x00209fc8 end=0x00209fdc symbol=FUN_00209fc8 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0x3c]; int slots[(0xe0 - 0x3c) / 4]; int cur; };
int FUN_00209fc8(struct Obj *self) { return self->slots[self->cur]; }
