/* HSTMATCH module=EBOOT.BIN start=0x0008ab10 end=0x0008ac88 symbol=FUN_0008ab10 lang=c++ */
extern "C" {
extern unsigned char DAT_003386b8;
extern void *DAT_00348c20;
extern void *DAT_00348b44;
extern char *DAT_002cfc8c;
extern int FUN_000fb7b8(void *p);
extern int FUN_001181c0(void *p);
extern int FUN_000def58(void *p);
}

struct Context {
    char pad0[0x3c];
    int state;         // 0x3c
    char pad1[0xc];
    unsigned char flag4c; // 0x4c
    char pad2[0xc];
    unsigned char flag59; // 0x59
    char pad3[0x12];
    int field_6c;      // 0x6c
    char pad4[0x2c];
    void *obj9c;       // 0x9c
};

extern "C" void FUN_0008ab10(Context *self)
{
    bool cond;
    if (DAT_003386b8 && self->field_6c < 1) {
        cond = false;
    } else {
        bool r = false;
        if (DAT_003386b8 && FUN_000fb7b8(DAT_00348c20)) {
            r = false;
        } else if (self->flag4c == 0) {
            switch (self->state) {
            case 5:
            case 7:
                r = (FUN_001181c0(self->obj9c) >= 0);
                break;
            case 6: {
                bool b = (FUN_001181c0(self->obj9c) >= 0);
                r = b;
                if (b) {
                    r = (FUN_000def58(DAT_00348b44) <= 0);
                }
                break;
            }
            }
        }
        cond = r;
        if (cond) {
            cond = !self->flag59;
        }
    }

    if (cond) {
        char f = DAT_002cfc8c[139];
        if (((int)f << 24 >> 30) == 0) {
            self->flag59 = 1;
        }
        if (self->state == 7) {
            char f2 = DAT_002cfc8c[139];
            if (((int)f2 << 30 >> 30) == 0) {
                self->flag59 = 1;
            }
        }
    }
}

// 53 differing words out of 94 words (improved from 82 differing words in FUN_0008ab10_near.c).
// Differs in:
// 1. FUN_000fb7b8 check exits to cond computation with `b 0x8abf8; move v1, zero`.
// 2. flag4c exits to cond computation with branch likely `bnezl v1, 0x8abf8; andi v1, s0, 0xff`.
// 3. case 6 uses v1 for the FUN_001181c0 result and branch likely delay slot `andi s0, v1, 0xff`.
// 4. cond computation uses a0 temporary with andi a0, v1, 0xff in both paths.
// near miss from lane dsn5 (unreviewed)
