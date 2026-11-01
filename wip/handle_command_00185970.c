/* HSTMATCH module=EBOOT.BIN start=0x00185970 end=0x0018629c symbol=FUN_00185970 lang=c99 */
#pragma opt_strength_reduction off

typedef unsigned char undefined;
typedef unsigned int uint;
typedef unsigned int undefined4;
typedef unsigned char undefined1;

typedef void (*code)(void *, ...);

extern void FUN_00184afc(int, void *, void *, int);
extern int FUN_0016bc70();
extern void FUN_001872a0(int);
extern void FUN_000ffb3c(int, int);
extern void FUN_0010b980(int, int, int);
extern void FUN_0010b6d4(int, int, int);
extern void FUN_0018659c(int, int, char);
extern void FUN_001851e0(int, int, int, int, int, int, char, int, int);
extern void FUN_00186d64(int, int, int, int);
extern int FUN_0016bc8c();
extern int FUN_00216de8(int);
extern void FUN_000ffc70(int, int);
extern int FUN_00184dd8(int, int);
extern void FUN_00186a2c(int, int, int, int);
extern int FUN_001fea10(int, int, ...);
extern void FUN_00110a78(int, int, int);

void FUN_00185970(int param_1,int param_2,undefined4 param_3,undefined4 param_4,int param_5,
                 undefined4 param_6,undefined4 param_7,char param_8,undefined4 uStackX_0)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  int iVar6;
  undefined4 uVar7;
  int *piVar8;
  undefined4 *puVar9;
  int *piVar10;
  undefined4 *puVar11;
  undefined4 local_4;
  undefined4 local_8;
  
  local_4 = 0;
  local_8 = 0;
  FUN_00184afc(param_1,&local_4,&local_8,param_2);
  piVar8 = (int *)(param_1 + 0x1f74);
  iVar1 = FUN_0016bc70();
  iVar1 = *(int *)(*(int *)(iVar1 + 0x5c0) + param_2 * 4);
  switch (iVar1) {
  case 4:
    iVar6 = *(int *)(param_1 + 0x1f78) + 6;
    iVar1 = *piVar8;
    *(int *)(iVar1 + 0x24) = iVar6;
    *(int *)(iVar1 + 0x28) = iVar6;
    break;
  case 3:
    iVar6 = *(int *)(param_1 + 0x1f78) + 4;
    iVar1 = *piVar8;
    *(int *)(iVar1 + 0x24) = iVar6;
    *(int *)(iVar1 + 0x28) = iVar6;
    break;
  case 2:
    iVar1 = FUN_0016bc70();
    if (*(int *)(iVar1 + 0x5bc) == 5) {
      iVar6 = *(int *)(param_1 + 0x1f78) + 3;
    } else {
      iVar6 = *(int *)(param_1 + 0x1f78) + 5;
    }
    iVar1 = *piVar8;
    *(int *)(iVar1 + 0x24) = iVar6;
    *(int *)(iVar1 + 0x28) = iVar6;
    break;
  case 1:
    iVar6 = *(int *)(param_1 + 0x1f78) + 2;
    iVar1 = *piVar8;
    *(int *)(iVar1 + 0x24) = iVar6;
    *(int *)(iVar1 + 0x28) = iVar6;
    break;
  case 0:
    iVar6 = *(int *)(param_1 + 0x1f78) + 1;
    iVar1 = *piVar8;
    *(int *)(iVar1 + 0x24) = iVar6;
    *(int *)(iVar1 + 0x28) = iVar6;
    break;
  }
  FUN_001872a0(param_1);
  if (param_2 == 4) {
    (**(code **)(**(int **)(param_1 + 0x1f50) + 0xc))(*(int **)(param_1 + 0x1f50),0);
    (**(code **)(**(int **)(param_1 + 0x1f54) + 0xc))(*(int **)(param_1 + 0x1f54),1);
    (**(code **)(**(int **)(param_1 + 0x1f58) + 0xc))(*(int **)(param_1 + 0x1f58),1);
    FUN_000ffb3c(*(undefined4 *)(param_1 + 0x23fc),0);
    FUN_000ffb3c(*(undefined4 *)(param_1 + 0x2410),0);
    FUN_000ffb3c(*(undefined4 *)(param_1 + 0x2424),0);
    FUN_000ffb3c(*(undefined4 *)(param_1 + 0x2320),0);
    FUN_000ffb3c(*(undefined4 *)(param_1 + 0x2334),0);
    (**(code **)(**(int **)(param_1 + 0x28a8) + 0xc))(*(int **)(param_1 + 0x28a8),1);
    (**(code **)(**(int **)(param_1 + 0x2878) + 0xc))(*(int **)(param_1 + 0x2878),1);
    (**(code **)(**(int **)(param_1 + 0x2880) + 0xc))(*(int **)(param_1 + 0x2880),1);
    (**(code **)(**(int **)(param_1 + 0x2888) + 0xc))(*(int **)(param_1 + 0x2888),1);
    (**(code **)(**(int **)(param_1 + 0x2894) + 0xc))(*(int **)(param_1 + 0x2894),1);
    (**(code **)(**(int **)(param_1 + 0x28a0) + 0xc))(*(int **)(param_1 + 0x28a0),1);
    if (param_8 == '\0') {
      FUN_0010b6d4(*(undefined4 *)(param_1 + 0x285c),0,0);
      FUN_0010b980(*(undefined4 *)(param_1 + 0x285c),0,0);
    }
    else {
      FUN_0010b980(*(undefined4 *)(param_1 + 0x285c),4,0);
    }
    FUN_0018659c(param_1,4,param_8);
    goto LAB_00185e58;
  }
  (**(code **)(**(int **)(param_1 + 0x1f50) + 0xc))(*(int **)(param_1 + 0x1f50),1);
  (**(code **)(**(int **)(param_1 + 0x1f54) + 0xc))(*(int **)(param_1 + 0x1f54),0);
  (**(code **)(**(int **)(param_1 + 0x1f58) + 0xc))(*(int **)(param_1 + 0x1f58),0);
  FUN_000ffb3c(*(undefined4 *)(param_1 + 0x23fc),0);
  (**(code **)(**(int **)(param_1 + 0x285c) + 0xc))(*(int **)(param_1 + 0x285c),0);
  iVar6 = 10;
  iVar1 = param_1 + 200;
  do {
    FUN_000ffb3c(*(undefined4 *)(iVar1 + 0x1f74),1);
    iVar6 = iVar6 + 1;
    iVar1 = iVar1 + 0x14;
  } while (iVar6 < 0x2e);
  FUN_001851e0(param_1,param_2,param_3,param_4,param_5,param_6,param_8,local_8,uStackX_0);
  if (param_8 == '\0') {
    FUN_0010b6d4(*(undefined4 *)(param_1 + 0x285c),1,0xc);
    FUN_00186d64(param_1,param_3,param_4,param_5);
    uVar7 = *(undefined4 *)(param_1 + 0x2460);
  }
  else {
    FUN_0010b980(*(undefined4 *)(param_1 + 0x285c),param_2,0);
    uVar7 = *(undefined4 *)(param_1 + 0x2460);
  }
  FUN_000ffc70(uVar7,local_4);
  iVar1 = -param_5;
  *(int *)(*(int *)(param_1 + 0x2878) + 0xec) = iVar1 * *(int *)(*(int *)(param_1 + 0x2878) + 0xd8);
  *(int *)(*(int *)(param_1 + 0x2880) + 0xec) = iVar1 * *(int *)(*(int *)(param_1 + 0x2878) + 0xd8);
  *(int *)(*(int *)(param_1 + 0x28a8) + 0xec) = iVar1 * *(int *)(*(int *)(param_1 + 0x2878) + 0xd8);
  if (param_2 == 3) {
    iVar6 = *(int *)(param_1 + 0x28a0);
LAB_00185dbc:
    *(int *)(iVar6 + 0xec) = iVar1 * *(int *)(iVar6 + 0xd8);
LAB_00185dcc:
    iVar1 = *(int *)(param_1 + 0x2860);
  }
  else {
    if (param_2 == 2) {
      iVar6 = *(int *)(param_1 + 0x28a0);
      goto LAB_00185dbc;
    }
    if (param_2 == 1) {
      *(int *)(*(int *)(param_1 + 0x2894) + 0xec) =
           iVar1 * *(int *)(*(int *)(param_1 + 0x2894) + 0xd8);
      goto LAB_00185dcc;
    }
    if (param_2 == 0) {
      *(int *)(*(int *)(param_1 + 0x2888) + 0xec) =
           iVar1 * *(int *)(*(int *)(param_1 + 0x2888) + 0xd8);
      goto LAB_00185dcc;
    }
    iVar1 = *(int *)(param_1 + 0x2860);
  }
  *(undefined4 *)(iVar1 + 0xd8) = param_7;
  *(undefined4 *)(iVar1 + 0xdc) = 3;
  piVar8 = (int *)(param_1 + 0x2438);
  *(float *)(*(int *)(param_1 + 0x2860) + 0xe0) = (float)param_5;
  if (param_2 == 2) {
    iVar1 = FUN_0016bc70();
    if (*(int *)(iVar1 + 0x5bc) == 5) {
      iVar1 = *piVar8;
      iVar6 = *(int *)(param_1 + 0x243c) + 2;
      *(int *)(iVar1 + 0x24) = iVar6;
      *(int *)(iVar1 + 0x28) = iVar6;
    }
    else {
      iVar1 = *piVar8;
      iVar6 = *(int *)(param_1 + 0x243c) + 4;
      *(int *)(iVar1 + 0x24) = iVar6;
      *(int *)(iVar1 + 0x28) = iVar6;
    }
  }
  else {
    iVar1 = *piVar8;
    iVar6 = *(int *)(param_1 + 0x243c) + param_2;
    *(int *)(iVar1 + 0x24) = iVar6;
    *(int *)(iVar1 + 0x28) = iVar6;
  }
