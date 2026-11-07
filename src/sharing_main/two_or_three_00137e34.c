/* HSTMATCH module=hstp_trans_main.prx start=0x00137e34 end=0x00137e4c symbol=FUN_00137e34 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0x254]; unsigned char flag; };
int FUN_00137e34(struct Obj *self) { return self->flag ? 2 : 3; }
