/* HSTMATCH module=hstp_trans_main.prx start=0x00064d80 end=0x00065058 symbol=FUN_00064d80 lang=c++ */
/* Hand-written reconstruction: clone of the source of FUN_00063ebc.cpp in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

extern "C" void FUN_00051d40(int, int);
extern "C" void FUN_00051e30(int, int, int, int, int, int, int);
extern "C" void FUN_00052178(int);
extern "C" void FUN_00051e18(int, int);
extern "C" int FUN_00050e10(int);
extern "C" void FUN_00051d38(int, float);
extern "C" void FUN_00052354(int, int, float, float, float, float, float, float, float, float, float);
extern "C" void FUN_000522c0(int);
extern "C" void FUN_00050cac(int, float);

extern "C" unsigned int DAT_003770f8, DAT_003770fc;
extern "C" char DAT_003ba4b0[];

static inline unsigned int Clamp(const unsigned int &v, const unsigned int &lo, const unsigned int &hi) {
    return v < lo ? lo : (hi < v ? hi : v);
}

struct Obj { int h; float f4, f8, f12, f16, f20, f24, f28; };

extern "C" void FUN_00064d80(Obj *o, float a, float b)
{
  float f20 = b;
  float f28 = a;
  int s3;
  if (f20 > 0.0f) {
    if (o->f4 < 0.0f) {
      s3 = *(int *)o;
      FUN_00051d40(s3, 0);
      FUN_00051e30(s3, 0, 0, 480, 272, 0, 0);
      FUN_00052178(s3);
      FUN_00051e18(s3, 0xdf000032);
    } else {
      s3 = FUN_00050e10((int)DAT_003ba4b0);
      FUN_00051d38(s3, *(float *)(*(int *)o + 0x18));
      FUN_00051d40(s3, 0);
      FUN_00051e30(s3, 0, 0, 480, 272, 0, 0);
    }
    float f27 = (o->f8 + o->f16) - o->f8 * o->f24;
    float f26 = (o->f12 + o->f20) - o->f12 * o->f28;
    float f25 = 128.0f * o->f24;
    float f1 = 128.0f * f20;
    float f24 = 16.0f * o->f28;
    unsigned int v1 = (unsigned int)f1;
    unsigned int cl = Clamp(v1, DAT_003770f8, DAT_003770fc);
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
        FUN_00052354(s3, s2, f23, f13, f22, f15, g20, f17, f21, f19, f28);
      }
    }
    FUN_000522c0(s3);
  }
  if (o->f4 >= 0.0f) FUN_00050cac((int)DAT_003ba4b0, o->f4);
}
