/* HSTMATCH module=hstp_trans_main.prx start=0x0002e68c end=0x0002e694 symbol=FUN_0002e68c lang=c99 */
/* Clone of src/sharing_main/call_inner_9924_0015738c.c (hstclone.py tag c085274): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[8]; void *inner; };
extern void FUN_0002d020(void *p);
void FUN_0002e68c(struct Obj *self) { FUN_0002d020(self->inner); }
