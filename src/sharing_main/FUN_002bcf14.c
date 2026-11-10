/* HSTMATCH module=hstp_trans_main.prx start=0x002bcf14 end=0x002bd088 symbol=FUN_002bcf14 lang=c99 */
/* Clone of src/eboot/FUN_002ad758.c (hstclone.py tag twin11100306-hstp_trans_main.prx): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

extern int FUN_0025e29c();
extern unsigned char FUN_002bc51c(char *base, int idx, int id);

extern int DAT_002dc680[40];
extern unsigned char DAT_002dc5b8[40];
extern unsigned int DAT_002dc5e0[40];
extern int DAT_003f48d0;
extern int DAT_003f4830[40];

void FUN_002bcf14(int mask, char *base, int idx)
{
    int cur;
    int i;
    int id;
    unsigned char *e;
    int n;
    int en;

    cur = idx;
    if (idx == 0)
        cur = FUN_0025e29c();
    DAT_003f48d0 = 0;
    for (i = 0; i < 40; i++) {
        id = DAT_002dc680[i];
        if (i >= 0 && i < 40)
            en = (DAT_002dc5b8[i] != 0);
        else
            en = 0;
        if (!en)
            continue;
        if ((mask & DAT_002dc5e0[id]) == 0)
            continue;
        if (id >= 0 && id < 40)
            e = (unsigned char *)base + id * 20 + 0x1058;
        else
            e = 0;
        if ((unsigned)(*e - 4) <= 1)
            continue;
        if (FUN_002bc51c(base, cur, id) == 0)
            continue;
        n = DAT_003f48d0;
        if (n < 39) {
            DAT_003f4830[n] = id;
            DAT_003f48d0 = n + 1;
        }
    }
}
