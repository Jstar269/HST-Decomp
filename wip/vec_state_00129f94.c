/* HSTMATCH module=EBOOT.BIN start=0x00129f94 end=0x0012a254 symbol=FUN_00129f94 lang=c99 */
extern float DAT_002da3a0;
extern float DAT_002da3b8;
extern void FUN_00022434(float *o, float *a, float *b);
extern float FUN_000224d8(float *a, float *b);
extern float FUN_00022e44(float v);
extern void FUN_0012b2b0(float f, int a, int b, int c);
extern void FUN_0012b360(int a, int b);
extern void FUN_0012b3d4(int a, int b);
extern void FUN_000937a0(float f, int a, int b);
extern void FUN_00093be0(int a, int b);

void FUN_00129f94(int param_1)
{
  float a[4];
  float b[4];
  float c[4];
  float d[4];
  float *pf;
  float len;
  float spd;
  int st;
  int n;

  st = *(int *)(param_1 + 400);
  if (st == 5 || st == 4) {
    FUN_00022434(c, (float *)(param_1 + 0x1c0), (float *)(param_1 + 0x1b0));
    d[2] = c[2];
    d[0] = c[0];
    d[1] = c[1];
    d[3] = c[3];
    len = FUN_000224d8(d, d);
    if (1e-05f < len) {
      if (*(int *)(param_1 + 400) == 5) {
        spd = FUN_00022e44(FUN_000224d8(d, d)) / *(float *)(param_1 + 0x324);
      } else {
        spd = FUN_00022e44(FUN_000224d8(d, d)) / *(float *)(param_1 + 800);
      }
      if (spd < DAT_002da3b8) pf = &spd; else pf = &DAT_002da3b8;
      FUN_000937a0(*pf, param_1, 0xffffffff);
    }
    FUN_00093be0(param_1, 0xffffffff);
  } else if (st == 0) {
    n = *(int *)(param_1 + 0x250);
    goto store;
  
  } else if (st == 0xc) {
    FUN_00022434(a, (float *)(param_1 + 0x1c0), (float *)(param_1 + 0x1b0));
    b[2] = a[2];
    b[0] = a[0];
    b[1] = a[1];
    b[3] = a[3];
    len = FUN_000224d8(b, b);
    if (len < 1e-05f) {
      FUN_0012b2b0(-1.0f, param_1, 1, 1);
    } else {
      len = FUN_000224d8(b, b);
      if (len < DAT_002da3a0 * DAT_002da3a0) {
        FUN_0012b360(param_1, 1);
        spd = FUN_00022e44(FUN_000224d8(b, b));
        spd = spd / *(float *)(param_1 + 800);
        if (spd < DAT_002da3b8) pf = &spd; else pf = &DAT_002da3b8;
        FUN_000937a0(*pf, param_1, 0xffffffff);
      } else {
        FUN_0012b3d4(param_1, 1);
        spd = FUN_00022e44(FUN_000224d8(b, b));
        spd = spd / *(float *)(param_1 + 0x324);
        if (spd < DAT_002da3b8) pf = &spd; else pf = &DAT_002da3b8;
        FUN_000937a0(*pf, param_1, 0xffffffff);
      }
    }
    FUN_00093be0(param_1, 0xffffffff);
  }
  n = *(int *)(param_1 + 0x250);
store:
  *(int *)(param_1 + 0x250) = n + 1;
}

/* NEAR 174/176 words, 169 differ, first +0x18. PROLOGUE IS IDENTICAL to retail (96-byte frame, ra at
   12, s0 at 8, `move s0,a0`, state load, `li v1,12`) -- first divergence is the dispatch shape: retail
   tests st==0xc with `beql a0,v1,0x12a0d0` and lays the 5/4 arm out in the fall-through, i.e. the 0xc
   block is LAST in the function, ours had it first. Reordering the arms (5/4, then 0, then 0xc) is the
   promoted body: 170->174 words, differ unchanged at 169 (the extra words are real blocks that moved,
   not new instructions). A companion with the four float arrays declared in reverse (d,c,b,a) is worse
   (174/176 at 170), so declaration order is not the handle. Body is still 2 words short.
   Structure: st=*(p+400); switch by st: st==0xc -> a=vec(p+0x1c0,p+0x1b0); b={a[2],a[0],a[1],a[3]};
   len=dot(b,b); if (len<1e-05) FUN_0012b2b0(-1.0f,p,1,1) else if (len<DAT_002da3a0^2) { FUN_0012b360(p,1);
   spd=acos?FUN_00022e44(dot(b,b))/ *(float*)(p+800) } else { FUN_0012b3d4(p,1); spd=.../ *(float*)(p+0x324) };
   clamp spd to DAT_002da3b8 and FUN_000937a0(spd,p,-1); FUN_00093be0(p,-1). st==5/4 -> same with the
   divisor chosen by a second load of *(p+400)==5. st==0 -> skip. Finally *(int*)(p+0x250)++ once.
   NEXT: the remaining gap is the clamp idiom -- retail holds the DAT_002da3b8 address in a register and
   selects with bc1fl before the call; try `float lim = DAT_002da3b8; FUN_000937a0(spd < lim ? spd : lim,
   p, -1);` so the ternary's address arms collapse and the float compare lands in the delay slot. */
// near miss from lane dsn8 (unreviewed)
