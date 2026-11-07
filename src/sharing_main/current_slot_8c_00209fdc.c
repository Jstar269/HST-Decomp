/* HSTMATCH module=hstp_trans_main.prx start=0x00209fdc end=0x00209ff0 symbol=FUN_00209fdc lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0x8c]; int slots[(0xe0 - 0x8c) / 4]; int cur; };
int FUN_00209fdc(struct Obj *self) { return self->slots[self->cur]; }
