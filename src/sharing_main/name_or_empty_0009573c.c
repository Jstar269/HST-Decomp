/* HSTMATCH module=hstp_trans_main.prx start=0x0009573c end=0x0009575c symbol=FUN_0009573c lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Sub { char pad[0x30]; char name[1]; };
struct Obj { char pad[0x58]; struct Sub *sub; };
extern char DAT_002cc260[];
char *FUN_0009573c(struct Obj *self) { return self->sub ? self->sub->name : DAT_002cc260; }
