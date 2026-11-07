/* HSTMATCH module=hstp_trans_main.prx start=0x001782a8 end=0x001782b8 symbol=FUN_001782a8 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0x23]; char sub[1]; };
extern char DAT_003eaad8[];
extern void FUN_001c0ce8(void *mgr, void *sub);
void FUN_001782a8(struct Obj *self) { FUN_001c0ce8(DAT_003eaad8, self->sub); }
