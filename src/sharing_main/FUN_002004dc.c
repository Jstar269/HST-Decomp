/* HSTMATCH module=hstp_trans_main.prx start=0x002004dc end=0x00200668 symbol=FUN_002004dc lang=c99 */
/* Hand-written reconstruction: clone of the source of FUN_001f20d8.c in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction (external matching lane); exact match verified; names and types await human review. */
extern int DAT_003ea8ec;
extern void FUN_001f5d60(int);
extern int FUN_0020d1e0(int, int, int);
extern void FUN_0011e940(int, int, int, int, int);
extern void FUN_00111aa4(int, int, int);
extern void FUN_0020a040(int, int, int);
extern int FUN_001204dc(int);
extern void FUN_00108f54(int, int);
extern int FUN_001205f0(int);
extern void FUN_00207b70(int);
extern void FUN_002031c4(int);
extern void FUN_00202dcc(int);

void FUN_002004dc(char *param_1)
{
    int n;

    if (*(int *)(param_1 + 0x44) == 0) {
        FUN_001f5d60(*(int *)(param_1 + 0x510));
        FUN_0011e940(DAT_003ea8ec, 1, FUN_0020d1e0(0x11, 0x12, 0), 0, 0);
        FUN_00111aa4(*(int *)(*(int *)(DAT_003ea8ec + 0x58)), 0, 0);
        *(int *)(param_1 + 0x8190) = 0;
        *(int *)(param_1 + 0x8194) = 0;
    }
    n = *(int *)(param_1 + 0x8190);
    switch (n) {
    case 0:
        *(int *)(param_1 + 0x8190) = 1;
        break;
    case 1:
        if (FUN_001204dc(DAT_003ea8ec) != 0) {
            FUN_00108f54(DAT_003ea8ec, 1);
            *(char *)(param_1 + 0x8198) = 1;
            if (FUN_001205f0(DAT_003ea8ec) == 0) {
                *(int *)(param_1 + 0x8190) = 2;
                *(int *)(param_1 + 0x8194) = 0;
            } else {
                *(char *)(param_1 + 0x517) = 1;
                FUN_00207b70((int)param_1);
            }
        }
        break;
    case 2:
        n = *(int *)(param_1 + 0x8194) + 1;
        *(int *)(param_1 + 0x8194) = n;
        if (n > 5)
            FUN_0020a040((int)param_1 + 0x3c, 0x13, 1);
        break;
    }
    FUN_002031c4((int)param_1);
    FUN_00202dcc((int)param_1);
}