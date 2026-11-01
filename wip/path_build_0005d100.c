/* HSTMATCH module=EBOOT.BIN start=0x0005d100 end=0x0005d4f0 symbol=FUN_0005d100 lang=c99 */
/* Hand-written reconstruction from the pseudocode; near miss, not installed. See the trailing note. */
#include "hst/types.h"

extern u8 DAT_00311170;
extern u32 *FUN_000149a8();
extern int FUN_00014d64();
extern char *FUN_00014b74();
extern u32 *FUN_00014788();
extern int FUN_00014a40();
extern u8 *FUN_00014834();
extern int FUN_0004f6b4();
extern void FUN_00049194();
extern void FUN_000491cc();
extern int FUN_0002e840();
extern void FUN_00003268();
extern int FUN_0002e650();
extern char *FUN_0002ea44();
extern int FUN_0002e40c();
extern int *FUN_0002e3d0();
extern int FUN_00014934();
extern void FUN_0005daf0();
extern void FUN_0005daac();
extern void FUN_00004138();

void FUN_0005d100(int param_1, int param_2, int param_3, u8 param_4, int param_5, int param_6, int param_7)
{
  u32 uVar13;
  int iVar10;
  int iVar11;
  int iVar15;
  int iVar5;
  int iVar7;
  int iVar9;
  int iVar12;
  int *piVar6;
  int *puVar8;
  u8 *puVar3;
  u8 *puVar14;
  int uVar2;
  char *uVar4;
  int bVar1;
  int local_218;
  int local_214;
  int local_210;
  u8 auStack_20c[256];
  int local_10c;
  int local_108;
  int local_104;
  u8 auStack_100[256];

  {
    int k2 = param_2;
    int k4 = param_4;
    int k7 = param_7;
    param_2 = k2;
    param_4 = (u8)k4;
    param_7 = k7;
    (void)k2; (void)k4; (void)k7;
  }
  if (*(int *)(param_1 + 0x28) == 0) {
    uVar13 = 0;
    if (*(int *)(param_1 + 0xc) != 0) {
      iVar10 = 0;
      do {
        iVar11 = *(int *)(*(int *)(param_1 + 8) + uVar13 * 4);
        FUN_000149a8(auStack_100, iVar11);
        if (param_5 != 0 && param_7 != 0 &&
            (iVar15 = FUN_00014d64((char *)iVar11, (char *)param_5), iVar15 != 0)) {
          FUN_00014b74(auStack_100, iVar11, iVar15 - iVar11);
          auStack_100[iVar15 - iVar11] = 0;
          iVar11 = FUN_00014a40(param_5);
          FUN_00014788(auStack_100, param_7);
          FUN_00014788(auStack_100, iVar15 + iVar11);
        }
        uVar2 = FUN_0004f6b4(&DAT_00311170, auStack_100, param_3, 0, 0, param_4, 0x40, 1);
        uVar13 = uVar13 + 1;
        *(int *)(*(int *)(param_1 + 0x20) + iVar10) = uVar2;
        iVar10 = iVar10 + 4;
      } while (uVar13 < *(u32 *)(param_1 + 0xc));
    }
    FUN_00049194();
    iVar15 = *(int *)(param_2 + 8) + 0x10;
    iVar10 = FUN_0002e840(iVar15);
    local_10c = 0;
    local_108 = 0;
    local_104 = 0;
    iVar11 = *(int *)(param_1 + 4);
    FUN_000149a8(auStack_20c, iVar11);
    if (param_5 != 0 && param_6 != 0 &&
        (iVar5 = FUN_00014d64((char *)iVar11, (char *)param_5), iVar5 != 0)) {
      FUN_00014b74(auStack_20c, iVar11, iVar5 - iVar11);
      auStack_20c[iVar5 - iVar11] = 0;
      iVar11 = FUN_00014a40(param_5);
      FUN_00014788(auStack_20c, param_6);
      FUN_00014788(auStack_20c, iVar5 + iVar11);
    }
    puVar3 = FUN_00014834(auStack_20c, 0x3a);
    if (puVar3 != (u8 *)0x0) {
      *puVar3 = 0;
      FUN_00003268(&local_10c, auStack_20c);
      *puVar3 = 0x3a;
      puVar14 = puVar3 + 1;
    }
    else {
      puVar14 = auStack_20c;
    }
    iVar11 = 0;
    if (0 < iVar10) {
      do {
        uVar2 = FUN_0002e650(iVar15, iVar11);
        bVar1 = 1;
        if (puVar3 != (u8 *)0x0) {
          uVar4 = FUN_0002ea44(iVar15, iVar11);
          iVar5 = local_104;
          if ((local_10c & 1) == 0) {
            iVar5 = (int)&local_10c + 1;
          }
          iVar5 = FUN_00014d64((char *)uVar4, (char *)iVar5);
          if (iVar5 == 0) {
            bVar1 = 0;
          }
        }
        if (bVar1) {
          iVar5 = FUN_0002e40c(uVar2);
          iVar12 = 0;
          if (0 < iVar5) {
            do {
              piVar6 = (int *)FUN_0002e3d0(uVar2, iVar12);
              iVar9 = *piVar6;
              iVar7 = FUN_00014934(*(int *)(iVar9 + 0x18), puVar14);
              if (iVar7 == 0) {
                FUN_000491cc();
                if (*(u32 *)(param_1 + 0x2c) < *(int *)(param_1 + 0x28) + 1U) {
                  FUN_0005daf0(param_1 + 0x24);
                }
                local_210 = 0xffffffff;
                local_218 = uVar2;
                local_214 = iVar9;
                local_210 = FUN_0004f6b4(&DAT_00311170, puVar14, param_3, 0, 0, 0, 0x40, 1);
                if (*(u32 *)(param_1 + 0x28) < *(u32 *)(param_1 + 0x2c)) {
                  puVar8 = (int *)(*(int *)(param_1 + 0x24) + *(u32 *)(param_1 + 0x28) * 0xc);
                  if (puVar8 == (int *)0x0) {
                    iVar5 = *(int *)(param_1 + 0x28);
                  }
                  else {
                    *puVar8 = local_218;
                    puVar8[1] = local_214;
                    puVar8[2] = local_210;
                    iVar5 = *(int *)(param_1 + 0x28);
                  }
                  *(int *)(param_1 + 0x28) = iVar5 + 1;
                }
                else {
                  FUN_0005daac(param_1 + 0x24, &local_218, 0);
                }
                FUN_00049194();
                break;
              }
              iVar12 = iVar12 + 1;
            } while (iVar12 < iVar5);
          }
        }
        iVar11 = iVar11 + 1;
      } while (iVar11 < iVar10);
    }
    FUN_000491cc();
    FUN_00004138(&local_10c, 0xffffffff);
  }
}

