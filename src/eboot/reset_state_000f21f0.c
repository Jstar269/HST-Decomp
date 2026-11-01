/* HSTMATCH module=EBOOT.BIN start=0x000f21f0 end=0x000f2220 symbol=FUN_000f21f0 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"

struct Obj_000f21f0 {
    char pad0[0x6c];
    u8 field_6c;
    char pad6d[0x74 - 0x6d];
    int field_74;
    char pad78[0x84 - 0x78];
    u8 field_84;
};

extern void FUN_00103628(void *self, u8 a1);

void FUN_000f21f0(struct Obj_000f21f0 *self)
{
    if (self->field_84 != 0) {
        self->field_74 = 0;
        self->field_6c = 1;
        FUN_00103628(self, 1);
    }
}
