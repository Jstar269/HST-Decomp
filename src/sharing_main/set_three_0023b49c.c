/* HSTMATCH module=hstp_trans_main.prx start=0x0023b49c end=0x0023b520 symbol=FUN_0023b49c lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern void FUN_00239f10();
extern const char DAT_0039af44[], DAT_0039aff4[], DAT_0039afa8[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_00239f10(DAT_0039af44, DAT_0039aff4, DAT_0039afa8, line); for (;;) {} } } while (0)
extern void FUN_0023a8c8(int i, int k, int v);
void FUN_0023b49c(int i)
{
    HST_ASSERT(i >= 0 && i <= 1024, 342);
    FUN_0023a8c8(i, 0, 0);
    FUN_0023a8c8(i, 2, 0);
    FUN_0023a8c8(i, 13, 0);
}
