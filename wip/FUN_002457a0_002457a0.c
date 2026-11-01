/* HSTMATCH module=EBOOT.BIN start=0x002457a0 end=0x00245b04 symbol=FUN_002457a0 lang=c99 */
#include "hst/types.h"

typedef struct {
    float x, y, z, w;
} Vec4;

struct Sub88 {
    char pad00[0x20];
    Vec4 target_vel;      // 0x20
    char pad30[0x2f0 - 0x30];
    Vec4 pos;             // 0x2f0
};

struct Sub9c {
    char pad00[0x58];
    int f58;              // 0x58
};

struct Slot {
    char pad00[0x580];
    Vec4 v1;              // 0x580
    Vec4 v2;              // 0x590
    Vec4 v3;              // 0x5a0
    char pad5b0[0xc];
    void *arg;            // 0x5bc
};

struct Mgr {
    char pad00[0x3c];
    int f3c;              // 0x3c
    char pad40[0x70 - 0x40];
    struct Slot *slots[4];// 0x70
    char pad80[8];        // 0x80
    struct Sub88 *sub88;  // 0x88
    char pad8c[0x9c - 0x8c];
    struct Sub9c *sub9c;  // 0x9c
};

struct GlobalSeedHolder {
    char pad[0x1360];
    int seed;
};

struct Obj_002457a0 {
    char pad00[0x4c];
    int f4c;              // 0x4c
    char pad50[0xc0 - 0x50];
    Vec4 pos;             // 0xc0
    char padcc[0x264 - 0xd0];
    int f264;             // 0x264
    char pad268[0x270 - 0x268];
    Vec4 vel;             // 0x270
    char pad280[0x2ac - 0x280];
    int mode;             // 0x2ac
    char pad2b0[0x2c0 - 0x2b0];
    int f2c0;             // 0x2c0
    int f2c4;             // 0x2c4
    char pad2c8[0x2e0 - 0x2c8];
    int f2e0;             // 0x2e0
    int f2e4;             // 0x2e4
};

extern struct Mgr *DAT_003386b0;
extern struct GlobalSeedHolder *DAT_003402a0;

extern Vec4 *FUN_00022434(Vec4 *out, const Vec4 *a, const Vec4 *b);
extern float FUN_000224d8(const Vec4 *a, const Vec4 *b);
extern float FUN_00022e44(float);
extern Vec4 *FUN_00022450(Vec4 *out, const Vec4 *a, const Vec4 *b);
extern Vec4 *FUN_00093838(void *arg);

int FUN_002457a0(struct Obj_002457a0 *self)
{
    int s;
    struct Slot *slot;
    Vec4 self_pos;
    Vec4 sub_pos;
    Vec4 slot_pos_copy;
    Vec4 d;
    Vec4 d2;
    Vec4 diff;
    Vec4 diff2;
    Vec4 temp2;
    Vec4 res;
    Vec4 temp;

    int mode = self->mode;
    if ((unsigned int)(mode - 2) < 2) {
        return 0;
    } else if ((unsigned int)(mode - 4) < 2) {
        struct Mgr *mgr = DAT_003386b0;
        if (mgr->f3c != 7) {
            int a0 = mgr->sub9c->f58;
            self->f2e4 = a0;
            int sum = self->f2e0 + a0;
            if (sum % (self->f2c0 * 30) == 0 && sum != 0) {
                if (self->mode == 5) {
                    int seed = DAT_003402a0->seed * 0x343fd + 0x269ec3;
                    DAT_003402a0->seed = seed;
                    if (((seed >> 16) & 0x7fff) % 100 < self->f2c4) {
                        self->f2e4 = 0;
                        self->f2e0 = 0;
                        return 1;
                    }
                }
                if (self->mode == 4) {
                    self->f2e4 = 0;
                    self->f2e0 = 0;
                    return 1;
                }
            }
        } else {
            if (self->f4c == mgr->sub9c->f58) {
                return 1;
            }
        }
        return 0;
    } else {
        self_pos.x = self->pos.x;
        self_pos.y = self->pos.y;
        self_pos.z = self->pos.z;
        self_pos.w = self->pos.w;

        sub_pos.x = DAT_003386b0->sub88->pos.x;
        sub_pos.y = DAT_003386b0->sub88->pos.y;
        sub_pos.z = DAT_003386b0->sub88->pos.z;
        sub_pos.w = DAT_003386b0->sub88->pos.w;

        FUN_00022434(&diff, &self_pos, &sub_pos);
        d = diff;
        float dist = FUN_00022e44(FUN_000224d8(&d, &d));
        if (dist < 2.0f) {
            self->vel.y = DAT_003386b0->sub88->target_vel.y;
            self->vel.z = DAT_003386b0->sub88->target_vel.z;
            self->vel.w = DAT_003386b0->sub88->target_vel.w;
            self->vel.x = DAT_003386b0->sub88->target_vel.x;
            self->vel.y = 0.0f;
            self->f264 = 1;
            return 1;
        } else {
            for (s = 0; s < 4; s++) {
                slot = DAT_003386b0->slots[s];
                if (slot != 0) {
                    Vec4 *slot_pos = FUN_00093838(slot->arg);
                    slot_pos_copy.x = slot_pos->x;
                    slot_pos_copy.y = slot_pos->y;
                    slot_pos_copy.z = slot_pos->z;
                    slot_pos_copy.w = slot_pos->w;

                    FUN_00022434(&diff2, &self_pos, &slot_pos_copy);
                    d2 = diff2;
                    float dist2 = FUN_00022e44(FUN_000224d8(&d2, &d2));
                    if (dist2 < 2.0f) {
                        FUN_00022434(&temp, &slot->v1, &slot->v2);
                        temp2 = temp;
                        FUN_00022450(&res, &temp2, &slot->v3);
                        self->vel = res;
                        self->f264 = 0;
                        return 1;
                    }
                }
            }
            return 0;
        }
    }
}
// Near miss: 217/217 words match in size and frame (192 bytes); 17 differ.
// Differs:
// - 1 word at 0x245810: addu a0, a0, v0 vs addu a0, v0, a0.
// - 8 words at 0x24599c-0x2459b8: copying target_vel to self->vel; retail loads offsets +4,+8,+12,+0 into f3,f2,f1,f0, then stores.
// - 8 words at 0x245aa8-0x245ac8: copying res to self->vel; same FP register order differences.
// Ruled out: struct assignment self->vel = ... directly loads +0,+4,+8,+12 into f3,f2,f1,f0; volatile DAT_003386b0 reload.
// near miss from lane dsn7 (unreviewed)
