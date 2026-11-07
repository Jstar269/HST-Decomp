/* HSTMATCH module=hstp_trans_main.prx start=0x001bcdcc end=0x001bcdd4 symbol=FUN_001bcdcc lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { int pad; void *owner; };
extern void FUN_001b7540(void *owner);
void FUN_001bcdcc(struct Obj *self) { FUN_001b7540(self->owner); }
