/* HSTMATCH module=EBOOT.BIN start=0x001ee998 end=0x001eed14 symbol=FUN_001ee998 lang=c++ */
extern "C" {
void FUN_0001cb88(int);
void FUN_001f2f08(void *);
void FUN_00219184(int, int);
int FUN_00216bc4(int, int);
void FUN_00194110(void);
void FUN_002ad74c(void);
void FUN_00054554(void *, int);
void FUN_00000788(void *, void *, int, int);
void FUN_00066338(void *, int);
void FUN_00064a08(void *, int);
void FUN_0001f0f0(void *, int);
void FUN_00000a00(void *);
int FUN_001ee87c(void);
int FUN_001ee8ec(void);
extern int DAT_003094e0, DAT_003094fc, DAT_003070bc, DAT_00309510;
extern int DAT_00341d93, DAT_00341d7d, DAT_00341d7e;
extern int DAT_003415ac;
}

extern "C" int * FUN_001ee998(int *param_1, short param_2)
{
    int iVar2;
    int *piVar1;

    if (param_1 != 0) {
        *param_1 = (int)&DAT_003094e0;
        param_1[15] = (int)&DAT_003094fc;
        FUN_0001cb88(0x30ebf4);
        FUN_001f2f08(param_1);
        piVar1 = (int *)param_1[0x161];
        if (piVar1 != 0) {
            (**(void (**)(int *, int))(*piVar1 + 8))(piVar1, 1);
            param_1[0x161] = 0;
        }
        iVar2 = param_1[0x159];
        if (iVar2 != 0) {
            FUN_00219184(iVar2, 1);
            param_1[0x159] = 0;
            iVar2 = param_1[0x158];
        }
        if (iVar2 != 0) {
            FUN_00216bc4(iVar2, 1);
            param_1[0x158] = 0;
        }
        FUN_00194110();
        iVar2 = DAT_003415ac;
        DAT_00341d93 = (param_1[0x205f] == 1 && param_1[0x205e] == 1);
        DAT_00341d7d = 0xff;
        DAT_00341d7e = 0xff;
        if (DAT_003415ac != 0) {
            *(int *)(DAT_003415ac + 0x74) = 0x3f800000;
            *(int *)(iVar2 + 0x78) = 0x3f800000;
            iVar2 = DAT_003415ac;
            *(int *)(**(int **)(DAT_003415ac + 4) + 0x60) = 0x43700000;
            *(int *)(**(int **)(iVar2 + 4) + 100) = 0x43080000;
        }
        FUN_002ad74c();
        if (param_1 + 0x2061 != 0) {
            FUN_00054554(param_1 + 0x2061, 0);
        }
        FUN_00000788(param_1 + 0x1ddb, FUN_001ee87c, 0x14, 0x80);
        FUN_00000788(param_1 + 0x4db, FUN_001ee87c, 0x14, 0x500);
        FUN_00000788(param_1 + 0x43f, FUN_001ee8ec, 0xe4, 2);
        if (param_1 != (int *)(-0x10ac)) {
            param_1[0x433] = (int)&DAT_003070bc;
            if (param_1 + 0x43b != 0) {
                FUN_00066338(param_1 + 0x43b, 0);
            }
            if (param_1 + 0x437 != 0) {
                FUN_00066338(param_1 + 0x437, 0);
            }
            FUN_00064a08(param_1 + 0x433, 0);
        }
        if (param_1 != (int *)(-0x1054)) {
            param_1[0x415] = (int)&DAT_003070bc;
            if (param_1 + 0x41d != 0) {
                FUN_00066338(param_1 + 0x41d, 0);
            }
            if (param_1 + 0x419 != 0) {
                FUN_00066338(param_1 + 0x419, 0);
            }
            FUN_00064a08(param_1 + 0x415, 0);
        }
        if (param_1 != (int *)(-0x5f0)) {
            param_1[0x17e] = (int)&DAT_003070bc;
            if (param_1 + 0x186 != 0) {
                FUN_00066338(param_1 + 0x186, 0);
            }
            if (param_1 + 0x182 != 0) {
                FUN_00066338(param_1 + 0x182, 0);
            }
            FUN_00064a08(param_1 + 0x17e, 0);
        }
        if (param_1 != (int *)(-0x60c)) {
            param_1[0xf] = (int)&DAT_00309510;
        }
        FUN_0001f0f0(param_1, 0);
        if (0 < param_2) {
            FUN_00000a00(param_1);
        }
    }
    return param_1;
}
// NEAR: 176/223 words; 203 differ, first at +0x14 (0x001ee998 prologue: retail sw s0 first? ours sw ra,s1; check hstdiff).
// near miss from lane oc-hst-20261121 (unreviewed)
