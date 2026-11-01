/* HSTMATCH module=EBOOT.BIN start=0x001b76ec end=0x001b7b40 symbol=FUN_001b76ec lang=c99 */
/* Hand-written from the pseudocode; float-range dispatch over foot bones with per-branch vector temporaries. */

extern int   FUN_000b3464(int obj);
extern int   FUN_000b342c(int obj);
extern int   FUN_000b3490(int obj);
extern int   FUN_000b34d0(int obj);
extern int   FUN_000b3558(int obj);
extern int   FUN_001b1d10(void *a, int b, float *v, void *d);
extern void  FUN_0005548c(float *mat, int a, void *name);
extern float *FUN_00093838(int a);
extern void  FUN_001b210c(float a, float b, void *c, void *d);
extern float DAT_002e9a04;
extern float DAT_002e9a08;
extern float DAT_002e9a0c;
extern float DAT_002e9a10;
extern float DAT_002e9a14;
extern float DAT_002e9a18;
extern float DAT_002e9a1c;
extern float DAT_002e9a20;
extern char  DAT_002e9a24[];
extern char  DAT_002e9a30[];

void FUN_001b76ec(int param_1, int param_2)
{
  float v20[4], v30[4], v40[4], v50[4], v60[4], v70[4], v80[4];
  float mat180[16], mat140[16], mat100[16], matc0[16];
  float fVar5, fVar6;
  float local_4;
  int local_c;
  int local_8;
  int iVar3, iVar4;
  float *puVar2;

  iVar4 = *(int *)(param_2 + 0x5bc);
  iVar3 = *(int *)(iVar4 + 0x58);
  fVar6 = *(float *)(iVar3 + 0x104);
  fVar5 = *(float *)(iVar3 + 0x100);
  if (!FUN_000b3464(param_2)) {
    if (FUN_000b342c(param_2) == 0) {
      if (FUN_000b3490(param_2) == 0) {
        if (FUN_000b34d0(param_2) == 0) {
          if (FUN_000b3558(param_2) != 0) {
           if (DAT_002e9a1c <= fVar6) {
            if (fVar6 < DAT_002e9a1c + fVar5) {
            puVar2 = (float *)FUN_00093838(iVar4);
            v80[0] = puVar2[0];
            v80[1] = puVar2[1];
            v80[2] = puVar2[2];
            v80[3] = puVar2[3];
            FUN_001b1d10((void *)0x34179c, 0x55, v80, 0);
            }
           }
          }
          else if (DAT_002e9a18 <= fVar6 && fVar6 < DAT_002e9a18 + fVar5) {
            puVar2 = (float *)FUN_00093838(iVar4);
            v70[0] = puVar2[0];
            v70[1] = puVar2[1];
            v70[2] = puVar2[2];
            v70[3] = puVar2[3];
            FUN_001b1d10((void *)0x34179c, 0x56, v70, 0);
          }
        }
        else if (DAT_002e9a14 <= fVar6 && fVar6 < DAT_002e9a14 + fVar5) {
          puVar2 = (float *)FUN_00093838(iVar4);
          v60[0] = puVar2[0];
          v60[1] = puVar2[1];
          v60[2] = puVar2[2];
          v60[3] = puVar2[3];
          FUN_001b1d10((void *)0x34179c, 0x56, v60, 0);
        }
      }
      else {
        local_4 = 1.0f;
        if (fVar6 < DAT_002e9a0c || DAT_002e9a0c + fVar5 <= fVar6) {
          if (DAT_002e9a10 <= fVar6 && fVar6 < DAT_002e9a10 + fVar5) {
            FUN_0005548c(mat180, iVar3, DAT_002e9a30);
            v50[2] = mat180[14];
            v50[0] = mat180[12];
            v50[1] = mat180[13];
            v50[3] = mat180[15];
            local_c = FUN_001b1d10((void *)0x34179c, 0x55, v50, &local_4);
            local_4 = local_4 * DAT_002e9a20;
            FUN_001b210c(local_4, local_4, (void *)0x34179c, &local_c);
          }
        }
        else {
          FUN_0005548c(mat140, iVar3, DAT_002e9a24);
          v40[2] = mat140[14];
          v40[0] = mat140[12];
          v40[1] = mat140[13];
          v40[3] = mat140[15];
          local_8 = FUN_001b1d10((void *)0x34179c, 0x55, v40, &local_4);
          local_4 = local_4 * DAT_002e9a20;
          FUN_001b210c(local_4, local_4, (void *)0x34179c, &local_8);
        }
      }
    }
    else if (fVar6 < DAT_002e9a04 || DAT_002e9a04 + fVar5 <= fVar6) {
      if (DAT_002e9a08 <= fVar6 && fVar6 < DAT_002e9a08 + fVar5) {
        FUN_0005548c(mat100, iVar3, DAT_002e9a30);
        v30[2] = mat100[14];
        v30[0] = mat100[12];
        v30[1] = mat100[13];
        v30[3] = mat100[15];
        FUN_001b1d10((void *)0x34179c, 0x55, v30, 0);
      }
    }
    else {
      FUN_0005548c(matc0, iVar3, DAT_002e9a24);
      v20[2] = matc0[14];
      v20[0] = matc0[12];
      v20[1] = matc0[13];
      v20[3] = matc0[15];
      FUN_001b1d10((void *)0x34179c, 0x55, v20, 0);
    }
  }
}