// Not yet measured. Two 256-byte buffers (auStack_100 for the header loop, auStack_20c for the body),
// the repeated copy/find/truncate/append block from src/eboot/FUN_001b177c.c, and the colon split that
// saves the separator byte in puVar3 and restores it after FUN_00003268.
// NEAR 246/252 words, 237 differ, first at +0x0: frame 592 vs retail's 624 (32 bytes).
// Ours packs to 8 bytes of slack after the two 256-byte buffers and the two 3-int groups; retail has 40,
// so retail holds one more 32-byte (or two 16-byte) slot that Ghidra's local list does not show.
// Ruled out: unused padding locals (dropped, frame unchanged at 592); wrapping both 3-int groups in a
// struct T3 (frame grows but diverge -> 254/252 words, 242 differ).
// NEXT LEVER: find the missing slot by reading sp-relative offsets in the disassembly (which of 0x30..0x260
// is actually referenced) rather than by adding declarations, then place it so the slack matches.
// SLOT MAP (read from hstdis, retail): saves s0-s8 at 8..44, then 60/64/68 = local_10c triple,
// 75(sp)=param_4 byte, 76(sp)=param_2 word, 96 and 356 = the two 256-byte buffers, frame 624.
// So the missing 32 bytes are SPILLED PARAMETERS (a1 at 76, a3 at 75, t1 at 68), not extra locals:
// retail spills param_2/param_4/param_7 to the frame at entry and reloads them after calls. That is a
// register-pressure consequence of the real body shape - do not add locals, make the body force the
// spill (the three params are live across the FUN_0004f6b4 calls in both loops).
// PROMOTED best: copy-through-locals = 247/252 words, 195 differ (was 237). Homing the three params via
// locals (`int k2 = param_2; ... param_2 = k2;`) is the direction that makes retail's frame slots appear;
// address-of-params is worse (240). Continue from here: the remaining gap is in which of the three gets
// its own homed slot (retail homes a1->76, a3->75, t1->68, i.e. param_2, param_4, param_7).
// BATCH 2 (three homing spellings, all worse): byte-first / retail-slot-order / reverse-stores each gave
// 248/252 words, 241 differ - so keep THIS body (copy-through-locals, 247/252, 195 differ).
// Smallest residual: 195 differ, first differing word at +0x0: `addiu sp,sp,-592` vs retail `-624`
// (frame still 32 short). Homing order in the source does not change it; the 32 bytes are the three
// homed parameter slots themselves (76 / 75 / 68 in retail) and MWCC is not giving this body those slots.
// PARKED after its two hsttry batches. Residual: 247/252 words, 195 differing; first differing word is the
// frame adjust `addiu sp,sp,-592` against retail's `-624`.
// NEXT LEVER (specific): a frame exactly 32 bytes short is the classic signature of a call that needs STACK
// ARGUMENTS. Grep the retail jal sites for `sw <reg>,16(sp)` / `24(sp)` / `32(sp)` immediately before a call
// and give the matching call site more than four arguments, instead of trying to home parameters (three
// homing spellings already failed: byte-first / retail-slot-order / reverse-stores all gave 241 differ).
// near miss from lane dsn8 (unreviewed)
