/* HSTMATCH module=EBOOT.BIN start=0x000fab94 end=0x000fabc0 symbol=FUN_000fab94 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"

struct Obj_000fab94 {
    char pad0[0x54];
    void *field_54;
    char pad58[0x154 - 0x58];
    u8 flag_154;
};

extern void FUN_000f4cf0(void *arg);

void FUN_000fab94(struct Obj_000fab94 *self)
{
    if (self->flag_154 != 0) {
        self->flag_154 = 0;
        FUN_000f4cf0(self->field_54);
    }
}
