/* HSTMATCH module=EBOOT.BIN start=0x00289cd4 end=0x00289cf4 symbol=FUN_00289cd4 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"

struct Vec3_00289cd4 {
    union {
        int v[3];
        struct {
            int x, y, z;
        };
    };
};

struct Vec3_00289cd4 *FUN_00289cd4(struct Vec3_00289cd4 *self)
{
    self->x = 0;
    self->y = 0;
    self->z = 0;
    self->y = self->v[1] - self->y;
    return self;
}
