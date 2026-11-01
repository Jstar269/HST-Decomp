/* HSTMATCH module=EBOOT.BIN start=0x000dcc6c end=0x000dcca0 symbol=FUN_000dcc6c lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"

struct Obj_000dcc6c {
    char pad0[0x5c];
    u8 field_5c;
    char pad5d[0x61 - 0x5d];
    u8 field_61;
    char pad62[0xac - 0x62];
    int field_ac;
};

void FUN_000dcc6c(struct Obj_000dcc6c *self)
{
    if (self->field_5c == 0 && self->field_61 == 0 && self->field_ac == 0) {
        self->field_61 = 1;
    }
}
