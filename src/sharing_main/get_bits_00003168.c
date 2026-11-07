/* HSTMATCH module=hstp_trans_main.prx start=0x00003168 end=0x00003190 symbol=FUN_00003168 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"
struct Bits_00003168 { u8 flag : 1; u8 val : 7; };
extern struct Bits_00003168 *FUN_000030a8(struct Bits_00003168 *p);
int FUN_00003168(struct Bits_00003168 *p) { return FUN_000030a8(p)->val; }
