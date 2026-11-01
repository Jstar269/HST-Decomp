/* HSTMATCH module=EBOOT.BIN start=0x000d8334 end=0x000d8370 symbol=FUN_000d8334 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"

struct Obj_000d8334 {
    char pad[0xa4];
    float values[5];
};

extern void FUN_000d8228(struct Obj_000d8334 *self);

float FUN_000d8334(struct Obj_000d8334 *self, int idx)
{
    FUN_000d8228(self);
    return self->values[idx];
}
