/* HSTMATCH module=hstp_trans_main.prx start=0x002be938 end=0x002be948 symbol=FUN_002be938 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Big { char pad[0x1414]; float value; };
extern struct Big *DAT_003ea8e8;
float FUN_002be938(void) { return DAT_003ea8e8->value; }