LAB_00185e58:
  piVar10 = (int *)(param_1 + 0x2410);
  piVar8 = (int *)(param_1 + 0x2424);
  iVar1 = FUN_0016bc70();
  if (*(int *)(iVar1 + 0x5bc) == 5) {
    FUN_000ffb3c(*(undefined4 *)(param_1 + 0x23fc),0);
    FUN_000ffb3c(*piVar10,0);
    FUN_000ffb3c(*piVar8,0);
    iVar1 = *piVar10;
    iVar6 = *(int *)(param_1 + 0x2414) + 1;
    *(int *)(iVar1 + 0x24) = iVar6;
    *(int *)(iVar1 + 0x28) = iVar6;
    iVar1 = *piVar8;
    iVar6 = *(int *)(param_1 + 0x2428) + 2;
    *(int *)(iVar1 + 0x24) = iVar6;
    *(int *)(iVar1 + 0x28) = iVar6;
  }
  else {
    FUN_000ffb3c(*(undefined4 *)(param_1 + 0x23fc),0);
    FUN_000ffb3c(*piVar10,1);
    FUN_000ffb3c(*piVar8,0);
    iVar1 = *piVar10;
    iVar6 = *(int *)(param_1 + 0x2414) + 1;
    *(int *)(iVar1 + 0x24) = iVar6;
    *(int *)(iVar1 + 0x28) = iVar6;
    iVar1 = *piVar8;
    iVar6 = *(int *)(param_1 + 0x2428) + 2;
    *(int *)(iVar1 + 0x24) = iVar6;
    *(int *)(iVar1 + 0x28) = iVar6;
  }
  iVar6 = 0;
  iVar1 = 0;
  do {
    puVar11 = (undefined4 *)(param_1 + iVar1 + 0x2348);
    puVar9 = (undefined4 *)(param_1 + iVar1 + 0x2398);
    iVar2 = FUN_0016bc70();
    if (iVar6 == 4) {
      puVar11 = (undefined4 *)0x0;
    }
    if (*(int *)(*(int *)(iVar2 + 0x5c0) + iVar6 * 4) == 0x7fffffff) {
      if (puVar11 == (undefined4 *)0x0) {
        uVar7 = *puVar9;
      }
      else {
        FUN_000ffb3c(*puVar11,1);
        uVar7 = *puVar9;
      }
      FUN_000ffb3c(uVar7,1);
    }
    else {
      FUN_000ffb3c(*puVar9,0);
      if (param_2 == iVar6) {
        if (puVar11 != (undefined4 *)0x0) {
          FUN_000ffb3c(*puVar11,0);
        }
        iVar2 = FUN_0016bc8c();
        uVar3 = FUN_00216de8(*(undefined4 *)(iVar2 + 0xa8));
        uVar4 = FUN_00216de8(*(undefined4 *)(iVar2 + 0xac));
        uVar5 = FUN_00216de8(*(undefined4 *)(iVar2 + 0xb0));
        FUN_000ffc70(*puVar9,uVar3 & 0xff |
                             (uVar5 & 0xff) << 0x10 | 0x80000000 | (uVar4 & 0xff) << 8);
      }
      else {
        if (puVar11 != (undefined4 *)0x0) {
          FUN_000ffb3c(*puVar11,1);
        }
        uVar7 = FUN_00184dd8(param_1,iVar6);
        FUN_000ffc70(*puVar9,uVar7);
      }
      if (puVar11 != (undefined4 *)0x0) {
        FUN_000ffc70(*puVar11,local_4);
      }
    }
    iVar6 = iVar6 + 1;
    iVar1 = iVar1 + 0x14;
  } while (iVar6 < 5);
  iVar6 = 0;
  iVar1 = param_1;
  do {
    puVar9 = (undefined4 *)(iVar1 + 0x280c);
    if (iVar6 == 3) {
      iVar2 = FUN_0016bc70();
      if (*(int *)(iVar2 + 0x5bc) == 5) {
        if (*(char *)(param_1 + 0x2a2f) != '\0') {
          FUN_000ffb3c(*puVar9,0);
          goto LAB_001860ec;
        }
        uVar7 = *puVar9;
      }
      else {
        uVar7 = *puVar9;
      }
      FUN_000ffb3c(uVar7,1);
    }
    else if (*(char *)(param_1 + iVar6 + 0x2a2c) == '\0') {
      FUN_000ffb3c(*puVar9,1);
    }
    else {
      FUN_000ffb3c(*puVar9,0);
    }
LAB_001860ec:
    iVar6 = iVar6 + 1;
    iVar1 = iVar1 + 0x14;
    if (3 < iVar6) {
      FUN_00186a2c(param_1,param_2,param_5,param_6);
      if (*(int *)(param_1 + 4) != 0) {
        if (param_2 == 4) {
          if (param_8 == '\0') {
            uVar7 = FUN_001fea10(0x12,0x1b,6);
            FUN_00110a78(*(undefined4 *)(param_1 + 4),uVar7,0);
          }
          else {
            uVar7 = FUN_001fea10(0x12,0x1b,5);
            FUN_00110a78(*(undefined4 *)(param_1 + 4),uVar7,0);
          }
        }
        else if (param_2 == 3) {
          uVar7 = FUN_001fea10(0x12,0x1b,3);
          FUN_00110a78(*(undefined4 *)(param_1 + 4),uVar7,0);
        }
        else if (param_2 == 2) {
          iVar1 = FUN_0016bc70();
          if (*(int *)(iVar1 + 0x5bc) == 5) {
            uVar7 = FUN_001fea10(0x12,0x1b,2);
            FUN_00110a78(*(undefined4 *)(param_1 + 4),uVar7,0);
          }
          else {
            uVar7 = FUN_001fea10(0x12,0x1b,4);
            FUN_00110a78(*(undefined4 *)(param_1 + 4),uVar7,0);
          }
        }
        else if (param_2 == 1) {
          uVar7 = FUN_001fea10(0x12,0x1b,1);
          FUN_00110a78(*(undefined4 *)(param_1 + 4),uVar7,0);
        }
        else if (param_2 == 0) {
          uVar7 = FUN_001fea10(0x12,0x1b,0);
          FUN_00110a78(*(undefined4 *)(param_1 + 4),uVar7,0);
        }
      }
      return;
    }
  } while( 1 );
}
// NEAR MISS: 594/587 words, 541 differ.
// What differs:
// 1. Switch case ordering: retail evaluates descending 4, 3, 2, 1, 0 with beql chain while MWCC sorts ascending
// 2. param_8 register allocation: retail keeps t3 in stack slot sb 67(sp) while ours allocates s2
// 3. Complex control-flow and register allocation across 2348-byte scene dispatch logic
// near miss from lane dsn5 (unreviewed)
