/* HSTMATCH module=EBOOT.BIN start=0x002af0b4 end=0x002af0c4 symbol=FUN_002af0b4 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Big { char pad[0x1414]; float value; };
extern struct Big *DAT_003415a8;
float FUN_002af0b4(void) { return DAT_003415a8->value; }
