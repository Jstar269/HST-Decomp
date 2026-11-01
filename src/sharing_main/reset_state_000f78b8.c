/* HSTMATCH module=hstp_trans_main.prx start=0x000f78b8 end=0x000f78e8 symbol=FUN_000f78b8 lang=c99 */
/* Clone of src/eboot/reset_state_000f21f0.c (hstclone.py tag dsn67): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"

struct Obj_000f78b8 {
    char pad0[0x6c];
    u8 field_6c;
    char pad6d[0x74 - 0x6d];
    int field_74;
    char pad78[0x84 - 0x78];
    u8 field_84;
};

extern void FUN_00108f54(void *self, u8 a1);

void FUN_000f78b8(struct Obj_000f78b8 *self)
{
    if (self->field_84 != 0) {
        self->field_74 = 0;
        self->field_6c = 1;
        FUN_00108f54(self, 1);
    }
}
