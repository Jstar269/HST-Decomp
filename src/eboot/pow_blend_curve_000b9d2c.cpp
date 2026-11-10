/* HSTMATCH module=EBOOT.BIN start=0x000b9d2c end=0x000b9edc symbol=FUN_000b9d2c lang=c++ */
/* Hand-written reconstruction from the disassembly (from the near miss wip/pow_blend_curve_000b9d2c.cpp); matching idiom: the blend point DAT_002d4a64 is a const global whose later reads go through a one-float struct view so they reload; exact match verified; names and types await human review. */
extern "C" {
extern const float DAT_002d4a64;
struct BlendF { float v; };
extern float DAT_002d4a68;
extern float DAT_002d4a6c;
extern float DAT_002d4a70;
extern float DAT_002d4a74;
extern float DAT_002d4a78;
extern float DAT_002d4a7c;
extern float FUN_0000ce3c(float a, float b);
}

static inline float Clamp(const float &value, const float &lo, const float &hi)
{
  return value < lo ? lo : (!(value > hi) ? value : hi);
}

extern "C" float FUN_000b9d2c(float x)
{
  float s = DAT_002d4a64;
  float t = 1.0f - s;
  float c = Clamp(x, DAT_002d4a68, DAT_002d4a6c);
  if (c <= s) {
    float a = FUN_0000ce3c(c, 2.0f);
    float b = FUN_0000ce3c(s, 2.0f);
    float r = Clamp(a / b, DAT_002d4a70, DAT_002d4a74);
    return ((const BlendF *)&DAT_002d4a64)->v * r;
  } else {
    float a = FUN_0000ce3c(t - (c - ((const BlendF *)&DAT_002d4a64)->v), 2.0f);
    float b = FUN_0000ce3c(t, 2.0f);
    float r = Clamp(1.0f - a / b, DAT_002d4a78, DAT_002d4a7c);
    return ((const BlendF *)&DAT_002d4a64)->v + t * r;
  }
}
