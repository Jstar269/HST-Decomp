/* HSTMATCH module=hstp_trans_main.prx start=0x001dff3c end=0x001e0fc4 symbol=FUN_001dff3c lang=c++ rodata=0x002d6be4 */
/* Hand-written reconstruction (port of src/eboot/wheel_menu_input_001d2138.cpp, its twin: addresses substituted; no other change); exact match verified; names and types await human review. */
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
extern Pad *DAT_003751d4;
extern char DAT_003eaad8[];
extern char DAT_002d72b4[];
extern int DAT_003f343c;
extern int DAT_003f3458;
extern int DAT_003f398c;
int FUN_002b0bec(void *);
int FUN_002b0c34(void *);
int FUN_0001e6b0(Pad *, unsigned int *, unsigned int, int, unsigned int);
void FUN_001bfc3c(void *, int);
void FUN_001dff3c(Self *self);
void FUN_001dff28(Target *);
void FUN_001df9f4(Target *);
void FUN_001e3fe8(int);
void FUN_001e4054(Cell *, unsigned int *);
void FUN_0029140c(Cell *);
int FUN_001dff1c(int, int);
void FUN_001dfe8c(Self *);
void FUN_001dfde4(Self *);
void FUN_0027ce90(void *);
void FUN_00241314(int);
void FUN_001df9e0(Self *);
int FUN_001e7d7c(int);
void FUN_002b0e70(int, int);
void FUN_001e35f4(Self *, int, int);
void FUN_001df9b4(...);
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
        FUN_001dff3c(self);
        FUN_001dff28(self->panel);
        for (unsigned i = 0; i < 4; i++) {
            FUN_001e3fe8(self->w114[i]);
        }
        FUN_001e3fe8(self->w84);
        FUN_001e3fe8(self->w88);
        FUN_001e4054(&self->cell90, 0);
        self->f178 = 1.86f;
        *self->color &= 0xffffff;
        break;
    case 1:
        self->f510 = 6.0f;
        if (!FUN_001dff1c(self->next, self->cur)) {
            FUN_001dfe8c(self);
        }
        self->next = self->cur - 1;
        self->f274 = 6.0f - (float)self->next;
        self->f270 = self->f274 - 1.0f;
        if (self->f274 > 6.0f) {
            self->f274 -= 6.0f;
            self->f270 -= 6.0f;
        }
        FUN_001dfde4(self);
        FUN_0029140c(&self->c124);
        FUN_0029140c(&self->c144);
        break;
    case 2:
        self->f510 = 6.0f;
        if (!FUN_001dff1c(self->next, self->cur)) {
            FUN_001dfe8c(self);
        }
        self->next = self->cur + 1;
        self->f274 = 6.0f - (float)self->next;
        self->f270 = self->f274 + 1.0f;
        if (self->f274 < 0.0f) {
            self->f274 += 6.0f;
            self->f270 += 6.0f;
        }
        FUN_001dfde4(self);
        FUN_0029140c(&self->c134);
        FUN_0029140c(&self->c154);
        break;
    case 3:
    case 4:
        break;
    case 5:
        DAT_003f343c = self->cur;
        if (self->mode == 1) {
            ((VObj *)self->obj1110)->v6();
            FUN_001df9f4(self->panel);
        } else if (self->mode == 2) {
            FUN_0027ce90(self->sub530);
            FUN_001df9f4(self->panel);
        } else if (self->mode == 3) {
            FUN_00241314(DAT_003f398c);
            FUN_001df9f4(self->panel);
        } else {
            FUN_001df9e0(self);
            switch (self->cur) {
            case 0:
            case 1:
                FUN_002b0e70(FUN_001e7d7c(DAT_003f3458), 0x18);
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
        FUN_001df9e0(self);
        break;
    case 7:
        FUN_001e35f4(self, 5, 0);
        break;
    default:
        FUN_001df9b4(DAT_002d72b4);
        break;
    }
}

/* Six-slot wheel menu input: LEFT/RIGHT (with key repeat) rotate the selection, the decide/cancel bits change state. */
void FUN_001dff3c(Self *self)
{
    if (FUN_002b0bec(self->sub290) == 0 || FUN_002b0c34(self->sub290) != 0) {
        int s = self->state;
        switch (s) {
        case 5:
        case 6:
        case 3:
        case 4:
            break;
        default:
            if (s == 0 && B2Clear(DAT_003751d4)) {
                FUN_001bfc3c(DAT_003eaad8, 3);
                if (self->mode == 0) {
                    self->flag524 = 1;
                    SetState(self, 3);
                } else {
                    SetState(self, 5);
                }
            } else if (s == 0 && B4Clear(DAT_003751d4)) {
                if (self->mode != 0) {
                    if (self->mode != 2) {
                        FUN_001bfc3c(DAT_003eaad8, 4);
                    }
                    self->flag524 = 0;
                    SetState(self, 3);
                }
            } else if (FUN_0001e6b0(DAT_003751d4, &self->repeat_left, 0x80 /* LEFT */, 4, 9)) {
                FUN_001bfc3c(DAT_003eaad8, 1);
                SetState(self, 1);
            } else if (FUN_0001e6b0(DAT_003751d4, &self->repeat_right, 0x20 /* RIGHT */, 4, 9)) {
                FUN_001bfc3c(DAT_003eaad8, 1);
                SetState(self, 2);
            }
            break;
        }
    }
}
