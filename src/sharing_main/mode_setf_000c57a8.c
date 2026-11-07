/* HSTMATCH module=hstp_trans_main.prx start=0x000c57a8 end=0x000c5828 symbol=FUN_000c57a8 lang=c99 */
/* Hand-written reconstruction from the disassembly, adapted from the twin src/sharing_main/mode_setf_000c567c.c in the same module: addresses and the assert line substituted; exact match verified; names and types await human review. */ /* matching idiom: MWCC tests the cases of a small switch in reverse source order, so the cases are written 1, 0, 2 for retail's 2, 0, 1 test order. */
extern void FUN_000c56fc();
extern const char DAT_0037c3e0[], DAT_0037c40c[], DAT_0037c414[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_000c56fc(DAT_0037c3e0, DAT_0037c40c, DAT_0037c414, line); for (;;) {} } } while (0)
extern int DAT_0037c3dc;
extern float DAT_003ea628, DAT_003ea630, DAT_003ea638;
void FUN_000c57a8(float f)
{
    switch (DAT_0037c3dc) {
    case 1: DAT_003ea628 = f; break;
    case 0: DAT_003ea630 = f; break;
    case 2: DAT_003ea638 = f; break;
    default: HST_ASSERT(0, 84); break;
    }
}
