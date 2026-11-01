/* HSTMATCH module=EBOOT.BIN start=0x000e7198 end=0x000e71d0 symbol=FUN_000e7198 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"

struct Inner {
    char pad0[0x2f8];
    float val_2f8;
};

struct Manager {
    char pad0[0x88];
    struct Inner *inner;
};

struct Obj_000e7198 {
    char pad0[0xd0];
    volatile u8 field_d0;
    char pad_d1[0xe4 - 0xd1];
    float field_e4;
    u8 field_e8;
    char pad_e9[0xec - 0xe9];
    float field_ec;
};

extern struct Manager *DAT_003386b0;

void FUN_000e7198(struct Obj_000e7198 *self, u8 val)
{
    self->field_d0 = val;
    if (self->field_d0 != 0) {
        self->field_e4 = DAT_003386b0->inner->val_2f8;
        self->field_e8 = 0;
        self->field_ec = 1.0f;
    }
}
