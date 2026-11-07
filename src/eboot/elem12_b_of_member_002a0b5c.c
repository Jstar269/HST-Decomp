/* HSTMATCH module=EBOOT.BIN start=0x002a0b5c end=0x002a0b78 symbol=FUN_002a0b5c lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Elem { int a; int b; int c; };
struct Obj { char pad[0xc]; struct Elem *items; };
int FUN_002a0b5c(struct Obj *self, int i) { return self->items[i].b; }
