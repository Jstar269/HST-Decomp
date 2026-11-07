/* HSTMATCH module=hstp_trans_main.prx start=0x000c567c end=0x000c56fc symbol=FUN_000c567c lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */ /* matching idiom: MWCC tests the cases of a small switch in reverse source order, so the cases are written 1, 0, 2 for retail's 2, 0, 1 test order. */
extern void FUN_000c56fc();
extern const char DAT_0037c3e0[], DAT_0037c40c[], DAT_0037c414[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_000c56fc(DAT_0037c3e0, DAT_0037c40c, DAT_0037c414, line); for (;;) {} } } while (0)
extern int DAT_0037c3dc;
extern float DAT_003ea624, DAT_003ea62c, DAT_003ea634;
void FUN_000c567c(float f)
{
    switch (DAT_0037c3dc) {
    case 1: DAT_003ea624 = f; break;
    case 0: DAT_003ea62c = f; break;
    case 2: DAT_003ea634 = f; break;
    default: HST_ASSERT(0, 48); break;
    }
}
