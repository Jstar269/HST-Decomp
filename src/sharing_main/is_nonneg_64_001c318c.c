/* HSTMATCH module=hstp_trans_main.prx start=0x001c318c end=0x001c319c symbol=FUN_001c318c lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0x64]; int value; };
int FUN_001c318c(struct Obj *self) { return self->value >= 0; }
