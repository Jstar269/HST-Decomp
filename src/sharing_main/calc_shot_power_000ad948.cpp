/* HSTMATCH module=hstp_trans_main.prx start=0x000ad948 end=0x000ada38 symbol=FUN_000ad948 lang=c++ */
/* Clone of src/eboot/calc_shot_power_000aaf54.cpp (hstclone.py tag dsn45): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
/* Normalizes param_1, clamps via inline Clamp between DAT_0037acb8 and DAT_0037acbc, lerps between fields 324 and 320 from FUN_000b6e80, and scales by -0.01 * field_24 * 0.01. */

extern float DAT_0037acb8;
extern float DAT_0037acbc;

extern "C" {
float fabsf(float);
void *FUN_000b6e80(void *param);

inline float Clamp(const float &v, const float &lo, const float &hi) {
    return v < lo ? lo : (!(v > hi) ? v : hi);
}

float FUN_000ad948(float param_1, void *param_2) {
    float norm = fabsf((param_1 - -2.0f) / -4.4007998f);
    float t = Clamp(norm, DAT_0037acb8, DAT_0037acbc);

    float *p1 = (float *)((char *)FUN_000b6e80(param_2) + 320);
    char *p2 = (char *)FUN_000b6e80(param_2);
    float f0 = *p1;
    float f1 = *(float *)(p2 + 324);
    float lerp = f1 + t * (f0 - f1);

    char *p3 = (char *)FUN_000b6e80(param_2);
    float s = *(float *)(p3 + 36);

    return (0.01f * lerp) * (-(0.01f * s));
}
}
