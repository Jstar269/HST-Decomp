/* HSTMATCH module=EBOOT.BIN start=0x001c1e60 end=0x001c1e98 symbol=FUN_001c1e60 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"

extern void *FUN_001fea10(int a0, int a1, int a2);

void *FUN_001c1e60(int type)
{
    void *res = 0;
    if (type < 0) goto end;
    if (type >= 2) goto end;
    res = FUN_001fea10(16, 0, type);
end:
    return res;
}
