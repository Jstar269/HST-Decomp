/* HSTMATCH module=EBOOT.BIN start=0x001fe984 end=0x001fe9b4 symbol=FUN_001fe984 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"

int FUN_001fe984(int *param_1, int param_2, u8 param_3)
{
    if (*param_1 == param_2 && *(u8 *)((char *)param_1 + 7) == param_3) {
        return 1;
    }
    return 0;
}
