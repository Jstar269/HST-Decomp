/* HSTMATCH module=EBOOT.BIN start=0x0027cc74 end=0x0027d0e0 symbol=FUN_0027cc74 lang=c99 */
/* Hand-written: straight-line vector math on the pose record at DAT_003313c4, ten named 16-byte vectors, no loops. */

extern void  FUN_00022518(float *a, float *b);
extern void  FUN_000224f8(float *a, float *b, float *c);
extern void  FUN_00022498(float s, float *out, float *in);
extern void  FUN_00022418(float *out, float *a, float *b);
extern void  FUN_00022434(float *out, float *a, float *b);
extern void  FUN_00022570(float *out, float *a, float *b);
extern void  FUN_0001d0b8(float *out, float *in);
extern void  FUN_0001d164(float *out, float *in);

extern char  DAT_003313c4[];
extern float DAT_002bc410;
extern float DAT_002bc414;
extern float DAT_002bc418;
extern float DAT_002bc41c;
extern float DAT_002fe6c0;
extern float DAT_002fe6c4;
extern float DAT_002fe6c8;

void FUN_0027cc74(int param_1)
{
  float local_160[4];
  float local_150[4];
  float local_140[4];
  float local_130[4];
  float local_120[4];
  float local_110[4];
  float local_100[4];
  float local_50[4];
  float local_40[4];
  float local_30[4];
  float local_20[4];
  float local_10[4];
  char auStack_e0[16];
  char auStack_d0[48];
  char auStack_a0[64];
  char auStack_60[16];
  int iVar2;
  int iVar3;
  float fVar1;
  float fVar4;

  local_40[0] = DAT_002bc410;
  local_40[1] = DAT_002bc414;
  local_40[2] = DAT_002bc418;
  local_40[3] = DAT_002bc41c;
  local_10[0] = *(float *)(DAT_003313c4 + 0x1d0);
  local_10[1] = *(float *)(DAT_003313c4 + 0x1d4);
  local_10[2] = *(float *)(DAT_003313c4 + 0x1d8);
  local_10[3] = *(float *)(DAT_003313c4 + 0x1dc);
  local_20[0] = *(float *)(DAT_003313c4 + 0x1e0);
  local_20[1] = *(float *)(DAT_003313c4 + 0x1e4);
  local_20[2] = *(float *)(DAT_003313c4 + 0x1e8);
  local_20[3] = *(float *)(DAT_003313c4 + 0x1ec);
  local_30[0] = *(float *)(DAT_003313c4 + 0x1f0);
  local_30[1] = *(float *)(DAT_003313c4 + 0x1f4);
  local_30[2] = *(float *)(DAT_003313c4 + 0x1f8);
  local_30[3] = *(float *)(DAT_003313c4 + 0x1fc);
  FUN_00022518(local_20, local_20);
  local_20[3] = 0.0f;
  FUN_000224f8(local_40, local_30, local_20);
  FUN_00022518(local_40, local_40);
  local_40[3] = 0.0f;
  FUN_000224f8(local_30, local_20, local_40);
  if (0 < *(int *)(param_1 + 0x48)) {
    *(int *)(param_1 + 0x48) = *(int *)(param_1 + 0x48) - 1;
    FUN_00022498(DAT_002fe6c8, local_130, local_20);
    local_110[0] = local_130[0];
    local_110[1] = local_130[1];
    local_110[2] = local_130[2];
    local_110[3] = local_130[3];
    FUN_00022418(local_120, local_10, local_110);
    local_50[0] = local_120[0];
    local_50[1] = local_120[1];
    local_50[2] = local_120[2];
    local_50[3] = local_120[3];
    iVar2 = *(int *)(param_1 + 0x48);
    fVar1 = DAT_002fe6c0 * *(float *)(param_1 + 0x3c);
    iVar3 = *(int *)(param_1 + 0x44);
    fVar4 = *(float *)(param_1 + 0x40) + DAT_002fe6c4 * 0.017453292f;
    if (3.1415927f < fVar4) {
      fVar4 = fVar4 - 6.2831855f;
    }
    else if (fVar4 < -3.1415927f) {
      fVar4 = fVar4 + 6.2831855f;
    }
    *(float *)(param_1 + 0x40) = fVar4;
    FUN_0001d0b8((float *)auStack_60, local_20);
    FUN_0001d164((float *)auStack_a0, (float *)auStack_60);
    FUN_00022570((float *)auStack_e0, (float *)auStack_a0, local_40);
    FUN_00022498(fVar1 * ((float)iVar2 / (float)iVar3), local_140, (float *)auStack_d0);
    local_100[0] = local_140[0];
    local_100[1] = local_140[1];
    local_100[2] = local_140[2];
    local_100[3] = local_140[3];
    FUN_00022418(local_10, local_10, local_100);
    iVar2 = (int)DAT_003313c4;
    *(float *)(iVar2 + 0x1d0) = local_10[0];
    *(float *)(iVar2 + 0x1d4) = local_10[1];
    *(float *)(iVar2 + 0x1d8) = local_10[2];
    *(float *)(iVar2 + 0x1dc) = local_10[3];
    FUN_00022434(local_160, local_50, local_10);
    local_110[0] = local_160[0];
    local_110[1] = local_160[1];
    local_110[2] = local_160[2];
    local_110[3] = local_160[3];
    FUN_00022518(local_150, local_110);
    local_20[0] = local_150[0];
    local_20[1] = local_150[1];
    local_20[2] = local_150[2];
    local_20[3] = local_150[3];
    local_150[3] = 0.0f;
    local_20[3] = 0.0f;
    FUN_00022518(local_20, local_20);
    local_20[3] = 0.0f;
    FUN_000224f8(local_40, local_30, local_20);
    FUN_00022518(local_40, local_40);
    local_40[3] = 0.0f;
    FUN_000224f8(local_30, local_20, local_40);
    iVar2 = (int)DAT_003313c4;
    *(float *)(iVar2 + 0x1e0) = local_20[0];
    *(float *)(iVar2 + 0x1e4) = local_20[1];
    *(float *)(iVar2 + 0x1e8) = local_20[2];
    *(float *)(iVar2 + 0x1ec) = local_20[3];
    iVar2 = (int)DAT_003313c4;
    *(float *)(iVar2 + 0x1f0) = local_30[0];
    *(float *)(iVar2 + 0x1f4) = local_30[1];
    *(float *)(iVar2 + 0x1f8) = local_30[2];
    *(float *)(iVar2 + 0x1fc) = local_30[3];
  }
}
// near miss from lane dsn8 (unreviewed)
