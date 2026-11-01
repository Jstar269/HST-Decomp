/* HSTMATCH module=EBOOT.BIN start=0x001c7e58 end=0x001c7e80 symbol=FUN_001c7e58 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"

struct Obj_001c7e58 {
    char pad[0x174];
    void *child;
};

extern void FUN_0027d0e0(void *child);

void FUN_001c7e58(struct Obj_001c7e58 *self)
{
    if (self->child != 0) {
        FUN_0027d0e0(self->child);
    }
}
