/* HSTMATCH module=EBOOT.BIN start=0x0024e188 end=0x0024e1c0 symbol=FUN_0024e188 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

extern unsigned char DAT_002fbef8;
extern int DAT_0034a780;
extern int DAT_002fbef4;

int FUN_0024e230(int a0, unsigned char a1, int a2, int a3);

int FUN_0024e188(int param_1)
{
    return FUN_0024e230(param_1, DAT_002fbef8, DAT_0034a780, DAT_002fbef4) >= 0;
}
