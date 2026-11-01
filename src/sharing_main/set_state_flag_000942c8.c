/* HSTMATCH module=hstp_trans_main.prx start=0x000942c8 end=0x000942f4 symbol=FUN_000942c8 lang=c99 */
/* Clone of src/eboot/set_state_flag_00092630.c (hstclone.py tag dsn67): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"

struct Obj_000942c8 {
    char pad0[0x75];
    u8 field_75;
};

extern void FUN_00093ef0(struct Obj_000942c8 *self);

void FUN_000942c8(struct Obj_000942c8 *self, u8 val)
{
    if (self->field_75 != val) {
        self->field_75 = val;
        FUN_00093ef0(self);
    }
}
