/* HSTMATCH module=EBOOT.BIN start=0x001f94a4 end=0x001f94ac symbol=FUN_001f94a4 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0x510]; void *member; };
extern void FUN_001e7ca8(void *member);
void FUN_001f94a4(struct Obj *self) { FUN_001e7ca8(self->member); }
