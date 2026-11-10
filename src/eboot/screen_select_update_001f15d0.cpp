/* HSTMATCH module=EBOOT.BIN start=0x001f15d0 end=0x001f1968 symbol=FUN_001f15d0 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Bits {
    signed char b0 : 2;
    signed char b1 : 2;
    signed char b2 : 2;
    signed char b3 : 2;
};

struct Config {
    char pad[0x89];
    Bits held;
    char pad8a;
    Bits trig;
};

struct Screen {
    char pad00[0x3c];
    char sub[8];
    int started;
    char pad48[0x510 - 0x48];
    void *view;
    char pad514[0x521 - 0x514];
    unsigned char alt;
    char pad522[0x8170 - 0x522];
    int state;
    int count;
    int sel;
    int done;
};

extern "C" {
extern char DAT_0034179c[];
extern Config *DAT_002cfc8c;
void FUN_001f474c(Screen *, int);
void FUN_001e74d4(void *);
void FUN_001e7714(void *);
void FUN_001e7700(void *, int);
void FUN_001e7730(void *);
void FUN_001e750c(void *);
void FUN_001b1c44(void *, int);
void FUN_001f3770(Screen *);
void FUN_001fb928(void *, int, int);
void FUN_001f4bfc(Screen *);
void FUN_001f47a4(Screen *);
}

extern "C" void FUN_001f15d0(Screen *self)
{
    if (self->started == 0) {
        if (self->alt != 0)
            self->sel = 2;
        else
            self->sel = 0;
        FUN_001f474c(self, 0);
        FUN_001e74d4(self->view);
        FUN_001e7714(self->view);
        FUN_001e7700(self->view, 1);
        self->state = 0;
        self->count = 0;
    }
    switch (self->state) {
    case 0:
        if (++self->count > 20) {
            self->count = 0;
            self->state = 1;
            FUN_001e7700(self->view, 0);
        }
        break;
    case 1:
        if (DAT_002cfc8c->trig.b2 == 0) {
            FUN_001b1c44(DAT_0034179c, 4);
            self->sel = 2;
            self->done = 1;
            FUN_001e7700(self->view, 1);
            FUN_001f3770(self);
        } else if (DAT_002cfc8c->trig.b1 == 0) {
            FUN_001b1c44(DAT_0034179c, 3);
            self->done = 1;
            switch (self->sel) {
            case 0:
                self->state = 2;
                self->count = 0;
                FUN_001e7700(self->view, 1);
                FUN_001e7730(self->view);
                break;
            case 1:
                self->state = 3;
                self->count = 0;
                FUN_001e7700(self->view, 1);
                FUN_001e7730(self->view);
                break;
            case 2:
                self->state = 3;
                self->count = 0;
                FUN_001e7700(self->view, 1);
                FUN_001e7730(self->view);
                break;
            }
        } else if (DAT_002cfc8c->held.b0 == 0) {
            FUN_001b1c44(DAT_0034179c, 1);
            if (--self->sel < 0)
                self->sel = 2;
            FUN_001e750c(self->view);
        } else if (DAT_002cfc8c->held.b2 == 0) {
            FUN_001b1c44(DAT_0034179c, 1);
            if (++self->sel >= 3)
                self->sel = 0;
            FUN_001e750c(self->view);
        }
        break;
    case 2:
        if (++self->count > 30)
            FUN_001fb928(self->sub, 19, 1);
        break;
    case 3:
        if (++self->count > 30)
            FUN_001f3770(self);
        break;
    }
    FUN_001f4bfc(self);
    FUN_001f47a4(self);
}
