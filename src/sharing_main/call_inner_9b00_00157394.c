/* HSTMATCH module=hstp_trans_main.prx start=0x00157394 end=0x0015739c symbol=FUN_00157394 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[8]; void *inner; };
extern void FUN_00159b00(void *p);
void FUN_00157394(struct Obj *self) { FUN_00159b00(self->inner); }
