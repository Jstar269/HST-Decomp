/* HSTMATCH module=hstp_trans_main.prx start=0x00120668 end=0x001206a4 symbol=FUN_00120668 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Sub { char pad[0x6d]; unsigned char v; };
struct Obj { char pad[0x58]; struct Sub *sub; char pad1[0x68 - 0x5c]; int st; char pad2[0x7d - 0x6c]; unsigned char v; };
int FUN_00120668(struct Obj *self) { switch (self->st) { case 3: case 4: return self->v; } return self->sub->v; }
