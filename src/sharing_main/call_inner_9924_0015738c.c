/* HSTMATCH module=hstp_trans_main.prx start=0x0015738c end=0x00157394 symbol=FUN_0015738c lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[8]; void *inner; };
extern void FUN_00159924(void *p);
void FUN_0015738c(struct Obj *self) { FUN_00159924(self->inner); }
