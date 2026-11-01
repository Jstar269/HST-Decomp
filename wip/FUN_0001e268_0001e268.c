/* HSTMATCH module=EBOOT.BIN start=0x0001e268 end=0x0001ea64 symbol=FUN_0001e268 lang=c99 */
#include "hst/types.h"

typedef struct {
    float x, y, z, w;
} Vec4;

typedef struct {
    u8 enabled;           // 0x00
    char pad01[3];
    int countdown;        // 0x04
    char pad08[0x80 - 0x08];
    u32 buttons_prev;     // 0x80
    u32 buttons_mask;     // 0x84
    u32 buttons_trans_lo; // 0x88
    u32 buttons_trans_hi; // 0x8c
    float stick_x;        // 0x90
    float stick_y;        // 0x94
    float f98;            // 0x98
    float f9c;            // 0x9c
    Vec4 stick_vec;       // 0xa0
    u32 padb0[4];         // 0xb0..0xbc
} Controller;

extern u8 DAT_0030fe12;
extern u8 DAT_0030fe13;
extern u16 DAT_0030fe10;

extern void FUN_00048f08(Vec4 *out, const Vec4 *in);
extern Vec4 *FUN_00022498(Vec4 *out, const Vec4 *in, float scale);

void FUN_0001e268(Controller *ctrl)
{
    if (ctrl->enabled == 0) {
        return;
    }

    if (ctrl->countdown != 0) {
        ctrl->countdown--;
    } else {
        u8 raw_x = DAT_0030fe12;
        u8 raw_y = DAT_0030fe13;

        int x = (int)raw_x - (u8)(raw_x > 128) - 127;
        int y = (int)raw_y - (u8)(raw_y > 128) - 127;

        float val_x = 0.0f;
        int abs_x = x < 0 ? -x : x;
        if (abs_x >= 64) {
            int sign = x < 0 ? -1 : 1;
            val_x = (float)(x - (sign << 6)) / 63.0f;
        }
        ctrl->stick_x = val_x;

        float val_y = 0.0f;
        int abs_y = y < 0 ? -y : y;
        if (abs_y >= 64) {
            int sign = y < 0 ? -1 : 1;
            val_y = (float)(y - (sign << 6)) / 63.0f;
        }
        ctrl->stick_y = val_y;

        float fy = (float)-y / 127.0f;
        float fx = (float)x / 127.0f;
        ctrl->f98 = 0.0f;
        ctrl->f9c = 0.0f;

        Vec4 dir;
        if (fx == 0.0f && fy == 0.0f) {
            dir.x = 0.0f;
            dir.y = 0.0f;
            dir.z = 1.0f;
            dir.w = 0.0f;
        } else {
            Vec4 temp;
            Vec4 in_vec;
            in_vec.x = fx;
            in_vec.y = 0.0f;
            in_vec.z = fy;
            in_vec.w = 0.0f;
            FUN_00048f08(&temp, &in_vec);
            dir = temp;
        }

        if (fy < 0.0f) fy = -fy;
        if (fx < 0.0f) fx = -fx;
        float mag = fx > fy ? fx : fy;

        float scale;
        if (mag <= 0.503937f) {
            scale = 0.0f;
        } else {
            scale = (mag - 0.503937f) / 0.496063f;
        }

        Vec4 scaled;
        FUN_00022498(&scaled, &dir, scale);
        ctrl->stick_vec = scaled;

        ctrl->padb0[0] = 0;
        ctrl->padb0[1] = 0;
        ctrl->padb0[2] = 0;
        ctrl->padb0[3] = 0;
    }

    u16 raw_buttons = DAT_0030fe10;
    u32 curr = (raw_buttons & 0xffff9fff)
             | (((raw_buttons & 0x2000) != 0) << 14)
             | (((raw_buttons & 0x4000) != 0) << 13);
    u32 prev = ctrl->buttons_prev;

    u32 stick_flags = ((u8)(ctrl->stick_y < 0.0f) << 16)
                    | ((u8)(ctrl->stick_x > 0.0f) << 17)
                    | ((u8)(ctrl->stick_y > 0.0f) << 18)
                    | ((u8)(ctrl->stick_x < 0.0f) << 19)
                    | ((u8)(ctrl->f9c < 0.0f) << 20)
                    | ((u8)(ctrl->f98 > 0.0f) << 21)
                    | ((u8)(ctrl->f9c > 0.0f) << 22)
                    | ((u8)(ctrl->f98 < 0.0f) << 23);

    curr |= stick_flags;

    if (ctrl->buttons_mask != 0) {
        u32 mask = ctrl->buttons_mask & curr;
        curr = curr & ~mask;
        ctrl->buttons_mask = mask;

        if ((mask & 0xf0000) != 0) {
            if (ctrl->stick_y < 0.0f) {
                if ((mask & 0x10000) != 0) *(int *)&ctrl->stick_y = 0;
            } else if (ctrl->stick_y > 0.0f) {
                if ((mask & 0x40000) != 0) *(int *)&ctrl->stick_y = 0;
            }
            if (ctrl->stick_x < 0.0f) {
                if ((mask & 0x80000) != 0) *(int *)&ctrl->stick_x = 0;
            } else if (ctrl->stick_x > 0.0f) {
                if ((mask & 0x20000) != 0) *(int *)&ctrl->stick_x = 0;
            }
        }

        if ((mask & 0xf00000) != 0) {
            if (ctrl->f9c < 0.0f) {
                if ((mask & 0x100000) != 0) *(int *)&ctrl->f9c = 0;
            } else if (ctrl->f9c > 0.0f) {
                if ((mask & 0x400000) != 0) *(int *)&ctrl->f9c = 0;
            }
            if (ctrl->f98 < 0.0f) {
                if ((mask & 0x800000) != 0) *(int *)&ctrl->f98 = 0;
            } else if (ctrl->f98 > 0.0f) {
                if ((mask & 0x200000) != 0) *(int *)&ctrl->f98 = 0;
            }
        }
    }
    ctrl->buttons_prev = curr;

    u32 t2 = 0;
    u32 cur = curr;
    u32 prv = prev;
    int shift = 0;
    for (int i = 0; i < 16; i += 8) {
        #define STEP(b) do { \
            int c_b = (cur >> (b)) & 1; \
            int p_b = (prv >> (b)) & 1; \
            int code = ((signed char)((c_b << 1) | p_b) - 2) & 3; \
            t2 |= code << (shift + (b) * 2); \
        } while (0)
        STEP(0); STEP(1); STEP(2); STEP(3);
        STEP(4); STEP(5); STEP(6); STEP(7);
        #undef STEP
        cur >>= 8;
        prv >>= 8;
        shift += 16;
    }
    ctrl->buttons_trans_lo = t2;

    u32 cur_raw = ctrl->buttons_prev;
    int c0 = (cur_raw >> 16) & 1;
    int p0 = (prev >> 16) & 1;
    u32 t3 = ((signed char)((c0 << 1) | p0) - 2) & 3;

    u32 cur_hi = cur_raw >> 16;
    u32 prv_hi = prev >> 16;
    #define STEP_HI(b) do { \
        int c_b = (cur_hi >> (b)) & 1; \
        int p_b = (prv_hi >> (b)) & 1; \
        int code = ((signed char)((c_b << 1) | p_b) - 2) & 3; \
        t3 |= code << ((b) * 2); \
    } while (0)
    STEP_HI(1); STEP_HI(2); STEP_HI(3);
    STEP_HI(4); STEP_HI(5); STEP_HI(6);
    #undef STEP_HI
    int c7 = (cur_hi >> 7) & 1;
    int p7 = (prv_hi >> 7) & 1;
    int code7 = ((signed char)((c7 << 1) | p7) - 2) & 3;
    t3 |= code7 << 14;
    ctrl->buttons_trans_hi = t3;
}
// Differs: 480 words differ out of 511 words (507/511 words compiled; 4 words length delta);
// Branch targets and delay slot scheduling slightly shifted in button flag generation and 2-bit transition loops.
// Ruled out: stack frame size (matched at 96 bytes with 4 Vec4 locals), Vec4 layout (matched), and analog deadzone formulas.
// near miss from lane dsn7 (unreviewed)
