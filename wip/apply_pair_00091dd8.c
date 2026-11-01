/* HSTMATCH module=EBOOT.BIN start=0x00091dd8 end=0x00092078 symbol=FUN_00091dd8 lang=c99 */
extern float DAT_002d28f8;
extern float DAT_002d28f4;
extern float DAT_002d28fc;
extern int FUN_00057934(float x, float y, int a, int b, int c);
extern void FUN_00058ddc(int a, int b, int c);
extern void FUN_00058f38(float f, int a, int b);
extern void FUN_0005f030(float f, int a, int b);
extern void FUN_0005f110(float f, int a);
extern void FUN_00093be0(int a, int b);
extern void FUN_00093c08(int a, int b);

void FUN_00091dd8(float param_1, float param_2, int param_3, int param_4,
                  int param_5, int param_6, int param_7)
{
  float local_18 = param_1;
  float local_14 = param_2;
  float local_10[2];
  int local_8[2];
  float *pf;
  float *px;
  char *p;
  int n;
  int i;
  int obj;
  int v;
  char flag;

  obj = *(int *)(param_3 + 0x58);
  flag = 0;
  if (obj != 0) {
    if (param_4 != 0x7fff) FUN_00058ddc(obj, param_5, 0);
    if (param_6 != 0x7fff) FUN_00058ddc(obj, param_7, 1);
    if (DAT_002d28f8 < local_14) pf = &local_14; else pf = &DAT_002d28f8;
    if (DAT_002d28f4 < local_18) px = &local_18; else px = &DAT_002d28f4;
    flag = (char)FUN_00057934(*px, *pf, obj, param_4, param_6);
  }
  if (param_4 != 0x7fff) {
    if (*(int *)(param_3 + 0x50) != 0 && param_4 != *(int *)(*(int *)(param_3 + 0x50) + 0xa0)) {
      if (DAT_002d28fc < local_18) pf = &local_18; else pf = &DAT_002d28fc;
      FUN_0005f030(*pf, *(int *)(param_3 + 0x50), param_4);
      *(char *)(*(int *)(param_3 + 0x50) + 0xc0) = (char)param_5;
      *(float *)(*(int *)(param_3 + 0x50) + 0xd0) = *(float *)(*(int *)(param_3 + 0x50) + 0xc4);
    }
  }
  n = 8; p = (char *)local_8;
  if (p != 0) { do { *p++ = 0; } while (--n); }
  local_8[0] = param_4;
  local_8[1] = param_6;
  n = 8; p = (char *)local_10;
  if (p != 0) { do { *p++ = 0; } while (--n); }
  local_10[0] = local_18;
  local_10[1] = local_14;
  for (i = 0; i < 2; i++) {
    v = local_8[i];
    if (v != 0x7fff) {
      if (v >= 0) {
        if (0.0f <= local_10[i]) {
          obj = *(int *)(param_3 + 0x58);
          if (obj != 0 && flag == 0) {
            FUN_00058f38(local_10[i], obj, i);
            FUN_0005f110(local_10[i], obj);
          }
          if (i == 0 && *(int *)(param_3 + 0x50) != 0) FUN_0005f110(local_10[i], *(int *)(param_3 + 0x50));
        }
        v = local_8[i];
      }
      if (v < 0) FUN_00093c08(param_3, i); else FUN_00093be0(param_3, i);
    }
  }
}

/* NEAR 166/168 words, 157 differ, first +0x0. Frame 64 vs retail 80: retail saves f20 and keeps the
   selected float live across the two FUN_0005f110 calls, ours re-reads local_10[i] and packs the
   two float spill slots the other way (ours f12->60, f13->56; retail f12->56, f13->60), so the
   first divergence is the prologue itself, not the body. Structure decoded fully from the listing:
   lw s0,88(a0); if (s0) { two FUN_00058ddc(s0, param5/param7, 0/1) guarded by 0x7fff; pointer-select
   max(DAT_002d28f8,param_2) and max(DAT_002d28f4,param_1); flag=byte FUN_00057934(x,y,s0,p4,p6); }
   then if (p4!=0x7fff) { obj=*(p3+0x50); if (obj && p4!=*(obj+0xa0)) { FUN_0005f030(sel, obj, p4);
   *(char*)(obj+0xc0)=p5; *(float*)(obj+0xd0)=*(float*)(obj+0xc4); } }
   two 8-byte zero loops at sp+72/sp+64 (byte loop with non-null guard, `li v1,8` in the delay slot),
   local_8={p4,p6}, local_10={param_1,param_2}; for (i<2) { v=local_8[i]; if (v!=0x7fff) {
   if (v>=0 && 0.0f<=local_10[i]) { obj=*(p3+0x58); if (obj&&flag==0) { FUN_00058f38(f,obj,i);
   FUN_0005f110(f,obj); } if (i==0 && *(p3+0x50)) FUN_0005f110(f,*(p3+0x50)); }
   v=local_8[i]; if (v<0) FUN_00093c08(p3,i); else FUN_00093be0(p3,i); } }
   NEXT: give the loop float its own named local held across both calls (that is what forces the
   f20 save and the 80-byte frame), and swap the two float copies' declaration order so local_18
   lands at 56 and local_14 at 60; the body order itself already matches retail at 166/168. */
// near miss from lane dsn8 (unreviewed)
