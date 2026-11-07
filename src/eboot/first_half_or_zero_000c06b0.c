/* HSTMATCH module=EBOOT.BIN start=0x000c06b0 end=0x000c06c8 symbol=FUN_000c06b0 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0xc]; unsigned short *data; };
int FUN_000c06b0(struct Obj *self) { return self->data ? *self->data : 0; }
