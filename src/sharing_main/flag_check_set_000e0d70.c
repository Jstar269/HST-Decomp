/* HSTMATCH module=hstp_trans_main.prx start=0x000e0d70 end=0x000e0da4 symbol=FUN_000e0d70 lang=c99 */
/* Clone of src/eboot/flag_check_set_000dcc6c.c (hstclone.py tag dsn67): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"

struct Obj_000e0d70 {
    char pad0[0x5c];
    u8 field_5c;
    char pad5d[0x61 - 0x5d];
    u8 field_61;
    char pad62[0xac - 0x62];
    int field_ac;
};

void FUN_000e0d70(struct Obj_000e0d70 *self)
{
    if (self->field_5c == 0 && self->field_61 == 0 && self->field_ac == 0) {
        self->field_61 = 1;
    }
}
