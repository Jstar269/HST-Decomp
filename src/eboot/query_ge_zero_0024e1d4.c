/* HSTMATCH module=EBOOT.BIN start=0x0024e1d4 end=0x0024e1f8 symbol=FUN_0024e1d4 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"

extern int FUN_0024e230(void *a, int b, int c, int d);

bool FUN_0024e1d4(void *a, int b, int c)
{
    int r = FUN_0024e230(a, b, c, 4);
    return r >= 0;
}
