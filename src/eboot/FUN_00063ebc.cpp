/* HSTMATCH module=EBOOT.BIN start=0x00063ebc end=0x00064194 symbol=FUN_00063ebc lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

extern "C" void FUN_000513a0(int, int);
extern "C" void FUN_00051490(int, int, int, int, int, int, int);
extern "C" void FUN_00051714(int);
extern "C" void FUN_00051478(int, int);
extern "C" int FUN_00050534(int);
extern "C" void FUN_00051398(int, float);
extern "C" void FUN_00051894(int, int, float, float, float, float, float, float, float, float, float);
extern "C" void FUN_0005185c(int);
extern "C" void FUN_000503d0(int, float);

extern "C" unsigned int DAT_002d11c0, DAT_002d11c4;
extern "C" char DAT_00311170[];

static inline unsigned int Clamp(const unsigned int &v, const unsigned int &lo, const unsigned int &hi) {
    return v < lo ? lo : (hi < v ? hi : v);
}

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
    float f27 = (o->f8 + o->f16) - o->f8 * o->f24;
    float f26 = (o->f12 + o->f20) - o->f12 * o->f28;
    float f25 = 128.0f * o->f24;
    float f1 = 128.0f * f20;
    float f24 = 16.0f * o->f28;
    unsigned int v1 = (unsigned int)f1;
    unsigned int cl = Clamp(v1, DAT_002d11c0, DAT_002d11c4);
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
  if (o->f4 >= 0.0f) FUN_000503d0((int)DAT_00311170, o->f4);
}
