/* HSTMATCH module=EBOOT.BIN start=0x000550ec end=0x0005510c symbol=FUN_000550ec lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0x84]; float value; float scaled; };
void FUN_000550ec(struct Obj *self, float v) { self->value = v; self->scaled = self->value * 0.01f; }
