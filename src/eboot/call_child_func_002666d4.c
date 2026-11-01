/* HSTMATCH module=EBOOT.BIN start=0x002666d4 end=0x002666fc symbol=FUN_002666d4 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"

struct Obj_002666d4 {
    char pad[0x70];
    void *child;
};

extern void FUN_0005f110(void *child);

void FUN_002666d4(struct Obj_002666d4 *self)
{
    if (self->child != 0) {
        FUN_0005f110(self->child);
    }
}
