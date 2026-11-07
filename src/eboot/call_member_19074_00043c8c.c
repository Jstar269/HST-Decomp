/* HSTMATCH module=EBOOT.BIN start=0x00043c8c end=0x00043ca0 symbol=FUN_00043c8c lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0x19074]; int handle; };
extern void FUN_00051344(int handle, int flag);
void FUN_00043c8c(struct Obj *self) { FUN_00051344(self->handle, 1); }
