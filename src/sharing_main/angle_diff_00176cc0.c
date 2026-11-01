/* HSTMATCH module=hstp_trans_main.prx start=0x00176cc0 end=0x00176e1c symbol=FUN_00176cc0 lang=c99 */
/* Clone of src/eboot/angle_diff_0016aafc.c (hstclone.py tag dsn45): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

extern float fabsf(float);
extern float FUN_0000c9dc(float, float);

static inline float normalize(float a)
{
    return (a > 180.0f) ? (FUN_0000c9dc(a + 180.0f, 360.0f) - 180.0f)
                        : ((a < -180.0f) ? (FUN_0000c9dc(a - 180.0f, 360.0f) + 180.0f) : a);
}

float FUN_00176cc0(float param_1, float param_2)
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
