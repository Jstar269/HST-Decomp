/* HSTMATCH module=hstp_trans_main.prx start=0x0004f864 end=0x0004f984 symbol=FUN_0004f864 lang=c99 rodata=0x002cc200 helpers=FUN_0004efe0@0x0004efe0-0x0004f00c */
/* Clone of src/eboot/log_code_name_0004f010.c (hstclone.py tag c085007): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; same-file static log stub FUN_0004efe0 placed by helpers=; exact match verified; names and types await human review. */
extern char DAT_00376548[];
extern char DAT_00376554[];
extern char DAT_00376560[];
extern char DAT_0037657c[];
extern char DAT_00376598[];
extern char DAT_003765ac[];
extern char DAT_003765d0[];
extern char DAT_003765e8[];
extern char DAT_0037660c[];
extern char DAT_00376630[];
extern char DAT_00376658[];
extern char DAT_00376674[];
extern char DAT_0037668c[];
extern char DAT_003766a4[];
extern char DAT_003766bc[];
extern char DAT_003766e4[];

static void FUN_0004efe0(...)
{
}

void FUN_0004f864(int param_1, int param_2, const char *param_3, int param_4, const char *param_5)
{
    const char *str = 0;
    switch (param_2) {
    case 0:  str = DAT_00376548; break;
    case 1:  str = DAT_00376554; break;
    case 2:  str = DAT_00376560; break;
    case 3:  str = DAT_0037657c; break;
    case 4:  str = DAT_00376598; break;
    case 5:  str = DAT_003765ac; break;
    case 6:  str = DAT_003765d0; break;
    case 7:  str = DAT_003765e8; break;
    case 8:  str = DAT_0037660c; break;
    case 9:  str = DAT_00376630; break;
    case 10: str = DAT_00376658; break;
    case 11: str = DAT_00376674; break;
    case 12: str = DAT_0037668c; break;
    case 13: str = DAT_003766a4; break;
    }

    if (str != 0) {
        FUN_0004efe0(DAT_003766bc, param_3, param_4, str, param_5);
    } else {
        FUN_0004efe0(DAT_003766e4, param_3, param_4, param_2, param_5);
    }
}

