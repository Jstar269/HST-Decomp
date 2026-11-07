/* HSTMATCH module=EBOOT.BIN start=0x00293b7c end=0x00293b94 symbol=FUN_00293b7c lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Elem { int a; int b; int c; };
int FUN_00293b7c(struct Elem *arr, int i) { return arr[i].b; }
