/* HSTMATCH module=EBOOT.BIN start=0x00125c4c end=0x00125d38 symbol=FUN_00125c4c lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
/* Updates actor timing/blend value unk60; on active edge (transition from <= 0 to > 0) sets unk90 timing from DAT_003386b0, configures DAT_00340284, restarts state via FUN_001258bc, and enables FUN_0001f1fc(self, 1); on inactive edge (transition from > 0 to <= 0) disables via FUN_0001f1fc(self, 0). */
#include "hst/types.h"
#include "hst/actor_state.h"

extern void FUN_0001f1fc(ActorState *self, int on);
extern void FUN_001258bc(ActorState *self, int a);
extern char *DAT_003386b0;
extern char *DAT_00340284;

void FUN_00125c4c(float v, ActorState *self)
{
    self->unk64 = 0;
    float cur = self->unk60;
    if (v == cur)
        return;

    if (cur <= 0.0f && v > 0.0f) {
        self->unk90 = *(int *)(*(int *)(DAT_003386b0 + 156) + 88);
        *(int *)(DAT_00340284 + 296) = 0;
        *(int *)(DAT_00340284 + 300) = 1;
        FUN_001258bc(self, self->stateIdx);
        FUN_0001f1fc(self, 1);
        self->active = 1;
    } else if (cur > 0.0f && v <= 0.0f) {
        FUN_0001f1fc(self, 0);
        self->active = 0;
    }

    self->unk60 = v;
}
