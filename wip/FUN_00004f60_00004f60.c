/* HSTMATCH module=EBOOT.BIN start=0x00004f60 end=0x00004fcc symbol=FUN_00004f60 lang=c99 rodata=0x002b96b0 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern unsigned int DAT_0030a11c;
extern unsigned int DAT_0030a120;
extern unsigned int FUN_002b7a40(unsigned int);
extern unsigned int FUN_002b7a18(unsigned int);

unsigned int FUN_00004f60(unsigned int param_1, unsigned int param_2)
{
	if (param_1 >= 5u)
		return 0;
	switch (param_1) {
	case 0:
		return FUN_002b7a40(param_2);
	case 3:
		return FUN_002b7a18(DAT_0030a11c);
	case 4:
		return FUN_002b7a18(DAT_0030a120);
	default:
		return 0;
	}
}
/* NEAR 27 words. Retail uses a dense jump table (lui a3,0x2c; addiu v0,a3,-26960; lw v1,0(a0); jr v1) over 0..4 with a separate sltiu v1,a0,5; beqz bounds check. mwccpsp emits a beql comparison chain for this switch regardless of spelling (tried sparse, dense all-5, no-default), so the table layout and jr-v1 indirect tail call cannot be reproduced. Globals DAT_0030a11c/DAT_0030a120 appended to symbols/EBOOT.BIN.txt. */
// near miss from lane dsn15 (unreviewed)
