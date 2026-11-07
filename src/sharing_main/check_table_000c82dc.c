/* HSTMATCH module=hstp_trans_main.prx start=0x000c82dc end=0x000c8330 symbol=FUN_000c82dc lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern void FUN_000c7fa4();
extern const char DAT_0037c478[], DAT_0037c500[], DAT_0037c4b8[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_000c7fa4(DAT_0037c478, DAT_0037c500, DAT_0037c4b8, line); for (;;) {} } } while (0)
extern int DAT_0037c4c8[];
int FUN_000c82dc(int u, int i) { int v = DAT_0037c4c8[i]; HST_ASSERT(v >= 0, 170); return v; }
