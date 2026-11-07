/* HSTMATCH module=EBOOT.BIN start=0x0022bf24 end=0x0022bf40 symbol=FUN_0022bf24 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct G { char pad[0x3500]; unsigned char b[1]; };
extern struct G *DAT_003415a8;
void FUN_0022bf24(int i, char v) { int j = i + 0x8c00; DAT_003415a8->b[j] = v; }
