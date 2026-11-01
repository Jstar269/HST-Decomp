/* HSTMATCH module=EBOOT.BIN start=0x0010cd24 end=0x0010cf4c symbol=FUN_0010cd24 lang=c++ */
/* Hand-written reconstruction from the disassembly; 138/138 words, 17 differ (parked). */

struct Obj {
    char pad0[0x10];
    void **tab;
    char pad1[0xd0 - 0x14];
    int fD0;
    int padD4;
    int fD8;
    int fDC;
    int fE0;
    int fE4;
    unsigned char bE8;
    unsigned char bE9;
    unsigned char bEA;
    char pad2[0x114 - 0xeb];
    signed char b114;
    char pad3[0x120 - 0x115];
    short *arr;
};

struct Self {
    Obj *o;
    int f4;
    int f8;
    int f0c;
    char pad[0x68 - 0x10];
    int state;
    signed char b6c;
    signed char b6d;
};

extern "C" void FUN_0010bc70(Obj *o);
extern "C" void FUN_001b1c44(void *fmt, int n);
extern "C" void FUN_0010c69c(Self *self, int n);
extern "C" void FUN_0001dfe8(void *p, int n);
extern "C" void FUN_0001e23c(void *p, int n, int m);
extern "C" char DAT_0034179c;
extern "C" signed char *DAT_002cfc8c;
extern "C" volatile int DAT_002d9218;

extern "C" void FUN_0010cd24(Self *self)
{
    self->b6c = 0;
    self->b6d = 0;
    if (self->state != 1)
        FUN_0010bc70(self->o);
    void *base = *(void **)((char *)self->o + 16);
    void *elem = *(void **)((char *)base + self->f0c * 4);
    unsigned short flags = *(unsigned short *)((char *)elem + 14);
    if ((flags & 3) != 0)
        return;
    if (self->o->bEA != 0)
        FUN_001b1c44(&DAT_0034179c, 1);
    if (self->state == 0) {
        if (self->o->bE8 != 0) {
            self->state = 1;
            self->f8 = 0;
            if (self->o->fDC == 0) {
                self->b6c = 1;
                FUN_001b1c44(&DAT_0034179c, 3);
            } else {
                self->b6d = 1;
                FUN_001b1c44(&DAT_0034179c, 4);
            }
            Obj *o = self->o;
            o->fD8 = 0;
            o->b114 = 1;
        } else {
            if ((1 ^ (((DAT_002cfc8c[0x8b] << 26) >> 30) != 0)) == 0)
                goto tail;

            self->state = 2;
            int n = DAT_002d9218;
            Obj *o = self->o;
            int diff = o->fE4 - o->fE0;
            int old;
            if (DAT_002d9218 >= 2) {
                old = o->fDC;
            } else {
                n = diff <= 0 ? diff : 1;
                old = o->fDC;
            }
            if (old != n) {
                short a = o->arr[old * 6];
                short b = o->arr[n * 6];
                o->fD0 = (a != b) ? 3 : 0;
                o->bEA = 1;
            }
            o->fDC = n;
            FUN_0001dfe8(DAT_002cfc8c, 0x4000);
            FUN_0001e23c(DAT_002cfc8c, 0x4000, 1);
            self->b6d = 1;
            FUN_001b1c44(&DAT_0034179c, 4);
        }
    } else if (self->state == 1) {
        if (self->f8 >= 20)
            self->state = 2;
    }
tail:
    FUN_0010c69c(self, self->o->fDC);
}

// Parked 2026-10-31 (lane dsn4). 138/138 words, 17 differ, exact structure and size.
// First difference at +0x118 (0x0010ce3c).
// Differs only by register colouring between $a1 and $a2 in the state 2 update block:
// retail loads DAT_002d9218 into $a1 and self->o into $a2 (and tests slti at, v0, 2),
// whereas ours loads DAT_002d9218 into $a2 and self->o into $a1 (testing slti v0, v0, 2).
// Ruled out: inline method on Self, static inline helper, variable declaration order at function/block scope,
// lang=c99 vs c++.
// improved by lane dsn4 (unreviewed): 78 -> 17 differing words
