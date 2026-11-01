/* HSTMATCH module=EBOOT.BIN start=0x002b2d78 end=0x002b2da4 symbol=FUN_002b2d78 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"

struct Obj_002b2d78 {
    char pad0[0x94];
    int idx;
    char pad98[0x3d0 - 0x98];
    int arr[1];
};

extern void FUN_002b2628(void *a0, int a1, int a2, int a3);

void FUN_002b2d78(struct Obj_002b2d78 *self, int a1, int a2)
{
    int val = self->arr[self->idx];
    FUN_002b2628(self, a2, a1 + (val - 1) * 2 - 1, a1);
}
