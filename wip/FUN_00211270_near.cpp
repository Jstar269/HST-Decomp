/* HSTMATCH module=EBOOT.BIN start=0x00211270 end=0x00212098 symbol=FUN_00211270 lang=c++ */

extern "C" {
typedef unsigned char undefined1;
typedef unsigned short undefined2;
typedef unsigned int undefined4;
typedef unsigned int uint;
typedef int undefined;

extern int DAT_002c97d8;
extern int DAT_002efa88;
extern int DAT_002efa8c;
extern int DAT_002efa90;
extern int DAT_002efa94;
extern int DAT_002efa98;
extern int DAT_002efa9c;
extern int DAT_002efaa0;
extern int DAT_002efaa4;
extern int DAT_002efaa8;
extern int DAT_002efaac;
extern int DAT_002efab0;
extern int DAT_002efab4;
extern int DAT_002efab8;
extern int DAT_003386b0;
extern int DAT_00338b44;
extern int DAT_00341174;
extern int DAT_0034117c;
extern int *DAT_0034a6b8;

extern int FUN_0001342c(...);
extern int FUN_00022450(...);
extern int FUN_000a8494(...);
extern int FUN_00212098(...);
extern int FUN_00215630(...);
extern int FUN_00215a20(...);
extern int FUN_00215a58(...);
extern int FUN_00215ab8(...);
extern int FUN_00215af0(...);
extern int FUN_00215b34(...);
extern int FUN_00215c20(...);


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00211270(int param_1)

{
  char cVar1;
  short sVar2;
  bool bVar3;
  uint uVar4;
  int iVar5;
  uint *puVar6;
  undefined4 *puVar7;
  undefined4 uVar8;
  uint uVar9;
  int iVar10;
  int iVar11;
  float *pfVar12;
  int iVar13;
  float *pfVar14;
  int *piVar15;
  int iVar16;
  float fVar17;
  float fVar18;
  float fVar19;
  float fVar20;
  undefined4 local_c0;
  undefined4 local_bc;
  undefined4 local_b8;
  undefined4 local_b4;
  float local_b0;
  float local_ac;
  float local_a8;
  float local_a4;
  undefined4 local_a0;
  undefined4 local_9c;
  undefined4 local_98;
  undefined4 local_94;
  undefined4 local_90;
  undefined4 local_8c;
  undefined4 local_88;
  undefined4 local_84;
  float local_80 [12];
  undefined1 local_50 [44];
  int local_24;
  uint local_20;
  uint local_1c;
  uint local_18;
  float local_14;
  int local_10 [4];
  
  uVar9 = *(int *)(param_1 + 0x148) + 2;
  local_10[0] = param_1;
  uVar4 = uVar9 & 3;
  iVar11 = DAT_003386b0 + 0x70;
  if (((int)uVar9 < 0) && (uVar4 != 0)) {
    uVar4 = uVar4 - 4;
  }
  local_10[1] = *(int *)(iVar11 + uVar4 * 4);
  if (*(int *)(local_10[1] + 0x148) < *(int *)(param_1 + 0x148)) {
    local_10[0] = local_10[1];
    local_10[1] = param_1;
  }
  iVar16 = 0;
  piVar15 = local_10;
  uVar4 = *(uint *)(param_1 + 0x144) ^ 1;
  pfVar14 = local_80;
  local_10[2] = *(undefined4 *)(iVar11 + uVar4 * 4);
  local_10[3] = *(undefined4 *)(iVar11 + (uVar4 + 2) * 4);
  do {
    iVar11 = *piVar15;
    local_a0 = *(undefined4 *)(iVar11 + 0x5a0);
    local_9c = *(undefined4 *)(iVar11 + 0x5a4);
    local_98 = *(undefined4 *)(iVar11 + 0x5a8);
    local_94 = *(undefined4 *)(iVar11 + 0x5ac);
    FUN_00022450(&local_c0,iVar11 + 0x580,&local_a0);
    local_88 = local_b8;
    local_90 = local_c0;
    local_8c = local_bc;
    local_84 = local_b4;
    FUN_00022450(&local_b0,&local_90,param_1 + 0x5a0);
    *pfVar14 = local_b0;
    pfVar14[1] = local_ac;
    pfVar14[2] = local_a8;
    iVar16 = iVar16 + 1;
    pfVar14[3] = local_a4;
    piVar15 = piVar15 + 1;
    pfVar14 = pfVar14 + 4;
  } while (iVar16 < 4);
  iVar13 = 0;
  iVar16 = 0;
  iVar11 = 1;
  do {
    pfVar12 = local_80 + iVar11 * 4;
    pfVar14 = local_80 + iVar16 * 4;
    if (*pfVar14 < *pfVar12) {
      fVar18 = *pfVar14;
      fVar20 = local_80[iVar16 * 4 + 1];
      fVar17 = local_80[iVar16 * 4 + 2];
      fVar19 = local_80[iVar16 * 4 + 3];
      *pfVar14 = *pfVar12;
      local_80[iVar16 * 4 + 1] = local_80[iVar11 * 4 + 1];
      local_80[iVar16 * 4 + 2] = local_80[iVar11 * 4 + 2];
      iVar10 = local_10[iVar16];
      iVar5 = local_10[iVar11];
      local_80[iVar16 * 4 + 3] = local_80[iVar11 * 4 + 3];
      *pfVar12 = fVar18;
      local_80[iVar11 * 4 + 1] = fVar20;
      local_10[iVar16] = iVar5;
      local_80[iVar11 * 4 + 2] = fVar17;
      local_10[iVar11] = iVar10;
      local_80[iVar11 * 4 + 3] = fVar19;
    }
    iVar13 = iVar13 + 1;
    iVar16 = iVar16 + 2;
    iVar11 = iVar11 + 2;
  } while (iVar13 < 2);
  iVar11 = FUN_00212098(0xc0ccd35a,param_1,local_80,local_80 + 4);
  iVar16 = FUN_00212098(0x40ccd35a,param_1,local_80 + 8,local_50);
  local_24 = 0;
  local_20 = 0;
  local_1c = 0;
  if (DAT_0034a6b8[1] != 0) {
    FUN_00215b34(&local_24);
  }
  local_18 = 0xffffffff;
  iVar13 = FUN_00215ab8(param_1);
  fVar19 = *(float *)(iVar13 + 0x24);
  iVar13 = FUN_00215a20(param_1);
  fVar19 = fVar19 + *(float *)(iVar13 + 0x20c);
  local_14 = DAT_002efa94;
  if (DAT_002efa94 <= fVar19) {
    local_14 = fVar19;
  }
  if (local_20 < local_1c) {
    puVar6 = (uint *)(local_24 + local_20 * 8);
    if (puVar6 != (uint *)0x0) {
      *puVar6 = local_18;
      puVar6[1] = (uint)local_14;
    }
    local_20 = local_20 + 1;
  }
  else {
    FUN_00215af0(&local_24,&local_18,0);
  }
  if (iVar16 == -1) {
    local_18 = 0xfffffffe;
    local_14 = 10.0f;
    if (local_20 < local_1c) {
      puVar7 = (undefined4 *)(local_24 + local_20 * 8);
      if (puVar7 != (undefined4 *)0x0) {
        *puVar7 = 0xfffffffe;
        puVar7[1] = 0x41200000;
      }
      local_20 = local_20 + 1;
    }
    else {
      FUN_00215af0(&local_24,&local_18,0);
    }
  }
  else {
    bVar3 = local_10[0] != param_1;
    iVar13 = FUN_00215ab8(param_1);
    fVar20 = *(float *)(iVar13 + 0x20);
    iVar13 = FUN_00215a20(param_1);
    fVar20 = fVar20 + *(float *)(iVar13 + 0x208);
    fVar19 = DAT_002efa98;
    if (DAT_002efa98 <= fVar20) {
      fVar19 = fVar20;
    }
    uVar4 = 0;
    if (DAT_0034a6b8[1] != 0) {
      iVar13 = *DAT_0034a6b8;
      while( true ) {
        iVar13 = iVar13 + uVar4 * 0x1c;
        if (((*(int *)(iVar13 + 4) == iVar11) && (*(int *)(iVar13 + 8) == iVar16)) &&
           (*(uint *)(iVar13 + 0xc) == (uint)bVar3)) {
          local_14 = (float)*(int *)(iVar13 + 0x18) * fVar19;
          local_18 = uVar4;
          if (local_20 < local_1c) {
            puVar6 = (uint *)(local_24 + local_20 * 8);
            if (puVar6 != (uint *)0x0) {
              *puVar6 = uVar4;
              puVar6[1] = (uint)local_14;
            }
            local_20 = local_20 + 1;
          }
          else {
            FUN_00215af0(&local_24,&local_18,0);
          }
        }
        uVar4 = uVar4 + 1;
        if ((uint)DAT_0034a6b8[1] <= uVar4) break;
        iVar13 = *DAT_0034a6b8;
      }
    }
  }
  fVar19 = 0.0f;
  uVar4 = 0;
  if (local_20 != 0) {
    do {
      iVar11 = uVar4 * 8;
      uVar4 = uVar4 + 1;
      fVar19 = fVar19 + *(float *)(local_24 + iVar11 + 4);
    } while (uVar4 < local_20);
  }
  uVar4 = FUN_0001342c();
  uVar9 = 0;
  fVar19 = fVar19 * (float)(int)(uVar4 & 0xfffff800) * 4.656613e-10f;
  if (local_20 != 1) {
    iVar11 = 0;
    while ((fVar19 = fVar19 - *(float *)(local_24 + iVar11 + 4), 0.0f <= fVar19 &&
           (uVar9 = uVar9 + 1, uVar9 < local_20 - 1))) {
      iVar11 = uVar9 * 8;
    }
  }
  iVar11 = FUN_000a8494(param_1,param_1 + 0x580);
  if (iVar11 == 0) {
    iVar11 = FUN_00215a58(param_1);
    sVar2 = *(short *)(iVar11 + 0xa8);
    iVar11 = FUN_00215a20(param_1);
    fVar20 = (float)((int)sVar2 + (int)*(short *)(iVar11 + 0xa8));
    fVar19 = DAT_002efaa0;
    if (DAT_002efaa0 <= fVar20) {
      fVar19 = fVar20;
    }
  }
  else {
    iVar11 = FUN_00215a58(param_1);
    sVar2 = *(short *)(iVar11 + 0xaa);
    iVar11 = FUN_00215a20(param_1);
    fVar20 = (float)((int)sVar2 + (int)*(short *)(iVar11 + 0xaa));
    fVar19 = DAT_002efa9c;
    if (DAT_002efa9c <= fVar20) {
      fVar19 = fVar20;
    }
  }
  iVar16 = -1;
  iVar11 = *(int *)(local_24 + uVar9 * 8);
  fVar19 = 5.4863997f - fVar19 * 0.01f;
  if (iVar11 != -1) {
    if (iVar11 == -2) {
      *(undefined4 *)(param_1 + 0x8e0) = 0;
      if (0.0f <= local_80[8]) {
        iVar16 = 1;
      }
      uVar4 = FUN_0001342c();
      *(float *)(param_1 + 0x8f0) =
           fVar19 * (float)(int)(uVar4 & 0xfffff800) * 4.656613e-10f * -(float)iVar16;
      uVar4 = FUN_0001342c();
      *(float *)(param_1 + 0x8f8) = (float)(int)(uVar4 & 0xfffff800) * 4.656613e-10f * 11.887199f;
    }
    else {
      iVar11 = *DAT_0034a6b8 + iVar11 * 0x1c;
      *(undefined4 *)(param_1 + 0x8e0) = (&DAT_002c97d8)[*(int *)(iVar11 + 0x14)];
      uVar4 = *(int *)(iVar11 + 0x10) - 5;
      if ((uVar4 & 1) == 0) {
        fVar20 = 1.0f;
      }
      else {
        fVar20 = -1.0f;
      }
      uVar9 = FUN_0001342c();
      *(float *)(param_1 + 0x8f0) =
           (fVar19 * fVar20 - 0.0f) * 4.656613e-10f * (float)(int)(uVar9 & 0xfffff800) + 0.0f;
      if ((uVar4 & 2) == 0) {
        uVar4 = FUN_0001342c();
        *(float *)(param_1 + 0x8f8) = (float)(int)(uVar4 & 0xfffff800) * -3.4924597e-09f + 7.5f;
      }
      else {
        uVar4 = FUN_0001342c();
        *(float *)(param_1 + 0x8f8) = (float)(int)(uVar4 & 0xfffff800) * 2.042949e-09f + 7.5f;
      }
    }
    goto LAB_0021202c;
  }
  iVar16 = 0;
  iVar11 = 0;
  do {
    iVar13 = FUN_00215ab8(param_1);
    cVar1 = *(char *)(iVar11 + iVar13 + 0x3c);
    iVar13 = FUN_00215a20(param_1);
    fVar20 = (float)((int)cVar1 + (int)*(char *)(iVar11 + iVar13 + 0x224));
    fVar19 = DAT_002efaa4;
    if (DAT_002efaa4 <= fVar20) {
      fVar19 = fVar20;
    }
    iVar11 = iVar11 + 1;
    iVar16 = (int)((float)iVar16 + fVar19);
  } while (iVar11 < 3);
  uVar4 = FUN_0001342c();
  iVar11 = 0;
  iVar16 = (int)((float)iVar16 * (float)(int)(uVar4 & 0xfffff800) * 4.656613e-10f);
  do {
    iVar13 = FUN_00215ab8(param_1);
    cVar1 = *(char *)(iVar11 + iVar13 + 0x3c);
    iVar13 = FUN_00215a20(param_1);
    fVar20 = (float)((int)cVar1 + (int)*(char *)(iVar11 + iVar13 + 0x224));
    fVar19 = DAT_002efaa8;
    if (DAT_002efaa8 <= fVar20) {
      fVar19 = fVar20;
    }
    iVar16 = (int)((float)iVar16 - fVar19);
  } while ((-1 < iVar16) && (iVar11 = iVar11 + 1, iVar11 < 2));
  iVar16 = 0;
  iVar13 = 0;
  *(undefined4 *)(param_1 + 0x8e0) = (&DAT_002c97d8)[iVar11];
  do {
    iVar11 = FUN_00215ab8(param_1);
    cVar1 = *(char *)(iVar13 + iVar11 + 0x3f);
    iVar11 = FUN_00215a20(param_1);
    fVar20 = (float)((int)cVar1 + (int)*(char *)(iVar13 + iVar11 + 0x227));
    fVar19 = DAT_002efaac;
    if (DAT_002efaac <= fVar20) {
      fVar19 = fVar20;
    }
    iVar13 = iVar13 + 1;
    iVar16 = (int)((float)iVar16 + fVar19);
  } while (iVar13 < 3);
  uVar4 = FUN_0001342c();
  iVar16 = (int)((float)iVar16 * (float)(int)(uVar4 & 0xfffff800) * 4.656613e-10f);
  iVar11 = 0;
  do {
    iVar13 = iVar11;
    iVar11 = FUN_00215ab8(param_1);
    cVar1 = *(char *)(iVar13 + iVar11 + 0x3f);
    iVar11 = FUN_00215a20(param_1);
    fVar20 = (float)((int)cVar1 + (int)*(char *)(iVar13 + iVar11 + 0x227));
    fVar19 = DAT_002efab0;
    if (DAT_002efab0 <= fVar20) {
      fVar19 = fVar20;
    }
    iVar16 = (int)((float)iVar16 - fVar19);
    if (iVar16 < 0) {
      iVar13 = iVar13 + -1;
      break;
    }
    iVar11 = iVar13 + 1;
  } while (iVar13 + 1 < 2);
  iVar11 = 0;
  iVar16 = 0;
  *(float *)(param_1 + 0x8f8) = DAT_00341174 * 0.01f + DAT_0034117c * 0.01f * (float)iVar13;
  do {
    iVar13 = FUN_00215ab8(param_1);
    cVar1 = *(char *)(iVar16 + iVar13 + 0x42);
    iVar13 = FUN_00215a20(param_1);
    fVar20 = (float)((int)cVar1 + (int)*(char *)(iVar16 + iVar13 + 0x22a));
    fVar19 = DAT_002efab4;
    if (DAT_002efab4 <= fVar20) {
      fVar19 = fVar20;
    }
    iVar16 = iVar16 + 1;
    iVar11 = (int)((float)iVar11 + fVar19);
  } while (iVar16 < 3);
  uVar4 = FUN_0001342c();
  iVar16 = 0;
  iVar11 = (int)((float)iVar11 * (float)(int)(uVar4 & 0xfffff800) * 4.656613e-10f);
  do {
    iVar13 = FUN_00215ab8(param_1);
    cVar1 = *(char *)(iVar16 + iVar13 + 0x42);
    iVar13 = FUN_00215a20(param_1);
    fVar20 = (float)((int)cVar1 + (int)*(char *)(iVar16 + iVar13 + 0x22a));
    fVar19 = DAT_002efab8;
    if (DAT_002efab8 <= fVar20) {
      fVar19 = fVar20;
    }
    iVar11 = (int)((float)iVar11 - fVar19);
  } while ((-1 < iVar11) && (iVar16 = iVar16 + 1, iVar16 < 2));
  if (*(char *)(DAT_00338b44 + 0x45) == '\0') {
    fVar19 = 5.4863997f;
  }
  else {
    fVar19 = 4.1148f;
  }
  if ((float)(iVar16 + -1) == 0.0f) {
LAB_00211d54:
    iVar11 = FUN_000a8494(param_1,param_1 + 0x580);
    if (iVar11 == 0) {
      iVar11 = FUN_00215a58(param_1);
      sVar2 = *(short *)(iVar11 + 0xa8);
      iVar11 = FUN_00215a20(param_1);
      fVar17 = (float)((int)sVar2 + (int)*(short *)(iVar11 + 0xa8));
      fVar20 = DAT_002efa90;
      if (DAT_002efa90 <= fVar17) {
        fVar20 = fVar17;
      }
    }
    else {
      iVar11 = FUN_00215a58(param_1);
      sVar2 = *(short *)(iVar11 + 0xaa);
      iVar11 = FUN_00215a20(param_1);
      fVar17 = (float)((int)sVar2 + (int)*(short *)(iVar11 + 0xaa));
      fVar20 = DAT_002efa8c;
      if (DAT_002efa8c <= fVar17) {
        fVar20 = fVar17;
      }
    }
    fVar20 = (fVar19 - fVar20 * 0.01f) * 0.6666667f;
    fVar19 = -fVar20 * 0.5f;
    uVar4 = FUN_0001342c();
    fVar19 = (float)(iVar16 + -1) * fVar20 +
             fVar19 + (fVar20 * 0.5f - fVar19) * 4.656613e-10f * (float)(int)(uVar4 & 0xfffff800);
  }
  else {
    iVar11 = FUN_00215a58(param_1);
    fVar17 = *(float *)(iVar11 + 0xa4);
    iVar11 = FUN_00215a20(param_1);
    fVar17 = fVar17 + *(float *)(iVar11 + 0xa4);
    fVar20 = DAT_002efa88;
    if (DAT_002efa88 <= fVar17) {
      fVar20 = fVar17;
    }
    uVar4 = FUN_0001342c();
    if (fVar20 * 0.01f <= (float)(int)(uVar4 & 0xfffff800) * 4.656613e-10f) goto LAB_00211d54;
    fVar19 = fVar19 * (float)(iVar16 + -1);
  }
  *(float *)(param_1 + 0x8f0) = fVar19;
LAB_0021202c:
  uVar8 = FUN_00215630(param_1,*(undefined4 *)(param_1 + 0x8e0));
  *(undefined4 *)(param_1 + 0x8e0) = uVar8;
  *(float *)(param_1 + 0x8f4) = -*(float *)(*(int *)(DAT_003386b0 + 0x88) + 0x110);
  *(undefined4 *)(param_1 + 0x8fc) = 0x7f7fffff;
  FUN_00215c20(&local_24,0);
  return;
}


}

// Differing words: 882 / 906 words. First at +0x0.
// Ruled out:
// - Double float promotion: Ghidra printed unadorned float constants without 'f', which caused missing symbols _dpfeq _dpfle; adding 'f' suffix restored single-precision FP operations (..).
// - Stack frame: retail uses 240-byte frame (0xf0) with 192 bytes of stack locals (0xc0) and 3 float callee-saved registers (..), whereas individual scalar locals collapse the frame to 192 bytes.
// - Needs recovery of the ~192-byte stack structure and caller-saved register spill order across the 906-word body.
// near miss from lane dsn4 (unreviewed)
