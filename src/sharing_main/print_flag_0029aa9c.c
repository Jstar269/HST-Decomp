/* HSTMATCH module=hstp_trans_main.prx start=0x0029aa9c end=0x0029aac0 symbol=FUN_0029aa9c lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern char DAT_003a7b0c[];
extern char DAT_003a7b24[];
extern char DAT_003a7b2c[];
extern void FUN_00297e90(const char *fmt, int a, const char *s);
void FUN_0029aa9c(int unused, int a, int flag) { FUN_00297e90(DAT_003a7b0c, a, flag ? DAT_003a7b24 : DAT_003a7b2c); }
