/* HSTMATCH module=EBOOT.BIN start=0x0016b4c0 end=0x0016b4f0 symbol=FUN_0016b4c0 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"

extern u8 *DAT_00341ccc;

u8 *FUN_0016b4c0(int idx)
{
    u8 *base = DAT_00341ccc;
    if (idx >= 0 && idx < 0x15) {
        return base + idx + 0xfcc;
    }
    return 0;
}
