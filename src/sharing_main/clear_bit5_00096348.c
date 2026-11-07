/* HSTMATCH module=hstp_trans_main.prx start=0x00096348 end=0x0009635c symbol=FUN_00096348 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0x74]; signed char flags; };
void FUN_00096348(struct Obj *self) { self->flags &= ~0x20; }
