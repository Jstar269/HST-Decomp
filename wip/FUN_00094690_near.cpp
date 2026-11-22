/* HSTMATCH module=EBOOT.BIN start=0x00094690 end=0x000947d4 symbol=FUN_00094690 lang=c++ */

struct Vec3 { float x, y, z; };
struct Vec4 { float x, y, z, w; };
struct LookAt;

extern "C" {
float FUN_0000bf34(float);
float FUN_0000bc34(float);
float FUN_0000c6dc(float, float);

void FUN_00094690(LookAt *c, Vec3 *out, const Vec4 *dir, const Vec4 &up) {
    float x1 = dir->x;
    float z1 = dir->z;
    float x2 = up.x;
    float y2 = up.y;
    float z2 = up.z;
    float y1 = dir->y;

    float yaw = FUN_0000c6dc(x1, z1);
    out->x = yaw;
    FUN_0000bf34(yaw);
    FUN_0000bc34(yaw);

    float f27 = x1 * FUN_0000bf34(yaw) + z1 * FUN_0000bc34(yaw);
    yaw = out->x;
    float f21 = x2 * FUN_0000bc34(yaw) - z2 * FUN_0000bf34(yaw);
    float f23 = x2 * FUN_0000bf34(yaw) + z2 * FUN_0000bc34(yaw);

    float pitch = FUN_0000c6dc(y1, f27);
    out->y = pitch;

    float f20 = y2 * FUN_0000bc34(pitch) - f23 * FUN_0000bf34(pitch);
    FUN_0000bc34(pitch);
    FUN_0000bf34(pitch);

    out->z = FUN_0000c6dc(f21, f20);
}
}


// near miss api-hstm-20261121: 81/81 words, 30 differ, all FPR colouring (retail f21=dir.x f26=dir.z f23=up.x f24=up.y f25=up.z
// f20=dir.y f27=cos-sum f22=yaw; the yaw is RE-READ from out->x into f26, dir.z's register). Fixed vs the older wip: callees named by
// address (FUN_0000c6dc atan2f, FUN_0000bf34 cosf, FUN_0000bc34 sinf; the lane has no atan2f/cosf/sinf symbols).
// Ruled out: all 720 orders of the six input loads (30 every time).
// improved by lane api-hstm-20261121 (unreviewed): no comparable result -> 30 differing words
