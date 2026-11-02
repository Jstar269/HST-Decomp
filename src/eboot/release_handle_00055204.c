/* HSTMATCH module=EBOOT.BIN start=0x00055204 end=0x00055240 symbol=FUN_00055204 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"

struct Obj_00055204 {
    char pad[0x8c];
    void *handle;
    u8 active;
};

extern void FUN_00053718(void *handle, int flag);

void FUN_00055204(struct Obj_00055204 *self)
{
    if (self->active != 0) {
        FUN_00053718(self->handle, 1);
        self->active = 0;
    }
    self->handle = 0;
}
