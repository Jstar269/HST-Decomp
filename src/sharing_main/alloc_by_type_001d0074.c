/* HSTMATCH module=hstp_trans_main.prx start=0x001d0074 end=0x001d00ac symbol=FUN_001d0074 lang=c99 */
/* Clone of src/eboot/alloc_by_type_001c1e60.c (hstclone.py tag dsn67): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"

extern void *FUN_0020d1e0(int a0, int a1, int a2);

void *FUN_001d0074(int type)
{
    void *res = 0;
    if (type < 0) goto end;
    if (type >= 2) goto end;
    res = FUN_0020d1e0(16, 0, type);
end:
    return res;
}
