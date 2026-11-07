/* HSTMATCH module=hstp_trans_main.prx start=0x0012da5c end=0x0012dc54 symbol=FUN_0012da5c lang=c99 */
/* Hand-written reconstruction: clone of the source of state_ctor_00122888.c in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly (near miss wip/state_ctor_00122888.c, completed: third argument of FUN_0012fdcc is the state pointer; matching idioms: #pragma opt_strength_reduction off, explicit byte-offset loop variable `off` next to the index `i`, and a char * cast on the first slot store so its address is not CSE'd with the later ones); exact match verified; names and types await human review. */
#include "hst/actor_state.h"

extern void FUN_0001ec44(void);
extern int FUN_00000bcc(int);
extern int FUN_00130f6c(int);
extern int FUN_00225178(int);
extern void FUN_0012fdcc(ActorState *, int, StateRow *);
extern void FUN_0013d6dc(int, void *);
extern int FUN_001304d0(ActorState *, int);
extern void FUN_0012fd6c(float, ActorState *, StateRow *, StateRow *);
extern int DAT_003b12f4;
extern StateRow DAT_003e48c4[];

#pragma opt_strength_reduction off
/* Constructor: creates the three child objects and the helper object, then enters the first state. */
ActorState *FUN_0012da5c(ActorState *param_1)
{
    ActorState *self;
    int i;
    int off;
    int v;
    ActorChild *child;
    int other;
    float f;
    int iv;
    int tmp;
    StateRow *cur;

    self = param_1;
    FUN_0001ec44();
    self->vtable = (const void *)&DAT_003b12f4;
    self->unk54 = 0;
    i = 0;
    off = 0;
    do {
        *(int *)((char *)self + off + 0x3c) = 0;
        v = FUN_00000bcc(0xa0);
        if (v != 0)
            v = FUN_00130f6c(v);
        *(int *)((int)self + off + 0x3c) = v;
        *(int *)((int)self + off + 0x48) = 0;
        child = *(ActorChild **)((int)self + off + 0x3c);
        child->slot = (StateRow **)((int)self + off + 0x48);
        child = *(ActorChild **)((int)self + off + 0x3c);
        child->index = i;
        i = i + 1;
        off = off + 4;
    } while (i < ACTOR_CHILD_COUNT);
    v = FUN_00000bcc(0x50);
    if (v != 0)
        v = FUN_00225178(v);
    self->unk54 = v;
    self->active = 0;
    self->unk79 = 0;
    self->unk7a = 1;
    self->unk7b = 0;
    self->unk7c = 0;
    self->unk80 = -1;
    self->unk84 = -1.0f;
    self->unk88 = 0.0f;
    self->unk8c = 0;
    self->unk90 = 0;
    self->unk94 = 0;
    self->unk95 = 0;
    self->unk98 = 0;
    self->unk9c = 0;
    self->stateIdx = 0;
    self->unk5c = 0;
    self->unk60 = 0.0f;
    self->unk64 = 0.0f;
    self->unk68 = 0.0f;
    self->unk6c = -1.0f;
    self->unk70 = 0.0f;
    self->unk74 = 0;
    if (DAT_003e48c4[self->stateIdx].unk4e == 0) {
        cur = &DAT_003e48c4[self->stateIdx];
        self->slotA = cur;
        FUN_0012fdcc(self, 0, cur);
    } else {
        f = DAT_003e48c4[self->stateIdx].unk1c;
        iv = (int)f;
        FUN_0013d6dc(DAT_003e48c4[self->stateIdx].unk46, &tmp);
        other = FUN_001304d0(self, tmp);
        FUN_0012fd6c((float)iv, self, &DAT_003e48c4[self->stateIdx], &DAT_003e48c4[other]);
    }
    return self;
}
