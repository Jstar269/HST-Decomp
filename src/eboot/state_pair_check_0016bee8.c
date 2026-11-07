/* HSTMATCH module=EBOOT.BIN start=0x0016bee8 end=0x0016bf78 symbol=FUN_0016bee8 lang=c99 */
/* Hand-written reconstruction: clone of the source of state_pair_check_00178160.c in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern int DAT_0034a678;
extern int DAT_0034a67c;

int FUN_0016bee8(void)
{
    int r;

    if (DAT_0034a678 == 3) {
        r = 1;
    } else if (DAT_0034a678 == 0x55) {
        if (DAT_0034a67c == 0x54 || DAT_0034a67c == 0x1d) {
            r = 1;
        } else {
            r = 0;
        }
    } else if (DAT_0034a678 == 0x24) {
        r = 0;
    } else {
        if (DAT_0034a67c != 0x30 && DAT_0034a67c != 0x29) {
            r = 1;
        } else {
            r = 0;
        }
    }
    return r;
}
