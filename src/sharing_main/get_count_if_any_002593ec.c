/* HSTMATCH module=hstp_trans_main.prx start=0x002593ec end=0x00259408 symbol=FUN_002593ec lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0x4f0]; int count; };
int FUN_002593ec(struct Obj *self, int *out) { if (self->count > 0) { *out = self->count; return 1; } return 0; }
