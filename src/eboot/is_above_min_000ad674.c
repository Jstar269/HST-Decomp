/* HSTMATCH module=EBOOT.BIN start=0x000ad674 end=0x000ad6a0 symbol=FUN_000ad674 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"

struct Obj_000ad674 {
    char pad[0x588];
    float val;
};

bool FUN_000ad674(struct Obj_000ad674 *self)
{
    return self->val >= -6.4007998f;
}
