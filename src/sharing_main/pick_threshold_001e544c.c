/* HSTMATCH module=hstp_trans_main.prx start=0x001e544c end=0x001e5484 symbol=FUN_001e544c lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"
u32 FUN_001e544c(u32 *pa, u32 *pb, u32 *pc) { if (*pa < *pb) { return *pb; } if (*pc < *pa) { return *pc; } return *pa; }
