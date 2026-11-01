/* HSTMATCH module=EBOOT.BIN start=0x0005cc18 end=0x0005ccac symbol=FUN_0005cc18 lang=c99 */
void FUN_0005cc18(int param_1, int param_2, float *param_3, int *param_4)
{
  int iVar1;
  float fVar2;

  if (param_3 != 0) {
    int off = param_2 * 0x20;
    iVar1 = off + param_1;
    float f0v;
    float f1v;
    float f2v;
    *param_3 = *(float *)(iVar1 + 0x90);
    param_3[1] = *(float *)(iVar1 + 0x94);
    param_3[2] = *(float *)(iVar1 + 0x98);
    fVar2 = *(float *)(iVar1 + 0x9c);
    param_3[3] = fVar2;
    float n3 = -param_3[3];
    param_3[0] = -param_3[0];
    param_3[2] = -param_3[2];
    param_3[1] = -param_3[1];
    param_3[3] = n3;
  }
  if (param_4 != 0) {
    int off2 = param_2 * 0x20;
    float *src = (float *)(off2 + param_1 + 0xa0);
    float *dst = (float *)param_4;
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
    dst[3] = src[3];
  }
}
// Ruling: PROGRESS.md VERDICTS (0x0005cc18); experiment: try/hsttry/get_quads_0005cc18-15496
// BATCH 2026-11-01 (6 variants, one run): negation-order variants (3-first, 3-locals, negate-at-load x3,
// base order) came out 21, 35, 35, 35, 35, 21 differ -- only 3-first matches the base, and every
// negate-at-load spelling collapses to 30 words, so the negation is genuinely a second pass over the stored
// vector.
// NEXT LEVER: retail negates into a DIFFERENT register first (neg.s f3,f0 while f0 still holds element 3),
// then reloads 0, 8, 4 -- i.e. load order 0, 2, 1, not 0, 1, 2 -- and stores 0, 1, 2, 3. Try expressing the
// second pass with an explicit destination temporary per element in that order.
// BATCH 2026-11-01 (offset-first base, 3 variants): `int off = param_2 * 0x20; iVar1 = off + param_1;`
// took 20 -> 19 differ (37/37 words); the both-blocks and declared-off forms are identical at 19, so keep the
// simple one. The cast-through-char pointer form does not compile.
// NEXT LEVER: the remaining first difference is the second block's own `addu` (retail `addu v1,v1,a0` after
// re-forming the base from param_1); give that block the same offset-first treatment plus its own base local.
// BATCH 2026-11-01 (second-block base, 4 variants): off2 and off2_decl both 19 differ (no gain), while
// sharing the first block's base collapses to 28 and hoisting it above the guards to 35. Retail therefore
// carries TWO independent base computations; keep them separate and stop trying to share.
// NEXT LEVER: the surviving first difference is the second block's `sll`/`addu` pair order, which the
// offset-first form does not change -- try declaring the second block's pointer as float* and storing with
// swc1 from the same expression, i.e. copy the first block's exact spelling into the second.
// BATCH 2026-11-01 (mirror the first block's spelling, 3 variants): full mirror 10 differ (was 19),
// mirror-with-declared-pointer also 10, mirror-with-shared-index 23/35 words. The float-pointer + swc1 copy
// is KEEP: the whole first block now matches retail byte-for-byte through +0x30 and the second block's copy
// matches too. 37/37 words, 10 differ.
// REMAINING: the negation pass only -- ours `neg.s $f0,$f0` where retail negates element 3 into `$f3` first
// (`neg.s $f3,$f0`) and then reloads 0/2/1. Next lever: give the element-3 negation its own destination
// register by writing `float n3 = -fVar2;` BEFORE the 0/2/1 reloads and storing it last.
// BATCH 2026-11-01 (n3 destination + load order, 5 variants): n3first / n3first_012 / alltemps all 10 differ
// (no gain), n3first_021 and n3first_direct 11. Giving element 3 its own destination does NOT change the
// count, because the real difference is which OBJECT the second pass reads and stores:
//   ours   neg.s $f0,$f0 ; swc1 12(a2) ; lwc1 144(v1) ; ... ; swc1 0 ; swc1 8 ; swc1 4   (reads v1 = source)
//   retail neg.s $f3,$f0 ; lwc1 0(a2) ; lwc1 8(a2) ; lwc1 4(a2) ; neg.s ... ; swc1 0 ; swc1 4 ; swc1 8 ; swc1 $f3,12
// i.e. retail negates the already-STORED vector in place (reads param_3, load order 0,2,1), stores 0,1,2 in
// order, and writes element 3 LAST from the f3 temporary.
// NEXT LEVER (exact): `float n3 = -fVar2;` then `*param_3 = -*param_3; param_3[2] = -param_3[2];
// param_3[1] = -param_3[1]; param_3[3] = n3;` -- read back through param_3, not through the source pointer.
// BATCH 2026-11-01 (in-place negation, 3 variants): inplace 10 differ, inplace_ptr 10, inplace_last 26
// (38 words). Reading back through param_3 is neutral too -- the body keeps 10 differ / 37 words either way.
// SINGLE REMAINING PAIR: first difference at +0x30 is `neg.s $f0,$f0` (ours) against retail's `neg.s $f3,$f0`,
// and the same 10-word cluster continues through the store order 0,8,4 vs 0,4,8. Both spellings that reach
// the destination pointer produce MWCC's in-place `neg.s $f0,$f0`; the f3 destination only appears when the
// negated value is NOT written back to param_3[3] at that point -- yet storing it later costs 26 differ, so
// the remaining lever is a different value-flow (keep the element-3 negation in an SSA local whose only use
// is the final store) rather than another in-place spelling.
// improved by lane dsn8 (unreviewed): 21 -> 10 differing words
