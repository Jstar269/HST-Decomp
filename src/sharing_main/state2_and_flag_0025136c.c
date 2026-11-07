/* HSTMATCH module=hstp_trans_main.prx start=0x0025136c end=0x0025139c symbol=FUN_0025136c lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0x3c]; unsigned char flag; char pad1[3]; int state; };
int FUN_0025136c(struct Obj *self) { if (self->state == 2 && self->flag) return 1; return 0; }
