/* HSTMATCH module=EBOOT.BIN start=0x00051344 end=0x00051358 symbol=FUN_00051344 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { int pad; int id; };
extern char DAT_00311170[];
extern void FUN_00050208(void *list, int id, int flag);
void FUN_00051344(struct Obj *self, int flag) { FUN_00050208(DAT_00311170, self->id, flag); }
