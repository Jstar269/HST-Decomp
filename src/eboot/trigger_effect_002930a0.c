/* HSTMATCH module=EBOOT.BIN start=0x002930a0 end=0x002930c8 symbol=FUN_002930a0 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"

struct Obj_002930a0 {
    char pad0[0x4d];
    u8 flag_4d;
    char pad4e[0x50 - 0x4e];
    float float_50;
};

extern void FUN_00291d40(float f);

void FUN_002930a0(struct Obj_002930a0 *self)
{
    if (self->flag_4d != 0) {
        FUN_00291d40(self->float_50);
    }
}
