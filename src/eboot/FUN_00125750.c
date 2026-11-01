/* HSTMATCH module=EBOOT.BIN start=0x00125750 end=0x001258bc symbol=FUN_00125750 lang=c99 */
/* Hand-written reconstruction (lane dsn4); exact match verified; names and types await human review. */
#include "hst/types.h"
#include "hst/actor_state.h"

typedef struct Vec4 {
    float x, y, z, w;
} Vec4;

extern char DAT_0033b5d1;
extern int DAT_003386b0;
extern int DAT_00340284;
extern float DAT_002da1ec;
extern int DAT_002da1f0;

int FUN_00131a78(int, int);
float FUN_000224d8(Vec4 *, Vec4 *);
float FUN_00022e44(float);
void FUN_0021624c(int, Vec4 *, Vec4 *);
void FUN_00130b3c(int, int);

void FUN_00125750(ActorState *param_1, unsigned char *param_2)
{
    Vec4 v1_vec;
    Vec4 v2_vec;
    Vec4 v0_vec;

    int id = param_1->stateIdx;
    char mode = (&DAT_0033b5d1)[id * 0x58];
    if (mode == 0) {
        return;
    }

    int v1 = *(int *)(DAT_003386b0 + 0x9c);
    int s2 = *(int *)(v1 + 0x58);
    int v0 = FUN_00131a78(DAT_00340284, mode);
    if ((float)s2 != (float)v0) {
        return;
    }

    float *p = (float *)(*(int *)(DAT_003386b0 + 0x88) + 0x20);
    v0_vec.x = p[0];
    v0_vec.y = p[1];
    v0_vec.z = p[2];
    v0_vec.w = p[3];

    float len_sq = FUN_000224d8(&v0_vec, &v0_vec);
    float speed = FUN_00022e44(len_sq);
    if (!(speed * 3.6f >= DAT_002da1ec)) {
        return;
    }

    float *p2 = (float *)(param_2 + 0x40);
    v1_vec.x = p2[0];
    v1_vec.y = p2[1];
    v1_vec.z = p2[2];
    v1_vec.w = p2[3];

    float *p3 = (float *)(param_2 + 0x60);
    v2_vec.x = p3[0];
    v2_vec.y = p3[1];
    v2_vec.z = p3[2];
    v2_vec.w = p3[3];

    FUN_0021624c(param_1->unk54, &v1_vec, &v2_vec);
    FUN_00130b3c(DAT_00340284, DAT_002da1f0);
}
