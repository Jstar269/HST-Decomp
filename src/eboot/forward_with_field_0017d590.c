/* HSTMATCH module=EBOOT.BIN start=0x0017d590 end=0x0017d59c symbol=FUN_0017d590 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { int pad; int value; };
extern void FUN_0017d59c(struct Obj *self, int a, int b, int c, int value, int flag);
void FUN_0017d590(struct Obj *self, int a, int b, int c) { FUN_0017d59c(self, a, b, c, self->value, 0); }
