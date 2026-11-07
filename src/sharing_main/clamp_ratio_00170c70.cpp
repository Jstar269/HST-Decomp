/* HSTMATCH module=hstp_trans_main.prx start=0x00170c70 end=0x00170d10 symbol=FUN_00170c70 lang=c++ */
/* Hand-written reconstruction: clone of the source of clamp_ratio_001646a0.cpp in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
// Matching idiom: the ratio must be an IN-PLACE divide of a variable that already holds
// the numerator (`float f = t - first; f = f / (last - first);`). The one-expression
// spelling `(t - first) / (last - first)` puts the numerator in the dead call-result FPR
// and swaps $f2/$f3 (4 words off).
#include "hst/stat_curve.h"

extern StatTable *DAT_003ea850; // curve rows
extern float DAT_00387954;
extern float DAT_00387958;
extern "C" float FUN_001703f0();

static inline float Clamp(float v, const float &lo, const float &hi)
{
  return v < lo ? lo : (!(v > hi) ? v : hi);
}

// Ratio of the value between the first and last curve row, clamped to the global limits.
extern "C" float FUN_00170c70()
{
  float t = FUN_001703f0();
  StatTable *tab = DAT_003ea850;
  StatCurveRow *rows = (StatCurveRow *)tab->base;
  float first = rows[0].col08;
  int n = tab->count - 1;
  StatCurveRow *lastrow = rows + n;
  float last = lastrow->col08;
  float f = t - first;
  f = f / (last - first);
  return Clamp(f, DAT_00387954, DAT_00387958);
}
