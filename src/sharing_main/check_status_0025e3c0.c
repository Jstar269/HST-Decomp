/* HSTMATCH module=hstp_trans_main.prx start=0x0025e3c0 end=0x0025e3f8 symbol=FUN_0025e3c0 */
/* Clone of src/eboot/check_status_0024e188.c (hstclone.py tag dsn67): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

extern unsigned char DAT_003a490c;
extern int DAT_003f3ab0;
extern int DAT_003a4908;

int FUN_0025e468(int a0, unsigned char a1, int a2, int a3);

int FUN_0025e3c0(int param_1)
{
    return FUN_0025e468(param_1, DAT_003a490c, DAT_003f3ab0, DAT_003a4908) >= 0;
}
