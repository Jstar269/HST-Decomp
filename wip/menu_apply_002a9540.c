/* HSTMATCH module=EBOOT.BIN start=0x002a9540 end=0x002a9950 symbol=FUN_002a9540 lang=c99 */
/* Hand-written reconstruction from the pseudocode; measured draft, see trailing note. */
#include "hst/types.h"
typedef int code();

extern char DAT_002bc410, DAT_002bc414, DAT_002bc418, DAT_002bc41c;
extern char DAT_002bc420, DAT_002bc424, DAT_002bc428, DAT_002bc42c;
extern char DAT_002bc430, DAT_002bc434, DAT_002bc438, DAT_002bc43c;
extern char DAT_002bc440, DAT_002bc444, DAT_002bc448, DAT_002bc44c;
extern char DAT_0034b4c8;
extern int DAT_0034b4d0, DAT_0034b4d4, DAT_0034b4d8, DAT_0034b4dc;
extern int DAT_0034b4e0, DAT_0034b4e4;
extern int DAT_00302be4, DAT_00302be8;
extern void FUN_00057b58();
extern void FUN_00058ddc();
extern void FUN_00058ec4();
extern void FUN_00059084();
extern void FUN_00055044();
extern void FUN_000550ec();
extern void FUN_001fe0f4();
extern void FUN_001fe0e8();
extern void FUN_00049094();
extern void FUN_00055e48();
extern void FUN_000b42f8();
extern void FUN_00000a00();
extern int FUN_00091830();
extern void FUN_00091c48();
extern int FUN_00000bcc();
extern char FUN_00003100[];
extern void thunk_FUN_00022668();
extern void thunk_FUN_0002260c();
extern void thunk_FUN_000225b0();

