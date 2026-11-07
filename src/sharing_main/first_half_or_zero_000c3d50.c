/* HSTMATCH module=hstp_trans_main.prx start=0x000c3d50 end=0x000c3d68 symbol=FUN_000c3d50 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0xc]; unsigned short *data; };
int FUN_000c3d50(struct Obj *self) { return self->data ? *self->data : 0; }
