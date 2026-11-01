/* HSTMATCH module=hstp_trans_main.prx start=0x00130984 end=0x00130af0 symbol=FUN_00130984 lang=c99 */
/* Clone of src/eboot/FUN_00125750.c (hstclone.py tag dsn45): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction (lane dsn4); exact match verified; names and types await human review. */
#include "hst/types.h"
#include "hst/actor_state.h"

typedef struct Vec4 {
    float x, y, z, w;
} Vec4;

extern char DAT_003e4911;
extern int DAT_003e19f0;
extern int DAT_003e95c4;
extern float DAT_00381218;
extern int DAT_0038121c;

int FUN_0013d030(int, int);
float FUN_00022500(Vec4 *, Vec4 *);
float FUN_00022e6c(float);
void FUN_00225250(int, Vec4 *, Vec4 *);
void FUN_0013c0f4(int, int);

void FUN_00130984(ActorState *param_1, unsigned char *param_2)
{
    Vec4 v1_vec;
    Vec4 v2_vec;
    Vec4 v0_vec;

    int id = param_1->stateIdx;
    char mode = (&DAT_003e4911)[id * 0x58];
    if (mode == 0) {
        return;
    }

    int v1 = *(int *)(DAT_003e19f0 + 0x9c);
    int s2 = *(int *)(v1 + 0x58);
    int v0 = FUN_0013d030(DAT_003e95c4, mode);
    if ((float)s2 != (float)v0) {
        return;
    }

    float *p = (float *)(*(int *)(DAT_003e19f0 + 0x88) + 0x20);
    v0_vec.x = p[0];
    v0_vec.y = p[1];
    v0_vec.z = p[2];
    v0_vec.w = p[3];

    float len_sq = FUN_00022500(&v0_vec, &v0_vec);
    float speed = FUN_00022e6c(len_sq);
    if (!(speed * 3.6f >= DAT_00381218)) {
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

    FUN_00225250(param_1->unk54, &v1_vec, &v2_vec);
    FUN_0013c0f4(DAT_003e95c4, DAT_0038121c);
}