void FUN_002a9540(int param_1)
{
  char cVar1;
  int iVar2;
  u32 uVar3;
  int *puVar4;
  int uVar5;
  int *piVar6;
  int *piVar7;
  int iVar8;
  u32 uVar9;
  int iVar10;
  int local_50, local_4c, local_48, local_44;
  int local_40, local_3c, local_38, local_34;
  int local_30, local_2c, local_28, local_24;
  int local_20, local_1c, local_18, local_14;

  if (*(int *)(param_1 + 0x30e4) == 0) {
    cVar1 = *(char *)(param_1 + 0x3114);
  }
  else {
    if (*(int *)(param_1 + 0x30e4) == *(int *)(param_1 + 0x30e8)) {
      *(u8 *)(param_1 + 0x3114) = 0;
      *(int *)(param_1 + 0x3118) = **(int **)(param_1 + 0x30e4);
      return;
    }
    cVar1 = *(char *)(param_1 + 0x3114);
  }
  if (cVar1 == 0) {
    *(u8 *)(param_1 + 0x30c6) = 0;
    *(u8 *)(param_1 + 0x3114) = 1;
    *(int *)(param_1 + 0x3118) = 0;
    if (*(int *)(param_1 + 0x311c) != 0) {
      FUN_00057b58(0, *(int *)(param_1 + 0x311c), 0, 0);
      FUN_00058ddc(*(int *)(param_1 + 0x311c), 1, 0xffffffff);
      FUN_00058ec4(0x3f800000, *(int *)(param_1 + 0x311c), 0xffffffff);
      FUN_00059084(*(int *)(param_1 + 0x311c), 0xffffffff);
      local_50 = DAT_002bc410;
      local_4c = DAT_002bc414;
      local_48 = DAT_002bc418;
      local_44 = DAT_002bc41c;
      local_40 = DAT_002bc420;
      local_3c = DAT_002bc424;
      local_38 = DAT_002bc428;
      local_34 = DAT_002bc42c;
      local_30 = DAT_002bc430;
      local_2c = DAT_002bc434;
      local_28 = DAT_002bc438;
      local_24 = DAT_002bc43c;
      local_20 = DAT_002bc440;
      local_1c = DAT_002bc444;
      local_18 = DAT_002bc448;
      local_14 = DAT_002bc44c;
      if (DAT_0034b4c8 == 0) {
        DAT_0034b4d0 = 0xbccccccd;
        DAT_0034b4d4 = 0x3db851ec;
        DAT_0034b4d8 = 0x3f800000;
        DAT_0034b4dc = 0x3f800000;
        DAT_0034b4c8 = 1;
      }
      thunk_FUN_00022668(DAT_0034b4e0 * 0.017453292, &local_50, &local_50);
      thunk_FUN_0002260c(DAT_00302be8 * 0.017453292, &local_50, &local_50);
      thunk_FUN_000225b0(DAT_0034b4e4 * 0.017453292, &local_50, &local_50);
      local_20 = DAT_0034b4d0;
      local_1c = DAT_0034b4d4;
      local_18 = DAT_0034b4d8;
      local_14 = DAT_0034b4dc;
      FUN_000550ec(DAT_00302be4, *(int *)(param_1 + 0x311c));
      FUN_00055044(*(int *)(param_1 + 0x311c), &local_50);
    }
    FUN_001fe0f4(1);
    iVar10 = 0;
    iVar8 = param_1;
    do {
      piVar7 = *(int **)((int)FUN_00003100 + iVar8 + 4);
      if (piVar7 == 0) {
        iVar2 = *(int *)((int)FUN_00003100 + iVar8 + 4);
      }
      else {
        FUN_00049094(1);
        piVar6 = (int *)*piVar7;
        if (piVar6 == 0) {
          iVar2 = piVar7[2];
        }
        else {
          (**(code **)(*piVar6 + 8))(piVar6, 1);
          *piVar7 = 0;
          iVar2 = piVar7[2];
        }
        uVar9 = 0;
        if (iVar2 != 0) {
          iVar2 = 0;
          do {
            if (*(int *)(piVar7[1] + iVar2) != 0) {
              FUN_00055e48(*(int *)(piVar7[1] + iVar2), 1);
              *(int *)(piVar7[1] + iVar2) = 0;
            }
            uVar9 = uVar9 + 1;
            iVar2 = iVar2 + 4;
          } while (uVar9 < (u32)piVar7[2]);
        }
        piVar6 = piVar7 + 1;
        piVar7[2] = 0;
        if (piVar6 != 0) {
          FUN_000b42f8(piVar6, 0);
        }
        FUN_00000a00(piVar7);
        *(int *)((int)FUN_00003100 + iVar8 + 4) = 0;
        iVar2 = *(int *)((int)FUN_00003100 + iVar8 + 4);
      }
      if (iVar2 == 0) {
        puVar4 = (int *)FUN_00000bcc(0x20);
        if (puVar4 == 0) {
          *(int *)((int)FUN_00003100 + iVar8 + 4) = 0;
        }
        else {
          *puVar4 = 0;
          puVar4[1] = 0;
          puVar4[2] = 0;
          puVar4[3] = 0;
          puVar4[4] = 0;
          *(u8 *)((int)puVar4 + 0x19) = 0;
          *(u8 *)((int)puVar4 + 0x1a) = 1;
          puVar4[5] = 0;
          *(u8 *)(puVar4 + 6) = 0;
          puVar4[7] = 0;
          iVar2 = FUN_00000bcc(0xe0);
          if (iVar2 == 0) {
            *puVar4 = 0;
            uVar5 = 0;
          }
          else {
            uVar5 = FUN_00091830(iVar2);
            *puVar4 = uVar5;
          }
          FUN_00091c48(uVar5, 0xffffffff);
          *(int **)((int)FUN_00003100 + iVar8 + 4) = puVar4;
        }
      }
      iVar10 = iVar10 + 1;
      iVar8 = iVar8 + 4;
    } while (iVar10 < 4);
    FUN_001fe0e8(0);
    *(int *)(param_1 + 0x30e8) = 0;
  }
}

// Full body from two bounded reads. Retail frame 112 bytes.
// near miss from lane dsn8 (unreviewed)
