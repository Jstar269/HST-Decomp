/* HSTMATCH module=hstp_trans_main.prx start=0x00207b5c end=0x00207b64 symbol=FUN_00207b5c lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0x510]; void *member; };
extern void FUN_001f5ec0(void *member);
void FUN_00207b5c(struct Obj *self) { FUN_001f5ec0(self->member); }
