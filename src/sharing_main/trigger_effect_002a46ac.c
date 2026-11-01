/* HSTMATCH module=hstp_trans_main.prx start=0x002a46ac end=0x002a46d4 symbol=FUN_002a46ac lang=c99 */
/* Clone of src/eboot/trigger_effect_002930a0.c (hstclone.py tag dsn67): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"

struct Obj_002a46ac {
    char pad0[0x4d];
    u8 flag_4d;
    char pad4e[0x50 - 0x4e];
    float float_50;
};

extern void FUN_002a334c(float f);

void FUN_002a46ac(struct Obj_002a46ac *self)
{
    if (self->flag_4d != 0) {
        FUN_002a334c(self->float_50);
    }
}
