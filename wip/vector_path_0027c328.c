/* HSTMATCH module=EBOOT.BIN start=0x0027c328 end=0x0027c728 symbol=FUN_0027c328 lang=c99 */
/* Hand-written from the pseudocode; straight-line vector path builder, one named 16-byte temporary per vector. */

typedef float Vec4[4];

extern void  FUN_0027c750(float *out, int self, float angle, int flag);
extern float FUN_00169f08(void *a, void *b);
extern void  FUN_00022418(Vec4 *out, Vec4 *a, Vec4 *b);
extern void  FUN_000609f8(void *obj);
extern void  FUN_00022434(Vec4 *out, Vec4 *a, Vec4 *b);
extern float FUN_000224d8(Vec4 *a, Vec4 *b);
extern float FUN_00022e44(float value);
extern void  FUN_0004d2cc(float angle, int *m, int *out);
extern int   DAT_002bc410[];

void FUN_0027c328(int param_1, int param_2, float *param_3)
{
  Vec4 v10, v20, v30, v40, va0, vb0, v90, v80, vc0, vd0, ve0, vf0;
  Vec4 v100, v110, v120, v130, v140;
  int mtx[12];
  int iVar1;
  int *piVar2;
  float uVar3;
  float fVar4;

  *(float **)(param_1 + param_2 * 4 + 0x48) = param_3;
  v10[0] = 0;
  v10[1] = 0;
  v10[2] = 0.0f;
  v20[0] = 0;
  v20[1] = 0;
  v20[2] = 0.0f;
  v10[3] = 1.0f;
  v20[3] = 1.0f;
  FUN_0027c750(v90, param_1, (float)((int *)param_3)[4], *(int *)(param_1 + 0x68));
  v10[2] = v90[2];
  v10[0] = v90[0];
  v10[1] = v90[1];
  v10[3] = v90[3];
  FUN_0027c750(va0, param_1, (float)((int *)param_3)[4], *(int *)(param_1 + 0x68));
  v20[0] = va0[0];
  v20[1] = va0[1];
  v20[2] = va0[2];
  v20[3] = va0[3];
  if ((unsigned)(((int *)param_3)[4] - 1) < 2) {
    FUN_0027c750(vb0, param_1, 3, -1);
    if (0.0f < vb0[2]) {
      fVar4 = *param_3 + 180.0f;
    }
    else {
      fVar4 = *param_3;
    }
  }
  else if (0.0f < v10[2]) {
    fVar4 = *param_3 + 180.0f;
  }
  else {
    fVar4 = *param_3;
  }
  if (((int *)param_3)[4] == 7) {
    fVar4 = (float)FUN_00169f08(&v10, &v20);
    fVar4 = *param_3 + fVar4 * 57.29578f;
  }
  mtx[0] = DAT_002bc410[0];
  mtx[1] = DAT_002bc410[1];
  mtx[2] = DAT_002bc410[2];
  mtx[3] = DAT_002bc410[3];
  mtx[4] = DAT_002bc410[4];
  mtx[5] = DAT_002bc410[5];
  mtx[6] = DAT_002bc410[6];
  mtx[7] = DAT_002bc410[7];
  mtx[8] = DAT_002bc410[8];
  mtx[9] = DAT_002bc410[9];
  mtx[10] = DAT_002bc410[10];
  mtx[11] = DAT_002bc410[11];
  v40[2] = param_3[1];
  v40[1] = -param_3[3];
  v40[3] = 1.0f;
  v40[0] = 0;
  FUN_0004d2cc(fVar4 * 0.017453292f, mtx, mtx);
  v30[2] = v40[2];
  v30[0] = v40[0];
  v30[1] = v40[1];
  v30[3] = v40[3];
  v80[2] = param_3[7];
  v80[0] = param_3[6];
  v80[1] = 0.0f;
  v80[3] = 0;
  FUN_00022418(&v130, &v10, &v30);
  vc0[2] = v130[2];
  vc0[0] = v130[0];
  vc0[1] = v130[1];
  vc0[3] = v130[3];
  FUN_00022418(&v110, &vc0, (Vec4 *)v80);
  iVar1 = param_2 * 4 + param_1;
  piVar2 = (int *)(iVar1 + 0x3c);
  iVar1 = *(int *)(iVar1 + 0x3c);
  *(int *)(iVar1 + 0x40) = v110[0];
  *(int *)(iVar1 + 0x44) = v110[1];
  *(int *)(iVar1 + 0x48) = v110[2];
  *(int *)(iVar1 + 0x4c) = v110[3];
  ve0[1] = -param_3[5];
  ve0[0] = 0;
  ve0[2] = 0;
  ve0[3] = 0;
  FUN_00022418(&v140, &v20, &ve0);
  vd0[2] = v140[2];
  vd0[0] = v140[0];
  vd0[1] = v140[1];
  vd0[3] = v140[3];
  FUN_00022418(&v120, &vd0, (Vec4 *)v80);
  iVar1 = *piVar2;
  *(int *)(iVar1 + 0x60) = v120[0];
  *(int *)(iVar1 + 0x64) = v120[1];
  *(int *)(iVar1 + 0x68) = v120[2];
  *(int *)(iVar1 + 0x6c) = v120[3];
  FUN_000609f8((void *)*piVar2);
  fVar4 = param_3[2];
  if (*(int *)(param_1 + 0x54) == 0) {
    if (*(float *)(param_1 + 0x6c) <= 0.0f) {
      iVar1 = *piVar2;
      goto LAB_0027c730;
    }
    if (*(float *)(param_1 + 0x6c) < *(float *)(param_1 + 0x70)) {
      FUN_00022434(&v100, &v10, &v20);
      vf0[2] = v100[2];
      vf0[0] = v100[0];
      vf0[1] = v100[1];
      vf0[3] = v100[3];
      uVar3 = FUN_000224d8(&vf0, &vf0);
      FUN_00022e44(uVar3);
    }
  }
  iVar1 = *piVar2;
LAB_0027c730:
  *(float *)(iVar1 + 0x80) = fVar4;
}

