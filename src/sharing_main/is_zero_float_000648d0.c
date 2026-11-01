/* HSTMATCH module=hstp_trans_main.prx start=0x000648d0 end=0x000648f4 symbol=FUN_000648d0 lang=c99 */
/* Clone of src/eboot/is_zero_float_00063a0c.c (hstclone.py tag dsn67): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"

struct Obj_000648d0 {
    char pad[0xc];
    float val;
};

int FUN_000648d0(struct Obj_000648d0 *self)
{
    return self->val == 0.0f;
}
