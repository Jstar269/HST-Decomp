/* HSTMATCH module=EBOOT.BIN start=0x001af6f4 end=0x001af6fc symbol=FUN_001af6f4 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { int pad; void *owner; };
extern void FUN_001a9f74(void *owner);
void FUN_001af6f4(struct Obj *self) { FUN_001a9f74(self->owner); }
