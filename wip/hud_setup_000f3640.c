/* HSTMATCH module=EBOOT.BIN start=0x000f3640 end=0x000f3948 symbol=FUN_000f3640 lang=c99 */
/* Hand-written reconstruction from the disassembly; near miss, not installed. See the trailing note. */
#include "hst/types.h"
typedef int code();

extern int DAT_002d7cf4[];
extern u8 DAT_003386b8;
extern u8 DAT_00338ac4;
extern u8 *DAT_00338b44;
extern u8 DAT_003418bc[];
extern void FUN_000f0644(int);
extern void FUN_000f5778(int *);
extern void FUN_000f3948(int *);
extern void FUN_000ffb3c(void *, int);
extern void FUN_000ffc3c(void *, int);

void FUN_000f3640(int *param_1)
{
  u16 uVar1;
  int iVar2;
  int iVar4;
  int *piVar5;
  int *puVar6;
  int *piVar7;
  int *puVar8;
  u32 uVar9;
  int *piVar10;
  int iVar11;
  int *piVar12;
  int *piVar13;
  int uVar3;

  FUN_000f0644(param_1[0x6e]);
  FUN_000f5778(param_1);
  param_1[0x71] = 0;
  param_1[0x72] = 0;
  *(u8 *)(param_1 + 0x84) = 0;
  param_1[0x85] = -1;
  *(u8 *)(param_1 + 0x86) = 1;
  puVar8 = DAT_002d7cf4;
  *(u8 *)(param_1 + 0x6f) = 0;
  iVar11 = 0;
  piVar5 = param_1;
  piVar7 = param_1;
  puVar6 = puVar8;
  piVar10 = param_1;
  piVar12 = param_1;
  do {
    *(u8 *)(piVar10 + 0x77) = 0;
    *(u8 *)((int)piVar10 + 0x1dd) = 0;
    piVar12[0x78] = puVar6[0x92];
    piVar12[0x79] = puVar6[0x95];
    *(u8 *)((int)param_1 + iVar11 + 0x204) = 0;
    piVar7[0x82] = puVar8[0xc2];
    *(u8 *)((int)param_1 + iVar11 + 0x1f0) = 0;
    *(u8 *)((int)param_1 + iVar11 + 0x1f2) = 0;
    *(u8 *)((int)param_1 + iVar11 + 0x1f4) = 0;
    *(u8 *)((int)param_1 + iVar11 + 0x1f6) = 0;
    uVar3 = puVar8[0xbc];
    iVar2 = piVar7[0x58];
    *(int *)(iVar2 + 0x24) = uVar3;
    *(int *)(iVar2 + 0x28) = uVar3;
    uVar3 = puVar8[0xc2];
    iVar2 = piVar7[0x5a];
    *(int *)(iVar2 + 0x24) = uVar3;
    *(int *)(iVar2 + 0x28) = uVar3;
    FUN_000ffb3c((void *)piVar7[0x38], 1);
    if (*(u8 *)(DAT_00338b44 + 0x45) == 0) {
      FUN_000ffb3c((void *)piVar7[0x3a], 1);
      iVar2 = piVar7[0x54];
    }
    else {
      iVar2 = piVar7[0x54];
    }
    FUN_000ffc3c((void *)iVar2, 0xffffff00);
    if (*(u8 *)(DAT_00338b44 + 0x45) == 0) {
      FUN_000ffc3c((void *)piVar7[0x56], 0xffffff00);
      iVar2 = piVar7[0x1a];
    }
    else {
      iVar2 = piVar7[0x1a];
    }
    FUN_000ffb3c((void *)iVar2, 1);
    *(int *)piVar7[0x1a] = 0x808080;
    iVar2 = (int)*(short *)(piVar5[0x48] + 4) + (int)*(short *)(piVar5[0x48] + 8);
    uVar1 = (u16)(iVar2 >> 1);
    if (iVar2 < 0) {
      uVar1 = (u16)((iVar2 + 1) >> 1);
    }
    *(u16 *)(piVar7 + 0x73) = uVar1;
    iVar2 = (int)*(short *)(piVar5[0x4a] + 6) + (int)*(short *)(piVar5[0x4a] + 10);
    uVar1 = (u16)(iVar2 >> 1);
    if (iVar2 < 0) {
      uVar1 = (u16)((iVar2 + 1) >> 1);
    }
    *(u16 *)(piVar7 + 0x75) = uVar1;
    iVar2 = (int)*(short *)(piVar5[0x49] + 4) + (int)*(short *)(piVar5[0x49] + 8);
    uVar1 = (u16)(iVar2 >> 1);
    if (iVar2 < 0) {
      uVar1 = (u16)((iVar2 + 1) >> 1);
    }
    *(u16 *)((int)piVar7 + 0x1ce) = uVar1;
    iVar2 = (int)*(short *)(piVar5[0x4b] + 6) + (int)*(short *)(piVar5[0x4b] + 10);
    uVar1 = (u16)(iVar2 >> 1);
    if (iVar2 < 0) {
      uVar1 = (u16)((iVar2 + 1) >> 1);
    }
    *(u16 *)((int)piVar7 + 0x1d6) = uVar1;
    iVar11 = iVar11 + 1;
    piVar10 = (int *)((int)piVar10 + 2);
    puVar6 = puVar6 + 0xc;
    piVar12 = piVar12 + 2;
    puVar8 = puVar8 + 3;
    piVar7 = piVar7 + 1;
    piVar5 = piVar5 + 4;
  } while (iVar11 < 2);
  iVar11 = 0;
  puVar6 = DAT_002d7cf4;
  piVar5 = param_1;
  while (1) {
    if (*(u8 *)(DAT_00338b44 + 0x45) == 0) {
      iVar2 = 4;
    }
    else {
      iVar2 = 2;
    }
    if (iVar2 <= iVar11) break;
    if (DAT_003386b8 == 0 || DAT_00338ac4 == 0) {
      iVar2 = 0;
      uVar9 = 0;
    }
    else {
      iVar2 = *(int *)(DAT_00338ac4 + 0x198);
      uVar9 = (DAT_003418bc[iVar2 * 0x120] != 0);
    }
    FUN_000ffb3c((void *)piVar5[0x30], iVar11 != iVar2);
    iVar4 = puVar6[0x44];
    iVar2 = piVar5[0x30];
    *(u32 *)(iVar2 + 0x24) = iVar4 + uVar9;
    iVar11 = iVar11 + 1;
    *(u32 *)(iVar2 + 0x28) = iVar4 + uVar9;
    piVar5 = piVar5 + 1;
    puVar6 = puVar6 + 3;
  }
  FUN_000f3948(param_1);
  (**(code **)(*param_1 + 0xc))(param_1);
}

