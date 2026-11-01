/* HSTMATCH module=EBOOT.BIN start=0x002334e0 end=0x00233518 symbol=FUN_002334e0 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"

extern int DAT_002f3668;
extern const int DAT_002f3688;

extern void FUN_0023302c(void *arg, int a1);
extern void FUN_0022c8d8(int a0, int a1);

void FUN_002334e0(void *arg)
{
    DAT_002f3668 = DAT_002f3688;
    FUN_0023302c(arg, 2);
    FUN_0022c8d8(7, 1);
}
