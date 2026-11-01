/* HSTMATCH module=EBOOT.BIN start=0x00130b10 end=0x00130b3c symbol=FUN_00130b10 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"

extern int DAT_002db1a0;

float FUN_00130b10(int *self)
{
    int *p;
    if (self[0x48] < DAT_002db1a0)
        p = &DAT_002db1a0;
    else
        p = &self[0x48];
    return (float)*p;
}
