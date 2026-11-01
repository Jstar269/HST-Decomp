/* HSTMATCH module=hstp_trans_main.prx start=0x000dc138 end=0x000dc174 symbol=FUN_000dc138 lang=c99 */
/* Clone of src/eboot/get_value_000d8334.c (hstclone.py tag dsn67): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"

struct Obj_000dc138 {
    char pad[0xa4];
    float values[5];
};

extern void FUN_000dbff4(struct Obj_000dc138 *self);

float FUN_000dc138(struct Obj_000dc138 *self, int idx)
{
    FUN_000dbff4(self);
    return self->values[idx];
}
