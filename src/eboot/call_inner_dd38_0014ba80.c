/* HSTMATCH module=EBOOT.BIN start=0x0014ba80 end=0x0014ba88 symbol=FUN_0014ba80 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[8]; void *inner; };
extern void FUN_0014dd38(void *p);
void FUN_0014ba80(struct Obj *self) { FUN_0014dd38(self->inner); }
