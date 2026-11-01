/* HSTMATCH module=EBOOT.BIN start=0x000a8494 end=0x000a84c0 symbol=FUN_000a8494 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"

struct Target {
    char pad[8];
    float val;
};

int FUN_000a8494(void *unused, struct Target *target)
{
    return target->val >= -6.4007998f;
}
