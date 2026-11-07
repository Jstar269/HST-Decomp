/* HSTMATCH module=hstp_trans_main.prx start=0x00189c84 end=0x00189c90 symbol=FUN_00189c84 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { int pad; int value; };
extern void FUN_00189c90(struct Obj *self, int a, int b, int c, int value, int flag);
void FUN_00189c84(struct Obj *self, int a, int b, int c) { FUN_00189c90(self, a, b, c, self->value, 0); }
