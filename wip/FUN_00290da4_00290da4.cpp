/* HSTMATCH module=EBOOT.BIN start=0x00290da4 end=0x00290f68 symbol=FUN_00290da4 lang=c++ */
extern "C" {
int FUN_002b7ee0(int *);
int FUN_002b7ed0(int *, int);
void FUN_0028fb44(int, ...);
int FUN_000615f4(int, int, int, int);
int FUN_002b7a78(void);
int FUN_0006171c(int);
int FUN_002b7958(int);
int FUN_002b79b0(int);
int FUN_000114c0(int, int, int);
extern int DAT_0034a80c;
extern int DAT_0034a810;
extern int DAT_0034a814;
extern int s_ERROR__scePsmfPlayerGetCurrentSt_002ff85c;
extern int s_ERROR__scePsmfPlayerGetAudioData_002ff890;
extern int s_ERROR__sceWaveAudioWriteBlocking_002ff8bc;
extern int s_ERROR__sceDisplayWaitVblankStart_002ff830;
}

extern "C" int FUN_00290da4(int *param_1)
{
    int iVar1;

    do {
        if (DAT_0034a80c != 0) {
            iVar1 = param_1[56];
            goto LAB_00290EC0;
        }
        iVar1 = FUN_002b7ee0(param_1);
        if (iVar1 < 0) {
            FUN_0028fb44(s_ERROR__scePsmfPlayerGetCurrentSt_002ff85c, iVar1);
            goto LAB_00290EBC;
        }
        if (iVar1 == 2) {
            goto LAB_00290EBC;
        }
        iVar1 = FUN_002b7ed0(param_1, param_1[param_1[56] + 0x35]);
        if (iVar1 != -0x7f9e9ff4) {
            if (iVar1 < 0) {
                FUN_0028fb44(s_ERROR__scePsmfPlayerGetAudioData_002ff890, iVar1);
                goto LAB_00290EBC;
            }
            if (iVar1 == 0) {
                iVar1 = FUN_000615f4(0, DAT_0034a810, DAT_0034a814, param_1[param_1[56] + 0x35]);
                if (iVar1 < 0) {
                    FUN_0028fb44(s_ERROR__sceWaveAudioWriteBlocking_002ff8bc, iVar1);
                }
                param_1[56] = (param_1[56] + 1) & 1;
            }
        }
        iVar1 = FUN_002b7a78();
    } while (iVar1 >= 0);
    FUN_0028fb44(s_ERROR__sceDisplayWaitVblankStart_002ff830, iVar1);
LAB_00290EBC:
    iVar1 = param_1[56];
LAB_00290EC0:
    FUN_000114c0(param_1[iVar1 + 0x35], 0, param_1[0x37]);
    iVar1 = FUN_000615f4(0, 0, 0, param_1[param_1[56] + 0x35]);
    if (iVar1 < 0) {
        FUN_0028fb44(s_ERROR__sceWaveAudioWriteBlocking_002ff8bc, iVar1);
    }
    param_1[56] = (param_1[56] + 1) & 1;
    iVar1 = FUN_0006171c(0);
    while (iVar1 != 0) {
        FUN_002b7958(1000);
        iVar1 = FUN_0006171c(0);
    }
    FUN_002b79b0(0);
    return 0;
}
// NEAR: 113/113 words; 26 differ (first at +0x14). Diff vs retail: (1) retail reads DAT_0034a80c with lbu (we declared extern int); (2) retail branch shape: beqzl v0,0x290ec0 / lw v0,224(s0) / jal 0x2b7ee0 vs ours; (3) string args: retail passes addresses (lui a0,0x30; addiu a0,a0,-1956) while ours lw -1956(v0) via extern int.
// near miss from lane oc-hst-20261121 (unreviewed)
