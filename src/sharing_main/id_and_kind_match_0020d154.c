/* HSTMATCH module=hstp_trans_main.prx start=0x0020d154 end=0x0020d184 symbol=FUN_0020d154 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { int id; char pad[3]; unsigned char kind; };
int FUN_0020d154(struct Obj *self, int id, unsigned char kind) { if (self->id == id && self->kind == kind) return 1; return 0; }
