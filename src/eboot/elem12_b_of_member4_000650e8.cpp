/* HSTMATCH module=EBOOT.BIN start=0x000650e8 end=0x00065104 symbol=FUN_000650e8 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Elem { int a; int b; int c; };
struct Vec { Elem *data; Elem &operator[](int i) { return data[i]; } };
struct Obj { int pad; Vec items; };
extern "C" int FUN_000650e8(Obj *self, int i) { return self->items[i].b; }
