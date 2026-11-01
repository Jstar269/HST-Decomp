/* HSTMATCH module=EBOOT.BIN start=0x0012d76c end=0x0012d7a8 symbol=FUN_0012d76c lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

extern unsigned char *DAT_0034a710;
extern int DAT_0033b578;

int FUN_0012d76c(void)
{
    int b = DAT_0034a710 != 0;
    if (b) {
        b = DAT_0034a710[100] != 0;
    }
    if (!b) {
        b = DAT_0033b578 == 1;
    }
    return b;
}
