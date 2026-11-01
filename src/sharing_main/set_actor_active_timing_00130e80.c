/* HSTMATCH module=hstp_trans_main.prx start=0x00130e80 end=0x00130f6c symbol=FUN_00130e80 lang=c99 */
/* Clone of src/eboot/set_actor_active_timing_00125c4c.c (hstclone.py tag dsn45): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
/* Updates actor timing/blend value unk60; on active edge (transition from <= 0 to > 0) sets unk90 timing from DAT_003e19f0, configures DAT_003e95c4, restarts state via FUN_00130af0, and enables FUN_0001ef14(self, 1); on inactive edge (transition from > 0 to <= 0) disables via FUN_0001ef14(self, 0). */
#include "hst/types.h"
#include "hst/actor_state.h"

extern void FUN_0001ef14(ActorState *self, int on);
extern void FUN_00130af0(ActorState *self, int a);
extern char *DAT_003e19f0;
extern char *DAT_003e95c4;

void FUN_00130e80(float v, ActorState *self)
{
    self->unk64 = 0;
    float cur = self->unk60;
    if (v == cur)
        return;

    if (cur <= 0.0f && v > 0.0f) {
        self->unk90 = *(int *)(*(int *)(DAT_003e19f0 + 156) + 88);
        *(int *)(DAT_003e95c4 + 296) = 0;
        *(int *)(DAT_003e95c4 + 300) = 1;
        FUN_00130af0(self, self->stateIdx);
        FUN_0001ef14(self, 1);
        self->active = 1;
    } else if (cur > 0.0f && v <= 0.0f) {
        FUN_0001ef14(self, 0);
        self->active = 0;
    }

    self->unk60 = v;
}
