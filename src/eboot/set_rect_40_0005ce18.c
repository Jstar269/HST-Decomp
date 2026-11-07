/* HSTMATCH module=EBOOT.BIN start=0x0005ce18 end=0x0005ce2c symbol=FUN_0005ce18 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0x40]; float x0; float y0; float x1; float y1; };
void FUN_0005ce18(struct Obj *self, float y0, float y1, float x0, float x1) { self->x0 = x0; self->y0 = y0; self->x1 = x1; self->y1 = y1; }
