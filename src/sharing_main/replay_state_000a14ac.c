/* HSTMATCH module=hstp_trans_main.prx start=0x000a14ac end=0x000a1634 symbol=FUN_000a14ac lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern int FUN_000dfc74();
extern unsigned char FUN_0025e0c0();
extern void *DAT_003e1e84;
extern void *DAT_003f3aac;
extern unsigned char DAT_003eab6f[];
int FUN_000a14ac(int *p, char *q)
{
    int r = -1;
    int c1, c2;
    if ((((int)q[0x8b] << 0x1c) >> 0x1e) == 0) {
        r = 0;
    } else {
        if ((((int)q[0x8b] << 0x1a) >> 0x1e) == 0) {
            c1 = !(FUN_000dfc74(DAT_003e1e84, 1) != 0 || FUN_000dfc74(DAT_003e1e84, 3) != 0);
            if (c1) { r = 1; goto done; }
        }
        if ((((int)q[0x8b] << 0x1e) >> 0x1e) == 0) {
            c2 = !(FUN_000dfc74(DAT_003e1e84, 2) != 0 || FUN_000dfc74(DAT_003e1e84, 3) != 0);
            if (c2) {
                unsigned char u;
                if (FUN_000dfc74(DAT_003e1e84, 0x11) || FUN_000dfc74(DAT_003e1e84, 0x12) || FUN_000dfc74(DAT_003e1e84, 0x13))
                    u = 1;
                else
                    u = FUN_0025e0c0(DAT_003f3aac, 8, DAT_003eab6f[p[0x52] * 0x120]);
                if (u) r = 2;
            }
        }
    }
done:
    return r;
}
