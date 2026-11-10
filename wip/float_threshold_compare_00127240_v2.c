/* HSTMATCH module=EBOOT.BIN start=0x00127240 end=0x00127308 symbol=FUN_00127240 lang=c99 */
extern float FUN_0000c6dc(float, float);
extern void FUN_00022434(float *, float *, float *);
extern void FUN_00022518(float *, float *);
extern float fabsf(float);

int FUN_00127240(int param_1)
{
    int uVar1;
    float fVar2;
    float fVar3;
    float a[4];
    float b[4];

    if ((!(fabsf(*(float *)(param_1 + 0x1e4) - *(float *)(param_1 + 0x1e8)) < 0.001f)) ||
        (*(unsigned char *)(param_1 + 0x1e0) != 0)) {
        FUN_00022518((float *)(param_1 + 0x100), a);
        a[3] = 0.0f;
        fVar2 = FUN_0000c6dc(a[0], a[2]);
        FUN_00022434((float *)(param_1 + 0x1b0), (float *)(param_1 + 0x1d0), b);
        fVar3 = FUN_0000c6dc(b[0], b[2]);
        if (fabsf(fVar3 - fVar2) < 0.001f) {
            uVar1 = 1;
        } else {
            uVar1 = 0;
        }
    } else {
        uVar1 = 0;
    }
    return uVar1;
}
// near miss from lane w3n2 (unreviewed)
