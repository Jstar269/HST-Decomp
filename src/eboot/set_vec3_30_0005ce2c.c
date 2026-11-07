/* HSTMATCH module=EBOOT.BIN start=0x0005ce2c end=0x0005ce3c symbol=FUN_0005ce2c lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0x30]; float x; float y; float z; };
void FUN_0005ce2c(struct Obj *self, float x, float y, float z) { self->x = x; self->y = y; self->z = z; }
