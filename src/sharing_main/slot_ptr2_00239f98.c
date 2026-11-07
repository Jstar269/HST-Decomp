/* HSTMATCH module=hstp_trans_main.prx start=0x00239f98 end=0x00239ff8 symbol=FUN_00239f98 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern void FUN_00239f10();
extern const char DAT_0039af44[], DAT_0039afbc[], DAT_0039afa8[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_00239f10(DAT_0039af44, DAT_0039afbc, DAT_0039afa8, line); for (;;) {} } } while (0)
extern char *DAT_003ea8e8;
void *FUN_00239f98(int i) { HST_ASSERT(i >= 0 && i < 256, 69); return (int *)(DAT_003ea8e8 + 0xe054) + i; }
