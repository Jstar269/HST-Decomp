/* HSTMATCH module=EBOOT.BIN start=0x00229774 end=0x0022977c symbol=FUN_00229774 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0x10]; char sub[4]; };
extern void FUN_00229ab0(void *sub);
void FUN_00229774(struct Obj *self) { FUN_00229ab0(self->sub); }
