/* HSTMATCH module=EBOOT.BIN start=0x0022aff4 end=0x0022b010 symbol=FUN_0022aff4 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct G { char pad[0x3500]; unsigned char b[1]; };
extern struct G *DAT_003415a8;
void FUN_0022aff4(int i, char v) { int j = i + 0x8800; DAT_003415a8->b[j] = v; }
