/* HSTMATCH module=hstp_trans_main.prx start=0x001e35b4 end=0x001e35d4 symbol=FUN_001e35b4 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Cfg { char pad[0x8b]; signed char lo : 4; signed char mode : 2; };
extern struct Cfg *DAT_003751d4;
int FUN_001e35b4(void) { return (DAT_003751d4->mode != 0) ^ 1; }
