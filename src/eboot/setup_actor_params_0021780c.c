/* HSTMATCH module=EBOOT.BIN start=0x0021780c end=0x00217834 symbol=FUN_0021780c lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"

struct Obj_0021780c {
    char pad0[0x3c];
    u8 field_3c;
    u8 field_3d;
    u8 field_3e;
    char pad3f;
    int field_40;
    int field_44;
    float field_48;
    float field_4c;
    float field_50;
};

void FUN_0021780c(struct Obj_0021780c *self, int a1, float f1, float f2)
{
    self->field_3c = 1;
    self->field_3d = 0;
    self->field_3e = 0;
    self->field_40 = a1;
    self->field_48 = f1;
    self->field_4c = f2;
    self->field_44 = 0;
    self->field_50 = f1;
}
