/* HSTMATCH module=EBOOT.BIN start=0x0014ba78 end=0x0014ba80 symbol=FUN_0014ba78 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[8]; void *inner; };
extern void FUN_0014dc3c(void *p);
void FUN_0014ba78(struct Obj *self) { FUN_0014dc3c(self->inner); }
