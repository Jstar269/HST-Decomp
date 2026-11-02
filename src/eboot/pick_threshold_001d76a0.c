/* HSTMATCH module=EBOOT.BIN start=0x001d76a0 end=0x001d76d8 symbol=FUN_001d76a0 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"

u32 FUN_001d76a0(u32 *pa, u32 *pb, u32 *pc)
{
    if (*pa < *pb) {
        return *pb;
    }
    if (*pc < *pa) {
        return *pc;
    }
    return *pa;
}
