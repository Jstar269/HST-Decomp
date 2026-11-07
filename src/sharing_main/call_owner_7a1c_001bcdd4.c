/* HSTMATCH module=hstp_trans_main.prx start=0x001bcdd4 end=0x001bcddc symbol=FUN_001bcdd4 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { int pad; void *owner; };
extern void FUN_001b7a1c(void *owner);
void FUN_001bcdd4(struct Obj *self) { FUN_001b7a1c(self->owner); }
