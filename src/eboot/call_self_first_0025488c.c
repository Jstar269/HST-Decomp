/* HSTMATCH module=EBOOT.BIN start=0x0025488c end=0x00254894 symbol=FUN_0025488c lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { int first; };
extern void FUN_00254b58(struct Obj *self, int first);
void FUN_0025488c(struct Obj *self) { FUN_00254b58(self, self->first); }
