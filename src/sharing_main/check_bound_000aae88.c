/* HSTMATCH module=hstp_trans_main.prx start=0x000aae88 end=0x000aaeb4 symbol=FUN_000aae88 lang=c99 */
/* Clone of src/eboot/check_bound_000a8494.c (hstclone.py tag dsn67): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"

struct Target {
    char pad[8];
    float val;
};

int FUN_000aae88(void *unused, struct Target *target)
{
    return target->val >= -6.4007998f;
}
