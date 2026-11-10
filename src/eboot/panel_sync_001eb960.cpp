/* HSTMATCH module=EBOOT.BIN start=0x001eb960 end=0x001ebc5c symbol=FUN_001eb960 lang=c++ */
/* Hand-written reconstruction from the disassembly; matching idiom: `(int)` on the second Min operand makes it a stack temporary (retail compares two address-taken temps); exact match verified; names and types await human review. */
struct Game {
    char pad0[0xb4];
    int fb4;                /* 0xb4 */
    int fb8[2];             /* 0xb8 */
    char padc0[0xd8 - 0xc0];
    int fd8[2][6];          /* 0xd8 */
};

struct Panel {
    char pad0[0x9c];
    void *f9c[2];           /* 0x9c */
    void *fa4[2];           /* 0xa4 */
    char padac[0xe0 - 0xac];
    int fe0;                /* 0xe0 */
    int fe4[2];             /* 0xe4 */
    int fec[2][6];          /* 0xec */
    int f11c[2];            /* 0x11c */
    int f124[2];            /* 0x124 */
    unsigned char f12c;     /* 0x12c */
    unsigned char f12d;     /* 0x12d */
    unsigned char f12e;     /* 0x12e */
    unsigned char f12f;     /* 0x12f */
};

extern "C" {
extern Game *DAT_00338b44;
unsigned char FUN_000def58(Game *);
unsigned char FUN_000def0c(Game *);
void FUN_000de100(Game *, int *, unsigned char *, int *);
int FUN_000dea20(Game *, int);
int FUN_000de51c(Game *, int);
int FUN_000de2c8(Game *, int);
int FUN_000de3f0(Game *, int);
void FUN_000ffb3c(void *, bool);
}

static inline const int &Min(const int &a, const int &b)
{
    return b < a ? b : a;
}

extern "C" void FUN_001eb960(Panel *self)
{
    unsigned char ok;
    int side;
    int i;
    unsigned char lock = FUN_000def58(DAT_00338b44);

    self->fe0 = Min(DAT_00338b44->fb8[0] + DAT_00338b44->fb8[1] + 1, (int)DAT_00338b44->fb4);
    FUN_000de100(DAT_00338b44, self->f11c, &ok, &side);
    for (i = 0; i < 2; i++) {
        int j;
        self->fe4[i] = DAT_00338b44->fb8[i];
        for (j = 0; j < self->fe0; j++)
            self->fec[i][j] = DAT_00338b44->fd8[i][j];
        if (ok != 0) {
            self->f11c[i] = 4;
        } else if (side == 0) {
            self->f11c[0] = 5;
            self->f11c[1] = 3;
        } else if (side == 1) {
            self->f11c[0] = 3;
            self->f11c[1] = 5;
        }
        if (FUN_000dea20(DAT_00338b44, i))
            self->f124[i] = 3;
        else if (FUN_000dea20(DAT_00338b44, 1 - i))
            self->f124[i] = 4;
        else if (FUN_000de51c(DAT_00338b44, i))
            self->f124[i] = 2;
        else if (FUN_000de2c8(DAT_00338b44, i))
            self->f124[i] = 0;
        else if (FUN_000de3f0(DAT_00338b44, i))
            self->f124[i] = 1;
        else
            self->f124[i] = -1;
    }
    self->f12c = FUN_000def0c(DAT_00338b44);
    self->f12d = FUN_000de2c8(DAT_00338b44, -1) != 0 || FUN_000de3f0(DAT_00338b44, -1) != 0;
    self->f12e = FUN_000de51c(DAT_00338b44, -1);
    self->f12f = FUN_000dea20(DAT_00338b44, -1);
    for (int k = 0; k < 2; k++) {
        FUN_000ffb3c(self->fa4[k], lock || self->f12c);
        FUN_000ffb3c(self->f9c[k], lock || !self->f12c);
    }
}
