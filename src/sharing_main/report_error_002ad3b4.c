/* HSTMATCH module=hstp_trans_main.prx start=0x002ad3b4 end=0x002ad3ec symbol=FUN_002ad3b4 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern int DAT_003f466c;
extern char DAT_003a8ef8[];
extern int FUN_002c7488(int h);
extern void FUN_002a9808(const char *fmt, ...);
void FUN_002ad3b4(void) { int r = FUN_002c7488(DAT_003f466c); if (r < 0) FUN_002a9808(DAT_003a8ef8, r); }
