/* HSTMATCH module=hstp_trans_main.prx start=0x00207a0c end=0x00207a48 symbol=FUN_00207a0c lang=c99 */
/* Clone of src/eboot/FUN_001f9354.c (hstclone.py tag dsn45): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct S { char pad[64]; int f40; char pad2[1384 - 68]; unsigned char f568; };

int FUN_00207a0c(struct S *s)
{
    int m = s->f40;
    int r = 0;
    switch (m) {
    case 5:
    case 6:
        if (s->f568 != 0) r = 1;
        break;
    }
    return r;
}
