/* HSTMATCH module=EBOOT.BIN start=0x0016b55c end=0x0016b58c symbol=FUN_0016b55c lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"

extern u8 *DAT_00341ccc;

u8 *FUN_0016b55c(int idx)
{
    u8 *base = DAT_00341ccc;
    if (idx >= 0 && idx < 8) {
        return base + idx + 0xfe1;
    }
    return 0;
}
