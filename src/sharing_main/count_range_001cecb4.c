/* HSTMATCH module=hstp_trans_main.prx start=0x001cecb4 end=0x001cecf0 symbol=FUN_001cecb4 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"
int FUN_001cecb4(u8 *base) { int count = 0; int i = 0; do { if ((u32)(base[0xfe9 + i] - 4) < 2) { count++; } i++; } while (i < 3); return count; }
