/* HSTMATCH module=EBOOT.BIN start=0x002a625c end=0x002a6664 symbol=FUN_002a625c lang=c++ */
/* Hand-written reconstruction from the pseudocode; measured draft, see the trailing note. */
#include "hst/types.h"

extern "C" {
extern int DAT_003415a8;
extern float DAT_00301040;
extern float DAT_00301044;
extern float DAT_00301048;
extern float DAT_0030104c;
extern float DAT_00301050;
extern float DAT_00301054;
extern float DAT_00301058;
extern int DAT_00300dac;
extern char DAT_00301038[];
extern int FUN_002af0c4(int);
extern float FUN_002af0b4(int);
extern int FUN_002af0cc(int);
extern int FUN_002af31c(int);
extern int FUN_002af32c(int, int);
extern float FUN_002af0dc(int);
extern int FUN_001bc630(float = 0.0f);
extern long long FUN_00020f5c(float);
extern void FUN_000143b0(void *, char *, int, int);

}
/* float clamp used by the sprite walk (reference recipe: this file must be C++) */
static inline float Clamp(const float &v, const float &lo, const float &hi)
{
  return v < lo ? lo : (hi < v ? hi : v);
}

extern "C" void FUN_002a625c(int param_1)
{
  short sVar1;
  int bVar2;
  int iVar3;
  int iVar4;
  int uVar5;
  float *pfVar6;
  int iVar7;
  int iVar8;
  u32 uVar9;
  int iVar10;
  int iVar11;
  int iVar12;
  int iVar13;
  int iVar14;
  float fVar15;
  float fVar16;
  float fVar17;
  float fVar18;
  long long uVar19;
  float local_8;
  float local_4;

  iVar11 = DAT_003415a8 + 0x87128;
  iVar3 = FUN_002af0c4(iVar11);
  local_4 = FUN_002af0b4(iVar11);
  if (local_4 < DAT_00301040) {
    pfVar6 = &DAT_00301040;
  }
  else {
    pfVar6 = &local_4;
  }
  *(float *)(param_1 + 0x16f4) = *pfVar6;
  iVar4 = FUN_002af0cc(iVar11);
  if (iVar4 < 0x65) {
    *(int *)(param_1 + 0x155c) = 0x40a00000;
  }
  uVar9 = (iVar4 < 0x65);
  iVar4 = FUN_002af31c(iVar11);
  iVar4 = iVar4 - 1;
  if (iVar4 < 0) {
    bVar2 = uVar9 < 2;
  }
  else {
    iVar14 = param_1 + uVar9 * 4;
    do {
      iVar7 = FUN_002af32c(iVar11, iVar4);
      if (*(char *)(iVar7 + 0xd8) != 0) {
        iVar7 = FUN_002af32c(iVar11, iVar4);
        iVar12 = *(int *)(iVar7 + 4);
        iVar7 = FUN_002af32c(iVar11, iVar4);
        local_4 = *(float *)(iVar7 + iVar12 * 0x34 + 0x20);
        if (*(float *)(param_1 + 0x16f4) < local_4) {
          *(float *)(param_1 + 0x16f4) = local_4;
          *(float *)(iVar14 + 0x155c) = local_4;
        }
        else {
          *(float *)(iVar14 + 0x155c) = local_4;
        }
        iVar14 = iVar14 + 4;
        uVar9 = uVar9 + 1;
      }
      iVar4 = iVar4 - 1;
    } while (-1 < iVar4);
    bVar2 = (int)uVar9 < 2;
  }
  if (bVar2) {
    iVar4 = param_1 + uVar9 * 4;
    do {
      uVar5 = FUN_002af0b4(iVar11);
      uVar9 = uVar9 + 1;
      *(int *)(iVar4 + 0x155c) = uVar5;
      iVar4 = iVar4 + 4;
    } while ((int)uVar9 < 2);
    *(u32 *)(param_1 + 0x16f0) = uVar9;
  }
  else {
    *(u32 *)(param_1 + 0x16f0) = uVar9;
  }
  uVar5 = FUN_001bc630(*(float *)(param_1 + 0x16f4));
  *(int *)(param_1 + 0x16f8) = uVar5;
  local_8 = FUN_002af0dc(iVar11);
  if (local_8 < DAT_00301044) {
    pfVar6 = &DAT_00301044;
  }
  else {
    pfVar6 = &local_8;
  }
  *(float *)(param_1 + 0x16f4) = *pfVar6;
  uVar5 = FUN_001bc630();
  *(int *)(param_1 + 0x16f8) = uVar5;
  uVar19 = FUN_00020f5c(*(float *)(param_1 + 0x16f4));
  FUN_000143b0((void *)(param_1 + 0x17d), DAT_00301038, (int)uVar19, (int)((unsigned long long)uVar19 >> 32));
  iVar11 = *(int *)(param_1 + 0x84);
  iVar4 = DAT_00300dac + *(int *)(param_1 + 0x16f8);
  *(int *)(iVar11 + 0x24) = iVar4;
  *(int *)(iVar11 + 0x28) = iVar4;
  if (iVar3 < 10) {
    *(u8 *)(param_1 + 0x1558) = 0;
  }
  else {
    *(u8 *)(param_1 + 0x1558) = 1;
    iVar3 = *(int *)(param_1 + 0x90);
    iVar11 = *(u32 *)(param_1 + 0x16f0);
    sVar1 = *(short *)(iVar3 + 4);
    iVar14 = (int)*(short *)(iVar3 + 10);
    iVar4 = (short)(*(short *)(iVar3 + 8) - *(short *)(iVar3 + 4)) + -3;
    iVar3 = 0;
    if (0 < iVar11 + -1) {
      fVar15 = DAT_00301048;
      iVar10 = 0;
      iVar8 = 0;
      iVar7 = param_1;
      iVar12 = param_1;
      do {
        fVar16 = (*(float *)(iVar12 + 0x155c) * 1.0f) / 12.0f;
        fVar18 = DAT_0030104c;
        if (DAT_0030104c <= fVar16 && fVar16 <= DAT_00301050) {
          fVar18 = Clamp(fVar16, DAT_0030104c, DAT_00301050);
        }
        fVar17 = (*(float *)(iVar12 + 0x1560) * 1.0f) / 12.0f;
        fVar16 = DAT_00301054;
        if (DAT_00301054 <= fVar17 && fVar17 <= DAT_00301058) {
          fVar16 = Clamp(fVar17, DAT_00301054, DAT_00301058);
        }
        iVar13 = (int)sVar1 + iVar10 / (iVar11 + -1);
        iVar11 = (int)sVar1 + (iVar8 + iVar4) / (iVar11 + -1);
        if (iVar3 == 8) {
          iVar11 = iVar11 + -1;
          *(int *)(iVar7 + 0x898) = iVar13;
        }
        else {
          if (iVar3 == 9) {
            iVar13 = iVar13 + -1;
            *(int *)(iVar7 + 0x898) = iVar13;
          }
          else {
            *(int *)(iVar7 + 0x898) = iVar13;
          }
        }
        *(int *)(iVar7 + 0x89c) = iVar13;
        *(int *)(iVar7 + 0x8a0) = iVar11;
        *(int *)(iVar7 + 0x8a4) = iVar11;
        *(int *)(iVar7 + 0x8a8) = iVar14 - (int)(fVar15 * fVar18);
        *(int *)(iVar7 + 0x8ac) = iVar14;
        *(int *)(iVar7 + 0x8b0) = iVar14;
        *(int *)(iVar7 + 0x8b4) = iVar14 - (int)(fVar15 * fVar16);
        iVar11 = *(u32 *)(param_1 + 0x16f0);
        iVar3 = iVar3 + 1;
        iVar12 = iVar12 + 4;
        iVar10 = iVar10 + iVar4;
        iVar8 = iVar8 + iVar4;
        iVar7 = iVar7 + 0x20;
      } while (iVar3 < iVar11 + -1);
    }
  }
}

// First measured draft. Retail facts to compare against: frame 64 bytes, ra at 44(sp), no stack args.
// near miss from lane dsn8 (unreviewed)
