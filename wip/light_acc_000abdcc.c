/* HSTMATCH module=EBOOT.BIN start=0x000abdcc end=0x000ac04c symbol=FUN_000abdcc lang=c99 */
extern float DAT_00341268;
extern float DAT_00341264;
extern float DAT_00341260;
extern int FUN_000ae050(int, int, int, int, int, int);
extern int FUN_000ae574(int, int);
extern int FUN_000ad6a0(int, void *, void *, void *, void *, int, int, int);
extern void FUN_00022434(int *, int, int);
extern void FUN_00022518(int *, int *);
extern unsigned int FUN_0001342c(void);
extern void FUN_00022498(float, void *, void *);
extern void FUN_00022418(int *, int *, int *);

void FUN_000abdcc(int *param_1, int param_2, int param_3, int param_4, int param_5, int param_6)
{
  unsigned int uVar1;
  int uVar2;
  float fVar3;
  float fVar4;
  float fVar5;
  int local_80;
  int local_7c;
  int local_78;
  int local_74;
  int local_70;
  int local_6c;
  int local_68;
  int local_64;
  int local_60[2];
  int local_58;
  int local_50;
  int local_4c;
  int local_48;
  int local_44;
  int local_40;
  int local_3c;
  int local_38;
  int local_34;
  int local_30;
  int local_2c;
  int local_28;
  int local_24;
  int local_20;
  int local_1c;
  int local_18;
  int local_14;
  unsigned char auStack_10[4];
  unsigned char auStack_c[4];
  unsigned char auStack_8[4];
  unsigned char auStack_4[4];

  local_20 = 0;
  local_1c = 0;
  local_18 = 0;
  local_14 = 0;
  fVar5 = 0.0f;
  if (param_5 == 2) {
    fVar5 = DAT_00341268 * 0.01f;
  }
  else if (param_5 == 1) {
    fVar5 = DAT_00341264 * 0.01f;
  }
  else if (param_5 == 0) {
    fVar5 = DAT_00341260 * 0.01f;
  }
  uVar2 = FUN_000ae050(param_2, param_5, 1, 0, 1, 0);
  fVar3 = (float)FUN_000ae574(uVar2, param_2);
  fVar4 = (float)FUN_000ad6a0(param_2, auStack_4, auStack_8, auStack_c, auStack_10, param_5, 1, param_3);
  fVar5 = fVar5 * fVar3 * fVar4;
  if (fVar4 != 0.0f) {
    FUN_00022434(local_60, param_4, param_3);
    local_30 = local_60[0];
    local_28 = local_58;
    local_2c = 0;
    local_24 = 0;
    FUN_00022518(&local_30, &local_30);
    local_24 = 0;
    uVar1 = FUN_0001342c();
    fVar3 = (float)(int)(uVar1 & 0xfffff800) * 4.656613e-10f;
    FUN_00022498(fVar5, &local_80, &local_30);
    local_48 = local_78;
    local_50 = local_80;
    local_4c = local_7c;
    local_44 = local_74;
    FUN_00022498(fVar3, &local_70, &local_50);
    local_38 = local_68;
    local_40 = local_70;
    local_3c = local_6c;
    local_34 = local_64;
    FUN_00022418(&local_20, &local_20, &local_40);
  }
  if (param_6 != 0) {
    *(int *)(param_2 + 0x4b0) = 0;
    *(int *)(param_2 + 0x4b4) = 0;
    *(int *)(param_2 + 0x4b8) = 0;
    *(int *)(param_2 + 0x4bc) = 0;
    *(int *)(param_2 + 0x4c0) = 0;
    if (0.0f < fVar5) {
      *(float *)(param_2 + 0x4c0) = fVar5;
    }
    else if (fVar5 < 0.0f) {
      *(float *)(param_2 + 0x4bc) = fVar5;
    }
  }
  *param_1 = local_20;
  param_1[1] = local_1c;
  param_1[2] = local_18;
  param_1[3] = local_14;
  return;
}

/* NEAR 144/160 words, 143 differ, first +0x0. Probe 1 = pseudocode transcription; BODY gap (16 words
   short, prologue/frame wrong at the first word), parked after one probe. (The first run measured 168
   retail words because the header end was 0x20 too long; corrected to 0x000ac04c = queue size 640.)
   Family note: NOT a sibling of the filed command builders -- this is the first item in the float
   vector-math family (FUN_00022434/FUN_00022518/FUN_00022498/FUN_00022418 accumulate into a 4-float
   result), so it starts its own group.
   Decoded: zero local_20..local_14 (4 floats out); fVar5 = 0 unless param_5==2/1/0 ->
   DAT_00341268/64/60 * 0.01f; uVar2=FUN_000ae050(param_2,param_5,1,0,1,0);
   fVar3=(float)FUN_000ae574(uVar2,param_2); fVar4=(float)FUN_000ad6a0(param_2,auStack_4,auStack_8,
   auStack_c,auStack_10,param_5,1,param_3) -- 8 args, four 4-byte out blocks; fVar5*=fVar3*fVar4;
   if (fVar4 != 0): FUN_00022434(local_60,param_4,param_3); local_30/local_28 <- local_60[0]/local_58,
   local_2c=local_24=0; FUN_00022518(&local_30,&local_30); local_24=0; uVar1=FUN_0001342c();
   fVar3=(float)(int)(uVar1 & 0xfffff800)*4.656613e-10f; FUN_00022498(fVar5,&local_80,&local_30);
   shuffle local_78/80/7c/74 -> local_48/50/4c/44; FUN_00022498(fVar3,&local_70,&local_50);
   shuffle local_68/70/6c/64 -> local_38/40/3c/34; FUN_00022418(&local_20,&local_20,&local_40).
   if (param_6): zero param_2+0x4b0..0x4c0, then fVar5>0 -> +0x4c0 else fVar5<0 -> +0x4bc.
   out[0..3] = local_20/1c/18/14.
   NEXT: frame at +0x0 and the 16-word shortfall are the two scalar->vector moves and the local-block
   order; read retail's prologue (frame size / saved set) and its sp offsets for local_30/local_50/
   local_40 before touching the float math. */
// near miss from lane dsn8 (unreviewed)
