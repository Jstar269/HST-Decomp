/* HSTMATCH module=EBOOT.BIN start=0x000aaf54 end=0x000ab044 symbol=FUN_000aaf54 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
/* Normalizes param_1, clamps via inline Clamp between DAT_002d46b0 and DAT_002d46b4, lerps between fields 324 and 320 from FUN_000b3c68, and scales by -0.01 * field_24 * 0.01. */

extern float DAT_002d46b0;
extern float DAT_002d46b4;

extern "C" {
float fabsf(float);
void *FUN_000b3c68(void *param);

inline float Clamp(const float &v, const float &lo, const float &hi) {
    return v < lo ? lo : (!(v > hi) ? v : hi);
}

float FUN_000aaf54(float param_1, void *param_2) {
    float norm = fabsf((param_1 - -2.0f) / -4.4007998f);
    float t = Clamp(norm, DAT_002d46b0, DAT_002d46b4);

    float *p1 = (float *)((char *)FUN_000b3c68(param_2) + 320);
    char *p2 = (char *)FUN_000b3c68(param_2);
    float f0 = *p1;
    float f1 = *(float *)(p2 + 324);
    float lerp = f1 + t * (f0 - f1);

    char *p3 = (char *)FUN_000b3c68(param_2);
    float s = *(float *)(p3 + 36);

    return (0.01f * lerp) * (-(0.01f * s));
}
}
