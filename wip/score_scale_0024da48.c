/* HSTMATCH module=EBOOT.BIN start=0x0024da48 end=0x0024dd40 symbol=FUN_0024da48 lang=c99 rodata=0x002fbed4 */
/* Hand-written reconstruction from the disassembly and pseudocode; near miss, not installed. */
#include "hst/types.h"

extern char DAT_002fbe08, DAT_002fbe14, DAT_002fbe1c, DAT_002fbe28, DAT_002fbe30, DAT_002fbe3c;
extern char DAT_002fbe44, DAT_002fbe50, DAT_002fbe5c, DAT_002fbe68, DAT_002fbe70, DAT_002fbe78;
extern char DAT_002fbe80, DAT_002fbe88, DAT_002fbe90, DAT_002fbe9c, DAT_002fbea8, DAT_002fbeb0;
extern char DAT_002fbebc, DAT_002fbec4, DAT_002fbecc;
extern float FUN_000657e4(int, char *, int);
extern void FUN_00020f5c(float);
extern void FUN_0024d8dc(char *);

float FUN_0024da48(int param_1, int param_2, int param_3, int param_4)
{
  float fVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  float fVar5;
  float fVar6;

  fVar5 = 0.0f;
  fVar6 = 0.0f;
  fVar3 = 0.0f;
  if (param_2 == 5) {
    fVar5 = FUN_000657e4(param_1, &DAT_002fbe30, 1);
    fVar6 = FUN_000657e4(param_1, &DAT_002fbe3c, 1);
  }
  else if (param_2 == 3) {
    fVar5 = FUN_000657e4(param_1, &DAT_002fbe1c, 1);
    fVar6 = FUN_000657e4(param_1, &DAT_002fbe28, 1);
  }
  else if (param_2 == 1) {
    fVar5 = FUN_000657e4(param_1, &DAT_002fbe08, 1);
    fVar6 = FUN_000657e4(param_1, &DAT_002fbe14, 1);
  }
  switch (param_4) {
  case 0:
    fVar3 = FUN_000657e4(param_1, &DAT_002fbe44, 1);
    break;
  case 1:
    fVar3 = FUN_000657e4(param_1, &DAT_002fbe50, 1);
    break;
  case 2:
    fVar3 = FUN_000657e4(param_1, &DAT_002fbe5c, 1);
    break;
  case 3:
    fVar3 = FUN_000657e4(param_1, &DAT_002fbe68, 1);
    break;
  case 4:
    fVar3 = FUN_000657e4(param_1, &DAT_002fbe70, 1);
    break;
  case 5:
    fVar3 = FUN_000657e4(param_1, &DAT_002fbe78, 1);
  }
  fVar1 = FUN_000657e4(param_1, &DAT_002fbe80, 1);
  fVar2 = FUN_000657e4(param_1, &DAT_002fbe88, 1);
  fVar4 = fVar3 * fVar2 * fVar6 * (fVar1 + (float)param_3 * fVar5);
  FUN_00020f5c((float)param_3);
  FUN_0024d8dc(&DAT_002fbe90);
  FUN_00020f5c(fVar5);
  FUN_0024d8dc(&DAT_002fbe9c);
  FUN_00020f5c(fVar1);
  FUN_0024d8dc(&DAT_002fbea8);
  FUN_00020f5c(fVar6);
  FUN_0024d8dc(&DAT_002fbeb0);
  FUN_00020f5c(fVar2);
  FUN_0024d8dc(&DAT_002fbebc);
  FUN_00020f5c(fVar3);
  FUN_0024d8dc(&DAT_002fbec4);
  FUN_00020f5c(fVar4);
  FUN_0024d8dc(&DAT_002fbecc);
  return fVar4;
}
// NEAR 175/190 words, 172 differ (first at +0x24). Retail's zero-init colouring differs: it issues
//   mtc1 zero,$f23 at the slot where ours issues mtc1 zero,$f25, then mov.s $f24,$f25 / $f23,$f25,
// so fVar3/fVar5/fVar6 land in f23/f24/f25 in a different order than this draft's declaration order.
// Everything before +0x24 already matches, including the 48-byte frame and the four swc1 saves.
// NEXT LEVER: permute the float declaration order (fVar3/fVar5/fVar6 first, fVar5/fVar6/fVar3 first,
//   fVar6/fVar5/fVar3 first) - this is the MWCC-DIGEST section 7/8 colouring lever, not a body issue;
//   then re-read 0x24db38 onward for the switch(param_4) dispatch shape (retail may lower it as a
//   6-way beq chain rather than a jump table).
// CORRECTION 2026-11-01: hstdiff prints "ours | retail". Ours issues mtc1 zero,$f23; RETAIL issues
// $f25. The declaration-order lever still applies, but mirrored: retail colours the first zero to f25.
// near miss from lane dsn8 (unreviewed)