// Retail shape (tools/hstdis.py): 48-byte frame, nine saved regs (s0-s7); the first loop is a
// 2-iteration loop whose six induction variables are real separate pointers in the source
// (s5 = byte index for +0x1f0..+0x204, s4 = +2 stride for 0x1dc/0x1dd, s6 = +8 stride for
// 0x1e0/0x1e4, s3 = DAT_002d7cf4 + 0xc stride, s1/s0 = +4 stride, and a plain int* for 0x1cc..).
// Note the (a+b)/2 fields: retail emits addiu/sra with a bgez guard, i.e. a signed division by 2,
// never a shift.
// NEAR 192/194 words, 192 differ, first at +0x8 (hstdiff prints "ours | retail": OURS saves s8 as well,
// retail saves only s0-s7). So this draft keeps ONE MORE live pointer than retail; the frame (48) and the
// body shape are already right.
// Retail's saved set maps to: s7 = param_1, s5 = i (byte index for +0x1f0..+0x204), s4 = param_1+2i
// (0x1dc/0x1dd), s6 = param_1+8i (0x1e0/0x1e4), s3 = &DAT_002d7cf4, s2 = DAT+12i (0x248/0x254/0x308),
// s1 = param_1+4i (0xe0/0xe8/0x150/0x158/0x160/0x168/0x1cc..0x1d6/0x208), s0 = param_1+16i (0x120..0x12c).
// NEXT LEVER: express one of the two stride-2 pointers inline (`*(u8 *)((char *)param_1 + iVar11 * 2 + 0x1dc)`
// and `*(int *)((char *)param_1 + iVar11 * 8 + 0x1e0)`) so MWCC drops the extra induction variable, and
// drop the unused piVar13. Do NOT add pointers - adding one was already tried and stayed at 192.
// near miss from lane dsn8 (unreviewed)
