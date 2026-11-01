/* HSTMATCH module=EBOOT.BIN start=0x002508b4 end=0x00250cb0 symbol=FUN_002508b4 lang=c99 */
/* Hand-written reconstruction from the pseudocode; measured draft, see the trailing note. */
#include "hst/types.h"

extern char DAT_002fbef9;
extern char DAT_002fbefa;
extern int DAT_002fc644;
extern int DAT_002fc674[];
extern int DAT_002fc6d4;
extern int DAT_002fc794;
extern int FUN_00250cc4();
extern int FUN_00193474();
extern int FUN_0016bb98();
extern void FUN_0010b980();
extern void FUN_000ffb3c();
extern int FUN_00250718();

void FUN_002508b4(int param_1)
{
  char cVar1;
  int iVar2;
  int iVar3;
  u32 uVar4;
  int *piVar5;
  int iVar6;
  int iVar7;
  int uVar8;
  int iVar9;
  int iVar10;
  int iVar11;

  iVar2 = FUN_00250cc4();
  if (iVar2 == 0) {
    uVar8 = **(int **)(*(int *)(param_1 + 0x350) + *(int *)(param_1 + 0x2fc) * 4);
  }
  else {
    uVar8 = **(int **)(*(int *)(param_1 + 0x350) + *(int *)(param_1 + 0x300) * 4);
  }
  iVar7 = *(int *)(param_1 + 0x35c);
  iVar11 = *(int *)(param_1 + 0x364);
  iVar2 = FUN_00193474(uVar8);
  iVar3 = FUN_00250cc4(param_1);
  cVar1 = DAT_002fbef9;
  if (iVar3 != 0) {
    cVar1 = DAT_002fbefa;
  }
  if (cVar1 == 0) {
    iVar2 = FUN_0016bb98(uVar8, iVar2 + 0x68);
    iVar3 = 0;
    if (iVar2 == 2) {
      iVar3 = 2;
    }
    else if (iVar2 == 3) {
      iVar3 = 1;
    }
    iVar2 = *(int *)(param_1 + 0x110);
    iVar6 = DAT_002fc644 + iVar3;
    *(int *)(iVar2 + 0x24) = iVar6;
    *(int *)(iVar2 + 0x28) = iVar6;
    uVar4 = FUN_00250cc4(param_1);
    iVar6 = *(int *)((char *)DAT_002fc674 + (uVar4 & 0xff) * 0x24);
    iVar2 = *(int *)((uVar4 & 0xff) * 0xc + param_1 + 0x120);
    *(int *)(iVar2 + 0x24) = iVar3 + iVar6;
    *(int *)(iVar2 + 0x28) = iVar3 + iVar6;
    iVar2 = *(int *)(param_1 + 0x35c);
  }
  else {
    iVar2 = *(int *)(param_1 + 0x35c);
  }
  FUN_0010b980(*(int *)(param_1 + 0x1d4), iVar2 + 6, 0);
  iVar3 = 0;
  iVar2 = param_1;
  do {
    iVar9 = *(int *)(iVar2 + 0x140);
    iVar6 = *(int *)(iVar2 + 0x180);
    if (iVar11 + iVar3 < *(int *)(param_1 + 0x308)) {
      piVar5 = *(int **)(*(int *)(param_1 + 0x350) + (iVar11 + iVar3) * 4);
      iVar10 = *piVar5;
      if (*(char *)((int)piVar5 + 10) == 0) {
        if ((char)piVar5[2] == 0) {
          FUN_000ffb3c(iVar6, 0);
          iVar10 = DAT_002fc794 + iVar10;
          *(int *)(iVar6 + 0x24) = iVar10;
          *(int *)(iVar6 + 0x28) = iVar10;
          if (iVar7 == iVar3) {
            FUN_000ffb3c(iVar9, 0);
            iVar6 = DAT_002fc6d4;
            *(int *)(iVar9 + 0x24) = DAT_002fc6d4;
            *(int *)(iVar9 + 0x28) = iVar6;
          }
          else {
            FUN_000ffb3c(iVar9, 0);
            iVar6 = DAT_002fc6d4 + 1;
            *(int *)(iVar9 + 0x24) = iVar6;
            *(int *)(iVar9 + 0x28) = iVar6;
          }
          iVar6 = FUN_00250cc4(param_1);
          if (iVar6 != 0 && *(int *)(param_1 + 0x360) == iVar3) {
            iVar6 = DAT_002fc6d4 + 4;
            *(int *)(iVar9 + 0x24) = iVar6;
            *(int *)(iVar9 + 0x28) = iVar6;
          }
        }
        else {
          FUN_000ffb3c(iVar6, 1);
          FUN_000ffb3c(iVar9, 0);
          iVar6 = DAT_002fc6d4 + 2;
          *(int *)(iVar9 + 0x24) = iVar6;
          *(int *)(iVar9 + 0x28) = iVar6;
        }
      }
      else {
        FUN_000ffb3c(iVar6, 0);
        iVar10 = DAT_002fc794 + 0x13;
        *(int *)(iVar6 + 0x24) = iVar10;
        *(int *)(iVar6 + 0x28) = iVar10;
        FUN_000ffb3c(iVar9, 0);
        iVar6 = DAT_002fc6d4;
        if (iVar7 == iVar3) {
          *(int *)(iVar9 + 0x24) = DAT_002fc6d4;
          *(int *)(iVar9 + 0x28) = iVar6;
        }
        else {
          iVar6 = DAT_002fc6d4 + 1;
          *(int *)(iVar9 + 0x24) = iVar6;
          *(int *)(iVar9 + 0x28) = iVar6;
        }
      }
    }
    else {
      FUN_000ffb3c(iVar6, 1);
      FUN_000ffb3c(iVar9, 0);
      iVar6 = DAT_002fc6d4 + 2;
      *(int *)(iVar9 + 0x24) = iVar6;
      *(int *)(iVar9 + 0x28) = iVar6;
    }
    iVar3 = iVar3 + 1;
    iVar2 = iVar2 + 4;
  } while (iVar3 < 0x10);
  iVar7 = *(int *)(param_1 + 0x364);
  iVar2 = FUN_00250718(param_1, iVar7, 0xffffffff, 0);
  iVar3 = FUN_00250718(param_1, iVar7 + 0xf, 1, 0);
  *(u8 *)(param_1 + 0x319) = 0;
  *(u8 *)(param_1 + 0x31a) = 0;
  if (*(int *)(param_1 + 0x308) < 0x11) {
    *(u8 *)(param_1 + 0x319) = 0;
    *(u8 *)(param_1 + 0x31a) = 0;
  }
  else if (iVar11 < 1) {
    *(u8 *)(param_1 + 0x319) = 0;
    *(u8 *)(param_1 + 0x31a) = 1;
  }
  else if (iVar11 + 0x10 < *(int *)(param_1 + 0x308)) {
    if (iVar7 == iVar2) {
      *(u8 *)(param_1 + 0x319) = 0;
    }
    else {
      *(u8 *)(param_1 + 0x319) = 1;
    }
    if (iVar7 + 0xf == iVar3) {
      *(u8 *)(param_1 + 0x31a) = 0;
    }
    else {
      *(u8 *)(param_1 + 0x31a) = 1;
    }
  }
  else {
    *(u8 *)(param_1 + 0x319) = 1;
    *(u8 *)(param_1 + 0x31a) = 0;
  }
}

// First measured draft. Banked facts to keep in mind: frame 48 bytes, no stack arguments.
// near miss from lane dsn8 (unreviewed)
