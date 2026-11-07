/* HSTMATCH module=EBOOT.BIN start=0x001a3fe0 end=0x001a3fe8 symbol=FUN_001a3fe0 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0x68]; char sub[4]; };
extern void FUN_001aef94(void *sub);
void FUN_001a3fe0(struct Obj *self) { FUN_001aef94(self->sub); }
