/* HSTMATCH module=hstp_trans_main.prx start=0x00043d70 end=0x00043d84 symbol=FUN_00043d70 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0x19074]; int handle; };
extern void FUN_00051c58(int handle, int flag);
void FUN_00043d70(struct Obj *self) { FUN_00051c58(self->handle, 1); }
