/* HSTMATCH module=hstp_trans_main.prx start=0x00063644 end=0x000636ac symbol=FUN_00063644 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern void FUN_00062fa8();
extern const char DAT_00377024[], DAT_003770dc[], DAT_00377064[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_00062fa8(DAT_00377024, DAT_003770dc, DAT_00377064, line); for (;;) {} } } while (0)
extern int DAT_003daec0;
void FUN_00063644(void)
{
    int n = DAT_003daec0;
    if (n > 0) {
        DAT_003daec0 = n - 1;
    } else {
        HST_ASSERT(n > 0, 912);
        DAT_003daec0 = 0;
    }
}
