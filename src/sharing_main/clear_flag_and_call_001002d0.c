/* HSTMATCH module=hstp_trans_main.prx start=0x001002d0 end=0x001002fc symbol=FUN_001002d0 lang=c99 */
/* Clone of src/eboot/clear_flag_and_call_000fab94.c (hstclone.py tag dsn67): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"

struct Obj_001002d0 {
    char pad0[0x54];
    void *field_54;
    char pad58[0x154 - 0x58];
    u8 flag_154;
};

extern void FUN_000fa3e4(void *arg);

void FUN_001002d0(struct Obj_001002d0 *self)
{
    if (self->flag_154 != 0) {
        self->flag_154 = 0;
        FUN_000fa3e4(self->field_54);
    }
}
