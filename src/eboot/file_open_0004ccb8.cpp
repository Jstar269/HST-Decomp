/* HSTMATCH module=EBOOT.BIN start=0x0004ccb8 end=0x0004cfd0 symbol=FUN_0004ccb8 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Net {
    char pad[0x28];
    int state;
};

extern "C" {
extern Net *DAT_00338ac4;
extern unsigned char DAT_0030fbfd;
extern unsigned char DAT_0030fbff;
extern unsigned char DAT_0030fc00;
extern char DAT_0030ab9c[];
extern char DAT_0030ebf4[];
extern int DAT_00311158;
extern unsigned int DAT_003417a4;
extern char *DAT_002d09b4[];
extern char DAT_002d09e4[];
extern char DAT_002d09f0[];
extern char DAT_002d0950[];
extern char DAT_002d09fc[];
extern char DAT_002d0a04[];
extern char DAT_002d0964[];
extern char DAT_002d0a0c[];
extern char DAT_002d0a18[];
extern char DAT_002d0a20[];
extern char DAT_002d0a34[];
extern char DAT_002d0a60[];
int FUN_00049200(void);
void FUN_000bb190(Net *);
char *FUN_000149a8(char *, const char *);
char *FUN_00014d64(const char *, const char *);
int FUN_000143b0(char *, const char *, ...);
int FUN_0001c560(void *, const char *);
void FUN_0001cbc8(void *, const char *);
void FUN_0004caa4(const char *, ...);
int FUN_002b7790(const char *, int, int);
}

extern "C" int FUN_0004ccb8(const char *path, int mode, int fix)
{
    if (DAT_00338ac4 != 0 && DAT_00338ac4->state == 11 && FUN_00049200() == 0)
        FUN_000bb190(DAT_00338ac4);
    if (*path == 0)
        return -1;
    char buf[256];
    char name[256];
    char full[256];
    char alt[256];
    char tmp[256];
    FUN_000149a8(buf, path);
    if (FUN_00014d64(buf, DAT_002d09e4) == 0 && FUN_00014d64(buf, DAT_002d09f0) == 0) {
        char *p = FUN_00014d64(buf, DAT_002d0950);
        if (p != 0)
            FUN_000149a8(p, DAT_002d09e4);
    }
    if (fix) {
        char *p = FUN_00014d64(buf, DAT_002d09fc);
        if (p != 0)
            FUN_000149a8(p, DAT_002d0a04);
    }
    FUN_000149a8(name, buf);
    if (mode == 1) {
        for (char *c = name; *c; c++) {
            if (*c == '/')
                *c = 92;
        }
        if (DAT_0030fbfd != 0 && FUN_0001c560(DAT_0030ab9c, name)) {
            FUN_0001cbc8(DAT_0030ebf4, name);
            return 0;
        }
        return -2;
    }
    char *used = full;
    int hasAlt = 0;
    if (DAT_0030fc00 != 0 && DAT_0030fbff == 0) {
        char *p = FUN_00014d64(buf, DAT_002d0964);
        if (p != 0) {
            FUN_000143b0(alt, DAT_002d0a0c, buf);
            FUN_000143b0(tmp, DAT_002d0a18, DAT_002d09b4[DAT_003417a4], p + 4);
            FUN_000149a8(buf, tmp);
            hasAlt = 1;
        } else {
            FUN_0004caa4(DAT_002d0a20);
        }
    }
    FUN_000143b0(full, DAT_002d0a0c, buf);
    int fd = FUN_002b7790(full, mode, mode == 1 ? 0 : 511);
    if (fd < 0 && hasAlt) {
        fd = FUN_002b7790(alt, mode, mode == 1 ? 0 : 511);
        used = alt;
    }
    if (fd >= 0) {
        DAT_00311158++;
        if (mode == 1) {
            FUN_0004caa4(DAT_002d0a34);
            FUN_0004caa4(DAT_002d0a60, used);
            FUN_0004caa4(DAT_002d0a34);
            FUN_0001cbc8(DAT_0030ebf4, name);
        }
    }
    return fd;
}
