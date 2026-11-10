/* HSTMATCH module=EBOOT.BIN start=0x000b1e34 end=0x000b2144 symbol=FUN_000b1e34 lang=c++ */
struct Info {
    char pad[0x8c];
    int f8c;
};

struct Body {
    char pad[4];
    int f4;
    char pad08[0x58 - 8];
    Info *info;
};

struct Rec {
    char pad[0x130];
    unsigned char f130;
    unsigned char f131;
    char pad132[2];
    int f134;
};

struct Team {
    char pad[0x780];
    int f780;
};

struct Ball {
    char pad[0x14];
    float z;
};

struct Game {
    char pad[0x3c];
    int mode;
    char pad40[0x88 - 0x40];
    Ball *ball;
};

struct Stat {
    char pad[0x24];
    float f24;
};

struct Peer {
    char pad[0x144];
    int team;
};

struct Player {
    char pad00[0x3c];
    int mode;
    char pad40[0x144 - 0x40];
    int team;
    int f148;
    char pad14c[0x1b4 - 0x14c];
    unsigned char f1b4;
    char pad1b5[0x41c - 0x1b5];
    int f41c;
    char pad420[0x480 - 0x420];
    unsigned char f480;
    char pad481[3];
    int f484;
    char pad488[4];
    float f48c;
    int f490;
    char pad494[0x4ce - 0x494];
    unsigned char f4ce;
    unsigned char f4cf;
    char pad4d0[0x4e8 - 0x4d0];
    int f4e8;
    char pad4ec[0x5bc - 0x4ec];
    Body *body;
};

struct Zero4 {
    int a, b, c, d;
};

extern "C" {
extern int DAT_00341284;
extern int DAT_00341288;
extern unsigned char DAT_003386b8;
extern Team *DAT_00338af0;
extern Game *DAT_003386b0;
extern float DAT_00341314;
extern float DAT_003411fc;
extern void *DAT_00338c20;
unsigned char FUN_000b3490(void);
Rec *FUN_000c4ba0(Team *, int);
Peer *FUN_0008882c(Game *);
Stat *FUN_000b3c68(Player *);
void FUN_000fb0d4(void *, int, Info *, int, unsigned char, Zero4 *, int, unsigned char);
}

extern "C" void FUN_000b1e34(Player *self, int side, int force)
{
    int kind;
    if (force != 0) {
        kind = 17;
    } else {
        int m = FUN_000b3490();
        int d = self->f484;
        int ad = d < 0 ? -d : d;
        int still = ad <= 0;
        if (side != 0) {
            kind = 3;
        } else if (m != 0) {
            if (still)
                kind = 13;
            else
                kind = d > 0 ? 9 : 10;
        } else if (DAT_00341284 < d) {
            kind = 8;
        } else if (d < DAT_00341288) {
            kind = 8;
        } else if (still) {
            kind = 0;
        } else if (d >= 8) {
            kind = 11;
        } else if (d < -7) {
            kind = 12;
        } else {
            kind = d > 0 ? 1 : 2;
        }
    }
    self->f480 = 1;
    self->f4ce = side;
    if (self->mode != 7) {
        int d2 = self->f484;
        bool b = DAT_003386b8 != 0;
        if (b)
            b = !(self->f1b4 != 0);
        if (b && DAT_00338af0 != 0) {
            Rec *r = FUN_000c4ba0(DAT_00338af0, self->f148);
            r->f134 = DAT_00338af0->f780;
            r->f130 = d2;
            r->f131 = side;
        }
    }
    if (self->body->info == 0 || self->body->info->f8c == 0)
        return;
    if (DAT_003386b0->mode != 7) {
        if (self->team != FUN_0008882c(DAT_003386b0)->team && force == 0)
            return;
    }
    float ratio = (float)self->f41c / DAT_00341314;
    int arg = self->body->f4;
    unsigned char hard = !(ratio < 1.0f);
    if (hard) {
        float z = DAT_003386b0->ball->z;
        float lim = 0.01f * DAT_003411fc * -(0.01f * FUN_000b3c68(self)->f24);
        hard = !(z > lim && z < 0.0f);
    }
    Zero4 zero;
    zero.a = 0;
    zero.b = 0;
    zero.c = 0;
    zero.d = 0;
    FUN_000fb0d4(DAT_00338c20, kind, self->body->info, arg, self->f4cf, &zero, self->f484, hard);
    if (self->f4e8 != 1 && force == 0) {
        self->f48c = (float)self->f484;
        self->f490 = -45;
    }
}
// NEAR (lane w4f1): this structured rewrite measures 177 by position (208/196 words); the older Ghidra-style
// draft wip/FUN_000b1e34_000b1e34.cpp still scores better by position (154, 196/196 words), so start from
// whichever is closer to what you change. Findings from this rewrite:
// (1) everything from `self->f480 = 1` onwards (the 7-mode test, the bool chain on DAT_003386b8/f1b4,
//     FUN_000c4ba0 record, the ratio/hard flag, the zeroed 16-byte arg, the tail) lines up apart from the
//     ladder's length; FUN_0008882c takes DAT_003386b0 (retail loads it into a0 before the call).
// (2) the ladder: `int still = ad <= 0;` gives retail's slt/xori without an andi; `int m = FUN_000b3490();`
//     (uchar return) masks once like retail; `DAT_00341284 < d || d < DAT_00341288` reproduces retail's
//     second, unoptimised `beqz at; nop; li 8; b` test. Still wrong: retail's `blezl a1,END; li 10` /
//     `blezl a1,END; li 2` arms; every spelling tried (if/else either way, ternaries either way, an inline
//     with returns) adds a duplicated `li s1,K; b END` tail, so the ladder is ~11 words long.
// (3) retail moves the params as `move s2,a2; move s3,a1; move s4,a0` (ours a0 before a1).
// near miss from lane w4f1 (unreviewed)
