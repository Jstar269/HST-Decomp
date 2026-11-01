/* HSTMATCH module=EBOOT.BIN start=0x0010a5b0 end=0x0010a674 symbol=FUN_0010a5b0 lang=c99 */
extern int DAT_002d8e08;
extern int DAT_002d8e44;
extern int DAT_002d8e84;
extern void FUN_0010a684(int self, int a);
extern int FUN_002b7c98(void);
extern int FUN_002b7c60(void);
extern void FUN_0010a4a8(const char *, ...);

void FUN_0010a5b0(int self, int *out, int a3)
{
    FUN_0010a684(self, a3);
    int r = FUN_002b7c98();
    if (r < 0) {
        FUN_0010a4a8((const char *)&DAT_002d8e08, r);
        return;
    }
    switch (r) {
    case 3:
    {
        int r2 = FUN_002b7c60();
        if (r2 < 0) {
            FUN_0010a4a8((const char *)&DAT_002d8e44, r2);
        }
        break;
    }
    case 0:
    {
        int v = *(int *)(self + 28);
        if (v == 0) {
            *out = 1;
        } else {
            FUN_0010a4a8((const char *)&DAT_002d8e84, v);
            *out = -1;
        }
        break;
    }
    }
}
// near miss: 12 differ out of 49 words.
// The entire second half (case 0 and epilogue, words 27-49) is a 100% exact match.
// Residual is a register coalescing / schedule difference in the two error-log paths:
// Retail keeps the return value in $v0 across tests (bgez v0, beqzl v0, beq v0, v1)
// with nop in bgez delay slot, and sinks move a1, v0 into the jal 0x10a4a8 delay slot.
// MWCC coalesces r into $a1 upfront (move a1, v0; bgez a1), putting addiu a0 in jal delay slot.
// Ruled out: if ((r = F()) < 0), helpers=FUN_0010a4a8, if/else-if chains (invert body order), returning int.
// improved by lane dsn4 (unreviewed): 36 -> 12 differing words
