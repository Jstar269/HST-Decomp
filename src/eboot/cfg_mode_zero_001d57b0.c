/* HSTMATCH module=EBOOT.BIN start=0x001d57b0 end=0x001d57d0 symbol=FUN_001d57b0 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Cfg { char pad[0x8b]; signed char lo : 4; signed char mode : 2; };
extern struct Cfg *DAT_002cfc8c;
int FUN_001d57b0(void) { return (DAT_002cfc8c->mode != 0) ^ 1; }
