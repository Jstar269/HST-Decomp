/* HSTMATCH module=EBOOT.BIN start=0x00063ebc end=0x00064194 symbol=FUN_00063ebc lang=c++ */
extern "C" void FUN_000513a0(int, int);
extern "C" void FUN_00051490(int, int, int, int, int, int, int);
extern "C" void FUN_00051714(int);
extern "C" void FUN_00051478(int, int);
extern "C" int FUN_00050534(int);
extern "C" void FUN_00051398(int, float);
extern "C" void FUN_00051894(int, int, float, float, float, float, float, float, float, float, float);
extern "C" void FUN_0005185c(int);
extern "C" void FUN_000503d0(int);
extern "C" volatile int DAT_002d11c0, DAT_002d11c4;
extern "C" char DAT_00311170[];
struct Obj { int h; float f4, f8, f12, f16, f20, f24, f28; };
extern "C" void FUN_00063ebc(Obj *o, float a, float b)
{
  float f20 = b;
  float f28 = a;
  int s3;
  if (f20 > 0.0f) {
  if (o->f4 < 0.0f) {
    s3 = *(int *)o;
    FUN_000513a0(s3, 0);
    FUN_00051490(s3, 0, 0, 480, 272, 0, 0);
    FUN_00051714(s3);
    FUN_00051478(s3, 0xdf000032);
  } else {
    s3 = FUN_00050534((int)DAT_00311170);
    FUN_00051398(s3, *(float *)(*(int *)o + 0x18));
    FUN_000513a0(s3, 0);
    FUN_00051490(s3, 0, 0, 480, 272, 0, 0);
  }
  float f6 = o->f24, f5 = o->f8, f4 = o->f16, f2 = o->f12, f0 = o->f20, f3 = o->f28;
  float f27 = (f5 + f4) - f5 * f6;
  float f26 = (f2 + f0) - f2 * f3;
  float f25 = 128.0f * f6;
  float f1 = 128.0f * f20;
  float f24 = 16.0f * f3;
  unsigned v1;
  if (2147483648.0f <= f1) { v1 = (int)(f1 - 2147483648.0f) | -32768; }
  else v1 = (int)f1;
  unsigned cl = DAT_002d11c0;
  if (v1 >= (unsigned)DAT_002d11c0) {
    cl = DAT_002d11c4;
    if ((unsigned)DAT_002d11c4 < v1) cl = v1;
  }
  int s2 = (cl << 24) | 0x00808080;
  for (int i = 0; i < 4; i++) {
    float fi = (float)i;
    float f23 = 128.0f * fi;
    float f22 = f27 + f25 * fi;
    float f21 = f22 + f25;
    float g20 = 128.0f + f23;
    for (int j = 0; j < 17; j++) {
      float fj = (float)j;
      float f13 = 16.0f * fj;
      float f15 = f26 + f24 * fj;
      float f17 = 16.0f + f13;
      float f19 = f15 + f24;
      FUN_00051894(s3, s2, f23, f13, f22, f15, g20, f17, f21, f19, f28);
    }
  }
  FUN_0005185c(s3);
  }
  if (o->f4 >= 0.0f) FUN_000503d0((int)DAT_00311170);
}
// PARKED at 180/182 (96 differ, first +0x50; pass 215, 3 compiles). Fixed this pass and measured:
// the clamp globals are VOLATILE (retail issues two loads per global, four in total - the six words
// the park was short), the inner clamp test is `hi < v1` (retail's beqzl + move a0,v1), and the
// destroy block sits OUTSIDE the (b>0) guard firing on f4 >= 0 (retail's early bc1t targets the f4
// test at 0x6412c, and c.nge.s+bc1tl jumps to the epilogue when f4 < 0). The call passes the 16*j
// value in f13 and f22 in f14, matching retail. Residual, aligned: 2 words of colour - the tail test
// loads f4 into f1 where retail uses f12 - plus a 2-word block-layout gap in the clamp (retail keeps
// the keep-arm inline with `b 0x64088` over an out-of-line else and a redundant `b`+nop after the
// inner `move a0,v1`), which is what leaves the +0x50 displacement 8 bytes short.
// improved by lane dsn3 (unreviewed): 99 -> 96 differing words
