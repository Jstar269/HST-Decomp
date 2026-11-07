/* HSTMATCH module=EBOOT.BIN start=0x002af32c end=0x002af348 symbol=FUN_002af32c lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Entry { char data[308]; };
struct Obj { char pad[0x903c]; struct Entry entries[1]; };
struct Entry *FUN_002af32c(struct Obj *self, int i) { return &self->entries[i]; }
