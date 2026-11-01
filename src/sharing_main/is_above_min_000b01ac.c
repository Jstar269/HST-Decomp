/* HSTMATCH module=hstp_trans_main.prx start=0x000b01ac end=0x000b01d8 symbol=FUN_000b01ac lang=c99 */
/* Clone of src/eboot/is_above_min_000ad674.c (hstclone.py tag dsn67): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"

struct Obj_000b01ac {
    char pad[0x588];
    float val;
};

bool FUN_000b01ac(struct Obj_000b01ac *self)
{
    return self->val >= -6.4007998f;
}
