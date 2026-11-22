/* HSTMATCH module=EBOOT.BIN start=0x0008ab10 end=0x0008ac88 symbol=FUN_0008ab10 lang=c++ */
/* Hand-written reconstruction from the disassembly (restructured from the near miss wip/FUN_0008ab10_near.cpp; same shape as the exact FUN_0008a72c: an inline helper returning unsigned char with early `return 0`); exact match verified; matching idioms: `(x != 0) ^ 1` for the negated callee result, an int `go` assigned on both paths; names and types await human review. */
extern "C" {
extern unsigned char DAT_003386b8;
extern void *DAT_00338c20;
extern void *DAT_00338b44;
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

static inline unsigned char Ready(Context *self)
{
    unsigned char r = 0;
    if (DAT_003386b8 != 0 && FUN_000fb7b8(DAT_00338c20) != 0) return 0;
    if (self->flag4c == 0) {
        switch (self->state) {
        case 5:
        case 7:
            r = FUN_001181c0(self->obj9c) >= 0;
            break;
        case 6:
            { int b = FUN_001181c0(self->obj9c) >= 0;
              if (b) b = (FUN_000def58(DAT_00338b44) != 0) ^ 1;
              r = b; }
            break;
        }
    }
    return r;
}

extern "C" void FUN_0008ab10(Context *self)
{
    int go;
    if (DAT_003386b8 != 0 && self->field_6c <= 0) {
        go = 0;
    } else {
        unsigned char cond = Ready(self);
        if (cond) cond = !self->flag59;
        go = cond;
    }
    if (go) {
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
