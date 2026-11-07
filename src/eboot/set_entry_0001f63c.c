/* HSTMATCH module=EBOOT.BIN start=0x0001f63c end=0x0001f64c symbol=FUN_0001f63c lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Entry { unsigned char a; unsigned char b; char pad[6]; };
struct Obj { char pad[0x24]; struct Entry entries[1]; };
void FUN_0001f63c(struct Obj *self, int i, unsigned char v) { self->entries[i].b = v; }
