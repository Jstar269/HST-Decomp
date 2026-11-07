/* HSTMATCH module=hstp_trans_main.prx start=0x0005dbf4 end=0x0005dc04 symbol=FUN_0005dbf4 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0x30]; float x; float y; float z; };
void FUN_0005dbf4(struct Obj *self, float x, float y, float z) { self->x = x; self->y = y; self->z = z; }
