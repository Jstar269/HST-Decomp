/* HSTMATCH module=EBOOT.BIN start=0x00123c94 end=0x001240fc symbol=FUN_00123c94 lang=c99 */
/* Hand-written: two camera vectors, the DAT_002da1ac wait loop, four-word face table pick. */

extern void  FUN_00216208(int a);
extern int   FUN_000defb4(void *p);
extern int   FUN_000dbc04(void *p);
extern void  FUN_00131198(float *out, void *src, int a, int b);
extern int   FUN_00131b54(void *p, int a, int b);
extern int   FUN_000b35c4(void *p);
extern int   FUN_001318dc(void *p, int a);
extern void  FUN_00132124(int a, int *out);
extern int   FUN_001252fc(int a, int b);
extern void  FUN_001258bc(int a, int b);
extern void  FUN_0001f1fc(int a, int b);

extern char  DAT_00338b44[];
extern char  DAT_003386b0[];
extern char  DAT_00310fc8[];
extern int   DAT_002da1ac;
extern int   DAT_002da1c8;
extern float DAT_002da1b8;
extern float DAT_002da1bc;
extern float DAT_002da1c0;
extern float DAT_002da1c4;
extern int   DAT_00340284[];

void FUN_00123c94(int param_1, int param_2)
{
  float local_40[4];
  char auStack_30[8];
  float local_28;
  int local_14;
  float local_10[4];
  int iVar1;
  int iVar4;
  unsigned int uVar3;
  unsigned int uVar5;
  int *piVar2;

  uVar3 = *(unsigned int *)(DAT_00340284 + 0x130);
  *(int *)(param_1 + 0x60) = 0;
  *(int *)(param_1 + 100) = 0;
  uVar5 = 0;
  FUN_00216208(*(int *)(param_1 + 0x54));
  if (*(int *)(DAT_00338b44 + 0x9c) == 0) {
    *(int *)(param_1 + 0x98) = 0;
  }
  else {
    if (*(int *)(DAT_00338b44 + 0xa0) != 0) {
      if (*(int *)(DAT_00338b44 + 0xa4) != 0) {
        if (*(int *)(DAT_00338b44 + 0xa8) != 0) {
          if (*(int *)(DAT_00338b44 + 0x9c) == 1) {
            *(int *)(param_1 + 0x98) = 0;
          }
          else {
            if (*(int *)(DAT_00338b44 + 0xa0) != 1) {
              if (*(int *)(DAT_00338b44 + 0xa4) != 1) {
                if (*(int *)(DAT_00338b44 + 0xa8) != 1) goto LAB_00123d40;
              }
            }
            *(int *)(param_1 + 0x98) = 0;
          }
          uVar5 = 1;
          goto LAB_00123d40;
        }
      }
    }
    *(int *)(param_1 + 0x98) = 0;
  }
  uVar5 = 0;
LAB_00123d40:
  iVar1 = FUN_000defb4(DAT_00338b44);
  if (iVar1 == 0) {
    iVar1 = FUN_000dbc04(DAT_00338b44);
    if (iVar1 == 0) {
      if (*(char *)(DAT_00340284 + 0xf1) == '\0') {
        if (*(char *)(DAT_00338b44 + 0x84) == '\0') {
          if (*(char *)(DAT_00338b44 + 0x85) == '\0') {
            *(int *)(param_1 + 0x98) = 0;
            uVar5 = uVar3;
            goto LAB_00123dfc;
          }
        }
      }
    }
  }
  if (*(int *)(DAT_00338b44 + 0xa0) != 0) {
    if (*(int *)(DAT_00338b44 + 0xa4) != 0) {
      if (*(int *)(DAT_00338b44 + 0xa8) != 0) {
        if (*(int *)(DAT_00338b44 + 0xa0) != 1) {
          if (*(int *)(DAT_00338b44 + 0xa4) != 1) {
            if (*(int *)(DAT_00338b44 + 0xa8) != 1) {
              *(unsigned int *)(param_1 + 0x98) = *(int *)(param_1 + 0x98) != 0 ^ 1;
              uVar5 = uVar3;
            }
          }
        }
      }
    }
  }
LAB_00123dfc:
  if (param_2 != 0) {
    *(int *)(DAT_00340284 + 0x128) = 0;
    FUN_00131198((float *)auStack_30, DAT_00340284, 5, -1);
    FUN_00131198(local_40, DAT_00340284, 0xe, -1);
    if (local_28 * local_40[0] < 0.0f) {
      uVar5 = 1;
    }
    else {
      uVar5 = 0;
    }
  }
  *(unsigned int *)(DAT_00340284 + 0x128) = uVar5;
  local_10[0] = DAT_002da1b8;
  local_10[1] = DAT_002da1bc;
  local_10[2] = DAT_002da1c0;
  local_10[3] = DAT_002da1c4;
  if (*(char *)(DAT_00338b44 + 0x45) == '\0') {
    if (param_2 == 0) {
      iVar4 = *(int *)DAT_00310fc8;
      iVar4 = iVar4 * 0x343fd + 0x269ec3;
      *(int *)DAT_00310fc8 = iVar4;
      uVar3 = uVar5 & 1;
      if ((int)uVar5 < 0) {
        if (uVar3 != 0) {
          uVar3 = uVar3 - 2;
        }
      }
      *(int *)(DAT_00340284 + 300) =
        (int)local_10[uVar3 + (((unsigned int)iVar4 & 0x7fffffff) >> 0x10 & 1) * 2];
    }
    else {
      *(int *)(DAT_00340284 + 300) = (int)local_10[uVar5];
    }
  }
  else {
    *(int *)(DAT_00340284 + 300) = (int)local_10[uVar5];
  }
  iVar4 = *(int *)DAT_00310fc8 * 0x343fd + 0x269ec3;
  *(int *)DAT_00310fc8 = iVar4;
  *(unsigned int *)(param_1 + 0x9c) = (unsigned int)(((unsigned int)iVar4 & 0x7fffffff) >> 0x10) % 100 < 0x32;
  iVar4 = *(int *)(param_1 + 0x98) * 10 + 100;
  iVar1 = FUN_00131b54(DAT_00340284, 8, 0);
  if (iVar1 == 0) {
    iVar4 = 200;
  }
  else {
    iVar1 = FUN_000b35c4(*(void **)(uVar5 * 4 + (int)DAT_003386b0 + 0x70));
    if (iVar1 != 0) {
      iVar4 = 200;
    }
    else {
      iVar1 = FUN_001318dc(DAT_00340284, 5);
      if (iVar1 != 0) {
        iVar4 = 200;
      }
    }
  }
  if (param_2 != 0) {
    iVar4 = 300;
  }
  FUN_00132124(iVar4, &local_14);
  iVar1 = 0;
  do {
    iVar4 = FUN_001252fc(param_1, local_14);
    iVar1 = iVar1 + 1;
    if (DAT_002da1ac != iVar4) break;
  } while (iVar1 < 100);
  *(int *)(param_1 + 0x58) = -1;
  FUN_001258bc(param_1, iVar4);
  DAT_002da1ac = iVar4;
  *(int *)(param_1 + 0x90) = 1;
  if (param_2 == 0) {
    if (*(int *)(param_1 + 0x90) < DAT_002da1c8) {
      piVar2 = &DAT_002da1c8;
    }
    else {
      piVar2 = (int *)(param_1 + 0x90);
    }
    *(float *)(param_1 + 100) = (float)*piVar2;
  }
  else {
    *(int *)(param_1 + 100) = 0x3f800000;
  }
  FUN_0001f1fc(param_1, 1);
  *(char *)(param_1 + 0x78) = 1;
}
// near miss from lane dsn8 (unreviewed)
