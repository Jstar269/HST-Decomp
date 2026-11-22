/* HSTMATCH module=EBOOT.BIN start=0x0022d250 end=0x0022d3f0 symbol=FUN_0022d250 lang=c++ */
extern "C" {
void FUN_0022ffe4(void);
void FUN_0015dd9c(int, int);
void FUN_00000788(void *, void *, int, int);
void FUN_00053618(void);
void FUN_000462a0(int *, int);
void FUN_00004138(int *, int);
void FUN_00102ea4(void *, int);
void FUN_00000a00(void *);
extern int DAT_0030983c;
extern int DAT_0034b350;
}

extern "C" int * FUN_0022d250(int *param_1, short param_2)
{
    int iVar3;
    int *puVar1;
    int *piVar2;
    int *piVar3;

    if (param_1 != 0) {
        *param_1 = (int)&DAT_0030983c;
        FUN_0022ffe4();
        iVar3 = 0;
        puVar1 = param_1;
        do {
            iVar3 = iVar3 + 1;
            puVar1[0xa5] = 0;
            puVar1[0xd2] = 0;
            puVar1 = puVar1 + 3;
        } while (iVar3 < 0xf);
        if (param_1[0x87] != 0) {
            FUN_0015dd9c(param_1[0x87], 1);
            param_1[0x87] = 0;
        }
        piVar2 = (int *)param_1[0x88];
        if (piVar2 != 0) {
            (**(void (**)(int *, int))(*piVar2 + 8))(piVar2, 1);
        }
        param_1[0x88] = 0;
        piVar3 = (int *)DAT_0034b350;
        if (piVar3 != 0) {
            (**(void (**)(int *, int))(*piVar3 + 8))(piVar3, 1);
        }
        DAT_0034b350 = 0;
        FUN_00000788(param_1 + 0xd1, FUN_00053618, 12, 15);
        FUN_00000788(param_1 + 0xa4, FUN_00053618, 12, 15);
        if (param_1 + 0x88 != 0) {
            FUN_000462a0(param_1 + 0x25, -1);
            FUN_00004138(param_1 + 0x22, 0);
        }
        if (param_1 + 0x70 != 0) {
            FUN_000462a0(param_1 + 0x1f, -1);
            FUN_00004138(param_1 + 0x1c, 0);
        }
        if (param_1 + 0x58 != 0) {
            FUN_000462a0(param_1 + 0x19, -1);
            FUN_00004138(param_1 + 0x16, 0);
        }
        FUN_00102ea4(param_1, 0);
        if (0 < param_2) {
            FUN_00000a00(param_1);
        }
    }
    return param_1;
}
// NEAR: 96/104 words; 94 differ, first at +0x0 - pseudocode-derived first pass from disassembly.
// near miss from lane oc-hst-20261121 (unreviewed)
