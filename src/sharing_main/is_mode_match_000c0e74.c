/* HSTMATCH module=hstp_trans_main.prx start=0x000c0e74 end=0x000c0ea0 symbol=FUN_000c0e74 lang=c99 */
/* Clone of src/eboot/is_mode_match_000bd8f8.c (hstclone.py tag dsn67): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"

struct Obj_000c0e74 {
    char pad[0xc8];
    int values[1];
};

int FUN_000c0e74(struct Obj_000c0e74 *self, int idx)
{
    int val = self->values[idx];
    return val == 3 || val == 10;
}
