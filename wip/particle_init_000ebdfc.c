/* HSTMATCH module=EBOOT.BIN start=0x000ebdfc end=0x000ec0ec symbol=FUN_000ebdfc lang=c99 */
/* Hand-written reconstruction from the disassembly; near miss, not installed. See the trailing note. */
#include "hst/types.h"

extern void *DAT_00307728;
extern float DAT_002c07b0[16];
extern float DAT_002c07f0[16];
extern void FUN_0001f06c();
extern void FUN_000ed628(void *, int);
extern void FUN_000ed8c0(void *, int, int);
extern void FUN_000ec204(void *);
extern void *FUN_00000bcc(int);
extern void *FUN_0005a8ec(void *);
extern void FUN_0005a500(void *, int, int, int, int, int);
extern void FUN_0005a650(void *);
extern void FUN_0005a670(void *, int);

void *FUN_000ebdfc(int *param_1)
{
  int iVar7;
  int iVar1;
  int iVar5;
  void *iVar6;

  FUN_0001f06c();
  *param_1 = (int)&DAT_00307728;
  param_1[0x16] = 0;
  param_1[0x17] = 0;
  param_1[0x18] = 0;
  FUN_000ed628(param_1 + 0x16, 0x10);
  param_1[0x1a] = 0;
  param_1[0x1b] = 0;
  param_1[0x1c] = 0;
  param_1[0x10] = 0;
  param_1[0x11] = 0;
  param_1[0x12] = 0;
  param_1[0x13] = 0;
  param_1[0x14] = 0;
  param_1[0x15] = 0;
  iVar7 = 0;
  do {
    iVar1 = (int)FUN_00000bcc(0x70);
    if (iVar1 == 0) {
      iVar1 = 0;
    }
    else {
      iVar1 = (int)FUN_0005a8ec((void *)iVar1);
    }
    *(int *)(param_1[0x16] + iVar7 * 0x60 + 0x40) = iVar1;
    iVar6 = (void *)*(int *)(param_1[0x16] + iVar7 * 0x60 + 0x40);
    FUN_0005a500(iVar6, 0, 4, 1, 1, 0);
    FUN_0005a650(iVar6);
    iVar5 = 0;
    iVar1 = 0;
    do {
      *(float *)(*(int *)((int)iVar6 + *(int *)((int)iVar6 + 0x54) * 4 + 100) + iVar1 + 0x10) =
          DAT_002c07b0[iVar5 * 4];
      *(float *)(*(int *)((int)iVar6 + *(int *)((int)iVar6 + 0x54) * 4 + 100) + iVar1 + 0x14) =
          DAT_002c07b0[iVar5 * 4 + 1];
      *(float *)(*(int *)((int)iVar6 + *(int *)((int)iVar6 + 0x54) * 4 + 100) + iVar1 + 0x18) =
          DAT_002c07b0[iVar5 * 4 + 2];
      *(float *)(*(int *)((int)iVar6 + *(int *)((int)iVar6 + 0x54) * 4 + 100) + iVar1 + 4) =
          DAT_002c07f0[iVar5 * 4];
      *(float *)(*(int *)((int)iVar6 + *(int *)((int)iVar6 + 0x54) * 4 + 100) + iVar1 + 8) =
          DAT_002c07f0[iVar5 * 4 + 1];
      *(float *)(*(int *)((int)iVar6 + *(int *)((int)iVar6 + 0x54) * 4 + 100) + iVar1 + 0xc) =
          DAT_002c07f0[iVar5 * 4 + 2];
      iVar5 = iVar5 + 1;
      iVar1 = iVar1 + 0x1c;
    } while (iVar5 < 4);
    FUN_0005a670(iVar6, 4);
    iVar7 = iVar7 + 1;
  } while (iVar7 < 0x10);
  FUN_000ed8c0(param_1 + 0x1a, param_1[0x1b], 0);
  param_1[0x19] = param_1[0x16];
  FUN_000ec204(param_1);
  param_1[0xf] = 0x42800000;
  return param_1;
}

// MISSING vs retail: the four byte stores at base+iVar1+0,1,2,3 in the inner loop. They are NOT
// constants: retail computes each from a float with MWCC's PSP (u8)-from-float sequence
//   lui v1,0x4300 (128.0f) / mtc1 f4 ; lui v1,0x4f00 (2^31) / mtc1 f3 ; sub.s ; trunc.w.s ;
//   lui t0,0x427f (127.5f) ; c.le.s f3,f4 ; bc1t ; mfc1 / or t1,v1,v0 ; sb t1,k(t0)
// i.e. `*(u8 *)(q + k) = (u8)(scale * something + 0.5f)` with the 128.0/127.5 rounding pair, three
// times from the same float, and the fourth byte uses a different compare (c.le.s f3,f1).
// Ghidra dropped those blocks as "unreachable" (0xebfc0, 0xebfec, 0xec018, 0xec044), which is why
// the pseudocode shows four literal byte stores of 0x80,0x80,0x80,0x3f.
// NEXT LEVER: read 0xebf60..0xec058 word by word and name the float source (it is materialised
// before the loop from 0x4300/0x4f00/0x427f, so it is probably a constant vector scaled by 128),
// then write the four stores as (u8) casts of that expression; everything else in this draft
// already follows retail statement by statement, including the per-store reload of
// *(int *)(iVar6 + *(int *)(iVar6 + 0x54) * 4 + 100).
// near miss from lane dsn8 (unreviewed)
