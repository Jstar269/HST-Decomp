/* HSTMATCH module=hstp_trans_main.prx start=0x0009635c end=0x00096370 symbol=FUN_0009635c lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0x74]; signed char flags; };
void FUN_0009635c(struct Obj *self) { self->flags &= ~0x18; }
