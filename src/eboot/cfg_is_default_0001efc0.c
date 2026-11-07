/* HSTMATCH module=EBOOT.BIN start=0x0001efc0 end=0x0001efdc symbol=FUN_0001efc0 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Cfg { int pad; };
extern struct Cfg *DAT_002cfc8c;
extern struct Cfg DAT_0030fd40;
int FUN_0001efc0(void) { return DAT_002cfc8c == &DAT_0030fd40; }
