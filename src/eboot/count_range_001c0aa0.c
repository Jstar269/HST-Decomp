/* HSTMATCH module=EBOOT.BIN start=0x001c0aa0 end=0x001c0adc symbol=FUN_001c0aa0 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"

int FUN_001c0aa0(u8 *base)
{
    int count = 0;
    int i = 0;
    do {
        if ((u32)(base[0xfe9 + i] - 4) < 2) {
            count++;
        }
        i++;
    } while (i < 3);
    return count;
}
