/* HSTMATCH module=EBOOT.BIN start=0x00109a20 end=0x00109ca8 symbol=FUN_00109a20 lang=c99 */
/* Probe. */

#define PUSH(slot, ctl, value) do {     int diff_ = (int)(slot) - (ctl)[2];     if (diff_ < 0) { diff_ += 3; }     if (diff_ >> 2 < (ctl)[1]) { *(slot) = (value); }     else { if ((slot) != 0) { *(slot) = (value); } (ctl)[1] = (ctl)[1] + 1; } } while (0)

void FUN_00109a20(int *param_1, int **param_2, int **param_3, int **param_4, int **param_5, int **param_6)
{
    int **local_8 = (int **)*param_6;
    int *iVar2 = (int *)param_6[1];

    while (param_2 != param_3 && param_4 != param_5) {
        if (*(float *)(*param_2 + 0x20) < *(float *)(*param_4 + 0x20)) {
            PUSH(local_8, iVar2, *param_4);
            param_4 = param_4 + 1;
        } else {
            PUSH(local_8, iVar2, *param_2);
            param_2 = param_2 + 1;
        }
        local_8 = local_8 + 1;
    }
    if (param_2 == param_3) {
        for (; param_4 != param_5; param_4 = param_4 + 1) {
            PUSH(local_8, iVar2, *param_4);
            local_8 = local_8 + 1;
        }
        *param_1 = (int)local_8;
        param_1[1] = (int)iVar2;
    } else {
        for (; param_2 != param_3; param_2 = param_2 + 1) {
            PUSH(local_8, iVar2, *param_2);
            local_8 = local_8 + 1;
        }
        *param_1 = (int)local_8;
        param_1[1] = (int)iVar2;
    }
}
// 2026-10-31 LINK FIX: replaced the `static void push(...)` helper with a PUSH macro, so the file
// now links FUN_00109a20 at its retail address instead of 0x109a70 (MISPLACED). Measured residual:
// 112/162 words -- four inline copies of a ~17-word push block plus the merge loop are missing.
// Next: the run body must be written inline four times (MWCC will not inline a static helper at
// -O4); start from the retail listing at 0x109a20 and keep pos/ctl in a two-int struct so they
// spill to sp+72/sp+76 as retail does.
// improved by lane dsn8 (unreviewed): no comparable result -> 161 differing words
