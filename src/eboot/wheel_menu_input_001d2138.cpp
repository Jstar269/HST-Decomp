/* HSTMATCH module=EBOOT.BIN start=0x001d2138 end=0x001d31c0 symbol=FUN_001d2138 lang=c++ rodata=0x002c669c */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Pad {
    char pad00[0x80];
    unsigned int held;
    char pad84[0x8b - 0x84];
    signed char b0 : 2;
    signed char b2 : 2;
    signed char b4 : 2;
};
struct Target { virtual void v0(); virtual void v1(int on); };
struct VObj {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
};
struct Cell { int a; int b; int c; int d; };
struct Self {
    char pad00[0x54];
    int state;
    int prev_state;
    int pad5c;
    int timer;
    int pad64;
    Target *panel;
    char pad6c[0x84 - 0x6c];
    int w84;
    int w88;
    char pad8c[0x90 - 0x8c];
    Cell cell90;
    char padA0[0xd4 - 0xa0];
    int cur;
    int next;
    unsigned int repeat_left;
    unsigned int repeat_right;
    char padE4[0x114 - 0xe4];
    int w114[4];
    Cell c124;
    Cell c134;
    Cell c144;
    Cell c154;
    char pad164[0x16c - 0x164];
    unsigned int *color;
    char pad170[0x178 - 0x170];
    float f178;
    char pad17c[0x270 - 0x17c];
    float f270;
    float f274;
    char pad278[0x290 - 0x278];
    char sub290[0x510 - 0x290];
    float f510;
    char pad514[0x520 - 0x514];
    int mode;
    unsigned char flag524;
    char pad525[0x530 - 0x525];
    char sub530[0x1110 - 0x530];
    char obj1110[4];
};

extern "C" {
extern Pad *DAT_002cfc8c;
extern char DAT_0034179c[];
extern char DAT_002c6ed0[];
extern int DAT_0034a104;
extern int DAT_0034a120;
extern int DAT_0034a65c;
int FUN_002a17e0(void *);
int FUN_002a1828(void *);
int FUN_0001ea64(Pad *, unsigned int *, unsigned int, int, unsigned int);
void FUN_001b1c44(void *, int);
void FUN_001d2138(Self *self);
void FUN_001d2124(Target *);
void FUN_001d1bf0(Target *);
void FUN_001d61e4(int);
void FUN_001d6264(Cell *, unsigned int *);
void FUN_0027ff98(Cell *);
int FUN_001d2118(int, int);
void FUN_001d2088(Self *);
void FUN_001d1fe0(Self *);
void FUN_0026beb8(void *);
void FUN_00231214(int);
void FUN_001d1bdc(Self *);
int FUN_001d9cc4(int);
void FUN_002a1a64(int, int);
void FUN_001d57f0(Self *, int, int);
void FUN_001d08f0(...);
}

/* Two-bit button-state fields of the pad record; zero means the button is not held. */
static inline unsigned char B2Clear(Pad *p) { return !p->b2; }
static inline unsigned char B4Clear(Pad *p) { return !p->b4; }

/* Inlined state change: records the previous state and runs the entry action of the new one. */
static inline void SetState(Self *self, int s)
{
    self->timer = 0;
    self->prev_state = self->state;
    self->state = s;
    switch (self->state) {
    case 0:
        FUN_001d2138(self);
        FUN_001d2124(self->panel);
        for (unsigned i = 0; i < 4; i++) {
            FUN_001d61e4(self->w114[i]);
        }
        FUN_001d61e4(self->w84);
        FUN_001d61e4(self->w88);
        FUN_001d6264(&self->cell90, 0);
        self->f178 = 1.86f;
        *self->color &= 0xffffff;
        break;
    case 1:
        self->f510 = 6.0f;
        if (!FUN_001d2118(self->next, self->cur)) {
            FUN_001d2088(self);
        }
        self->next = self->cur - 1;
        self->f274 = 6.0f - (float)self->next;
        self->f270 = self->f274 - 1.0f;
        if (self->f274 > 6.0f) {
            self->f274 -= 6.0f;
            self->f270 -= 6.0f;
        }
        FUN_001d1fe0(self);
        FUN_0027ff98(&self->c124);
        FUN_0027ff98(&self->c144);
        break;
    case 2:
        self->f510 = 6.0f;
        if (!FUN_001d2118(self->next, self->cur)) {
            FUN_001d2088(self);
        }
        self->next = self->cur + 1;
        self->f274 = 6.0f - (float)self->next;
        self->f270 = self->f274 + 1.0f;
        if (self->f274 < 0.0f) {
            self->f274 += 6.0f;
            self->f270 += 6.0f;
        }
        FUN_001d1fe0(self);
        FUN_0027ff98(&self->c134);
        FUN_0027ff98(&self->c154);
        break;
    case 3:
    case 4:
        break;
    case 5:
        DAT_0034a104 = self->cur;
        if (self->mode == 1) {
            ((VObj *)self->obj1110)->v6();
            FUN_001d1bf0(self->panel);
        } else if (self->mode == 2) {
            FUN_0026beb8(self->sub530);
            FUN_001d1bf0(self->panel);
        } else if (self->mode == 3) {
            FUN_00231214(DAT_0034a65c);
            FUN_001d1bf0(self->panel);
        } else {
            FUN_001d1bdc(self);
            switch (self->cur) {
            case 0:
            case 1:
                FUN_002a1a64(FUN_001d9cc4(DAT_0034a120), 0x18);
                break;
            case 2:
            case 3:
            case 4:
            case 5:
                break;
            }
        }
        break;
    case 6:
        FUN_001d1bdc(self);
        break;
    case 7:
        FUN_001d57f0(self, 5, 0);
        break;
    default:
        FUN_001d08f0(DAT_002c6ed0);
        break;
    }
}

/* Six-slot wheel menu input: LEFT/RIGHT (with key repeat) rotate the selection, the decide/cancel bits change state. */
void FUN_001d2138(Self *self)
{
    if (FUN_002a17e0(self->sub290) == 0 || FUN_002a1828(self->sub290) != 0) {
        int s = self->state;
        switch (s) {
        case 5:
        case 6:
        case 3:
        case 4:
            break;
        default:
            if (s == 0 && B2Clear(DAT_002cfc8c)) {
                FUN_001b1c44(DAT_0034179c, 3);
                if (self->mode == 0) {
                    self->flag524 = 1;
                    SetState(self, 3);
                } else {
                    SetState(self, 5);
                }
            } else if (s == 0 && B4Clear(DAT_002cfc8c)) {
                if (self->mode != 0) {
                    if (self->mode != 2) {
                        FUN_001b1c44(DAT_0034179c, 4);
                    }
                    self->flag524 = 0;
                    SetState(self, 3);
                }
            } else if (FUN_0001ea64(DAT_002cfc8c, &self->repeat_left, 0x80 /* LEFT */, 4, 9)) {
                FUN_001b1c44(DAT_0034179c, 1);
                SetState(self, 1);
            } else if (FUN_0001ea64(DAT_002cfc8c, &self->repeat_right, 0x20 /* RIGHT */, 4, 9)) {
                FUN_001b1c44(DAT_0034179c, 1);
                SetState(self, 2);
            }
            break;
        }
    }
}
