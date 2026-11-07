/* HSTMATCH module=hstp_trans_main.prx start=0x00177684 end=0x001776b4 symbol=FUN_00177684 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"
extern u8 *DAT_003eb004;
u8 *FUN_00177684(int idx)
{
    u8 *base = DAT_003eb004;
    if (idx >= 0 && idx < 0x15) {
        return base + idx + 0xfcc;
    }
    return 0;
}
