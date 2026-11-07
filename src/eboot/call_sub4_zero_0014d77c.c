/* HSTMATCH module=EBOOT.BIN start=0x0014d77c end=0x0014d788 symbol=FUN_0014d77c lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { int pad; char sub[4]; };
extern void FUN_0014c9b8(void *sub, int value);
void FUN_0014d77c(struct Obj *self) { FUN_0014c9b8(self->sub, 0); }
