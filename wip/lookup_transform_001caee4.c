/* HSTMATCH module=EBOOT.BIN start=0x001caee4 end=0x001cb204 symbol=FUN_001caee4 lang=c99 */
/* Hand-written reconstruction from the pseudocode; near miss, not installed. See the trailing note. */
#include "hst/types.h"
typedef struct { int *begin; int count; int cap; } Vec;

extern u8 DAT_002bc430;
extern u8 DAT_0033b574[];
extern void FUN_00022418(void *, int *, void *);
extern void FUN_001ce440(void *, void *, void *, int);
extern void FUN_001474d0(void *, int);

int FUN_001caee4(int param_1, int param_2, int *param_3, int *param_4)
{
  int uVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  int local_70;
  int local_6c;
  int local_68;
  int local_64;
  int local_60;
  int local_5c;
  int local_58;
  int local_54;
  int local_50;
  int local_4c;
  int local_48;
  int local_44;
  int local_40;
  int local_3c;
  int local_38;
  int local_34;
  u8 local_28;
  u8 local_24;
  Vec local_1c;
  Vec local_c;

  if (param_2 == 1000) {
    *param_3 = *(int *)(param_1 + 0xa0);
    param_3[1] = *(int *)(param_1 + 0xa4);
    param_3[2] = *(int *)(param_1 + 0xa8);
    param_3[3] = *(int *)(param_1 + 0xac);
    FUN_00022418(&local_40, param_3, &DAT_002bc430);
    uVar1 = 1;
    *param_4 = local_40;
    param_4[1] = local_3c;
    param_4[2] = local_38;
    param_4[3] = local_34;
  }
  else if (param_2 == 0x3e9) {
    *param_3 = *(int *)(param_1 + 0xb0);
    param_3[1] = *(int *)(param_1 + 0xb4);
    param_3[2] = *(int *)(param_1 + 0xb8);
    param_3[3] = *(int *)(param_1 + 0xbc);
    FUN_00022418(&local_50, param_3, &DAT_002bc430);
    uVar1 = 1;
    *param_4 = local_50;
    param_4[1] = local_4c;
    param_4[2] = local_48;
    param_4[3] = local_44;
  }
  else {
    local_c.begin = 0;
    local_c.count = 0;
    local_c.cap = 0;
    FUN_001ce440(&local_c, *(void **)(DAT_0033b574 + 0x9ec),
                 (void *)(*(int *)(DAT_0033b574 + 0x9ec) +
                          *(int *)(DAT_0033b574 + 0x9f0) * 0x50),
                 (int)local_24);
    if (local_c.begin == local_c.begin + local_c.count * 0x14) {
LAB_001cb0c0:
      if (-10 < param_2 && param_2 < 1) {
        iVar3 = *(int *)(param_1 + 0xd0);
        local_1c.begin = 0;
        local_1c.count = 0;
        local_1c.cap = 0;
        FUN_001ce440(&local_1c, *(void **)(DAT_0033b574 + 0x9ec),
                     (void *)(*(int *)(DAT_0033b574 + 0x9ec) +
                              *(int *)(DAT_0033b574 + 0x9f0) * 0x50),
                     (int)local_28);
        if (local_1c.begin != local_1c.begin + local_1c.count * 0x14) {
          iVar2 = *local_1c.begin;
          piVar4 = local_1c.begin;
          while (1) {
            if (iVar2 == -iVar3) {
              *param_3 = piVar4[0x10];
              param_3[1] = piVar4[0x11];
              param_3[2] = piVar4[0x12];
              param_3[3] = piVar4[0x13];
              FUN_00022418(&local_70, param_3, piVar4 + 0xc);
              *param_4 = local_70;
              param_4[1] = local_6c;
              param_4[2] = local_68;
              param_4[3] = local_64;
              FUN_001474d0(&local_1c, 0);
              FUN_001474d0(&local_c, 0);
              return 1;
            }
            piVar4 = piVar4 + 0x14;
            if (piVar4 == local_1c.begin + local_1c.count * 0x14) break;
            iVar2 = *piVar4;
          }
        }
        FUN_001474d0(&local_1c, 0);
      }
      FUN_001474d0(&local_c, 0);
      uVar1 = 0;
    }
    else {
      iVar3 = *local_c.begin;
      piVar4 = local_c.begin;
      while (iVar3 != param_2) {
        piVar4 = piVar4 + 0x14;
        if (piVar4 == local_c.begin + local_c.count * 0x14) goto LAB_001cb0c0;
        iVar3 = *piVar4;
      }
      *param_3 = piVar4[0x10];
      param_3[1] = piVar4[0x11];
      param_3[2] = piVar4[0x12];
      param_3[3] = piVar4[0x13];
      FUN_00022418(&local_60, param_3, piVar4 + 0xc);
      *param_4 = local_60;
      param_4[1] = local_5c;
      param_4[2] = local_58;
      param_4[3] = local_54;
      FUN_001474d0(&local_c, 0);
      uVar1 = 1;
    }
  }
  return uVar1;
}

// Not yet measured. Retail frame is 144 bytes with ra at 28(sp) and no stack arguments, so the
// fourth argument of FUN_001ce440 is a plain register value (Ghidra shows it as an uninitialised
// byte local - kept here as local_24 / local_28). Element stride is 0x14 ints (80 bytes).
// NEAR 183/200 words, 199 differ, first at +0x0 because the frame is 80 vs retail's 144 (64 bytes = the four
// 16-byte temporaries local_70/60/50/40: retail gives each branch its own slot, ours overlaps them because the
// branches are disjoint). Ours also saves s4 (5 regs) where retail saves s0-s3.
// IMPORTANT: with three bare ints for the container (`local_c/local_8/local_4`) MWCC constant-folded
// begin==begin+count*0x14 to true and PRUNED the whole else branch (frame 32, 93/200 words). Wrapping the
// container in `typedef struct { int *begin; int count; int cap; } Vec;` is what restored the body.
// NEXT LEVER: force all four temporaries live at once (e.g. declare them in one array `int tmp[4][4]` and index
// it, or hoist the vector copies out of the branches into a common helper call) so the frame grows to 144.
// near miss from lane dsn8 (unreviewed)
