/* HSTMATCH module=EBOOT.BIN start=0x0029398c end=0x293a24 symbol=FUN_0029398c lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

int FUN_0029398c(int param_1)
{
    int v1 = *(int *)(param_1 + 0x78);
    int v0;
    int a1;
    int a0;
    float f1;
    float f2;
    float f0;
    int at;

    if (v1 == 0) {
        return 1;
    }
    a1 = *(int *)(v1 + 0x158);
    v0 = *(int *)(v1 + 0x15c);
    if (v0 < a1) {
        return v0 & 0xff;
    }
    a0 = v1 + a1 * 52;
    f1 = 0.0f;
    for (;;) {
        v0 = *(int *)(a0 + 0xf4);
        f2 = f1;
        if (v0 != 0) {
            v0 = *(int *)(v0);
            f2 = *(float *)(v0 + 8);
        }
        f0 = *(float *)(a0 + 0x104);
        if (f0 < f2) {
            return 0;
        }
        a1++;
        at = *(int *)(v1 + 0x15c) < a1;
        a0 += 52;
        if (at == 0) {
            continue;
        }
        return 1;
    }
}
// near: 
// near miss from lane oc-hst-20261121 (unreviewed)
