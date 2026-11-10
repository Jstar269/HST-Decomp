/* HSTMATCH module=EBOOT.BIN start=0x0004f010 end=0x0004f130 symbol=FUN_0004f010 lang=c99 rodata=0x002bc3b0 helpers=FUN_0004ea98@0x0004ea98-0x0004eac4 */
/* Hand-written reconstruction from the disassembly; same-file static log stub FUN_0004ea98 placed by helpers=; exact match verified; names and types await human review. */
extern char DAT_002d0a8c[];
extern char DAT_002d0a98[];
extern char DAT_002d0aa4[];
extern char DAT_002d0ac0[];
extern char DAT_002d0adc[];
extern char DAT_002d0af0[];
extern char DAT_002d0b14[];
extern char DAT_002d0b2c[];
extern char DAT_002d0b50[];
extern char DAT_002d0b74[];
extern char DAT_002d0b9c[];
extern char DAT_002d0bb8[];
extern char DAT_002d0bd0[];
extern char DAT_002d0be8[];
extern char DAT_002d0c00[];
extern char DAT_002d0c28[];

static void FUN_0004ea98(...)
{
}

void FUN_0004f010(int param_1, int param_2, const char *param_3, int param_4, const char *param_5)
{
    const char *str = 0;
    switch (param_2) {
    case 0:  str = DAT_002d0a8c; break;
    case 1:  str = DAT_002d0a98; break;
    case 2:  str = DAT_002d0aa4; break;
    case 3:  str = DAT_002d0ac0; break;
    case 4:  str = DAT_002d0adc; break;
    case 5:  str = DAT_002d0af0; break;
    case 6:  str = DAT_002d0b14; break;
    case 7:  str = DAT_002d0b2c; break;
    case 8:  str = DAT_002d0b50; break;
    case 9:  str = DAT_002d0b74; break;
    case 10: str = DAT_002d0b9c; break;
    case 11: str = DAT_002d0bb8; break;
    case 12: str = DAT_002d0bd0; break;
    case 13: str = DAT_002d0be8; break;
    }

    if (str != 0) {
        FUN_0004ea98(DAT_002d0c00, param_3, param_4, str, param_5);
    } else {
        FUN_0004ea98(DAT_002d0c28, param_3, param_4, param_2, param_5);
    }
}

