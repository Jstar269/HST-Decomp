/* HSTMATCH module=EBOOT.BIN start=0x00204ac8 end=0x00204c80 symbol=FUN_00204ac8 lang=c++ */
extern "C" {
int FUN_00091c48(int, int);
int FUN_00092ff8(int);
int FUN_00205428(int);
int FUN_000b42f8(int, int);
int FUN_00000a00(int);
extern int DAT_0034b340[];
}

extern "C" int FUN_00204ac8(int *param_1, short param_2)
{
    int *piVar2;

    if (param_1 != 0) {
        if (*(char *)(param_1 + 2) == 0) {
            piVar2 = (int *)DAT_0034b340[param_1[1]];
            if (piVar2 != 0) {
                (**(void (**)(int *, int))(*piVar2 + 8))(piVar2, 1);
                DAT_0034b340[param_1[1]] = 0;
            }
        }
        else if (*((char *)param_1 + 9) == 0) {
            piVar2 = (int *)DAT_0034b340[param_1[1]];
            if (piVar2 != 0) {
                if (piVar2[0x14] == 0) {
                    (**(void (**)(int *, int))(*piVar2 + 8))(piVar2, 1);
                    DAT_0034b340[param_1[1]] = 0;
                }
                else {
                    FUN_00091c48((int)piVar2, 0);
                    FUN_00092ff8(DAT_0034b340[param_1[1]]);
                }
            }
        }
        else {
            piVar2 = (int *)DAT_0034b340[param_1[1]];
            if (piVar2 != 0) {
                (**(void (**)(int *, int))(*piVar2 + 8))(piVar2, 1);
                DAT_0034b340[param_1[1]] = 0;
            }
        }
        FUN_00205428((int)param_1 + 0xc);
        if ((int)param_1 + 0xc != 0) {
            FUN_000b42f8((int)param_1 + 0xc, 0);
        }
        if (0 < param_2) {
            FUN_00000a00((int)param_1);
        }
    }
    return (int)param_1;
}
// NEAR: 102/110 words; 108 differ, first at +0x8. First pass from pseudocode + disassembly; vcall idiom used.
// near miss from lane oc-hst-20261121 (unreviewed)
