/* HSTMATCH module=EBOOT.BIN start=0x001eb000 end=0x001eb4a0 symbol=FUN_001eb000 lang=c++ */

extern "C" {
typedef unsigned char undefined1;
typedef unsigned short undefined2;
typedef unsigned int undefined4;
typedef unsigned int uint;
typedef int undefined;

extern int DAT_002ec54c;
extern int DAT_002eda10;
extern int DAT_002eda18;
extern int DAT_002eda24;
extern int DAT_002eda30;
extern int DAT_002eda3c;
extern int DAT_002eda48;
extern int DAT_002ee0f8;
extern int DAT_002ee0fc;
extern int DAT_002ee100;
extern int DAT_002ee104;
extern int DAT_002ee108;
extern int DAT_002ee10c;
extern int DAT_00341508;
extern int DAT_003417d0;
extern int DAT_003417d2;
extern int DAT_003417d4;
extern int DAT_003417d6;
extern int DAT_003417d8;
extern int DAT_003417da;
extern int DAT_003417f0;
extern int DAT_00341837;
extern int DAT_00341870;

extern int FUN_00022498(...);
extern int FUN_000d7d80(...);
extern int FUN_000d7e30(...);
extern int FUN_000ffb3c(...);
extern int FUN_0010d4a8(...);
extern int FUN_00164c60(...);
extern int FUN_00164d00(...);
extern int FUN_00164e24(...);
extern int FUN_00164ec4(...);
extern int FUN_00164f64(...);
extern int FUN_00165004(...);
extern int FUN_001fea10(...);
extern int FUN_002045e8(...);
extern int FUN_002182a0(...);


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_001eb000(int param_1,int param_2,char param_3)

{
  int *piVar1;
  int iVar2;
  undefined4 uVar3;
  int iVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  unsigned char *pbVar8;
  int iVar9;
  int iVar10;
  int *piVar11;
  undefined4 *puVar12;
  int *piVar13;
  int *piVar14;
  int *piVar15;
  undefined4 *local_108;
  undefined4 local_100;
  undefined4 local_fc;
  undefined4 local_f8;
  undefined4 local_f4;
  undefined4 local_f0;
  undefined4 local_ec;
  undefined4 local_e8;
  undefined4 local_e4;
  int local_e0 [4];
  undefined4 local_d0;
  undefined4 local_cc;
  undefined1 auStack_c8 [200];
  
  iVar2 = param_2 * 0x120;
  if (*(short *)(&DAT_003417d0 + iVar2) < 0xf) {
    *(undefined1 *)(param_2 + param_1 + 0x34c) = 0;
  }
  else {
    *(undefined1 *)(param_2 + param_1 + 0x34c) = 1;
  }
  pbVar8 = (unsigned char *)(param_2 + param_1 + 0x34c);
  if (*pbVar8 == 0) {
    iVar4 = param_2 * 4 + param_1;
    *(int *)(iVar4 + 0x32c) = (int)*(short *)(&DAT_003417d0 + iVar2);
  }
  else {
    iVar4 = param_2 * 4 + param_1;
    *(uint *)(iVar4 + 0x32c) = (uint)*(unsigned short *)(&DAT_003417f0 + iVar2);
  }
  local_108 = (undefined4 *)(iVar4 + 0x32c);
  iVar6 = param_2 * 4;
  *(int *)(iVar6 + param_1 + 0x33c) = iVar2 + 0x341838;
  iVar4 = FUN_00165004(DAT_00341508 + (uint)*pbVar8 * 0x18);
  iVar10 = *(int *)(DAT_00341508 + (uint)*pbVar8 * 0x18) + iVar4 * 0x22c;
  FUN_000d7d80(auStack_c8);
  FUN_002045e8(auStack_c8,(int)*(short *)(&DAT_003417d0 + iVar2),
               (int)*(short *)(&DAT_003417d2 + iVar2),(int)*(short *)(&DAT_003417d4 + iVar2),
               (int)*(short *)(&DAT_003417d6 + iVar2),(int)*(short *)(&DAT_003417d8 + iVar2),
               (int)*(short *)(&DAT_003417da + iVar2),(&DAT_00341837)[iVar2],
               (int)(char)(&DAT_00341870)[iVar2]);
  piVar13 = (int *)(iVar6 + param_1 + 0x350);
  iVar4 = FUN_00164c60(iVar10,auStack_c8);
  *piVar13 = iVar4;
  piVar14 = (int *)(iVar6 + param_1 + 0x360);
  iVar4 = FUN_00164d00(iVar10,auStack_c8);
  *piVar14 = iVar4;
  piVar15 = (int *)(iVar6 + param_1 + 0x370);
  iVar4 = FUN_00164e24(iVar10,auStack_c8);
  *piVar15 = iVar4;
  piVar1 = (int *)(iVar6 + param_1 + 0x380);
  iVar4 = FUN_00164f64(iVar10,auStack_c8);
  piVar11 = (int *)(iVar6 + param_1 + 0x390);
  *piVar1 = iVar4;
  iVar4 = FUN_00164ec4(iVar10,auStack_c8);
  *piVar11 = iVar4;
  iVar10 = param_1 + param_2 * 0x10;
  uVar5 = 0;
  iVar4 = iVar10;
  do {
    uVar5 = uVar5 + 1;
    *(undefined4 *)(iVar4 + 0x3a0) = 0;
    iVar4 = iVar4 + 4;
  } while (uVar5 < 4);
  FUN_002182a0(iVar10 + 0x3a0,*local_108,*pbVar8,iVar2 + 0x341838);
  local_e0[0] = DAT_002ee0f8;
  local_e0[1] = DAT_002ee0fc;
  local_e0[2] = DAT_002ee100;
  local_e0[3] = DAT_002ee104;
  local_d0 = DAT_002ee108;
  local_cc = DAT_002ee10c;
  if (param_3 == '\0') {
    iVar2 = 0;
  }
  else {
    iVar2 = 2;
  }
  iVar4 = local_e0[param_2 + iVar2];
  iVar9 = param_1 + (param_2 + iVar2) * 0x10;
  param_1 = iVar4 * 4 + param_1;
  iVar2 = iVar4 * 0xc;
  iVar6 = *(int *)(param_1 + 0x70);
  uVar5 = 0;
  iVar7 = *piVar13 + (&DAT_002eda18)[iVar4 * 3];
  *(int *)(iVar6 + 0x24) = iVar7;
  *(int *)(iVar6 + 0x28) = iVar7;
  iVar6 = *(int *)(param_1 + 0x74);
  iVar7 = *piVar14 + *(int *)(&DAT_002eda24 + iVar2);
  *(int *)(iVar6 + 0x24) = iVar7;
  *(int *)(iVar6 + 0x28) = iVar7;
  iVar6 = *(int *)(param_1 + 0x78);
  iVar7 = *piVar15 + *(int *)(&DAT_002eda30 + iVar2);
  *(int *)(iVar6 + 0x24) = iVar7;
  *(int *)(iVar6 + 0x28) = iVar7;
  iVar6 = *(int *)(param_1 + 0x7c);
  iVar2 = *piVar1 + *(int *)(&DAT_002eda3c + iVar2);
  *(int *)(iVar6 + 0x24) = iVar2;
  *(int *)(iVar6 + 0x28) = iVar2;
  iVar2 = *(int *)(param_1 + 0x80);
  iVar6 = *piVar11 + (&DAT_002eda48)[iVar4 * 3];
  *(int *)(iVar2 + 0x24) = iVar6;
  *(int *)(iVar2 + 0x28) = iVar6;
  puVar12 = (undefined4 *)&DAT_002eda10 + iVar4 * 3;
  do {
    if (*(int *)(iVar10 + 0x3a0) == 0xff) {
      FUN_000ffb3c(*(undefined4 *)(param_1 + 0x98),1);
      iVar2 = *(int *)(iVar9 + 0x29c);
      *(undefined **)(iVar2 + 0xf8) = (undefined *)&DAT_002ec54c;
      *(undefined4 *)(iVar2 + 0x138) = 0;
      FUN_0010d4a8();
    }
    else {
      FUN_000ffb3c(*(undefined4 *)(param_1 + 0x98),0);
      iVar2 = *(int *)(param_1 + 0x98);
      iVar4 = puVar12[0x20] + (*(int *)(iVar10 + 0x3a0) < 0xd ^ 1);
      *(int *)(iVar2 + 0x24) = iVar4;
      *(int *)(iVar2 + 0x28) = iVar4;
      iVar2 = *(int *)(iVar10 + 0x3a0);
      if (0xd < iVar2) {
        iVar2 = iVar2 + -0xe;
      }
      uVar3 = FUN_001fea10(0xb,3,iVar2);
      iVar2 = *(int *)(iVar9 + 0x29c);
      *(undefined4 *)(iVar2 + 0xf8) = uVar3;
      *(undefined4 *)(iVar2 + 0x138) = 1;
      FUN_0010d4a8();
      iVar2 = *(int *)(iVar9 + 0x29c);
      *(undefined4 *)(iVar2 + 0x118) = 0x80000000;
      local_e4 = 0x43000000;
      local_f0 = 0;
      local_ec = 0;
      local_e8 = 0;
      FUN_00022498(0x3c03126f,&local_100,&local_f0);
      *(undefined4 *)(iVar2 + 0x120) = local_100;
      *(undefined4 *)(iVar2 + 0x124) = local_fc;
      *(undefined4 *)(iVar2 + 0x128) = local_f8;
      *(undefined4 *)(iVar2 + 300) = local_f4;
    }
    uVar5 = uVar5 + 1;
    iVar10 = iVar10 + 4;
    param_1 = param_1 + 4;
    puVar12 = puVar12 + 3;
    iVar9 = iVar9 + 4;
  } while (uVar5 < 4);
  FUN_000d7e30(auStack_c8,0xffffffff);
  return;
}


}

// Differing words: 300 / 296 words. First at +0x0.
// Ruled out:
// - Frame size: 336 bytes in retail vs 320 bytes in ours (16-byte alignment of local Vec4 variables).
// - Struct array at &DAT_003417b8 has element size 288 (0x120), with short at offset 24.
// - VFPU helper call FUN_00022498 with aligned Vec4 parameters at local_100 and local_f0.
// near miss from lane dsn4 (unreviewed)
