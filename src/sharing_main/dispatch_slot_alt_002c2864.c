/* HSTMATCH module=hstp_trans_main.prx start=0x002c2864 end=0x002c288c symbol=FUN_002c2864 lang=c99 */
/* Clone of src/eboot/dispatch_slot_alt_002b2da4.c (hstclone.py tag dsn67): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"

struct Obj_002c2864 {
    char pad0[0x94];
    int idx;
    char pad98[0x3d0 - 0x98];
    int arr[1];
};

extern void FUN_002c20ac(void *a0, int a1, int a2, int a3);

void FUN_002c2864(struct Obj_002c2864 *self, int a1, int a2)
{
    int val = self->arr[self->idx];
    FUN_002c20ac(self, a2, a1 + val * 2 - 1, a1);
}
