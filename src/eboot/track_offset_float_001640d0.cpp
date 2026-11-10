/* HSTMATCH module=EBOOT.BIN start=0x001640d0 end=0x00164290 symbol=FUN_001640d0 lang=c++ */
/* Hand-written reconstruction from the disassembly (lane fm8 near miss wip/track_offset_float_001640d0.cpp, finished by lane pn4: the raw value with its early return and table clamp is an inline helper whose result is stored in a local before the global Clamp, which gives retail's FPR numbering); exact match verified; the global bounds go to Clamp by const& (double lwc1); names and types await human review. */
extern "C" int *DAT_0034150c;
extern "C" int *DAT_00341510;
extern "C" float DAT_002e0378;
extern "C" float DAT_002e037c;
extern "C" float DAT_0034128c;
extern "C" float DAT_00341290;
extern "C" float DAT_003414e4;
extern "C" float FUN_000d8334(int, int);

static inline float Clamp(float v, const float &lo, const float &hi)
{
    return v < lo ? lo : (v > hi ? hi : v);
}

static inline float RawValue(int param_1, int param_2)
{
    float v;
    float lo;
    float hi;

    if (param_2 != 0) {
        if (*(short *)(param_2 + 10) / 10 == 0x32)
            return *(float *)(*DAT_00341510 + (DAT_00341510[1] - 1) * 0x14 + 0x10);
    }
    v = *(float *)(param_1 + 0x58);
    if (param_2 != 0) {
        if (*(short *)(param_2 + 10) >= 0)
            v = v + *(float *)(*DAT_0034150c
                               + *(short *)(DAT_0034150c[3] + *(short *)(param_2 + 10) * 2) * 0x22c
                               + 0x58);
        v = v + FUN_000d8334(param_2, 4);
    }
    lo = *(float *)(*DAT_00341510 + 0x10);
    hi = *(float *)(*DAT_00341510 + (DAT_00341510[1] - 1) * 0x14 + 0x10);
    return v < lo ? lo : (v > hi ? hi : v);
}

extern "C" float FUN_001640d0(int param_1, int param_2, unsigned param_3)
{
    float v = RawValue(param_1, param_2);
    v = Clamp(v, DAT_002e0378, DAT_002e037c);
    v = DAT_00341290 + (v / 9.0f) * (DAT_0034128c - DAT_00341290);
    if (param_3 - 1 < 2)
        v = v + DAT_003414e4;
    return v;
}
