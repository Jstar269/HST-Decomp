/* HSTMATCH module=EBOOT.BIN start=0x00029da0 end=0x00029e58 symbol=FUN_00029da0 lang=c99 */

typedef struct Vec3 {
    float x, y, z;
} Vec3;

extern float DAT_002bb924;
extern float fabsf(float);
extern float sqrtf(float);
extern void FUN_002bf9c(int arg, Vec3 *v, float *out);

/* NEAR MISS 43/46 words. Residual: retail keeps the raw x,y in f3/f2 across the guard and
   reloads them for the last store of each block (3 extra words); the closest form loads
   v->z first and puts the guard's abs in f1/f0. Ruled out: pointer-form guard (destroys
   f3/f2, 45 words), variant with z as a function-scope local (MWCC sinks the z load after
   x*x and reuses f3), *p > id style operand swap (changes to c.nge), block-ordered if/else
   swaps, and a separate ax/ay pair at function scope. */
void FUN_00029da0(float *out, int arg, Vec3 *v)
{
    float x = v->x;
    float y = v->y;
    float ax = fabsf(x);
    float ay = fabsf(y);
    float z;
    float len;
    float k;

    if (ay <= ax) {
        z = v->z;
        len = sqrtf(x * x + z * z);
        k = DAT_002bb924 / len;
        out[1] = 0.0f;
        out[0] = k * -z;
        out[2] = k * v->x;
    }
    else {
        z = v->z;
        len = sqrtf(y * y + z * z);
        k = DAT_002bb924 / len;
        out[0] = 0.0f;
        out[1] = k * z;
        out[2] = k * -v->y;
    }
    FUN_002bf9c(arg, v, out);
}
// near miss from lane dsn8 (unreviewed)
