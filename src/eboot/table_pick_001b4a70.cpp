/* HSTMATCH module=EBOOT.BIN start=0x001b4a70 end=0x001b4b64 symbol=FUN_001b4a70 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern char DAT_002e7700[];
struct Owner { char pad[0x768]; int slots[8]; };
struct Rec { int kind; unsigned char cnt[4]; int items[1]; };
extern "C" {
extern int DAT_0034178c;
extern unsigned char *DAT_00338b44;
int FUN_00062b64(int, int, int);

int FUN_001b4a70(int *param_1, int param_2, int param_3)
{
    Owner *base = *(Owner **)param_1;
    Rec *t = (Rec *)(DAT_002e7700 + param_3 * 0x1c);
    int flag = (unsigned char)!DAT_00338b44[0x45];
    int sel = *(unsigned char *)(param_2 + (int)base + 0x798) != 0 ? 2 : 0;
    int k = sel + flag;
    int n = t->cnt[k];

    if (n <= 0)
        return -1;
    int kind = t->kind;
    DAT_0034178c = DAT_0034178c * 0x343fd + 0x269ec3;
    int r = (int)(((unsigned)DAT_0034178c >> 16) & 0x7fff) % n;
    if (kind == 0xc3) {
        int p = base->slots[param_2];
        if (p != 0)
            return FUN_00062b64(p, t->items[r], 0);
    }
    return -1;
}
}
