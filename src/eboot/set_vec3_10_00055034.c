/* HSTMATCH module=EBOOT.BIN start=0x00055034 end=0x00055044 symbol=FUN_00055034 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0x10]; float x; float y; float z; };
void FUN_00055034(struct Obj *self, float x, float y, float z) { self->x = x; self->y = y; self->z = z; }
