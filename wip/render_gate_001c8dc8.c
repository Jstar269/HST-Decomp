/* HSTMATCH module=EBOOT.BIN start=0x001c8dc8 end=0x001c9220 symbol=FUN_001c8dc8 lang=c99 */
/* Hand-written from the pseudocode; six-flag render gate, 16-word record copy, two virtual calls at slots 0x9c and 0x20. */

typedef int code();

extern void *FUN_00093818(void *global);
extern void  FUN_001ce354(int *out, int base, int end, int tag);
extern void  FUN_001c921c(void *out, void *mtx);
extern void  FUN_001cde30(int *out, int a, int b, void *mtx);
extern void  FUN_001cd8dc(int *a, int b, int c, int d);
extern void  FUN_001ce748(int *a, int b);
extern void  FUN_001c8c2c(void *out, int *in);
extern int   FUN_001c8c14(int *a, int *b);
extern void  FUN_001c8c00(int *a);
extern void *FUN_001c8bf8(int *a);
extern int   FUN_001c8b14(void *a, int b, int c);
extern void  FUN_001c8b04(void *a, int *b);
extern void  FUN_00022434(float *out, float *a, float *b);
extern float FUN_000224d8(float *a, float *b);
extern int   FUN_0022ab44(int a, int b, int c);
extern int   FUN_0022abdc(int a, int b, int c);
extern int   FUN_00288a84(void *obj);
extern void  FUN_00225d68(void *ctx, int a, int b);
extern void  FUN_00225d74(void *ctx, int a);
extern int  *DAT_0034a118;
extern void *DAT_0034a2e0;
extern float DAT_002eaa30;

void FUN_001c8dc8(int param_1)
{
  float mtx[16];
  int local_c[3];
  int local_30;
  int local_c0;
  int local_20;
  int local_14;
  int local_2c;
  int local_40;
  int local_24;
  int local_3c[2];
  int local_130[4];
  int local_110[4];
  int local_28;
  int *local_10;
  int auStack_44[4];
  int auStack_b0[4];
  float local_120[4];
  int local_34;
  int bVar1;
  int iVar3;
  int *piVar5;
  int *piVar6;
  float *puVar2;
  float u0, u1, u2, u3, u4, u5, u6, u7, u8, u9, u10, u11, u12, u13, u14, u15;
  float fVar10, fVar13;
  int uVar6, uVar7;

  bVar1 = 0;
  if (*(int *)(*(int *)(param_1 + 0x15c) + 4) == 0) {
    if (*(char *)(param_1 + 0xf3) == 0) {
      if (*(char *)(param_1 + 0xf7) == 0) {
        if (*(char *)(param_1 + 0xf9) == 0) {
          if (*(char *)(param_1 + 0xfe) == 0) {
            if (*(int *)(param_1 + 0xc4) != 4) {
              puVar2 = (float *)FUN_00093818(DAT_0034a118);
              u0 = puVar2[0];
              u1 = puVar2[4];
              u2 = puVar2[5];
              u3 = puVar2[1];
              u4 = puVar2[6];
              u5 = puVar2[7];
              u6 = puVar2[2];
              u7 = puVar2[8];
              u8 = puVar2[9];
              u9 = puVar2[3];
              u10 = puVar2[10];
              u11 = puVar2[0xb];
              u12 = puVar2[0xc];
              u13 = puVar2[0xd];
              u14 = puVar2[0xe];
              u15 = puVar2[0xf];
              local_c[0] = 0;
              local_c[1] = 0;
              local_c[2] = 0;
              FUN_001ce354(local_c, *(int *)(param_1 + 0x40),
                           *(int *)(param_1 + 0x40) + *(int *)(param_1 + 0x44) * 4, local_34);
              local_30 = local_c[0] + local_c[1] * 4;
              local_c0 = 0x40490fdb;
              mtx[0] = u0;
              mtx[1] = u3;
              mtx[2] = u6;
              mtx[3] = u9;
              mtx[4] = u1;
              mtx[5] = u2;
              mtx[6] = u4;
              mtx[7] = u5;
              mtx[8] = u7;
              mtx[9] = u8;
              mtx[10] = u10;
              mtx[11] = u11;
              mtx[12] = u12;
              mtx[13] = u13;
              mtx[14] = u14;
              mtx[15] = u15;
              local_20 = local_30;
              FUN_001c921c(auStack_b0, mtx);
              FUN_001cde30(&local_14, local_c[0], local_c[0] + local_c[1] * 4, auStack_b0);
              local_2c = local_14;
              FUN_001cd8dc(local_c, local_14, local_30, 0);
              if (local_c[1] != 0) {
                local_40 = local_c[0] + local_c[1] * 4;
                local_24 = local_c[0];
                local_3c[0] = local_c[0];
                local_130[0] = u12;
                local_130[1] = u13;
                local_130[2] = u14;
                local_130[3] = u15;
                local_110[0] = u12;
                local_110[1] = u13;
                local_110[2] = u14;
                local_110[3] = u15;
                local_28 = local_40;
                FUN_001c8c2c(auStack_44, local_3c);
                iVar3 = FUN_001c8c14(local_3c, &local_40);
                if (iVar3 != 0) {
                  FUN_001c8c00(local_3c);
                  iVar3 = FUN_001c8c14(local_3c, &local_40);
                  while (iVar3 != 0) {
                    piVar6 = (int *)FUN_001c8bf8(local_3c);
                    piVar5 = (int *)FUN_001c8bf8(auStack_44);
                    iVar3 = FUN_001c8b14(local_130, *piVar6, *piVar5);
                    if (iVar3 != 0) {
                      FUN_001c8b04(auStack_44, local_3c);
                    }
                    FUN_001c8c00(local_3c);
                    iVar3 = FUN_001c8c14(local_3c, &local_40);
                  }
                }
                FUN_001c8c2c(&local_10, auStack_44);
                if (local_10 != (int *)(local_c[0] + local_c[1] * 4)) {
                  piVar5 = (int *)*local_10;
                  uVar6 = (**(code **)(*DAT_0034a118 + 0x9c))();
                  uVar7 = (**(code **)(*piVar5 + 0x20))(piVar5);
                  FUN_00022434(local_120, (float *)uVar7, (float *)uVar6);
                  fVar13 = DAT_002eaa30 * DAT_002eaa30;
                  fVar10 = FUN_000224d8(local_120, local_120);
                  if (fVar10 < fVar13) {
                    uVar6 = FUN_0022ab44(piVar5[0xd], piVar5[0xe], 0);
                    iVar3 = FUN_0022abdc(piVar5[0xd], piVar5[0xe], 0);
                    if (iVar3 == 100) {
                      uVar6 = 7;
                    }
                    uVar7 = FUN_00288a84(piVar5);
                    FUN_00225d68(DAT_0034a2e0, uVar7, uVar6);
                    bVar1 = 1;
                  }
                }
              }
              FUN_001ce748(local_c, 0);
            }
          }
        }
      }
    }
  }
  if (!bVar1) {
    FUN_00225d74(DAT_0034a2e0, 0);
  }
}

