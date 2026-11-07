/* HSTMATCH module=EBOOT.BIN start=0x001fd154 end=0x001fd170 symbol=FUN_001fd154 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0x3c]; int value; };
extern struct Obj *DAT_0034a13c;
int FUN_001fd154(void) { return DAT_0034a13c ? DAT_0034a13c->value : 0; }
