/* HSTMATCH module=EBOOT.BIN start=0x0012c9c8 end=0x0012c9e0 symbol=FUN_0012c9c8 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0x254]; unsigned char flag; };
int FUN_0012c9c8(struct Obj *self) { return self->flag ? 2 : 3; }
