/* HSTMATCH module=hstp_trans_main.prx start=0x00238ce8 end=0x00238cf0 symbol=FUN_00238ce8 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0x10]; char sub[4]; };
extern void FUN_00239024(void *sub);
void FUN_00238ce8(struct Obj *self) { FUN_00239024(self->sub); }
