/* HSTMATCH module=hstp_trans_main.prx start=0x0020b924 end=0x0020b940 symbol=FUN_0020b924 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0x3c]; int value; };
extern struct Obj *DAT_003f3474;
int FUN_0020b924(void) { return DAT_003f3474 ? DAT_003f3474->value : 0; }
