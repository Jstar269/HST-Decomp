/* HSTMATCH module=hstp_trans_main.prx start=0x001b15ac end=0x001b15b4 symbol=FUN_001b15ac lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0x68]; char sub[4]; };
extern void FUN_001bc66c(void *sub);
void FUN_001b15ac(struct Obj *self) { FUN_001bc66c(self->sub); }
