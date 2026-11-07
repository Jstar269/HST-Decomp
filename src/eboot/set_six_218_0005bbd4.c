/* HSTMATCH module=EBOOT.BIN start=0x0005bbd4 end=0x0005bbf0 symbol=FUN_0005bbd4 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0x218]; float v[6]; };
void FUN_0005bbd4(struct Obj *self, float a, float b, float c, float d, float e, float f) { self->v[0] = a; self->v[1] = b; self->v[2] = c; self->v[3] = d; self->v[4] = e; self->v[5] = f; }
