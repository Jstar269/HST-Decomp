/* HSTMATCH module=EBOOT.BIN start=0x001efcd8 end=0x001f00c8 symbol=FUN_001efcd8 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Pad {                /* DAT_002cfc8c */
    char pad0[0x8b];
    signed char b0 : 2;     /* 0x8b bits 0-1 */
    signed char b1 : 2;     /* 0x8b bits 2-3 */
};

struct Owner {
    char pad[0x90];
    void *f90;
};

struct Self {
    char pad0[0x44];
    int f44;
    char pad48[0x3c4 - 0x48];
    int f3c4;
    char pad3c8[0x510 - 0x3c8];
    void *f510;
    char pad514[0x584 - 0x514];
    Owner *f584;
    char pad588[0x58c - 0x588];
    unsigned char f58c;
    char pad58d[0x5b8 - 0x58d];
    int f5b8;
    char pad5bc[0x5dc - 0x5bc];
    int state;
    int counter;
    char pad5e4[0x5ec - 0x5e4];
    float f5ec;
    float f5f0;
    unsigned char f5f4;
    unsigned char f5f5;
};

extern "C" {
extern Pad *DAT_002cfc8c;
extern int DAT_002ee828;
extern int DAT_002ee82c;
extern char DAT_002ee830[];
extern char DAT_0034179c[];

int FUN_00065740(void *, char *, int);
void FUN_001b1c44(char *, int);
int FUN_001b2af0(char *, int);
void FUN_001e09dc(void *, int);
void FUN_001e28e0(void *);
void FUN_001e2990(void *);
void FUN_001e54dc(void *, int);
void FUN_001e5524(void *, unsigned char);
void FUN_001e5958(void *);
void FUN_001e59d0(void *);
void FUN_001e59d8(void *);
void FUN_001f3770(Self *);
void FUN_001f461c(Self *, int);
void FUN_001f46b4(Self *, int);
void FUN_001f474c(Self *, int);
void FUN_001f47a4(Self *);
void FUN_001f4bfc(Self *);
void FUN_001f4ee0(Self *, int);
void FUN_001f9390(Self *);
void FUN_001f93dc(Self *);
void FUN_001f94a4(Self *);
void FUN_001f94ac(Self *);
}

static inline int Clamp(const int &v, const int &lo, const int &hi)
{
    return v < lo ? lo : (hi < v ? hi : v);
}

static inline bool Pressed(Self *self)
{
    if (self->f58c != 0)
        return false;
    return DAT_002cfc8c->b1 == 0;
}

extern "C" void FUN_001efcd8(Self *self)
{
    if (self->f44 == 0) {
        self->state = 0;
        self->counter = 0;
        FUN_001f9390(self);
        self->f5f0 = self->f5ec;
        FUN_001f474c(self, 0);
        FUN_001e09dc(self->f510, 2);
        FUN_001e2990(self->f510);
        FUN_001e54dc(self->f510, 0);
        FUN_001e59d8(self->f510);
    }
    switch (self->state) {
    case 0:
        if (++self->counter > 40) {
            FUN_001b1c44(DAT_0034179c, 8);
            self->state = 1;
            self->counter = 0;
            FUN_001e5524(self->f510, 0);
        }
        break;
    case 1:
        if (++self->counter > 25) {
            FUN_001b1c44(DAT_0034179c, 8);
            self->state++;
            self->counter = 0;
            FUN_001e5524(self->f510, 0);
        } else if (Pressed(self)) {
            FUN_001b1c44(DAT_0034179c, 3);
            self->state = 3;
            self->counter = 0;
        }
        break;
    case 2:
        if (++self->counter > 25) {
            FUN_001b1c44(DAT_0034179c, 8);
            self->state++;
            self->counter = 0;
        } else if (Pressed(self)) {
            FUN_001b1c44(DAT_0034179c, 3);
            self->state = 3;
            self->counter = 0;
            FUN_001e5524(self->f510, 0);
        }
        break;
    case 3:
        self->state++;
        self->counter = 0;
        if (self->f5f4 != 0) {
            if (self->f5f5 != 0) {
                self->f5b8 = FUN_001b2af0(DAT_0034179c, 14);
                FUN_001f46b4(self, 0);
                FUN_001e5958(self->f510);
                FUN_001f93dc(self);
            } else {
                self->f5b8 = FUN_001b2af0(DAT_0034179c, 15);
                FUN_001f461c(self, 0);
                FUN_001f4ee0(self, self->f3c4);
                FUN_001f94a4(self);
            }
        }
        FUN_001e5524(self->f510, self->f5f4);
        break;
    case 4: {
        ++self->counter;
        int c = Clamp(self->counter, DAT_002ee828, DAT_002ee82c);
        self->counter = c;
        if (c == FUN_00065740(self->f584->f90, DAT_002ee830, 1)) {
            if (self->f5f4 != 0 && self->f5f5 != 0)
                FUN_001e59d0(self->f510);
        }
        if (self->counter == 10)
            FUN_001e28e0(self->f510);
        if (self->counter > 10) {
            if (Pressed(self)) {
                FUN_001b1c44(DAT_0034179c, 3);
                FUN_001f9390(self);
                FUN_001f94ac(self);
                FUN_001f3770(self);
            }
        }
        break;
    }
    }
    FUN_001f4bfc(self);
    FUN_001f47a4(self);
}
