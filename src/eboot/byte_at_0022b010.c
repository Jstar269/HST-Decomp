/* HSTMATCH module=EBOOT.BIN start=0x0022b010 end=0x0022b02c symbol=FUN_0022b010 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct G { char pad[0x3500]; unsigned char b[1]; };
extern struct G *DAT_003415a8;
unsigned char FUN_0022b010(int i) { int j = i + 0x8800; return DAT_003415a8->b[j]; }
