/* HSTMATCH module=hstp_trans_main.prx start=0x00177720 end=0x00177750 symbol=FUN_00177720 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"
extern u8 *DAT_003eb004;
u8 *FUN_00177720(int idx)
{
    u8 *base = DAT_003eb004;
    if (idx >= 0 && idx < 8) {
        return base + idx + 0xfe1;
    }
    return 0;
}
