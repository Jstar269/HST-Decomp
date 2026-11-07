/* HSTMATCH module=hstp_trans_main.prx start=0x00055c0c end=0x00055c2c symbol=FUN_00055c0c lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0x84]; float value; float scaled; };
void FUN_00055c0c(struct Obj *self, float v) { self->value = v; self->scaled = self->value * 0.01f; }
