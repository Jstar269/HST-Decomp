/* HSTMATCH module=hstp_trans_main.prx start=0x002775dc end=0x00277604 symbol=FUN_002775dc lang=c99 */
/* Clone of src/eboot/call_child_func_002666d4.c (hstclone.py tag dsn67): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"

struct Obj_002775dc {
    char pad[0x70];
    void *child;
};

extern void FUN_0005ff00(void *child);

void FUN_002775dc(struct Obj_002775dc *self)
{
    if (self->child != 0) {
        FUN_0005ff00(self->child);
    }
}
