/* HSTMATCH module=hstp_trans_main.prx start=0x002bd218 end=0x002bd27c symbol=FUN_002bd218 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern void FUN_002bd27c();
extern const char DAT_003abd4c[], DAT_003abd78[], DAT_003abd7c[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_002bd27c(DAT_003abd4c, DAT_003abd78, DAT_003abd7c, line); for (;;) {} } } while (0)
extern int FUN_00066260(void *t, const char *a, int b, int c, const char *d);
extern char DAT_003f48e0[];
extern const char DAT_003abd30[], DAT_003abd48[];
void FUN_002bd218(void)
{
    HST_ASSERT(FUN_00066260(DAT_003f48e0, DAT_003abd30, 1, 0, DAT_003abd48), 17);
}
