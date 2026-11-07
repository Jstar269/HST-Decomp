/* HSTMATCH module=hstp_trans_main.prx start=0x002612ac end=0x002612bc symbol=FUN_002612ac lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0x318]; unsigned char flag; };
extern unsigned char DAT_003a490c;
void FUN_002612ac(struct Obj *self, unsigned char value) { self->flag = value; DAT_003a490c = value; }
