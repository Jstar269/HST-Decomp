/* HSTMATCH module=hstp_trans_main.prx start=0x00209c3c end=0x00209c50 symbol=FUN_00209c3c lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0x3dc]; int cur; char pad1[0x534 - 0x3e0]; int slots[1]; };
int FUN_00209c3c(struct Obj *self) { return self->slots[self->cur]; }
