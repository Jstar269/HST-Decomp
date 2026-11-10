/* HSTMATCH module=EBOOT.BIN start=0x00283874 end=0x00283c14 symbol=FUN_00283874 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. players[i] = work is the implicit memberwise operator= of Player (S_002831c0 sub-records through their out-of-line operator=, symbol appended); each loop has its own block-scope index. */

struct V4 { float x, y, z, w; };
struct V3 { float x, y, z; };
struct F2 { float a, b; };


struct Pair { unsigned char a, b; };

struct Pose {
    V4 pos;
    V3 dir;
    F2 sc;
    Pair c;
};

struct S_002831c0 {
    char pad0[0xed];
    unsigned char active;
    char padee[0xf8 - 0xee];
    int slot;
    char padfc[0x120 - 0xfc];
    S_002831c0();
    S_002831c0 &operator=(const S_002831c0 &);
};

struct Player {
    V4 pos;
    int i10;
    unsigned char b14;
    Pose pose;
    S_002831c0 sub[3];
    signed char tags[6];
    int vals[43];
} __attribute__((aligned(8)));

struct Ent {
    unsigned char on;
    char pad[6];
};

struct Global {
    char pad0[0x3e];
    unsigned char key[0x20];
    Ent ent[4];
    char pad7a[0x198 - 0x7a];
    int cur;
};

struct Self {
    char pad0;
    unsigned char mode;
    char pad2[0x58 - 2];
    Pose pose;
    char pad80[0x4dc - 0x80];
    unsigned char saved;
    char pad4dd[0x4e8 - 0x4dd];
    Player work;
    Player players[4];
    char pad1aa0[0x2c14 - 0x1aa0];
    int frame;
};

extern Global *DAT_00338ac4;
extern unsigned char DAT_003417c8;

extern "C" {
int FUN_000bbba4(Global *, unsigned char *);
void FUN_000bd568(Global *);
void FUN_000bd6a4(Global *, int);
void FUN_00283358(Self *);
void FUN_00282664(Self *);
void FUN_000bb190(Global *);
}

extern "C" void FUN_00283874(Self *self)
{
    for (int i = 0; i < 4; i++) {
        Global *g = DAT_00338ac4;
        if (g->ent[i].on != 0 && self->players[i].sub[0].active != 0 && i == g->cur)
            FUN_000bbba4(g, g->key);
    }
    for (int i = 0; i < 4; i++) {
        Global *g = DAT_00338ac4;
        if (g->ent[i].on != 0 && self->players[i].sub[0].active != 0) {
            if (i == g->cur) {
                if (i == FUN_000bbba4(g, g->key)) {
                    self->work.pose = self->pose;
                } else {
                    g = DAT_00338ac4;
                    self->work.pose = self->players[FUN_000bbba4(g, g->key)].pose;
                }
                self->players[i] = self->work;
                self->players[i].sub[0].slot = i;
                if (self->frame % 4 == 0)
                    FUN_000bd568(DAT_00338ac4);
            } else {
                FUN_000bd6a4(g, i);
                g = DAT_00338ac4;
                if (i == FUN_000bbba4(g, g->key))
                    self->pose = self->players[i].pose;
            }
        }
    }
    if (self->mode == 0)
        FUN_00283358(self);
    else
        DAT_003417c8 = self->saved;
    FUN_00282664(self);
    FUN_000bb190(DAT_00338ac4);
}
