/* HSTMATCH module=EBOOT.BIN start=0x000f2550 end=0x000f255c symbol=FUN_000f2550 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0x84]; unsigned char flag; };
extern void FUN_00103628(struct Obj *self, int value);
void FUN_000f2550(struct Obj *self) { self->flag = 1; FUN_00103628(self, 1); }
