/* HSTMATCH module=EBOOT.BIN start=0x002b34b0 end=0x002b34cc symbol=FUN_002b34b0 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0x41c]; char sub[4]; };
extern void FUN_0021780c(void *sub, int steps, float a, float b);
void FUN_002b34b0(struct Obj *self) { FUN_0021780c(self->sub, 5, 2.0f, 1.0f); }
