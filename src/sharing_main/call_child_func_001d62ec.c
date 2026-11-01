/* HSTMATCH module=hstp_trans_main.prx start=0x001d62ec end=0x001d6314 symbol=FUN_001d62ec lang=c99 */
/* Clone of src/eboot/call_child_func_001c7e58.c (hstclone.py tag dsn67): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"

struct Obj_001d62ec {
    char pad[0x174];
    void *child;
};

extern void FUN_0028e450(void *child);

void FUN_001d62ec(struct Obj_001d62ec *self)
{
    if (self->child != 0) {
        FUN_0028e450(self->child);
    }
}
