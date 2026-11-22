/* HSTMATCH module=EBOOT.BIN start=0x0020ea40 end=0x0020ebb0 symbol=FUN_0020ea40 lang=c99 */
extern int DAT_002efa68[];
extern int DAT_0034a6b0;
extern int DAT_003386b0;
extern int FUN_00215a20(char *);
extern void FUN_0020ebb0(char *, int *, int *, int, int, int, int);
extern int FUN_0020f540(char *, int, int);
extern int FUN_00215630(char *, int);
extern float FUN_00210740(char *, int, int);
extern float FUN_0020fbf0(char *, int, int);

void FUN_0020ea40(char *param_1)
{
    int i;
    int r;
    int base;

    for (i = 0; i < 3; i++) {
        if (*(int *)(param_1 + i * 4 + 0x8b4) <= 0) {
            base = FUN_00215a20(param_1) + i * 0x18;
            FUN_0020ebb0(param_1, (int *)(param_1 + i * 4 + 0x8a8),
                         (int *)(param_1 + i * 4 + 0x8b4),
                         DAT_0034a6b0 + i * 0xc,
                         base + 0xc4,
                         FUN_00215a20(param_1) + i * 0x18 + 0xd0,
                         DAT_002efa68[i]);
        }
        if (i != 0)
            continue;
        *(char *)(param_1 + 0x8a0) = 0;
        r = FUN_0020f540(param_1, *(int *)(param_1 + 0x8a8),
                         *(int *)(param_1 + 0x8b4));
        *(int *)(param_1 + 0x8e0) = r;
        *(int *)(param_1 + 0x8e0) = FUN_00215630(param_1, r);
    }
    *(float *)(param_1 + 0x8f0) = FUN_00210740(param_1, *(int *)(param_1 + 0x8b0),
                                               *(int *)(param_1 + 0x8bc));
    *(float *)(param_1 + 0x8f8) = FUN_0020fbf0(param_1, *(int *)(param_1 + 0x8ac),
                                               *(int *)(param_1 + 0x8b8));
    *(float *)(param_1 + 0x8f4) = -*(float *)(*(int *)(DAT_003386b0 + 0x88) + 0x110);
    *(int *)(param_1 + 0x8fc) = 0x7f7fffff;
    *(int *)(param_1 + 0x8b4) -= 1;
    *(int *)(param_1 + 0x8b8) -= 1;
    *(int *)(param_1 + 0x8bc) -= 1;
}
// lane api-hstnear2-20261121 (13 -> 8): (a) DAT_003486b0 was a mis-decoded address: `lui 0x34; lw -31056` is DAT_003386b0 (fixes the lui word);
// (b) `char *param_1` with pointer-typed callee params (`FUN_0020ebb0(char *, int *, int *, int, int, int, int)`, other callees `char *`) fixes the
// argument-setup order. Left: the strength-reduced IV pair colouring (retail DAT ptr s2 / i*0x18 s1, ours s1 / s2).
// Ruled out for it: all 6 declaration orders of i/r/base, do/while (++i<3), `int dv = DAT[i]` hoisted, base removed (CSE: 73 off), `*(DAT + i)`, byte-offset
// spelling of DAT[i], `DAT[3]` extern, `i * 12` spelling, `if (i)`, `#pragma opt_strength_reduction off` (90 off).
// improved by lane api-hstnear2-20261121 (unreviewed): 13 -> 8 differing words
