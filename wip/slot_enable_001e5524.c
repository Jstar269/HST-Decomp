/* HSTMATCH module=EBOOT.BIN start=0x001e5524 end=0x001e5934 symbol=FUN_001e5524 lang=c99 */
/* Hand-written from the pseudocode; enables/disables a family of HUD handles by mode, with five virtual slot-0xc calls. */

typedef int code();

extern char *FUN_001ee200(void);
extern int   FUN_001bc630(float value);
extern void  FUN_000ffb3c(void *handle, int on);
extern void  FUN_0011020c(float value, void *obj, int flag);
extern int   FUN_00216e00(int *out);
extern int   DAT_002c8ce0[];
extern int   DAT_002ec19c[];

void FUN_001e5524(int param_1, int param_2)
{
  char cVar1;
  int bVar2;
  int bVar3;
  int bVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar10;
  int *piVar8;
  int *pcVar9;
  float fVar11;
  float fVar12;
  float fVar13;
  int uVar13;
  int local_c;
  int local_8;
  int local_4;

  iVar5 = (int)FUN_001ee200();
  fVar13 = *(float *)(iVar5 + 0x5e4);
  iVar5 = (int)FUN_001ee200();
  fVar11 = *(float *)(iVar5 + 0x5e8);
  iVar5 = (int)FUN_001ee200();
  fVar12 = *(float *)(iVar5 + 0x5f0);
  iVar5 = (int)FUN_001ee200();
  cVar1 = *(char *)(iVar5 + 0x5f5);
  iVar5 = FUN_001bc630(fVar13);
  iVar6 = FUN_001bc630(fVar12);
  iVar10 = DAT_002c8ce0[iVar5];
  iVar6 = DAT_002c8ce0[iVar6];
  bVar2 = 0;
  bVar3 = 0;
  bVar4 = 0;
  iVar5 = (int)FUN_001ee200();
  iVar5 = *(int *)(iVar5 + 0x5dc);
  if (iVar5 == 4 || iVar5 == 3) {
    bVar3 = 1;
    bVar4 = 1;
  }
  else if (iVar5 == 2) {
    bVar3 = 1;
  }
  else if (iVar5 != 1) goto LAB_001e5620;
  bVar2 = 1;
LAB_001e5620:
  piVar8 = (int *)(param_1 + 0x1778);
  if (bVar2) {
    FUN_000ffb3c((void *)*piVar8, 0);
    iVar7 = *(int *)(param_1 + 0x177c);
    iVar5 = *piVar8;
    *(int *)(iVar5 + 0x24) = iVar7 + iVar10;
    *(int *)(iVar5 + 0x28) = iVar7 + iVar10;
    (**(code **)(**(int **)(param_1 + 0x181c) + 0xc))(*(int **)(param_1 + 0x181c), 0);
    FUN_0011020c(fVar13, *(void **)(param_1 + 0x181c), 0);
  }
  else {
    FUN_000ffb3c((void *)*piVar8, 1);
    FUN_000ffb3c(*(void **)(param_1 + 0x1818), 1);
  }
  if (bVar3) {
    (**(code **)(**(int **)(param_1 + 0x1828) + 0xc))(*(int **)(param_1 + 0x1828), 0);
    FUN_0011020c(fVar11, *(void **)(param_1 + 0x1828), 1);
    local_c = DAT_002ec19c[0];
    local_8 = DAT_002ec19c[1];
    local_4 = DAT_002ec19c[2];
    if (fVar11 < 0.0f) {
      uVar13 = *(int *)(param_1 + 0x182c);
    }
    else {
      uVar13 = FUN_00216e00(&local_c);
    }
    iVar5 = 0;
    if (0 < *(int *)(*(int *)(param_1 + 0x1828) + 0x14)) {
      iVar10 = *(int *)(*(int *)(param_1 + 0x1828) + 0x10);
      while (1) {
        iVar7 = iVar5 * 4;
        iVar5 = iVar5 + 1;
        **(int **)(iVar10 + iVar7) = uVar13;
        if (*(int *)(*(int *)(param_1 + 0x1828) + 0x14) <= iVar5) break;
        iVar10 = *(int *)(*(int *)(param_1 + 0x1828) + 0x10);
      }
    }
  }
  else {
    (**(code **)(**(int **)(param_1 + 0x1828) + 0xc))(*(int **)(param_1 + 0x1828), 1);
  }
  piVar8 = (int *)(param_1 + 0x178c);
  if (bVar4) {
    FUN_000ffb3c((void *)*piVar8, 0);
    iVar5 = *piVar8;
    iVar10 = *(int *)(param_1 + 0x1790) + iVar6;
    *(int *)(iVar5 + 0x24) = iVar10;
    *(int *)(iVar5 + 0x28) = iVar10;
    (**(code **)(**(int **)(param_1 + 0x1834) + 0xc))(*(int **)(param_1 + 0x1834), 0);
    FUN_0011020c(fVar12, *(void **)(param_1 + 0x1834), 0);
    uVar13 = *(int *)(param_1 + 0x1764);
  }
  else {
    FUN_000ffb3c((void *)*piVar8, 1);
    (**(code **)(**(int **)(param_1 + 0x1834) + 0xc))(*(int **)(param_1 + 0x1834), 1);
    uVar13 = *(int *)(param_1 + 0x1764);
  }
  pcVar9 = (int *)(param_1 + 0x17b4);
  piVar8 = (int *)(param_1 + 0x17a0);
  FUN_000ffb3c((void *)uVar13, 1);
  FUN_000ffb3c((void *)*pcVar9, 1);
  FUN_000ffb3c((void *)*piVar8, 1);
  FUN_000ffb3c(*(void **)(param_1 + 0x17c8), 1);
  FUN_000ffb3c(*(void **)(param_1 + 0x17dc), 1);
  FUN_000ffb3c(*(void **)(param_1 + 0x17f0), 1);
  if (param_2 != 0) {
    FUN_000ffb3c(*(void **)(param_1 + 0x1764), 0);
    iVar5 = *(int *)(param_1 + 0x1764);
    iVar6 = *(int *)(param_1 + 0x1768) + iVar6;
    *(int *)(iVar5 + 0x24) = iVar6;
    *(int *)(iVar5 + 0x28) = iVar6;
    if (cVar1 == 0) {
      FUN_000ffb3c((void *)*pcVar9, 0);
      iVar5 = *pcVar9;
      iVar6 = *(int *)(param_1 + 0x17b8) + 1;
      *(int *)(iVar5 + 0x24) = iVar6;
      *(int *)(iVar5 + 0x28) = iVar6;
      FUN_000ffb3c((void *)*piVar8, 0);
      iVar5 = *piVar8;
      iVar6 = *(int *)(param_1 + 0x17a4) + 1;
      *(int *)(iVar5 + 0x24) = iVar6;
      *(int *)(iVar5 + 0x28) = iVar6;
    }
    else {
      FUN_000ffb3c((void *)*pcVar9, 0);
      uVar13 = *(int *)(param_1 + 0x17b8);
      iVar5 = *pcVar9;
      *(int *)(iVar5 + 0x24) = uVar13;
      *(int *)(iVar5 + 0x28) = uVar13;
      FUN_000ffb3c((void *)*piVar8, 0);
      uVar13 = *(int *)(param_1 + 0x17a4);
      iVar5 = *piVar8;
      *(int *)(iVar5 + 0x24) = uVar13;
      *(int *)(iVar5 + 0x28) = uVar13;
    }
    FUN_000ffb3c(*(void **)(param_1 + 0x17f0), 0);
    if (fVar12 <= 0.01f) {
      FUN_000ffb3c(*(void **)(param_1 + 0x17c8), 0);
      FUN_000ffb3c(*(void **)(param_1 + 0x17dc), 0);
    }
  }
}

