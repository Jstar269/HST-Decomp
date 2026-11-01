/* HSTMATCH module=EBOOT.BIN start=0x00255824 end=0x00255c20 symbol=FUN_00255824 lang=c99 */
/* Hand-written reconstruction from the pseudocode; not yet measured, not installed. Structure banked from the
   previous read: 0x29-iteration table loop, 4-iteration init loop, eight vtable-dispatch groups. */
#include "hst/types.h"
typedef int code();

extern char *PTR_s_owner0_002fd5c0[];
extern char DAT_002fd66c[];
extern char DAT_002fd664[];
extern char DAT_002fd674[];
extern char DAT_002fd684[];
extern char DAT_002fd68c[];
extern u32 *FUN_000143b0();
extern int FUN_001026b8();

void FUN_00255824(int *param_1, int param_2, int *param_3, int *param_4)
{
  int iVar1;
  int iVar2;
  int *piVar3;
  int *piVar4;
  int uVar5;
  u32 uVar6;
  char **ppuVar7;
  int *piVar8;
  int iVar9;
  int *piVar10;
  u8 auStack_40[64];

  *(u8 *)(param_1 + 0xb4) = (u8)param_2;
  param_1[0xb5] = 0;
  iVar9 = 0;
  ppuVar7 = PTR_s_owner0_002fd5c0;
  piVar3 = param_1;
  do {
    if ((u8)param_1[0xb4] == 0) {
      FUN_000143b0(auStack_40, DAT_002fd66c, *ppuVar7);
      iVar1 = FUN_001026b8(*param_4, auStack_40);
      *piVar3 = iVar1;
      iVar1 = FUN_001026b8(param_4[1], auStack_40);
      piVar3[0x29] = iVar1;
      iVar1 = FUN_001026b8(param_4[2], auStack_40);
      piVar3[0x52] = iVar1;
      iVar1 = FUN_001026b8(param_4[3], auStack_40);
      piVar3[0x7b] = iVar1;
      if (iVar9 == 0x24) {
        if (*piVar3 == 0) {
          iVar1 = piVar3[0x29];
        }
        else {
          param_1[0xf0] = (int)*(short *)(*piVar3 + 4);
          iVar1 = piVar3[0x29];
        }
        if (iVar1 == 0) {
          iVar1 = piVar3[0x52];
        }
        else {
          param_1[0xf1] = (int)*(short *)(iVar1 + 4);
          iVar1 = piVar3[0x52];
        }
        if (iVar1 == 0) {
          iVar1 = piVar3[0x7b];
        }
        else {
          param_1[0xf2] = (int)*(short *)(iVar1 + 4);
          iVar1 = piVar3[0x7b];
        }
        if (iVar1 != 0) {
          param_1[0xf3] = (int)*(short *)(iVar1 + 4);
        }
      }
    }
    else {
      FUN_000143b0(auStack_40, DAT_002fd664, *ppuVar7);
      iVar1 = FUN_001026b8(*param_3, auStack_40);
      *piVar3 = iVar1;
      iVar1 = FUN_001026b8(param_3[1], auStack_40);
      piVar3[0x29] = iVar1;
      if (iVar9 == 0x24) {
        param_1[0xf0] = (int)*(short *)(*piVar3 + 4);
        param_1[0xf1] = (int)*(short *)(piVar3[0x29] + 4);
      }
    }
    iVar9 = iVar9 + 1;
    ppuVar7 = ppuVar7 + 1;
    piVar3 = piVar3 + 1;
  } while (iVar9 < 0x29);
  if (param_1[6] == 0) {
    iVar9 = 0;
  }
  else {
    iVar9 = *(int *)(param_1[6] + 0x24);
  }
  param_1[0xa4] = iVar9;
  uVar6 = 0;
  param_1[0xa5] = *(int *)(param_1[10] + 0x24);
  param_1[0xa6] = *(int *)(param_1[5] + 0x24);
  piVar3 = param_1;
  do {
    piVar3[0xac] = 0;
    uVar6 = uVar6 + 1;
    piVar3[0xb0] = 0;
    piVar3 = piVar3 + 1;
  } while (uVar6 < 4);
  if (param_2 == 0) {
    iVar9 = 4;
  }
  else {
    iVar9 = 2;
  }
  iVar1 = 0;
  piVar3 = param_1;
  piVar8 = param_1;
  piVar10 = param_1;
  if (iVar9 != 0) {
    do {
      if (piVar10[1] == 0) {
        iVar2 = piVar10[3];
      }
      else {
        piVar8[200] = *(int *)(piVar10[1] + 0x20);
        iVar2 = piVar10[3];
      }
      if (iVar2 == 0) {
        iVar2 = piVar10[0x10];
      }
      else {
        piVar8[0xc4] = *(int *)(iVar2 + 0x20);
        iVar2 = piVar10[0x10];
      }
      piVar3[0xcc] = *(int *)(iVar2 + 0x20);
      piVar3[0xcd] = *(int *)(piVar10[0x11] + 0x20);
      piVar3[0xce] = *(int *)(piVar10[0x12] + 0x20);
      piVar3[0xcf] = *(int *)(piVar10[0x13] + 0x20);
      (**(code **)(*(int *)piVar3[0xcc] + 0x18))((int *)piVar3[0xcc], 0x80000000);
      (**(code **)(*(int *)piVar3[0xcd] + 0x18))((int *)piVar3[0xcd], 0x80000000);
      (**(code **)(*(int *)piVar3[0xce] + 0x18))((int *)piVar3[0xce], 0x80000000);
      (**(code **)(*(int *)piVar3[0xcf] + 0x18))((int *)piVar3[0xcf], 0x80000000);
      if (piVar10[0x21] == 0) {
        piVar8[0xdc] = 0;
      }
      else {
        piVar4 = *(int **)(piVar10[0x21] + 0x20);
        piVar8[0xdc] = (int)piVar4;
        (**(code **)(*piVar4 + 0x18))(piVar4, 0x80000000);
      }
      if (piVar10[0x22] == 0) {
        piVar8[0xe0] = 0;
      }
      else {
        piVar4 = *(int **)(piVar10[0x22] + 0x20);
        piVar8[0xe0] = (int)piVar4;
        (**(code **)(*piVar4 + 0x18))(piVar4, 0x80000000);
      }
      if (piVar10[0x25] == 0) {
        piVar8[0xe4] = 0;
      }
      else {
        piVar4 = *(int **)(piVar10[0x25] + 0x20);
        piVar8[0xe4] = (int)piVar4;
        (**(code **)(*piVar4 + 0x18))(piVar4, 0x80000000);
      }
      piVar8[0xa7] = 0;
      if (piVar10[0x27] == 0) {
        piVar8[0xe8] = 0;
      }
      else {
        piVar4 = *(int **)(piVar10[0x27] + 0x20);
        piVar8[0xe8] = (int)piVar4;
        (**(code **)(*piVar4 + 0x18))(piVar4, 0x80000000);
      }
      if (piVar10[0x28] == 0) {
        piVar8[0xec] = 0;
      }
      else {
        piVar4 = *(int **)(piVar10[0x28] + 0x20);
        piVar8[0xec] = (int)piVar4;
        (**(code **)(*piVar4 + 0x18))(piVar4, 0x80000000);
      }
      piVar8[0xb8] = 0;
      piVar8[0xbc] = 0;
      piVar8[0xc0] = 0;
      if ((u8)param_1[0xb4] == 0) {
        iVar2 = FUN_001026b8(*param_4, DAT_002fd674);
        if (iVar2 == 0) {
          uVar5 = *param_4;
        }
        else {
          piVar8[0xb8] = *(int *)(iVar2 + 0x20);
          uVar5 = *param_4;
        }
        iVar2 = FUN_001026b8(uVar5, DAT_002fd684);
        if (iVar2 == 0) {
          uVar5 = *param_4;
        }
        else {
          piVar8[0xbc] = *(int *)(iVar2 + 0x20);
          uVar5 = *param_4;
        }
      }
      iVar1 = iVar1 + 1;
      piVar3 = piVar3 + 1;
      piVar8 = piVar8 + 1;
      piVar10 = piVar10 + 1;
    } while (iVar1 < iVar9);
  }
}