/* PARKED 2026-11-01 (batch 1): NEAR 272/277 words, 254 differ, first at +0x38.
 * FRAME LANDS: 416 (`27bdfe60` identical) with the named-temporaries spelling; the prologue through
 *   ra/s2/s1/s0/f21/f20 matches.
 * ONE FIX NEEDED: FUN_001b1d10 RETURNS INT - it is assigned to local_c/local_8 in the two foot
 *   branches, so a `void` prototype aborts the compile ("illegal use of type 'void'"). Every other
 *   EXACT file declares it void; this item is the counterexample, so declare `int` here.
 * UNRESOLVED GUESS (settle with the diff): the FUN_0005548c outputs are copied from the block at
 *   `buffer - 0x30` (Ghidra: `local_48 = local_148` after writing auStack_180). This draft reads
 *   mat[12..15]; if the copies target the first row instead, switch to mat[0..3] - the diff at the
 *   first FUN_0005548c branch will say which.
 * NEXT LEVER: the nested chain order is right (five separate tests, not &&), so work the else-if
 *   ladder: retail checks 0x5e4-order globals 002e9a1c -> 002e9a18 -> 002e9a14 -> 002e9a10 -> 002e9a08,
 *   descending; if ours emits them ascending the branch tree is inverted somewhere.
 */

/* BATCH 2 (2026-11-01): settled the FUN_0005548c mapping from the DISASSEMBLY, not the pseudo:
 *   site 1: `addiu a0,sp,224` writes the buffer, then the copy reads sp+0x110..0x11c = buffer+0x30
 *   (mat[12..15]) and stores to sp+0x180..0x18c - a SEPARATE destination vector. So mat[12..15] is
 *   confirmed correct, and the row0-copy variant (reading mat[0..3]) measures BYTE-IDENTICAL
 *   (254 differ / 272 words), which means the whole copy block is codegen-neutral either way.
 *   shared-dest (one vector reused for all four copy-backs) does not compile; early-local4 (hoisting
 *   local_4 = 1.0f to the top) is WORSE: 263 differ / 274 words. Base stays best at 272.
 * REAL FINDING AT +0x38: retail branches `beqzl v0,0x1b782c` after the first predicate (continue on
 *   ZERO, forward), while ours emits `bnezl v0,0x1b7b10` (bail out on NON-zero). Same semantics,
 *   inverted layout - so the chain's branch polarity is the remaining structural difference.
 * NEXT LEVER: write the five predicate tests so the non-zero case is the fall-through and the
 *   zero case jumps forward - e.g. `if (f(...)) { } else if (g(...)) { } ...` with the real body on
 *   the zero side, or an early `if (x != 0) goto done;` ladder guarded by the else-branch work.
 */

/* BATCH 2 (2026-11-01): settled the FUN_0005548c mapping from the DISASSEMBLY, not the pseudo:
 *   site 1: `addiu a0,sp,224` writes the buffer, then the copy reads sp+0x110..0x11c = buffer+0x30
 *   (mat[12..15]) and stores to sp+0x180..0x18c - a SEPARATE destination vector. So mat[12..15] is
 *   confirmed correct, and the row0-copy variant (reading mat[0..3]) measures BYTE-IDENTICAL
 *   (254 differ / 272 words), which means the whole copy block is codegen-neutral either way.
 *   shared-dest (one vector reused for all four copy-backs) does not compile; early-local4 (hoisting
 *   local_4 = 1.0f to the top) is WORSE: 263 differ / 274 words. Base stays best at 272.
 * REAL FINDING AT +0x38: retail branches `beqzl v0,0x1b782c` after the first predicate (continue on
 *   ZERO, forward), while ours emits `bnezl v0,0x1b7b10` (bail out on NON-zero). Same semantics,
 *   inverted layout - so the chain's branch polarity is the remaining structural difference.
 * NEXT LEVER: write the five predicate tests so the non-zero case is the fall-through and the
 *   zero case jumps forward - e.g. `if (f(...)) { } else if (g(...)) { } ...` with the real body on
 *   the zero side, or an early `if (x != 0) goto done;` ladder guarded by the else-branch work.
 */

/* BATCH 3 (2026-11-01, six variants): first real movement in three batches.
 *   nested-range  (the `!= 0 && a <= f && f < a+b` test split into three nested ifs)  242 differ / 271
 *   bang          (`if (!FUN_000b3464(...))`)                                          254 / 272
 *   bang-nested   (both)                                                               242 / 271  KEPT
 *   inline-floats, bang-inline-floats, nested-inline-floats                             COMPILE_FAILED
 * So the branch-polarity lever is REAL but it is the RANGE test that carries it: splitting
 *   `if (p != 0 && lo <= f && f < lo + w)` into three nested ifs (instead of the `&&` chain) moved
 *   differ 254 -> 242 and words 272 -> 271. The `!` spelling alone changes nothing.
 * NEXT LEVER: apply the same split to the remaining four range tests (0x002e9a18, 0x002e9a14,
 *   0x002e9a0c+0x002e9a20, 0x002e9a04/0x002e9a08 and the two foot branches) - the first split paid
 *   for itself, so the other five should compound. Keep the `!` spelling, it is harmless.
 */
// near miss from lane dsn8 (unreviewed)
