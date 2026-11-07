/* HSTMATCH module=hstp_trans_main.prx start=0x0001ef24 end=0x0001ef34 symbol=FUN_0001ef24 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Entry { unsigned char a; unsigned char b; char pad[6]; };
struct Obj { char pad[0x24]; struct Entry entries[1]; };
void FUN_0001ef24(struct Obj *self, int i, unsigned char v) { self->entries[i].a = v; }
