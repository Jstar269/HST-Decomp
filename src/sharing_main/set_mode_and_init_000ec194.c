/* HSTMATCH module=hstp_trans_main.prx start=0x000ec194 end=0x000ec1cc symbol=FUN_000ec194 lang=c99 */
/* Clone of src/eboot/set_mode_and_init_000e7198.c (hstclone.py tag dsn67): addresses substituted; exact match verified; names and types await human review. */
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

struct Obj_000ec194 {
    char pad0[0xd0];
    volatile u8 field_d0;
    char pad_d1[0xe4 - 0xd1];
    float field_e4;
    u8 field_e8;
    char pad_e9[0xec - 0xe9];
    float field_ec;
};

extern struct Manager *DAT_003e19f0;

void FUN_000ec194(struct Obj_000ec194 *self, u8 val)
{
    self->field_d0 = val;
    if (self->field_d0 != 0) {
        self->field_e4 = DAT_003e19f0->inner->val_2f8;
        self->field_e8 = 0;
        self->field_ec = 1.0f;
    }
}
