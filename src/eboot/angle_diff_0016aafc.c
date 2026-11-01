/* HSTMATCH module=EBOOT.BIN start=0x0016aafc end=0x0016ac58 symbol=FUN_0016aafc lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

extern float fabsf(float);
extern float FUN_0000c9c4(float, float);

static inline float normalize(float a)
{
    return (a > 180.0f) ? (FUN_0000c9c4(a + 180.0f, 360.0f) - 180.0f)
                        : ((a < -180.0f) ? (FUN_0000c9c4(a - 180.0f, 360.0f) + 180.0f) : a);
}

float FUN_0016aafc(float param_1, float param_2)
{
    float a = normalize(param_1 * 57.29578f);
    float b = normalize(param_2 * 57.29578f);

    float diff = fabsf(b - a);
    if (diff > 180.0f) {
        diff = 360.0f - diff;
    }

    diff *= 0.017453292f;
    return diff;
}
