/* HSTMATCH module=EBOOT.BIN start=0x000df8e8 end=0x000dfa40 symbol=FUN_000df8e8 lang=c99 */

extern int FUN_002b77c8(int *lock, int a, int b);
extern int FUN_002b77d0(int *lock, int a);
extern int FUN_000dfb90(int *entry, int key);
extern int FUN_000dfdd0(int *entry);
extern void FUN_002b78b8(int p);

/* NEAR MISS 80/86 words. Residual: retail keeps 5 s-registers (s0-s3,s5, 32-byte frame) and
   sets s3 = param_2 before saving; this form uses s0-s3 only. All control flow matches the
   pseudocode (two error returns -0x7fffff00 / -0x7ffffefd, the list walk from param_1[0x16]
   with FUN_000dfb90, the sentinel check param_1+0x16, the unlink block with the
   (iVar4+0xff)&0xffffff00 size fixup, and the closing FUN_002b77d0). Ruled out: splitting
   the two guard tests. */
int FUN_000df8e8(int *param_1, int param_2)
{
    int iVar1;
    int iVar2;
    int iVar3;
    int iVar4;
    int *piVar5;
    int *piVar6;

    if (param_1 == 0 || (piVar6 = param_1 + 0xe, *param_1 != (int)(param_1 + 1))) {
        iVar1 = -0x7fffff00;
    }
    else {
        iVar1 = FUN_002b77c8(piVar6, 1, 0);
        if (iVar1 >= 0) {
            if (param_2 != 0) {
                piVar5 = (int *)param_1[0x16];
                while (1) {
                    iVar1 = FUN_000dfb90(piVar5 + 2, param_2);
                    if (iVar1 == 0) {
                        break;
                    }
                    if (piVar5 == param_1 + 0x16) {
                        FUN_002b77d0(piVar6, 1);
                        return -0x7ffffefd;
                    }
                    piVar5 = (int *)*piVar5;
                }
                if (piVar5 == param_1 + 0x16) {
                    iVar1 = param_1[0xc];
                }
                else {
                    iVar1 = FUN_000dfdd0(piVar5 + 2);
                    if (iVar1 == 0) {
                        iVar1 = param_1[0xc];
                    }
                    else {
                        piVar5[2] = 0;
                        iVar2 = *piVar5;
                        iVar1 = piVar5[1];
                        param_1[0xd] = param_1[0xd] - 1;
                        iVar4 = piVar5[4];
                        iVar3 = param_1[0xb];
                        *(int *)(iVar2 + 4) = iVar1;
                        param_1[0xb] = iVar3 - (iVar4 + 0xffU & 0xffffff00);
                        iVar1 = piVar5[3];
                        *(int *)piVar5[1] = iVar2;
                        FUN_002b78b8(iVar1);
                        iVar1 = param_1[0xc];
                    }
                }
                param_1[0xc] = iVar1 - 1;
            }
            FUN_002b77d0(piVar6, 1);
            iVar1 = 0;
        }
    }
    return iVar1;
}
// near miss from lane dsn8 (unreviewed)
