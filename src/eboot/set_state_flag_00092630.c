/* HSTMATCH module=EBOOT.BIN start=0x00092630 end=0x0009265c symbol=FUN_00092630 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"

struct Obj_00092630 {
    char pad0[0x75];
    u8 field_75;
};

extern void FUN_00092258(struct Obj_00092630 *self);

void FUN_00092630(struct Obj_00092630 *self, u8 val)
{
    if (self->field_75 != val) {
        self->field_75 = val;
        FUN_00092258(self);
    }
}
