/* HSTMATCH module=EBOOT.BIN start=0x000bd8f8 end=0x000bd924 symbol=FUN_000bd8f8 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"

struct Obj_000bd8f8 {
    char pad[0xc8];
    int values[1];
};

int FUN_000bd8f8(struct Obj_000bd8f8 *self, int idx)
{
    int val = self->values[idx];
    return val == 3 || val == 10;
}
