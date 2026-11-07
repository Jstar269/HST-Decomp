/* HSTMATCH module=hstp_trans_main.prx start=0x001592a4 end=0x001592b0 symbol=FUN_001592a4 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { int pad; char sub[4]; };
extern void FUN_001584a0(void *sub, int value);
void FUN_001592a4(struct Obj *self) { FUN_001584a0(self->sub, 0); }
