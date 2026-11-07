/* HSTMATCH module=hstp_trans_main.prx start=0x0008ed94 end=0x0008ede8 symbol=FUN_0008ed94 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern void FUN_0008d950();
extern const char DAT_002cea44[], DAT_002cea20[], DAT_002cea10[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_0008d950(DAT_002cea44, DAT_002cea20, DAT_002cea10, line); for (;;) {} } } while (0)
void FUN_0008ed94(short *a, int i, short v) { HST_ASSERT(i >= 0 && i < 6, 84); a[i] = v; }
