/* HSTMATCH module=EBOOT.BIN start=0x0028ff38 end=0x00290288 symbol=FUN_0028ff38 lang=c99 helpers=FUN_0028fb44@0x0028fb44-0x0028fb70 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

struct Dev {
    char pad0[0x28];
    unsigned char active;
    unsigned char f29;
    unsigned char busy;
    char pad2b;
    void *buf2c;
    char pad30[0x48 - 0x30];
    int h48;
    char pad4c[0x64 - 0x4c];
    int h64;
    char pad68[0x80 - 0x68];
    int h80;
    char pad84[4];
    int h88;
    char pad8c[0xd4 - 0x8c];
    void *bufs[2];
    char paddc[4];
    int fe0;
    void *bufe4;
    char pade8[0x1ec - 0xe8];
    unsigned char f1ec;
};

extern unsigned char DAT_0034a80c;
extern char DAT_002cabbc[];
extern char DAT_002cabe4[];
extern char DAT_002ff27c[];
extern char DAT_002ff2a8[];
extern char DAT_002ff2d0[];
extern char DAT_002ff300[];
extern char DAT_002ff320[];
extern char DAT_002ff34c[];
extern char DAT_002ff3e0[];
extern char DAT_002ff454[];

static void FUN_0028fb44(...) {}
extern void FUN_000104e0(void *);
extern int FUN_0006165c(void);
extern void FUN_002903cc(void);
extern void FUN_002908cc(struct Dev *);
extern int FUN_002b78d0(int);
extern int FUN_002b7908(int);
extern void FUN_002b7958(int);
extern int FUN_002b7980(int, int);
extern int FUN_002b79a8(int);
extern int FUN_002b7a08(int);
extern int FUN_002b7cc0(int);
extern int FUN_002b7d50(void);
extern int FUN_002b7e90(struct Dev *);
extern int FUN_002b7ec0(struct Dev *);
extern int FUN_002b7ee0(struct Dev *);

int FUN_0028ff38(struct Dev *d)
{
    int e;
    unsigned i;
    if (d->active == 0)
        return 1;
    FUN_0028fb44(DAT_002ff454);
    for (;;) {
        if (DAT_0034a80c != 0)
            break;
        e = FUN_002b7ee0(d);
        if (e < 0 || e == 1)
            break;
        FUN_002b7958(1000);
    }
    d->busy = 1;
    for (;;) {
        if (FUN_002b7908(d->h48) >= 0)
            break;
        e = FUN_002b7e90(d);
        if (e != 0)
            FUN_0028fb44(DAT_002ff3e0, e);
        FUN_002b7958(100000);
    }
    if (d->h48 >= 0) {
        e = FUN_002b7980(d->h48, 0);
        if (e != 0)
            FUN_0028fb44(DAT_002cabe4, e);
    }
    if (d->h80 >= 0) {
        e = FUN_002b7980(d->h80, 0);
        if (e != 0)
            FUN_0028fb44(DAT_002cabe4, e);
    }
    if (d->h64 >= 0) {
        e = FUN_002b7980(d->h64, 0);
        if (e != 0)
            FUN_0028fb44(DAT_002cabe4, e);
    }
    if (d->h48 >= 0) {
        e = FUN_002b79a8(d->h48);
        if (e != 0)
            FUN_0028fb44(DAT_002cabbc, e);
        d->h48 = -1;
    }
    if (d->h80 >= 0) {
        e = FUN_002b79a8(d->h80);
        if (e != 0)
            FUN_0028fb44(DAT_002cabbc, e);
        d->h80 = -1;
    }
    if (d->h64 >= 0) {
        e = FUN_002b79a8(d->h64);
        if (e != 0)
            FUN_0028fb44(DAT_002cabbc, e);
        d->h64 = -1;
    }
    for (i = 0; i < 2; i++) {
        if (d->bufs[i] != 0) {
            FUN_000104e0(d->bufs[i]);
            d->bufs[i] = 0;
        }
    }
    if (d->bufe4 != 0) {
        FUN_000104e0(d->bufe4);
        d->bufe4 = 0;
    }
    e = FUN_002b78d0(d->h88);
    if (e != 0)
        FUN_0028fb44(DAT_002ff27c, e);
    FUN_002908cc(d);
    e = FUN_002b7ec0(d);
    if (e != 0)
        FUN_0028fb44(DAT_002ff2a8, e);
    e = FUN_002b7a08(0);
    if (e < 0)
        FUN_0028fb44(DAT_002ff2d0, e);
    e = FUN_002b7d50();
    if (e != 0)
        FUN_0028fb44(DAT_002ff300, e);
    e = FUN_002b7cc0(0x303);
    if (e < 0)
        FUN_0028fb44(DAT_002ff320, e);
    e = FUN_0006165c();
    if (e != 0)
        FUN_0028fb44(DAT_002ff34c, e);
    if (d->f1ec != 0)
        FUN_002903cc();
    if (d->buf2c != 0) {
        FUN_000104e0(d->buf2c);
        d->buf2c = 0;
    }
    d->fe0 = 0;
    d->f29 = 0;
    d->active = 0;
    d->busy = 0;
    return 0;
}
