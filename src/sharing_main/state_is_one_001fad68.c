/* HSTMATCH module=hstp_trans_main.prx start=0x001fad68 end=0x001fad84 symbol=FUN_001fad68 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0xc]; int state; };
int FUN_001fad68(struct Obj *self) { int r = 1; if (self->state != 1) r = 0; return r; }