/* PARKED 2026-11-01 (batch 1): NEAR 268/256 words, 262 differ, first at +0x4.
 * FRAME LANDS: 352 (`27bdfea0` byte-identical first word) - the named-temporaries spelling works.
 * SAVED SETS: ours ra@44 + s3,s2,s1,s0 (5 saves, area shifted 16 bytes up);
 *             retail ra@28 + s2,s1,s0 (4 saves). One callee-saved register too many.
 * TWO SPELLING FIXES ARE BAKED IN AND MUST STAY: param_3[4] is an INT mode word, not a float
 *   (`((int *)param_3)[4]` at the two FUN_0027c750 angle args, the 1..2 gate and the == 7 test) -
 *   spelling it as float pulls in MWCC's _dpfle/_dpflt double helpers; and every float compare
 *   literal needs the f suffix (0.0f, not 0.0) for the same reason.
 * NEXT LEVER: v40 is a pure permutation of v30 and is then fed to FUN_00022418 - drop v40, write
 *   the permuted components back into v30 in place, and pass &v30. That kills the last live
 *   temporary across FUN_0004d2cc and should free the fifth saved register.
 */

/* PARKED 2026-11-01 (batch 1): NEAR 268/256 words, 262 differ, first at +0x4.
 * FRAME LANDS: 352 (`27bdfea0` byte-identical first word) - the named-temporaries spelling works.
 * SAVED SETS: ours ra@44 + s3,s2,s1,s0 (5 saves, area shifted 16 bytes up);
 *             retail ra@28 + s2,s1,s0 (4 saves). One callee-saved register too many.
 * TWO SPELLING FIXES ARE BAKED IN AND MUST STAY: param_3[4] is an INT mode word, not a float
 *   (`((int *)param_3)[4]` at the two FUN_0027c750 angle args, the 1..2 gate and the == 7 test) -
 *   spelling it as float pulls in MWCC's _dpfle/_dpflt double helpers; and every float compare
 *   literal needs the f suffix (0.0f, not 0.0) for the same reason.
 * NEXT LEVER: v40 is a pure permutation of v30 and is then fed to FUN_00022418 - drop v40, write
 *   the permuted components back into v30 in place, and pass &v30. That kills the last live
 *   temporary across FUN_0004d2cc and should free the fifth saved register.
 */

/* BATCH 2 (2026-11-01): best kept = roles-right, NEAR 270/256, 257 differ, frame still 352.
 *   roles-right  (build into v40, copy identity to v30, pass &v30): 270 words, 257 differ - KEPT
 *     because it is the only variant that keeps retail's 352-byte frame.
 *   fold-v40     (build v30 directly, no v40): 268 words, 262 differ - same as the old base.
 *   drop-vc0     (pass &v130 straight to the second FUN_00022418): 262 words, 251 differ, but the
 *     frame collapses to 336. REJECTED: fewer differing words, but a body that cannot reach EXACT
 *     while the frame is wrong. Do not chase it again without restoring the copy.
 * NEXT LEVER: the copy chain v90->v10, va0->v20, v130->vc0, v140->vd0, v100->vf0 is the remaining
 *   pressure. Retail keeps all five copies, so the fifth saved register is elsewhere - try giving
 *   local_74/v80 its own vec4 temporary (currently v80[3] = 0 shares the block) and check whether
 *   the two `param_2 * 4 + param_1` computations collapse into one address register.
 */
// near miss from lane dsn8 (unreviewed)
