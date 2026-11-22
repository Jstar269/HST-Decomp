/* HSTMATCH module=EBOOT.BIN start=0x001faa10 end=0x001fabb4 symbol=FUN_001faa10 lang=c++ */
extern "C" {
int FUN_0016bb88(void);
int FUN_0022c484(void);
void FUN_0015d1e8(int);
int FUN_002acd28(int);
int FUN_0024e064(void);
int FUN_0024e230(int, int, int, int);
int FUN_002ada80(int);
int FUN_0015d3fc(void);
extern int DAT_0034a678;
}

extern "C" void FUN_001faa10(int param_1)
{
    int iVar1;
    int iVar6;
    int bVar1;
    int iVar2;
    int bVar2;
    int iVar3;
    unsigned char *pbVar4;
    int uVar5;

    iVar2 = FUN_0016bb88();
    if (DAT_0034a678 == 0x2a && *(char *)(param_1 + 0x521) == 0 && 2 < FUN_0022c484()) {
        FUN_0015d1e8(0);
    }
    bVar1 = 1;
    iVar1 = 0;
    do {
        iVar3 = FUN_002acd28(iVar1);
        if (iVar3 != 0) {
            if (iVar1 < 0) {
                pbVar4 = (unsigned char *)0x0;
            }
            else if (iVar1 < 0x28) {
                pbVar4 = (unsigned char *)(iVar2 + 0x1058);
            }
            else {
                pbVar4 = (unsigned char *)0x0;
            }
            if ((unsigned)(*pbVar4 - 4) >= 2) {
                bVar1 = 0;
            }
        }
        iVar1 = iVar1 + 1;
        iVar2 = iVar2 + 0x14;
    } while (iVar1 < 0x28);
    if (bVar1) {
        FUN_0015d1e8(3);
    }
    bVar2 = 1;
    iVar2 = 0;
    iVar6 = 0;
    do {
        do {
            uVar5 = FUN_0024e064();
            uVar5 = FUN_0024e230(uVar5, iVar2 != 0, iVar6, 3);
            iVar3 = FUN_002ada80(uVar5);
            if (iVar3 == 3) {
                iVar6 = iVar6 + 1;
            }
            else if (iVar3 == 4) {
                /* v0 == 4: ok, nothing */
            }
            else {
                bVar2 = 0;
                if (iVar3 != 5) {
                    iVar6 = iVar6 + 1;
                }
            }
        } while (iVar6 < 0xf);
        iVar2 = iVar2 + 1;
        iVar6 = 0;
    } while (iVar2 < 2);
    if (bVar2) {
        FUN_0015d1e8(4);
    }
    if (FUN_0015d3fc() != 0) {
        *(unsigned char *)(param_1 + 0x5a2) = 1;
        *(unsigned char *)(param_1 + 0x59f) = 0;
        *(unsigned char *)(param_1 + 0x5a0) = 0;
        *(unsigned char *)(param_1 + 0x5a1) = 0;
    }
}
// near miss from lane oc-hst-20261121 (unreviewed)
