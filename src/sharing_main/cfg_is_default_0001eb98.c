/* HSTMATCH module=hstp_trans_main.prx start=0x0001eb98 end=0x0001ebb4 symbol=FUN_0001eb98 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Cfg { int pad; };
extern struct Cfg *DAT_003751d4;
extern struct Cfg DAT_003b9040;
int FUN_0001eb98(void) { return DAT_003751d4 == &DAT_003b9040; }
