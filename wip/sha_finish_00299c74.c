/* HSTMATCH module=EBOOT.BIN start=0x00299c74 end=0x00299e08 symbol=FUN_00299c74 lang=c99 */

extern void FUN_00299f00(unsigned char *ctx, unsigned int *block);

/* NEAR MISS 107/101 words. Residual: MWCC sinks the ra/s0/s1 saves after the null test in
   retail (frame first, then sltiu/sltiu/or/bnez, then the saves) and reuses t-registers
   (t2..t8) for the unrolled length loop; this form saves the registers first and uses v0/v1.
   Structure transcribed from the pseudocode: SHA-1-style finish (zero 64-byte block, copy
   key bytes from ctx+0x20, pad 0x80, transform if len >= 56, bit-length bytes at block+56,
   transform, set ctx+0x16 = 1, clear 0x40 key bytes, copy 16 bytes out, return 0). Ruled out:
   two separate arrays (79 words, buffer overlapped), early-return form (same 107). */
unsigned int FUN_00299c74(unsigned char *param_1, unsigned char *param_2)
{
    unsigned char bVar1;
    unsigned int uVar2;
    unsigned int *puVar3;
    unsigned int *puVar7;
    unsigned int *puVar4;
    unsigned char *puVar5;
    unsigned int uVar6;
    unsigned int uVar8;
    int iVar9;
    int iVar10;
    unsigned int local_50[16];

    puVar3 = local_50;
    puVar7 = local_50;
    puVar4 = local_50;
    uVar2 = 0x80000103;
    if (param_1 == 0 || param_2 == 0) {
        return uVar2;
    }
    {
        if (*(unsigned short *)(param_1 + 0x16) == 0) {
            uVar8 = *(unsigned short *)(param_1 + 0x14);
            iVar9 = 0xf;
            do {
                iVar9 = iVar9 - 1;
                *puVar3 = 0;
                puVar3 = puVar3 + 1;
            } while (iVar9 >= 0);
            puVar5 = param_1 + 0x20;
            uVar6 = 0;
            if (uVar8 != 0) {
                do {
                    uVar6 = uVar6 + 1;
                    *(unsigned char *)puVar7 = *puVar5;
                    puVar5 = puVar5 + 1;
                    puVar7 = (unsigned int *)((int)puVar7 + 1);
                } while (uVar6 < uVar8);
            }
            *(unsigned char *)((int)local_50 + uVar8) = 0x80;
            if (0x37 < uVar8) {
                FUN_00299f00(param_1, local_50);
                iVar9 = 0xf;
                do {
                    iVar9 = iVar9 - 1;
                    *puVar4 = 0;
                    puVar4 = puVar4 + 1;
                } while (iVar9 >= 0);
            }
            uVar8 = *(unsigned int *)(param_1 + 0x18);
            uVar6 = *(int *)(param_1 + 0x1c) << 3 | uVar8 >> 0x1d;
            iVar9 = 0;
            do {
                iVar10 = iVar9 + 1;
                if (iVar9 << 0x1d < 0) {
                    bVar1 = (unsigned char)(uVar6 >> (iVar9 * 8 & 0x1f));
                }
                else {
                    bVar1 = (unsigned char)((uVar8 << 3) >> (iVar9 * 8 & 0x1f));
                    if (iVar9 << 0x1d != 0) {
                        bVar1 = bVar1 | (unsigned char)(uVar6 << (iVar9 * -8 & 0x1f));
                    }
                }
                ((unsigned char *)local_50)[56 + iVar9] = bVar1;
                iVar9 = iVar10;
            } while (iVar10 < 8);
            FUN_00299f00(param_1, local_50);
            *(unsigned short *)(param_1 + 0x16) = 1;
            iVar9 = 0;
            do {
                iVar10 = iVar9 + 1;
                param_1[iVar9 + 0x20] = 0;
                iVar9 = iVar10;
            } while (iVar10 < 0x40);
        }
        uVar8 = 0;
        do {
            uVar8 = uVar8 + 1;
            *param_2 = *param_1;
            param_1 = param_1 + 1;
            param_2 = param_2 + 1;
        } while (uVar8 < 0x10);
        uVar2 = 0;
    }
    return uVar2;
}
// near miss from lane dsn8 (unreviewed)
