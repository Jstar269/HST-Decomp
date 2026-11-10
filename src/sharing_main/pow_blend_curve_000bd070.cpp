/* HSTMATCH module=hstp_trans_main.prx start=0x000bd070 end=0x000bd220 symbol=FUN_000bd070 lang=c++ */
/* Hand-written reconstruction from the disassembly (from the near miss wip/pow_blend_curve_000bd070.cpp); matching idiom: the blend point DAT_0037b32c is a const global whose later reads go through a one-float struct view so they reload; exact match verified; names and types await human review. */
extern "C" {
extern const float DAT_0037b32c;
struct BlendF { float v; };
extern float DAT_0037b330;
extern float DAT_0037b334;
extern float DAT_0037b338;
extern float DAT_0037b33c;
extern float DAT_0037b340;
extern float DAT_0037b344;
extern float FUN_0000ce54(float a, float b);
}

static inline float Clamp(const float &value, const float &lo, const float &hi)
{
  return value < lo ? lo : (!(value > hi) ? value : hi);
}

extern "C" float FUN_000bd070(float x)
{
  float s = DAT_0037b32c;
  float t = 1.0f - s;
  float c = Clamp(x, DAT_0037b330, DAT_0037b334);
  if (c <= s) {
    float a = FUN_0000ce54(c, 2.0f);
    float b = FUN_0000ce54(s, 2.0f);
    float r = Clamp(a / b, DAT_0037b338, DAT_0037b33c);
    return ((const BlendF *)&DAT_0037b32c)->v * r;
  } else {
    float a = FUN_0000ce54(t - (c - ((const BlendF *)&DAT_0037b32c)->v), 2.0f);
    float b = FUN_0000ce54(t, 2.0f);
    float r = Clamp(1.0f - a / b, DAT_0037b340, DAT_0037b344);
    return ((const BlendF *)&DAT_0037b32c)->v + t * r;
  }
}
