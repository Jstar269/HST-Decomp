/* HSTMATCH module=EBOOT.BIN start=0x001826b8 end=0x00182ac8 symbol=FUN_001826b8 lang=c99 */
/* Hand-written reconstruction from the pseudocode; partial draft (first loop only), see trailing note. */
#include "hst/types.h"
typedef int code();

extern int DAT_0034a77c;
extern int FUN_0016bc70();
extern int FUN_00193474();
extern void FUN_000d7d80();
extern int FUN_0024dd40();
extern void FUN_002045e8();
extern float FUN_002180a8();
extern int FUN_002181a4();
extern void FUN_000ffb3c();
extern int FUN_001fea10();
extern void FUN_0010d4a8();
extern void FUN_000d7e30();

void FUN_001826b8(int param_1, int param_2, int param_3, int param_4, int param_5, int param_6, int param_7)
{
  int iVar1;
  int iVar2;
  int uVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int *piVar7;
  int iVar8;
  int iVar10;
  int uVar11;
  int *piVar12;
  int *piVar9;
  float fVar13;
  float fVar14;
  int local_268[4];
  u8 auStack_258[200];
  u8 auStack_190[200];
  u8 auStack_c8[200];

  iVar1 = FUN_0016bc70();
  iVar1 = (**(code **)(iVar1 + 1000))(param_2);
  iVar2 = FUN_0016bc70();
  uVar11 = *(int *)(iVar2 + 0x8c);
  iVar2 = FUN_00193474(param_2);
  FUN_000d7d80(auStack_c8);
  FUN_000d7d80(auStack_190);
  FUN_000d7d80(auStack_258);
  uVar3 = FUN_0024dd40(*(int *)(iVar1 + 4), DAT_0034a77c);
  FUN_002045e8(auStack_c8, param_2, param_3, param_4, param_5, param_6, param_7, uVar3, uVar11);
  uVar3 = FUN_0024dd40(*(int *)(iVar1 + 4), DAT_0034a77c);
  FUN_002045e8(auStack_258, param_2, param_3, param_4, param_5, param_6, 0, uVar3, uVar11);
  FUN_002045e8(auStack_190, param_2, param_3, param_4, param_5, param_6, 0, 0, uVar11);
  iVar10 = 0;
  iVar8 = 0;
  iVar1 = param_1;
  do {
    piVar7 = (int *)(param_1 + iVar8 + 0xddc);
    piVar12 = (int *)(param_1 + iVar8 + 0xe40);
    fVar13 = FUN_002180a8(param_2, 0, iVar2 + 0x68, iVar10, auStack_190);
    fVar14 = FUN_002180a8(param_2, 0, iVar2 + 0x68, iVar10, 0);
    uVar3 = FUN_002180a8(param_2, 0, iVar2 + 0x68, iVar10, auStack_258);
    uVar11 = FUN_002180a8(param_2, 0, iVar2 + 0x68, iVar10, auStack_c8);
    *(float *)(iVar1 + 0x118c) = fVar13;
    *(float *)(iVar1 + 0x11b4) = fVar13 - fVar14;
    *(int *)(iVar1 + 0x11dc) = uVar3;
    *(int *)(iVar1 + 0x1204) = uVar11;
    iVar4 = FUN_002181a4(param_2, 0, iVar2 + 0x68, iVar10, 0);
    iVar5 = FUN_002181a4(param_2, 0, iVar2 + 0x68, iVar10, auStack_c8);
    FUN_000ffb3c((void *)*piVar7, 0);
    iVar6 = *piVar7;
    iVar4 = *(int *)(param_1 + iVar8 + 0xde0) + iVar4;
    *(int *)(iVar6 + 0x24) = iVar4;
    *(int *)(iVar6 + 0x28) = iVar4;
    FUN_000ffb3c((void *)*piVar12, 0);
    iVar4 = *piVar12;
    iVar10 = iVar10 + 1;
    iVar5 = *(int *)(param_1 + iVar8 + 0xe44) + iVar5;
    *(int *)(iVar4 + 0x24) = iVar5;
    *(int *)(iVar4 + 0x28) = iVar5;
    iVar8 = iVar8 + 0x14;
    iVar1 = iVar1 + 8;
  } while (iVar10 < 5);
  FUN_002182a0(local_268, param_2, 0, iVar2 + 0x68);
  iVar2 = 0;
  iVar8 = 0;
  piVar7 = local_268;
  iVar1 = param_1;
  do {
    iVar4 = *piVar7;
    iVar10 = param_1 + iVar8;
    piVar9 = *(int **)(iVar1 + 0x1154);
    piVar12 = (int *)(iVar10 + 0x105c);
    if (iVar4 < 0 || 0xc < iVar4) {
      if (iVar4 < 0xe) {
        iVar10 = *piVar9;
      }
      else {
        if (iVar4 < 0x1b) {
          (**(code **)(*piVar9 + 0xc))(piVar9, 0);
          iVar4 = FUN_001fea10(0xb, 3, *piVar7 + -0xe);
          piVar9[0x3e] = iVar4;
          piVar9[0x4e] = 1;
          FUN_0010d4a8(piVar9);
          FUN_000ffb3c((void *)*piVar12, 0);
          iVar4 = *piVar12;
          iVar10 = *(int *)(iVar10 + 0x1060) + 1;
          *(int *)(iVar4 + 0x24) = iVar10;
          *(int *)(iVar4 + 0x28) = iVar10;
          goto LAB_00182a50;
        }
        iVar10 = *piVar9;
      }
      (**(code **)(iVar10 + 0xc))(piVar9, 1);
      FUN_000ffb3c((void *)*piVar12, 1);
    }
    else {
      (**(code **)(*piVar9 + 0xc))(piVar9, 0);
      iVar4 = FUN_001fea10(0xb, 3, *piVar7);
      piVar9[0x3e] = iVar4;
      piVar9[0x4e] = 1;
      FUN_0010d4a8(piVar9);
      FUN_000ffb3c((void *)*piVar12, 0);
      uVar3 = *(int *)(iVar10 + 0x1060);
      iVar10 = *piVar12;
      *(int *)(iVar10 + 0x24) = uVar3;
      *(int *)(iVar10 + 0x28) = uVar3;
    }
LAB_00182a50:
    iVar2 = iVar2 + 1;
    iVar1 = iVar1 + 0xc;
    iVar8 = iVar8 + 0x14;
    piVar7 = piVar7 + 1;
    if (3 < iVar2) {
      FUN_000d7e30(auStack_258, 0xffffffff);
      FUN_000d7e30(auStack_190, 0xffffffff);
      FUN_000d7e30(auStack_c8, 0xffffffff);
      return;
    }
  } while (1);
}

// Full body now: prologue, the 5-iteration loop, and the unbounded do/while with the iVar4 range gate,
// the piVar9 vtable dispatch at +0xc, the FUN_001fea10(0xb, 3, x) calls, the 0x3e/0x4e stores and the
// LAB_00182a50 tail that tears the three buffers down once iVar2 > 3.
// near miss from lane dsn8 (unreviewed)
