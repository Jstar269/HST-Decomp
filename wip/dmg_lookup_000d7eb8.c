/* HSTMATCH module=EBOOT.BIN start=0x000d7eb8 end=0x000d8160 symbol=FUN_000d7eb8 lang=c99 */
extern int DAT_002c065c;
extern int DAT_002d6868;
extern int DAT_002d686c;
extern int DAT_002d6870;
extern int DAT_002d6880;
extern void FUN_000114c0(void *p, int c, int n);
extern int FUN_000d8c24(int a, int b, int c);

void FUN_000d7eb8(short *param_1, unsigned int *param_2)
{
  float local_14[5];
  float *pf;
  float *src;
  short id;
  int node;
  int sum;
  int v;
  int i;
  int k;
  int j;
  int n;

  FUN_000114c0(param_2, 0, 0x14);
  id = *param_1;
  node = FUN_000d8c24(*(int *)0x338b08, 0, (int)id);
  if (node != 0 && (((int)*(char *)(node + 0x62) << 0x1c) >> 0x1c) == 0) {
    FUN_000114c0(local_14, 0, 0x14);
    j = 0;
    for (i = 0; i < 4; i++) {
      node = FUN_000d8c24(*(int *)0x338b08, (&DAT_002c065c)[i], (int)param_1[(&DAT_002c065c)[i]]);
      if (node != 0) {
        sum = (int)param_1[((((int)*(char *)(node + 0x65)) << 0x18) >> 0x1c) + 0x45] +
              (int)param_1[((((int)*(char *)(node + 0x66)) << 0x1c) >> 0x1c) + 0x48];
        k = 0;
        while ((int)*(char *)(node + k + 0x67) != (int)id) {
          k = k + 1;
          if (2 < k) goto after;
        }
        sum = sum + param_1[0x4c];
      after:
        v = DAT_002d6868;
        if (DAT_002d6868 <= sum && sum <= DAT_002d686c) v = sum;
        for (k = 0; k < 3; k++) {
          if (v <= (&DAT_002d6870)[k]) break;
        }
        src = (float *)(node + (k * 4 + k) * 4);
        pf = local_14;
        for (n = 0; n < 5; n++) {
          pf[n] = pf[n] + src[n + 1];
        }
      }
      j = j + 1;
    }
    pf = local_14;
    for (i = 0; i < 5; i++) {
      v = (int)pf[i];
      if (9 < v && v < 100 && v / 10 == v % 10) v = v + 0x19;
      for (k = 0; k < 3; k++) {
        if (v <= (&DAT_002d6880)[k]) break;
      }
      *param_2 = k;
      pf = pf + 1;
      param_2 = param_2 + 1;
    }
  }
}

/* NEAR 171/170 words, 134 differ, first +0x4c. Prologue is EXACT (64-byte frame, ra+s4..s0 in the
   same slots, `move s3,a1`/`move s4,a0` before the saves) -- this one is a BODY/BRANCH-shape gap, one
   word long. First real difference at +0x60 (0x7f18): retail shapes the two-term guard as
   `beqzl v1,0x7f2c` entering the body with `addiu a0,sp,44` in the delay slot, ours as
   `bnez v1,exit`; the +1 word then shifts every later branch displacement (the +0x4c "difference"
   is only beqzl 0xd8148 vs 0xd8144). Structure: memset(param_2,0,0x14); id=*param_1;
   node=lookup(*(int*)0x338b08,0,id); guard = node && nibble4(node+0x62)==0; memset(local_14,0,0x14);
   for 4 entries of DAT_002c065c: node=lookup(*(int*)0x338b08,entry,param_1[entry]); if node:
   sum = param_1[nib4(node+0x65)+0x45] + param_1[nib4(node+0x66)+0x48]; scan bytes node+0x67 for id
   (up to 3, else skip the +param_1[0x4c] term); clamp sum to [DAT_002d6868,DAT_002d686c] else
   DAT_002d6868; bucket sum against DAT_002d6870[0..2]; add 5 floats from node+(bucket*5)*4+4 into
   local_14. Then for 5 floats: v=(int)f; if 9<v<100 and v/10==v%10 v+=0x19; bucket against
   DAT_002d6880[0..2]; *param_2++ = bucket.
   NEXT: the guard shape is the whole gap -- write `if (node == 0) goto done;` plus a separate
   `if (nibble(node+0x62) != 0) goto done;` (two one-term tests, else-first) so MWCC emits the
   `beqzl` into the body, which should also absorb the extra word. */
// 2026-10-31 second probe (4 variants via hsttry): nested one-term ifs, inverted `!(a==0 || b!=0)`,
// goto-done with a bare block, and a named boolean for the guard. First three compile BYTE-IDENTICAL
// (171/170, 134 differ, same md5 text.bin) -- MWCC already canonicalises the two-term `&&` guard, so
// branch shape at +0x60 is not source-addressable and the +1 word is the same in every spelling.
// named-bool is worse (174/170, 153). Lever dead; next idea is elsewhere: retail puts `addiu a0,sp,44`
// in the beqzl delay slot because its memset(local_14) argument is materialised before the branch, so
// try hoisting `float *dst = local_14;` above the guard and passing `dst` to FUN_000114c0.
// near miss from lane dsn8 (unreviewed)
