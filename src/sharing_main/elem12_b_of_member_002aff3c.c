/* HSTMATCH module=hstp_trans_main.prx start=0x002aff3c end=0x002aff58 symbol=FUN_002aff3c lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Elem { int a; int b; int c; };
struct Obj { char pad[0xc]; struct Elem *items; };
int FUN_002aff3c(struct Obj *self, int i) { return self->items[i].b; }
