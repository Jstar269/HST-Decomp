/* HSTMATCH module=hstp_trans_main.prx start=0x0023c2b4 end=0x0023c320 symbol=FUN_0023c2b4 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern void FUN_00239f10();
extern const char DAT_0039af44[], DAT_0039b290[], DAT_0039afa8[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_00239f10(DAT_0039af44, DAT_0039b290, DAT_0039afa8, line); for (;;) {} } } while (0)
extern unsigned FUN_00014a40(const char *s);
extern char *FUN_000149a8(char *d, const char *s);
extern char *DAT_003ea8e8;
void FUN_0023c2b4(const char *s)
{
    HST_ASSERT(FUN_00014a40(s) < 16, 733);
    FUN_000149a8(DAT_003ea8e8 + 0xe010, s);
}
