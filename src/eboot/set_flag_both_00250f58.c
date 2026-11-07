/* HSTMATCH module=EBOOT.BIN start=0x00250f58 end=0x00250f68 symbol=FUN_00250f58 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0x318]; unsigned char flag; };
extern unsigned char DAT_002fbef8;
void FUN_00250f58(struct Obj *self, unsigned char value) { self->flag = value; DAT_002fbef8 = value; }
