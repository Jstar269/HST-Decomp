/* HSTMATCH module=EBOOT.BIN start=0x0023028c end=0x002310d0 symbol=FUN_0023028c lang=c++ */

extern "C" {
typedef unsigned char undefined1;
typedef unsigned short undefined2;
typedef unsigned int undefined4;
typedef unsigned int uint;
typedef int undefined;
extern "C" float fabsf(float);
#define ABS(x) fabsf(x)

extern int DAT_002f2fb4;
extern int DAT_002f3330;
extern int DAT_002f3364;
extern int DAT_002f337c;
extern int DAT_002f3388;
extern int DAT_002f33b8;
extern int DAT_002f3478;
extern int DAT_002f347c;
extern int DAT_002f3538;
extern int DAT_002f3640;
extern int DAT_002f3644;
extern int DAT_002f364c;
extern int DAT_003415a8;

extern int FUN_0000bf34(...);
extern int FUN_000143b0(...);
extern int FUN_00022498(...);
extern int FUN_00054554(...);
extern int FUN_000d5304(...);
extern int FUN_000fe430(...);
extern int FUN_000ffb3c(...);
extern int FUN_0010b8cc(...);
extern int FUN_0010d4a8(...);
extern int FUN_00110a78(...);
extern int FUN_0015e268(...);
extern int FUN_001b1c44(...);
extern int FUN_001fea10(...);
extern int FUN_0022e020(...);


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0023028c(int param_1)

{
  char cVar1;
  short sVar2;
  bool bVar3;
  undefined4 uVar4;
  short sVar5;
  int iVar6;
  char cVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  int iVar12;
  undefined4 *puVar13;
  char cVar14;
  float fVar15;
  float fVar16;
  undefined4 uVar17;
  undefined4 local_160;
  undefined4 local_15c;
  undefined4 local_158;
  undefined4 local_154;
  undefined4 local_150;
  undefined4 local_14c;
  undefined4 local_148;
  undefined4 local_144;
  undefined4 local_140;
  undefined4 local_13c;
  undefined4 local_138;
  undefined4 local_134;
  undefined4 local_130;
  undefined4 local_12c;
  undefined4 local_128;
  undefined4 local_124;
  undefined4 local_120;
  undefined4 local_11c;
  undefined4 local_118;
  undefined4 local_114;
  undefined4 local_110;
  undefined4 local_10c;
  undefined4 local_108;
  undefined4 local_104;
  undefined4 local_100;
  undefined4 local_fc;
  undefined4 local_f8;
  undefined4 local_f4;
  undefined4 local_f0;
  undefined4 local_ec;
  undefined4 local_e8;
  undefined4 local_e4;
  undefined4 local_e0;
  undefined4 local_dc;
  undefined4 local_d8;
  undefined4 local_d4;
  undefined4 local_d0;
  undefined4 local_cc;
  undefined4 local_c8;
  undefined4 local_c4;
  undefined4 local_c0;
  undefined4 local_bc;
  undefined4 local_b8;
  undefined4 local_b4;
  undefined4 local_b0;
  undefined4 local_ac;
  undefined4 local_a8;
  undefined4 local_a4;
  undefined4 local_a0;
  undefined4 local_9c;
  undefined4 local_98;
  undefined4 local_94;
  undefined4 local_90;
  undefined4 local_8c;
  undefined4 local_88;
  undefined4 local_84;
  undefined4 local_80;
  undefined4 local_7c;
  undefined4 local_78;
  undefined4 local_74;
  undefined4 local_70;
  undefined4 local_6c;
  undefined4 local_68;
  undefined4 local_64;
  undefined4 local_60;
  undefined4 local_5c;
  undefined4 local_58;
  undefined4 local_54;
  undefined4 local_50;
  undefined4 local_4c;
  undefined4 local_48;
  undefined4 local_44;
  undefined4 local_40;
  undefined4 local_3c;
  undefined4 local_38;
  undefined4 local_34;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  char local_20;
  char local_1c;
  undefined4 local_18;
  undefined4 local_14;
  undefined4 local_10;
  int local_c;
  int local_8;
  undefined4 local_4;
  
  iVar11 = *(int *)(param_1 + 0x228);
  iVar9 = *(int *)(param_1 + 0x224);
  FUN_0022e020();
  if (iVar11 == *(int *)(param_1 + 0x228)) {
    iVar11 = *(int *)(param_1 + 0x224);
  }
  else {
    FUN_001b1c44(0x34179c,1);
    if (*(int *)(param_1 + 0x214) != 0) {
      if (*(char *)(param_1 + 0x54) == '\0') {
        iVar11 = *(int *)(&DAT_002f3330 + *(int *)(param_1 + 0x228) * 4);
      }
      else {
        iVar11 = *(int *)(&DAT_002f3364 + *(int *)(param_1 + 0x228) * 4);
      }
      if (*(char *)(iVar11 + param_1 + 0x280) == '\0') {
        uVar17 = FUN_001fea10(0x12,0x14,0);
        FUN_00110a78(*(undefined4 *)(param_1 + 0x214),uVar17,0);
      }
      else if (iVar11 == 2) {
        uVar17 = FUN_001fea10(0x12,0x13,
                              *(undefined4 *)(&DAT_002f33b8 + *(int *)(param_1 + 0x24c) * 4));
        FUN_00110a78(*(undefined4 *)(param_1 + 0x214),uVar17,0);
      }
      else {
        uVar17 = FUN_001fea10(0x12,0x13,*(undefined4 *)(&DAT_002f337c + iVar11 * 4));
        FUN_00110a78(*(undefined4 *)(param_1 + 0x214),uVar17,0);
      }
    }
    iVar11 = *(int *)(param_1 + 0x224);
  }
  if (iVar9 == iVar11) {
    iVar9 = *(int *)(param_1 + 0x3fc);
  }
  else {
    FUN_0010b8cc(*(undefined4 *)(param_1 + 0x210),iVar11,2);
    iVar9 = *(int *)(param_1 + 0x3fc);
  }
  fVar15 = (float)FUN_0000bf34((float)iVar9 * 0.2f);
  fVar16 = (float)FUN_0000bf34((float)*(int *)(param_1 + 0x400) * 0.2f);
  FUN_000ffb3c(*(undefined4 *)(param_1 + 0x1d4),*(int *)(param_1 + 0x3f8) == 0);
  if (*(char *)(param_1 + 0x54) == '\0') {
    iVar9 = FUN_000fe430();
    iVar9 = 0xd - (iVar9 != 0 ^ 1);
  }
  else {
    iVar9 = 6;
  }
  FUN_000ffb3c(*(undefined4 *)(param_1 + 0x1d8),*(int *)(param_1 + 0x3f8) == iVar9 + -6);
  iVar9 = *(int *)(param_1 + 0x1d4);
  sVar2 = *(short *)(iVar9 + 6);
  iVar11 = 0;
  puVar13 = (undefined4 *)&DAT_002f2fb4;
  sVar5 = (short)(int)((float)*(int *)(param_1 + 0x404) - ABS(fVar15) * 8.0f);
  *(short *)(iVar9 + 6) = sVar5;
  *(short *)(iVar9 + 10) = *(short *)(iVar9 + 10) + (sVar5 - sVar2);
  iVar9 = *(int *)(param_1 + 0x1d8);
  sVar2 = *(short *)(iVar9 + 6);
  sVar5 = (short)(int)((float)*(int *)(param_1 + 0x408) + ABS(fVar15) * 8.0f);
  *(short *)(iVar9 + 6) = sVar5;
  *(short *)(iVar9 + 10) = *(short *)(iVar9 + 10) + (sVar5 - sVar2);
  iVar9 = param_1;
  do {
    if (*(char *)(param_1 + 0x54) == '\0') {
      iVar10 = *(int *)(&DAT_002f3330 + (iVar11 + *(int *)(param_1 + 0x3f8)) * 4);
    }
    else {
      iVar10 = *(int *)(&DAT_002f3364 + (iVar11 + *(int *)(param_1 + 0x3f8)) * 4);
    }
    local_c = 0;
    local_8 = 0;
    local_4 = 0;
    iVar12 = param_1 + iVar10 * 0xc;
    FUN_000d5304(&local_c,*(int *)(iVar12 + 0x344),
                 *(int *)(iVar12 + 0x344) + *(int *)(iVar12 + 0x348) * 4,(int)local_1c);
    local_18 = 0;
    local_14 = 0;
    local_10 = 0;
    FUN_000d5304(&local_18,*(int *)(iVar12 + 0x290),
                 *(int *)(iVar12 + 0x290) + *(int *)(iVar12 + 0x294) * 4,(int)local_20);
    iVar12 = *(int *)(param_1 + iVar10 * 4 + 0x244);
    cVar1 = *(char *)(param_1 + iVar10 + 0x280);
    FUN_000ffb3c(*(undefined4 *)(iVar9 + 0x144),1);
    FUN_000ffb3c(*(undefined4 *)(iVar9 + 0x174),1);
    FUN_000ffb3c(*(undefined4 *)(iVar9 + 0x15c),0);
    FUN_000ffb3c(*(undefined4 *)(iVar9 + 0x18c),1);
    if (cVar1 == '\0') {
      iVar8 = *(int *)(iVar9 + 0x1e0);
      *(undefined **)(iVar8 + 0xf8) = &DAT_002f3538;
      *(undefined4 *)(iVar8 + 0x138) = 0;
      FUN_0010d4a8();
      iVar8 = *(int *)(iVar9 + 0x1f8);
      *(undefined **)(iVar8 + 0xf8) = &DAT_002f3538;
      *(undefined4 *)(iVar8 + 0x138) = 0;
      FUN_0010d4a8();
      iVar6 = puVar13[0x71];
      iVar8 = *(int *)(iVar9 + 0x15c);
      *(int *)(iVar8 + 0x24) = iVar6 + -1;
      *(int *)(iVar8 + 0x28) = iVar6 + -1;
      FUN_000ffb3c(*(undefined4 *)(iVar9 + 0xcc),1);
      FUN_000ffb3c(*(undefined4 *)(iVar9 + 0xe4),1);
    }
    else {
      if ((iVar10 == 4) || (iVar10 == 6)) {
        iVar8 = *(int *)(local_c + iVar12 * 4);
        if (iVar10 != 4) {
          if (DAT_002f3478 < iVar8) {
            if (DAT_002f347c < iVar8) {
              iVar8 = iVar8 - DAT_002f347c;
              uVar17 = 0x15;
            }
            else {
              iVar8 = iVar8 - DAT_002f3478;
              uVar17 = 0x14;
            }
          }
          else {
            uVar17 = 0x13;
          }
          iVar6 = (uint)(iVar10 != 4) * 0x100;
          uVar17 = FUN_001fea10(0x13,6,uVar17);
          FUN_000143b0(iVar6 + 0x34a35c,DAT_002f3644,uVar17,iVar8);
          iVar8 = *(int *)(iVar9 + 0x1e0);
          *(int *)(iVar8 + 0xf8) = iVar6 + 0x34a35c;
          *(undefined4 *)(iVar8 + 0x138) = 1;
          FUN_0010d4a8();
          goto LAB_0023082c;
        }
        FUN_000143b0(0x34a35c,&DAT_002f3640);
        iVar8 = *(int *)(iVar9 + 0x1e0);
        *(undefined4 *)(iVar8 + 0xf8) = 0x34a35c;
        *(undefined4 *)(iVar8 + 0x138) = 1;
        FUN_0010d4a8();
        iVar8 = *(int *)(iVar9 + 0x1f8);
      }
      else {
        if (iVar10 == 3) {
          FUN_000143b0(0x34a55c,&DAT_002f364c,DAT_003415a8 + 0x20);
          iVar8 = *(int *)(iVar9 + 0x1e0);
          *(undefined4 *)(iVar8 + 0xf8) = 0x34a55c;
          *(undefined4 *)(iVar8 + 0x138) = 1;
          FUN_0010d4a8();
        }
        else if (iVar10 == 5) {
          iVar8 = *(int *)(local_c + iVar12 * 4);
          if (iVar8 < 0xf) {
            uVar17 = FUN_001fea10(0xc,0);
            iVar8 = *(int *)(iVar9 + 0x1e0);
            *(undefined4 *)(iVar8 + 0xf8) = uVar17;
            *(undefined4 *)(iVar8 + 0x138) = 1;
            FUN_0010d4a8();
          }
          else {
            uVar17 = FUN_001fea10(5,2,*(undefined4 *)(&DAT_002f3388 + iVar8 * 4));
            iVar8 = *(int *)(iVar9 + 0x1e0);
            *(undefined4 *)(iVar8 + 0xf8) = uVar17;
            *(undefined4 *)(iVar8 + 0x138) = 1;
            FUN_0010d4a8();
          }
        }
        else {
          uVar17 = FUN_001fea10(0x13,6,*(undefined4 *)(local_c + iVar12 * 4));
          iVar8 = *(int *)(iVar9 + 0x1e0);
          *(undefined4 *)(iVar8 + 0xf8) = uVar17;
          *(undefined4 *)(iVar8 + 0x138) = 1;
          FUN_0010d4a8();
        }
LAB_0023082c:
        iVar8 = *(int *)(iVar9 + 0x1f8);
      }
      *(undefined **)(iVar8 + 0xf8) = &DAT_002f3538;
      *(undefined4 *)(iVar8 + 0x138) = 0;
      FUN_0010d4a8();
      if (iVar11 == *(int *)(param_1 + 0x224)) {
        iVar8 = 1;
      }
      else {
        iVar8 = -3;
      }
      iVar6 = *(int *)(iVar9 + 0x15c);
      iVar8 = puVar13[0x71] + iVar8 + -1;
      *(int *)(iVar6 + 0x24) = iVar8;
      *(int *)(iVar6 + 0x28) = iVar8;
      FUN_000ffb3c(*(undefined4 *)(iVar9 + 0xcc),iVar11 != *(int *)(param_1 + 0x224));
      FUN_000ffb3c(*(undefined4 *)(iVar9 + 0xe4),iVar11 == *(int *)(param_1 + 0x224));
      iVar6 = puVar13[5];
      iVar8 = *(int *)(iVar9 + 0xcc);
      *(int *)(iVar8 + 0x24) = iVar10 + iVar6;
      *(int *)(iVar8 + 0x28) = iVar10 + iVar6;
      iVar6 = puVar13[0x17];
      iVar8 = *(int *)(iVar9 + 0xe4);
      *(int *)(iVar8 + 0x24) = iVar10 + iVar6;
      *(int *)(iVar8 + 0x28) = iVar10 + iVar6;
    }
    if (iVar11 == *(int *)(param_1 + 0x224)) {
      cVar7 = iVar12 != 0;
      cVar14 = iVar12 != local_8 + -1;
      if (iVar10 == 4) {
        cVar7 = *(char *)(param_1 + 0xa0);
        cVar14 = cVar7;
      }
      bVar3 = cVar1 == '\0';
      uVar17 = 1;
      if (bVar3) {
LAB_00230970:
        uVar4 = *(undefined4 *)(iVar9 + 0xfc);
      }
      else {
        if (cVar7 != '\0') {
          uVar17 = 0;
          goto LAB_00230970;
        }
        uVar4 = *(undefined4 *)(iVar9 + 0xfc);
      }
      FUN_000ffb3c(uVar4,uVar17);
      if (!bVar3) {
        uVar17 = *(undefined4 *)(iVar9 + 0x114);
      }
      else {
        uVar17 = *(undefined4 *)(iVar9 + 0x114);
      }
      FUN_000ffb3c(uVar17,bVar3 || cVar14 == '\0');
      iVar8 = *(int *)(iVar9 + 0xfc);
      sVar2 = *(short *)(iVar8 + 4);
      sVar5 = (short)(int)((float)*(int *)(param_1 + 0x40c) - ABS(fVar16) * 6.0f);
      *(short *)(iVar8 + 4) = sVar5;
      *(short *)(iVar8 + 8) = *(short *)(iVar8 + 8) + (sVar5 - sVar2);
      iVar8 = *(int *)(iVar9 + 0x114);
      sVar2 = *(short *)(iVar8 + 4);
      sVar5 = (short)(int)((float)*(int *)(param_1 + 0x410) + ABS(fVar16) * 6.0f);
      *(short *)(iVar8 + 4) = sVar5;
      *(short *)(iVar8 + 8) = *(short *)(iVar8 + 8) + (sVar5 - sVar2);
      FUN_000ffb3c(*(undefined4 *)(iVar9 + 300),bVar3);
      **(uint **)(iVar9 + 0x144) = **(uint **)(iVar9 + 0x144) & 0xffffff;
      **(uint **)(iVar9 + 0x174) = **(uint **)(iVar9 + 0x174) & 0xffffff;
      if (cVar1 != '\0') {
        if (iVar10 - 4U < 3) {
          iVar8 = *(int *)(iVar9 + 0x1e0);
          *(undefined4 *)(iVar8 + 0x118) = 0x80252525;
          local_30 = 0x42140000;
          local_2c = 0x42140000;
          local_28 = 0x42140000;
          local_24 = 0x43000000;
          uVar17 = 0x43000000;
          FUN_00022498(0x3c03126f,&local_d0,&local_30);
          *(undefined4 *)(iVar8 + 0x120) = local_d0;
          *(undefined4 *)(iVar8 + 0x124) = local_cc;
          *(undefined4 *)(iVar8 + 0x128) = local_c8;
          *(undefined4 *)(iVar8 + 300) = local_c4;
        }
        else {
          iVar8 = *(int *)(local_c + iVar12 * 4);
          if (iVar8 == 0) {
            iVar8 = *(int *)(iVar9 + 0x1e0);
            *(undefined4 *)(iVar8 + 0x118) = 0x80000064;
            local_40 = 0x42c80000;
            local_34 = 0x43000000;
            uVar17 = 0x43000000;
            local_3c = 0;
            local_38 = 0;
            FUN_00022498(0x3c03126f,&local_e0,&local_40);
            *(undefined4 *)(iVar8 + 0x120) = local_e0;
            *(undefined4 *)(iVar8 + 0x124) = local_dc;
            *(undefined4 *)(iVar8 + 0x128) = local_d8;
            *(undefined4 *)(iVar8 + 300) = local_d4;
          }
          else if (iVar8 == 1) {
            iVar8 = *(int *)(iVar9 + 0x1e0);
            *(undefined4 *)(iVar8 + 0x118) = 0x80640000;
            local_48 = 0x42c80000;
            local_44 = 0x43000000;
            uVar17 = 0x43000000;
            local_50 = 0;
            local_4c = 0;
            FUN_00022498(0x3c03126f,&local_f0,&local_50);
            *(undefined4 *)(iVar8 + 0x120) = local_f0;
            *(undefined4 *)(iVar8 + 0x124) = local_ec;
            *(undefined4 *)(iVar8 + 0x128) = local_e8;
            *(undefined4 *)(iVar8 + 300) = local_e4;
          }
          else {
            iVar8 = *(int *)(iVar9 + 0x1e0);
            *(undefined4 *)(iVar8 + 0x118) = 0x80252525;
            local_60 = 0x42140000;
            local_5c = 0x42140000;
            local_58 = 0x42140000;
            local_54 = 0x43000000;
            uVar17 = 0x43000000;
            FUN_00022498(0x3c03126f,&local_100,&local_60);
            *(undefined4 *)(iVar8 + 0x120) = local_100;
            *(undefined4 *)(iVar8 + 0x124) = local_fc;
            *(undefined4 *)(iVar8 + 0x128) = local_f8;
            *(undefined4 *)(iVar8 + 300) = local_f4;
          }
        }
        if (((iVar10 != 4) && (iVar10 != 5)) && (iVar10 != 6)) {
          if (*(int *)(local_c + iVar12 * 4) == 0x10) {
            iVar10 = *(int *)(iVar9 + 0x1e0);
            *(undefined **)(iVar10 + 0xf8) = &DAT_002f3538;
            *(undefined4 *)(iVar10 + 0x138) = 0;
            FUN_0010d4a8();
            uVar4 = FUN_001fea10(0x13,6,*(undefined4 *)(local_c + iVar12 * 4));
            iVar10 = *(int *)(iVar9 + 0x1f8);
            *(undefined4 *)(iVar10 + 0xf8) = uVar4;
            *(undefined4 *)(iVar10 + 0x138) = 0;
            FUN_0010d4a8();
            iVar10 = *(int *)(iVar9 + 0x1f8);
            *(undefined4 *)(iVar10 + 0x118) = 0x80252525;
            local_70 = 0x42140000;
            local_6c = 0x42140000;
            local_68 = 0x42140000;
            local_64 = uVar17;
            FUN_00022498(0x3c03126f,&local_110,&local_70);
            *(undefined4 *)(iVar10 + 0x120) = local_110;
            *(undefined4 *)(iVar10 + 0x124) = local_10c;
            *(undefined4 *)(iVar10 + 0x128) = local_108;
            *(undefined4 *)(iVar10 + 300) = local_104;
            FUN_000ffb3c(*(undefined4 *)(iVar9 + 0x18c),0);
            FUN_000ffb3c(*(undefined4 *)(iVar9 + 0x18c),0);
          }
          else {
            iVar10 = *(int *)(iVar9 + 0x1f8);
            *(undefined **)(iVar10 + 0xf8) = &DAT_002f3538;
            *(undefined4 *)(iVar10 + 0x138) = 0;
            FUN_0010d4a8();
          }
        }
      }
    }
    else {
      FUN_000ffb3c(*(undefined4 *)(iVar9 + 0xfc),1);
      FUN_000ffb3c(*(undefined4 *)(iVar9 + 0x114),1);
      FUN_000ffb3c(*(undefined4 *)(iVar9 + 300),1);
      **(uint **)(iVar9 + 0x144) = **(uint **)(iVar9 + 0x144) & 0xffffff;
      **(uint **)(iVar9 + 0x174) = **(uint **)(iVar9 + 0x174) & 0xffffff;
      if (cVar1 != '\0') {
        if (iVar10 - 4U < 3) {
          iVar8 = *(int *)(iVar9 + 0x1e0);
          *(undefined4 *)(iVar8 + 0x118) = 0x80252525;
          local_80 = 0x42140000;
          local_7c = 0x42140000;
          local_78 = 0x42140000;
          local_74 = 0x43000000;
          uVar17 = 0x43000000;
          FUN_00022498(0x3c03126f,&local_120,&local_80);
          *(undefined4 *)(iVar8 + 0x120) = local_120;
          *(undefined4 *)(iVar8 + 0x124) = local_11c;
          *(undefined4 *)(iVar8 + 0x128) = local_118;
          *(undefined4 *)(iVar8 + 300) = local_114;
        }
        else {
          iVar8 = *(int *)(local_c + iVar12 * 4);
          if (iVar8 == 0) {
            iVar8 = *(int *)(iVar9 + 0x1e0);
            *(undefined4 *)(iVar8 + 0x118) = 0x80000064;
            local_90 = 0x42c80000;
            local_84 = 0x43000000;
            uVar17 = 0x43000000;
            local_8c = 0;
            local_88 = 0;
            FUN_00022498(0x3c03126f,&local_130,&local_90);
            *(undefined4 *)(iVar8 + 0x120) = local_130;
            *(undefined4 *)(iVar8 + 0x124) = local_12c;
            *(undefined4 *)(iVar8 + 0x128) = local_128;
            *(undefined4 *)(iVar8 + 300) = local_124;
          }
          else if (iVar8 == 1) {
            iVar8 = *(int *)(iVar9 + 0x1e0);
            *(undefined4 *)(iVar8 + 0x118) = 0x80640000;
            local_98 = 0x42c80000;
            local_94 = 0x43000000;
            uVar17 = 0x43000000;
            local_a0 = 0;
            local_9c = 0;
            FUN_00022498(0x3c03126f,&local_140,&local_a0);
            *(undefined4 *)(iVar8 + 0x120) = local_140;
            *(undefined4 *)(iVar8 + 0x124) = local_13c;
            *(undefined4 *)(iVar8 + 0x128) = local_138;
            *(undefined4 *)(iVar8 + 300) = local_134;
          }
          else {
            iVar8 = *(int *)(iVar9 + 0x1e0);
            *(undefined4 *)(iVar8 + 0x118) = 0x80252525;
            local_b0 = 0x42140000;
            local_ac = 0x42140000;
            local_a8 = 0x42140000;
            local_a4 = 0x43000000;
            uVar17 = 0x43000000;
            FUN_00022498(0x3c03126f,&local_150,&local_b0);
            *(undefined4 *)(iVar8 + 0x120) = local_150;
            *(undefined4 *)(iVar8 + 0x124) = local_14c;
            *(undefined4 *)(iVar8 + 0x128) = local_148;
            *(undefined4 *)(iVar8 + 300) = local_144;
          }
        }
        if (((iVar10 != 4) && (iVar10 != 5)) && (iVar10 != 6)) {
          if (*(int *)(local_c + iVar12 * 4) == 0x10) {
            iVar10 = *(int *)(iVar9 + 0x1e0);
            *(undefined **)(iVar10 + 0xf8) = &DAT_002f3538;
            *(undefined4 *)(iVar10 + 0x138) = 0;
            FUN_0010d4a8();
            uVar4 = FUN_001fea10(0x13,6,*(undefined4 *)(local_c + iVar12 * 4));
            iVar10 = *(int *)(iVar9 + 0x1f8);
            *(undefined4 *)(iVar10 + 0xf8) = uVar4;
            *(undefined4 *)(iVar10 + 0x138) = 0;
            FUN_0010d4a8();
            iVar10 = *(int *)(iVar9 + 0x1f8);
            *(undefined4 *)(iVar10 + 0x118) = 0x80252525;
            local_c0 = 0x42140000;
            local_bc = 0x42140000;
            local_b8 = 0x42140000;
            local_b4 = uVar17;
            FUN_00022498(0x3c03126f,&local_160,&local_c0);
            *(undefined4 *)(iVar10 + 0x120) = local_160;
            *(undefined4 *)(iVar10 + 0x124) = local_15c;
            *(undefined4 *)(iVar10 + 0x128) = local_158;
            *(undefined4 *)(iVar10 + 300) = local_154;
            FUN_000ffb3c(*(undefined4 *)(iVar9 + 0x18c),1);
          }
          else {
            iVar10 = *(int *)(iVar9 + 0x1f8);
            *(undefined **)(iVar10 + 0xf8) = &DAT_002f3538;
            *(undefined4 *)(iVar10 + 0x138) = 0;
            FUN_0010d4a8();
          }
        }
      }
    }
    FUN_00054554(&local_18,0);
    FUN_00054554(&local_c,0);
    iVar11 = iVar11 + 1;
    iVar9 = iVar9 + 4;
    puVar13 = puVar13 + 3;
    if (5 < iVar11) {
      *(int *)(param_1 + 0x3fc) = *(int *)(param_1 + 0x3fc) + 1;
      *(int *)(param_1 + 0x400) = *(int *)(param_1 + 0x400) + 1;
      if (*(int *)(param_1 + 0x21c) != 0) {
        FUN_0015e268();
      }
      return;
    }
  } while( true );
}


}

// Differing words: 878 / 913 words. First at +0x0.
// Ruled out:
// - Float intrinsic: Ghidra emitted ABS macro on floats, replaced with extern "C" float fabsf(float) / #define ABS(x) fabsf(x).
// - Frame size: retail frame is 416 bytes with 128 additional bytes of stack buffer/locals; ours is 288 bytes.
// - Branch polarity: Ghidra inverted beql / bnel conditions and signedness of lbu v0,84(s4) vs lb.
// near miss from lane dsn4 (unreviewed)
