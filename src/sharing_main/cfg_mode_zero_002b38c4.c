/* HSTMATCH module=hstp_trans_main.prx start=0x002b38c4 end=0x002b38e4 symbol=FUN_002b38c4 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Cfg { char pad[0x8b]; signed char lo : 4; signed char mode : 2; };
extern struct Cfg *DAT_003751d4;
int FUN_002b38c4(void) { return (DAT_003751d4->mode != 0) ^ 1; }
