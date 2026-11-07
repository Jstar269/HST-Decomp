/* HSTMATCH module=hstp_trans_main.prx start=0x00170910 end=0x001709a0 symbol=FUN_00170910 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */ /* matching idiom: the clamp is an inline `v < lo ? lo : (!(v > hi) ? v : hi)` taking the bounds by const reference (retail's c.lt.s / c.ngt.s pair and double loads; cf. 0x170c70). */
extern "C" float FUN_001706b0(void);
extern "C" float DAT_0038793c, DAT_00387940, DAT_003ea5d8, DAT_003ea5d4;
static inline float Clamp(float v, const float &lo, const float &hi) { return v < lo ? lo : (!(v > hi) ? v : hi); }
extern "C" float FUN_00170910(void)
{
    float t = FUN_001706b0();
    float x = Clamp(t, DAT_0038793c, DAT_00387940);
    return DAT_003ea5d8 + (x / 9.0f) * (DAT_003ea5d4 - DAT_003ea5d8);
}
