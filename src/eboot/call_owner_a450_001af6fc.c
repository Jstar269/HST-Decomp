/* HSTMATCH module=EBOOT.BIN start=0x001af6fc end=0x001af704 symbol=FUN_001af6fc lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { int pad; void *owner; };
extern void FUN_001aa450(void *owner);
void FUN_001af6fc(struct Obj *self) { FUN_001aa450(self->owner); }
