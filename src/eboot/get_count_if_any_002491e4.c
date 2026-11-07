/* HSTMATCH module=EBOOT.BIN start=0x002491e4 end=0x00249200 symbol=FUN_002491e4 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0x4f0]; int count; };
int FUN_002491e4(struct Obj *self, int *out) { if (self->count > 0) { *out = self->count; return 1; } return 0; }
