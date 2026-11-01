/* HSTMATCH module=hstp_trans_main.prx start=0x00158f40 end=0x00158f6c symbol=FUN_00158f40 lang=c99 */
/* Clone of src/eboot/timer_countdown_0014d458.c (hstclone.py tag dsn67): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"

struct Obj_00158f40 {
    char pad[4];
    float timer;
};

int FUN_00158f40(struct Obj_00158f40 *self, float dt)
{
    float val = self->timer - dt;
    self->timer = val;
    return val <= 0.0f;
}
