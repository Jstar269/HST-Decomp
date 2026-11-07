/* HSTMATCH module=EBOOT.BIN start=0x002a41ac end=0x002a41cc symbol=FUN_002a41ac lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Cfg { char pad[0x8b]; signed char lo : 4; signed char mode : 2; };
extern struct Cfg *DAT_002cfc8c;
int FUN_002a41ac(void) { return (DAT_002cfc8c->mode != 0) ^ 1; }