/* JAL PARITY (2026-11-01): retail's 26 jal sites and this draft's 26 calls have the SAME multiset in the
 *   same order (93818, 1ce354, 1c921c, 1cde30, 1cd8dc, 1c8c2c, 1c8c14, 1c8c00, 1c8c14, 1c8bf8 x2, 1c8b14,
 *   1c8b04, 1c8c00, 1c8c14, 1c8c2c, 2x jalr, 22434, 224d8, 22ab44, 22abdc, 288a84, 225d68, 1ce748,
 *   225d74). Nothing is missing: the 27-word shortfall is local traffic around those calls, not calls.
 */
/* BATCH 2 of the local-slot family (2026-11-01): five spellings -- separate 16-word record copies, a
 *   second matrix copy, enlarged auStack_b0/auStack_44/local_3c and four unused 4-word temporaries -- all
 *   compiled BYTE-IDENTICAL to the base (251/278, 247 differ, frame still 240). Dead and duplicated locals
 *   are eliminated, so they never claim stack space. Frame growth has to come from values that are live
 *   across a call, i.e. the body is missing calls retail makes, not slots for values we already have.
 * NEXT LEVER: local traffic around the 26 jal sites (NOT missing calls -- the jal multisets are identical,
 *   recorded as V13). Retail keeps values in the frame across the call boundaries: it touches sp+0x50..0xc0
 *   as a 29-word contiguous run and 0x120..0x17c separately, while the draft recomputes and spills almost
 *   nothing. Reproduce those stores by making the corresponding values live across their call (the V-notes'
 *   forced-spill lever) and by keeping the two 16-word record copies as separate live objects, rather than
 *   adding declarations (V12: dead locals never grow a frame).
 */
/* PARKED 2026-11-01 (batch 2): NEAR 251/278 words, 247 differ, first at +0x0.
 * LEVER TRIED AND KEPT: declaring the record copy and matrix as float[16] rather than int brought retail's
 *   twelve float saves (f31..f20 at 44..0(sp)) into the prologue byte-for-byte; they now match.
 * FRAME STILL SHORT: ours 240 (`27bdff10`) against retail 384 (`27bdfe80`) -- 144
 *   bytes of locals missing. The float-register saving is done, so the gap is purely local-slot count:
 *   retail touches sp+0x50..0xc0 as a 29-word contiguous block plus 0x120..0x17c, ours only reaches ~0xb0.
 * NEXT LEVER: give each stage its own named 4-word temporary (the two FUN_00022434 results, the two
 *   8-byte auStack_b0/auStack_44 objects and the mtx) instead of reusing one buffer, and check the two
 *   16-word record copies are two separate slots rather than one reused.
 */
/* PARKED 2026-11-01 (batch 1): NEAR 243/278 words, 273 differ, first at +0x0.
 * FRAME MISSES: ours 272 (`27bdfef0`) against retail's 384 (`27bdfe80`) - 112 bytes short, and retail
 *   saves EIGHT float registers (f31..f24) that this draft never uses, so the miss is the float
 *   register pressure: retail keeps ten float values live across FUN_001c921c/FUN_001cde30, ours
 *   spills to the int matrix (mtx[16]) and lets the floats die.
 * One fix was needed: auStack_44/auStack_b0 must be `int[4]`, not `char[16]` - FUN_001c8bf8 and
 *   FUN_001c8c2c take `int *` and the char spelling aborts the compile.
 * The two jalr sites are now settled: slot +0x9c on `_DAT_0034a118`'s vtable with NO arguments
 *   (`lw a0,-24296(v0); lw t9,0(a0); lw t9,156(t9)`), then slot +0x20 on the object's vtable with
 *   (object, firstResult) - `lw t9,0(s0); lw t9,32(t9); jalr t9; move a0,s0; move a1,v0`.
 * NEXT LEVER: the 16-word record is copied into locals uVar6..uVar22 and then re-laid into
 *   mtx[16] in transposed order; retail holds those as FLOATS (hence f24-f31). Declare mtx as
 *   `float mtx[16]` (and the two 20-word buffers as float) so the eight float registers appear.
 */
// near miss from lane dsn8 (unreviewed)
