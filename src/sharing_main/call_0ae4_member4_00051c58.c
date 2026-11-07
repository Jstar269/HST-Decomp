/* HSTMATCH module=hstp_trans_main.prx start=0x00051c58 end=0x00051c6c symbol=FUN_00051c58 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { int pad; int id; };
extern char DAT_003ba4b0[];
extern void FUN_00050ae4(void *list, int id, int flag);
void FUN_00051c58(struct Obj *self, int flag) { FUN_00050ae4(DAT_003ba4b0, self->id, flag); }