// BEST MEASURED BODY is this one: 255/255 words, 211 differ, first differing word at +0x40.
// The unread tail WAS read and applied in a separate attempt; it made the item worse (270/255 words,
// 266 differ, first at +0x0) so it was reverted. Tail facts, recorded so nobody re-reads it: last lookup is
// FUN_001026b8(uVar5, s_d_danibg0 @ 0x2fd68c) storing the RAW result into piVar8[0xc0] (no +0x20), then
// *(u8 *)((int)param_1 + iVar1 + 0x2d8) = 0 and +0x2dc, then param_4+1 / piVar3+4 / piVar8+1 / piVar10+0x29,
// and finally *(u8 *)(param_1 + 0xab) = 1. Since adding the real tail overshoots by 15 words, this body
// carries ~15 words too many elsewhere - that is where the next lever goes, not the tail.
// PARKED after its second (final) hsttry batch. Residual unchanged: 255/255 words, 211 differ,
// first differing word at +0x40.
// Batch 3 variants were all codegen-NEUTRAL (each exactly 211 differ, 255/255): vtable group as a
// ternary initialiser, vtable dispatch through a named `code *fn` local, and the four FUN_001026b8
// lookups stored directly without the named iVar1. So neither the vtable spelling nor the lookup
// spelling is the ~15 surplus words - stop varying those.
// NEXT LEVER: +0x40 is inside the first loop, before the vtable block, so attack the loop prologue:
// the FUN_000143b0(auStack_40, fmt, ppuVar7[i]) fill and the `(char)param_1[0xb4] == 0` test on the
// MEMORY field (retail may keep that flag in a register across the whole loop), and check whether
// the 0x29 bound is compared against an immediate each iteration or hoisted.
// near miss from lane dsn8 (unreviewed)
