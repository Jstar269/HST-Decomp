/* HSTMATCH module=hstp_trans_main.prx start=0x001c16b4 end=0x001c170c symbol=FUN_001c16b4 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern void FUN_001bd5cc();
extern const char DAT_00390e3c[], DAT_00390e68[], DAT_00390e78[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_001bd5cc(DAT_00390e3c, DAT_00390e68, DAT_00390e78, line); for (;;) {} } } while (0)
extern void FUN_001be964(int, int, int, int, int);
void FUN_001c16b4(int *p, int i) { int h = *p; HST_ASSERT(i < 50, 2210); FUN_001be964(h, i, 0, 0, 0); }
