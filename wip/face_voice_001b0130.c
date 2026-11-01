/* HSTMATCH module=EBOOT.BIN start=0x001b0130 end=0x001b043c symbol=FUN_001b0130 lang=c99 */
/* Hand-written reconstruction from the pseudocode; near miss, not installed. See the trailing note. */
#include "hst/types.h"

extern u8 DAT_002c619c;
extern u8 DAT_002c61a0;
extern u8 DAT_002e9618;
extern u8 DAT_002e9614;
extern char DAT_002e95f0[];
extern char *PTR_DAT_002e95c0[];
extern u8 DAT_00338b44[];
extern char DAT_0030ebf4[];
extern void FUN_001b052c();
extern u32 FUN_00014a40(void *);
extern void FUN_000149a8(void *, int);
extern int FUN_001b1744(int);
extern void FUN_0001ca58(void *, char *, void *, void *, void *);
extern int FUN_00014a58(int, char *, u32);
extern void FUN_000111c0(void *, void *, int);
extern int FUN_0000d62c(void *);
extern void FUN_001b0ef8(int, void *, int, int, int);
extern void FUN_0001cb88(void *);

void FUN_001b0130(int param_1, int param_2, int param_3, int param_4)
{
  char *pcVar1;
  u32 uVar2;
  int iVar3;
  int uVar4;
  char *pcVar5;
  int iVar6;
  char *pcVar7;
  u8 *puVar8;
  u8 *puVar9;
  char **ppuVar10;
  u8 uVar11;
  char local_44[68];

  iVar6 = param_2 * 4 + param_1;
  if (*(int *)(iVar6 + 0x768) != 0) {
    FUN_001b052c();
  }
  if (param_3 == 0) {
    return;
  }
  uVar2 = FUN_00014a40((void *)param_3);
  if (uVar2 < 2) {
    return;
  }
  if (param_4 < 0 || 9 < param_4) {
    param_4 = 0;
  }
  FUN_000149a8(local_44 + 4, param_3);
  iVar3 = FUN_00014a40(local_44 + 4);
  if (local_44[iVar3 + 3] == '*') {
    iVar3 = FUN_00014a40(local_44 + 4);
    local_44[iVar3 + 3] = (char)param_4 + 'a';
    iVar3 = *(int *)(param_1 + 0x79c);
  }
  else {
    iVar3 = *(int *)(param_1 + 0x79c);
  }
  if (iVar3 < 0) {
    iVar3 = *(int *)(param_1 + 0x7a0);
  }
  iVar3 = FUN_001b1744(iVar3);
  if (iVar3 == 0) {
    puVar9 = &DAT_002c619c;
  }
  else {
    puVar9 = &DAT_002c61a0;
  }
  if (DAT_00338b44 == 0 || DAT_00338b44[0x45] != 0) {
    puVar8 = &DAT_002e9618;
  }
  else {
    puVar8 = &DAT_002e9614;
  }
  FUN_0001ca58(&DAT_0030ebf4, DAT_002e95f0, puVar8, local_44 + 4, puVar9);
  puVar9 = (u8 *)0x1da;
  uVar11 = 0;
  uVar2 = 0;
  ppuVar10 = PTR_DAT_002e95c0;
  while (1) {
    uVar4 = FUN_00014a40(*ppuVar10);
    iVar3 = FUN_00014a58(param_3, *ppuVar10, uVar4);
    if (iVar3 == 0) break;
    uVar2 = uVar2 + 1;
    ppuVar10 = ppuVar10 + 6;
    if (1 < uVar2) goto LAB_001b036c;
  }
  pcVar5 = (char *)4;
  pcVar7 = local_44;
  pcVar1 = pcVar7;
  while (pcVar1 != 0) {
    *pcVar7 = 0;
    pcVar7 = pcVar7 + 1;
    pcVar5 = pcVar5 + -1;
    pcVar1 = pcVar5;
  }
  FUN_000111c0(local_44, ppuVar10[4] + param_3, 2);
  if (DAT_00338b44 == 0) {
    puVar9 = (u8 *)ppuVar10[1];
  }
  else if (DAT_00338b44[0x45] == 0) {
    puVar9 = (u8 *)ppuVar10[2];
  }
  else {
    puVar9 = (u8 *)ppuVar10[1];
  }
  iVar3 = FUN_0000d62c(local_44);
  puVar9 = puVar9 + param_4 + (int)ppuVar10[3] * iVar3;
  if (uVar2 == 1) {
    if (ppuVar10[5][param_3] == 'w') {
      puVar9 = puVar9 + 0x1e;
      uVar11 = 1;
    }
    else {
      uVar11 = 1;
    }
  }
LAB_001b036c:
  if ((((int)puVar9 < 0xc3 || 0xe0 < (int)puVar9) &&
       ((int)puVar9 < 0xe1 || 0xfe < (int)puVar9)) &&
      ((int)puVar9 < 0xff || 300 < (int)puVar9)) {
    if ((int)puVar9 < 0x12d) {
      puVar9 = (u8 *)0xe1;
    }
    else if (0x15a < (int)puVar9) {
      puVar9 = (u8 *)0xe1;
    }
  }
  FUN_001b0ef8(param_1, puVar9, 0, 0, 0);
  *(int *)(iVar6 + 0x768) = *(int *)(param_1 + (int)puVar9 * 4);
  *(u8 *)(param_2 + param_1 + 0x798) = uVar11;
  FUN_0001cb88(&DAT_0030ebf4);
}

// Not yet measured. Structure read from ram-0x001b0130.c; the table walk uses PTR_DAT_002e95c0 with a
// 6-word stride, and the four-byte zero loop in the pseudocode is a plain `for (k = 0; k < 4; k++)`.
// NEAR 190/195 words, 174 differ, first at +0x150 (hstdiff prints "ours | retail"). The 128-byte frame and
// s8..s3 saves already match; then retail issues `move s6,a1` before `sw s2,16(sp)` and colours param_1 as
// s5 where this draft uses s4 - a saved-register colouring difference, not a body difference, so the whole
// tail shifts. 16 new symbols were appended to symbols/EBOOT.BIN.txt for this item (PTR_DAT_002e95c0 and the
// FUN_00014a40/149a8/14a58/111c0/0d62c/1ca58/1cb88/1b052c/1b1744/1b0ef8 callees plus the five strings).
// NEXT LEVER: permute the local declaration order (declaration order drives saved-register colouring:
// MWCC-DIGEST 7/8) so param_1 lands in s5 rather than s4, and keep the 68-byte local as `char local_44[68]`.
// near miss from lane dsn8 (unreviewed)
