/* HSTMATCH module=EBOOT.BIN start=0x00094690 end=0x000947d4 symbol=FUN_00094690 lang=c++ */

struct Vec3 { float x, y, z; };
struct Vec4 { float x, y, z, w; };
struct LookAt;

extern "C" {
float cosf(float);
float sinf(float);
float atan2f(float, float);

void FUN_00094690(LookAt *c, Vec3 *out, const Vec4 *dir, const Vec4 &up) {
    float x1 = dir->x;
    float z1 = dir->z;
    float x2 = up.x;
    float y2 = up.y;
    float z2 = up.z;
    float y1 = dir->y;

    float yaw = atan2f(x1, z1);
    out->x = yaw;
    cosf(yaw);
    sinf(yaw);

    float f27 = x1 * cosf(yaw) + z1 * sinf(yaw);
    yaw = out->x;
    float f21 = x2 * sinf(yaw) - z2 * cosf(yaw);
    float f23 = x2 * cosf(yaw) + z2 * sinf(yaw);

    float pitch = atan2f(y1, f27);
    out->y = pitch;

    float f20 = y2 * sinf(pitch) - f23 * cosf(pitch);
    sinf(pitch);
    cosf(pitch);

    out->z = atan2f(f21, f20);
}
}

// 81/81 words, 30 differing words. First diff at +0x30.
// Frame size (48 bytes) and all saved GPRs (s0, ra) and FPRs (f20-f27) match retail.
// The algorithm is exact LookAt Euler angle decomposition (yaw/pitch/roll).
// Unused cosf/sinf calls after yaw and before roll are verified.
// Differences are strictly FP register colouring ($f20-$f26 assignment between local variables):
// Retail has: f21=dir.x, f26=dir.z, f23=up.x, f24=up.y, f25=up.z, f20=dir.y, f27=f27, f22=yaw.
// MWCC persistent colouring puts up.y in f20 and dir.y in f25.
// Ruled out: struct copy assignments (causes stack frame growth), declaration order tricks (MWCC prioritizes live range end over decl order for floats).
// near miss from lane dsn5 (unreviewed)