/* PARKED 2026-11-01 (batch 1): NEAR 249/260 words, 237 differ, first at +0x38.
 * FRAME LANDS: 80 (`27bdffb0`) and the prologue matches word for word through ra/s8/s7/s6 at
 *   0x001e5528..0x001e5534 - the saved-set spelling (s8..s0 + f22/f21) is right.
 * The first difference is at +0x38, i.e. the first body instruction after the saves, so the whole
 * divergence is the body, not the frame.
 * BANKED TRAPS THAT ARE ALREADY APPLIED: FUN_0011020c takes a FLOAT first argument (f12) and the
 *   +0x5e4 field is a float, not an int (Ghidra typed it int); FUN_001ee200 is called five times and
 *   never cached; `FUN_000017b0 + param_1 + 4` is the field at param_1 + 0x17b4, not a label.
 * NEXT LEVER: at +0x38 retail's first body instruction decides the order - if it is `jal 0x1ee200`
 *   then our cast `(int)FUN_001ee200()` is the problem (spell it `char *` and cast at the field);
 *   if it is `lwc1` from 0x5e4 then the float read order is right and the next difference is the
 *   table lookup into DAT_002c8ce0.
 */

/* BATCH 2 (2026-11-01): all three variants measure IDENTICAL - 249/260 words, 237 differ.
 *   lbu-char (unsigned char cVar1), int-getter (int-returning FUN_001ee200, no casts),
 *   idx-locals (separate locals for the two FUN_001bc630 results).
 * That means the remaining 11-word gap is not the char signedness, not the getter's return type and
 *   not the index locals - MWCC emits the same body for all three spellings.
 * WHAT THE BODY COMPARISON SHOWED (first 20 body words): retail stores the FIRST FUN_001bc630 result
 *   to a stack local (`sw v0,64(sp)` in the delay slot) and reloads it; ours keeps it in a register.
 *   Retail also keeps the 0x5f5 byte in a register across both conversions (`lbu v0,1525(v0)` then
 *   `mov.s $f12,$f22` before the first jal), which ours does too. So the missing 11 words are the
 *   spill/reload pair plus the mode-switch bookkeeping, not a dropped call - every one of the 24
 *   calls in the pseudo is present in our body.
 * NEXT LEVER: force the spill by declaring the first conversion's result as a variable that is
 *   assigned before the second call and only used after it (`int n1 = FUN_001bc630(fVar13);` then
 *   the second call, then `DAT_002c8ce0[n1]`), so MWCC must home it at 64(sp).
 */
// near miss from lane dsn8 (unreviewed)
