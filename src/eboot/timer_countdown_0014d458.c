/* HSTMATCH module=EBOOT.BIN start=0x0014d458 end=0x0014d484 symbol=FUN_0014d458 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"

struct Obj_0014d458 {
    char pad[4];
    float timer;
};

int FUN_0014d458(struct Obj_0014d458 *self, float dt)
{
    float val = self->timer - dt;
    self->timer = val;
    return val <= 0.0f;
}
