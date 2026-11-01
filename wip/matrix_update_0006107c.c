/* HSTMATCH module=EBOOT.BIN start=0x0006107c end=0x000611d0 symbol=FUN_0006107c lang=c99 */

typedef struct Float4 {
    float x, y, z, w;
} Float4;

typedef struct Obj {
    char pad0[0x30];
    Float4 f30;    /* +0x30 */
    Float4 f40;    /* +0x40 */
    char pad50[0x10];
    float f50;     /* +0x50: handle */
} Obj;

extern void FUN_00022498(Float4 *out, Float4 *in, float handle);
extern void FUN_0001d0b8(Float4 *out, void *param, float angle);
extern void FUN_0001d164(Float4 *out, Float4 *in);
extern void FUN_00022548(Float4 *out, Float4 *a, Float4 *b);
extern void FUN_00022418(Float4 *out, Float4 *a, Float4 *b);

/* NEAR MISS 77/85 words. Residual: 176-byte frame vs retail's 240; retail keeps every
   float4 intermediate in its own stack slot (neg at 112, q0 at 96, local_10 at 224, the
   0x1d164 matrix at 144 which is 64 bytes = a 4x4 float matrix, 0x1d0b8's output at 208)
   and builds the negated vector by loading w,z,y,x and then negating all four; this form
   coalesces the temps. Calls and argument order match: 0x22498(out,in,handle at self+0x50),
   0x1d0b8(out, param_2, angle in f12), 0x1d164(out64, in), 0x22548(in place), 0x22418. */
void FUN_0006107c(Obj *self, void *param_2, float angle)
{
    Float4 neg;
    Float4 q0;
    Float4 q1;
    Float4 m0;
    Float4 m1;
    Float4 q2;
    Float4 q3;
    Float4 r0;
    Float4 r1;

    neg.x = -self->f30.x;
    neg.y = -self->f30.y;
    neg.z = -self->f30.z;
    neg.w = -self->f30.w;
    FUN_00022498(&q0, &neg, self->f50);
    q1 = q0;
    FUN_0001d0b8(&m0, param_2, angle);
    FUN_0001d164(&m1, &m0);
    FUN_00022548(&q1, &m1, &q1);
    FUN_00022498(&q2, &self->f30, self->f50);
    q3 = q2;
    FUN_00022418(&r0, &self->f40, &q3);
    FUN_00022418(&r1, &r0, &q1);
    self->f40 = r1;
}
// near miss from lane dsn8 (unreviewed)
