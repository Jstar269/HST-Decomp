/* HSTMATCH module=EBOOT.BIN start=0x0009ef00 end=0x0009f088 symbol=FUN_0009ef00 lang=c99 */
/* Hand-written reconstruction: clone of the source of replay_state_000a14ac.c in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern int FUN_000dbc54();
extern unsigned char FUN_0024de88();
extern void *DAT_00338b44;
extern void *DAT_0034a77c;
extern unsigned char DAT_00341837[];
int FUN_0009ef00(int *p, char *q)
{
    int r = -1;
    int c1, c2;
    if ((((int)q[0x8b] << 0x1c) >> 0x1e) == 0) {
        r = 0;
    } else {
        if ((((int)q[0x8b] << 0x1a) >> 0x1e) == 0) {
            c1 = !(FUN_000dbc54(DAT_00338b44, 1) != 0 || FUN_000dbc54(DAT_00338b44, 3) != 0);
            if (c1) { r = 1; goto done; }
        }
        if ((((int)q[0x8b] << 0x1e) >> 0x1e) == 0) {
            c2 = !(FUN_000dbc54(DAT_00338b44, 2) != 0 || FUN_000dbc54(DAT_00338b44, 3) != 0);
            if (c2) {
                unsigned char u;
                if (FUN_000dbc54(DAT_00338b44, 0x11) || FUN_000dbc54(DAT_00338b44, 0x12) || FUN_000dbc54(DAT_00338b44, 0x13))
                    u = 1;
                else
                    u = FUN_0024de88(DAT_0034a77c, 8, DAT_00341837[p[0x52] * 0x120]);
                if (u) r = 2;
            }
        }
    }
done:
    return r;
}
