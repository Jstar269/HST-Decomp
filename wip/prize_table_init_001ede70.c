/* HSTMATCH module=EBOOT.BIN start=0x001ede70 end=0x001ee0f8 symbol=FUN_001ede70 lang=c99 */
/* Probe. */

extern char DAT_002c8e04[];
extern char DAT_002ee6f4[];
extern char DAT_002ee718[];
extern char DAT_002ee720[];
extern char DAT_002ee728[];
extern char DAT_002ee744[];
extern char DAT_0034179c[];
extern char DAT_0030ebf4[];
extern int DAT_003386b0;

extern void FUN_001ee0f8(void);
extern int FUN_001b3544(int);
extern void FUN_0001ca58(char *, char *, int);
extern void FUN_000143b0(char *, char *, int, int, char *);
extern int FUN_00000bcc(int);
extern int FUN_00055df8(int, char *, int);
extern int FUN_0005d638(int, char *, int, char *, int, int, int, int);
extern int FUN_000492e4(int, int);
extern void FUN_000493fc(int, int, char *);
extern void FUN_001b574c(int, int, int, int);
extern void FUN_0001cb88(char *);

void FUN_001ede70(int *param_1, int param_2, int param_3, int param_4)
{
    char buf[256];
    unsigned int tag;
    int i;
    char **tab;
    int *out;
    int v;
    int u;
    int saved;
    char *p;

    FUN_001ee0f8();
    tag = (unsigned char)FUN_001b3544((int)DAT_0034179c);
    FUN_0001ca58(DAT_0030ebf4, DAT_002ee6f4, param_3);
    i = 0;
    saved = *(int *)(DAT_003386b0 + 0xa8);
    tab = (char **)DAT_002c8e04;
    out = param_1;
    do {
        FUN_000143b0(buf, *tab, param_3, param_3, DAT_002ee718);
        u = 0;
        v = FUN_00000bcc(0x10);
        if (v != 0) {
            u = FUN_00055df8(v, buf, 0);
        }
        *out = u;
        FUN_000143b0(buf, *tab, param_3, param_3, DAT_002ee720);
        u = 0;
        v = FUN_00000bcc(0x14);
        if (v != 0) {
            u = FUN_0005d638(v, buf, param_4, DAT_002ee728, 0, 0, 0, 0);
        }
        out[1] = u;
        FUN_000143b0(buf, *tab, param_3, param_3, DAT_002ee744);
        out[2] = 0;
        v = FUN_00000bcc(0x1f0);
        if (v != 0) {
            out[2] = FUN_000492e4(v, 1);
        }
        FUN_000493fc(out[2], 0, buf);
        i++;
        tab++;
        out += 3;
    } while (i < 10);
    FUN_001b574c(saved, param_2, param_3, (int)tag);
    FUN_0001cb88(DAT_0030ebf4);
    FUN_0001ca58(DAT_0030ebf4, DAT_002ee6f4, 99);
    i = 0;
    tab = (char **)DAT_002c8e04;
    for (;;) {
        if (i == 4) {
            p = *tab;
        } else if (i == 3 || i == 2) {
            p = *tab;
        } else {
            goto next;
        }
        FUN_000143b0(buf, p, 99, 99, DAT_002ee744);
        v = FUN_00000bcc(0x1f0);
        if (v == 0) {
            param_1[0x20] = 0;
            u = 0;
        } else {
            u = FUN_000492e4(v, 1);
            param_1[0x20] = u;
        }
        FUN_000493fc(u, 0, buf);
    next:
        i++;
        tab++;
        param_1 += 3;
        if (i > 9) {
            FUN_0001cb88(DAT_0030ebf4);
            return;
        }
    }
}

/* Residual (167/162 words, 129 differ with the `p = *tab` reload form; the last edit in the probe
   above -- positive-form null tests with a shared `u` local -- grew the frame to 320 and is worse):
   FUN_001ede70 builds ten prize entries: each iteration formats four paths into a 256-byte stack
   buffer and allocates three objects (0x10 / 0x14 / 0x1f0), then a second pass over the same
   ten-table re-loads only entries 2, 3 and 4 as the "cam" object at out[0x20].
   Two things are already right: passing 0x30ebf4 as `extern char DAT_0030ebf4[]` (address, not a
   value load) reproduces retail's `lui a0,0x31; addiu a0,a0,-5132` at each call, and the
   FUN_000143b0 call needs its string argument re-read as `*tab` inline (never a local).
   Left: (a) retail's prologue copies the parameters in the order a1,a2,a3,a0 (s6,s4,s3,s5) while
   MWCC copies a0 first, and (b) each alloc site is retail's `beqzl v0, <null arm>` with the
   non-null call falling through and `move v0,zero` on the null side -- i.e. `u = 0; v = ALLOC();
   if (v != 0) u = CTOR(v, ...);` with the store after the join.  The shared-`u` form above is
   that shape but spills `u`; next attempt should keep the three result stores separate
   (`*out`, `out[1]`, `out[2]`) and let each arm write its own store. */
// near miss from lane dsn8 (unreviewed)
