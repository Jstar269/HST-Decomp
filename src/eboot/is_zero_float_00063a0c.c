/* HSTMATCH module=EBOOT.BIN start=0x00063a0c end=0x00063a30 symbol=FUN_00063a0c lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"

struct Obj_00063a0c {
    char pad[0xc];
    float val;
};

int FUN_00063a0c(struct Obj_00063a0c *self)
{
    return self->val == 0.0f;
}
