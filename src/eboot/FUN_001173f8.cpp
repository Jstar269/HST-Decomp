/* HSTMATCH module=EBOOT.BIN start=0x001173f8 end=0x001178f0 symbol=FUN_001173f8 lang=c++ helpers=FUN_00116f98@0x00116f98-0x00116fc4 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#pragma exceptions off

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

struct Vec4 {
    float x, y, z, w;
};

struct Quat {
    float x, y, z, w;
};

struct Matrix4 {
    float m[4][4];
    Matrix4 &operator=(const Matrix4 &o) {
        m[0][0] = o.m[0][0];
        m[0][1] = o.m[0][1];
        m[0][2] = o.m[0][2];
        m[0][3] = o.m[0][3];
        m[1][0] = o.m[1][0];
        m[1][1] = o.m[1][1];
        m[1][2] = o.m[1][2];
        m[1][3] = o.m[1][3];
        m[2][0] = o.m[2][0];
        m[2][1] = o.m[2][1];
        m[2][2] = o.m[2][2];
        m[2][3] = o.m[2][3];
        m[3][0] = o.m[3][0];
        m[3][1] = o.m[3][1];
        m[3][2] = o.m[3][2];
        m[3][3] = o.m[3][3];
        return *this;
    }
};

struct ShotPart {
    short s;
    u8 b;
    u8 pad3;
    float f;
    int pad8;
};

struct PlayerFrame {
    Quat rot;               // 0x00
    Vec4 pos;               // 0x10
    u16 f20;                // 0x20
    struct {
        char b0 : 3;          // 0x22
        char b1 : 5;
        char b2 : 2;          // 0x23
        char b3 : 1;
        char b4 : 1;
        char b5 : 1;
        char b6 : 3;
    } flags;
    int f24;                // 0x24
    int f28;                // 0x28
    int pad2c;              // 0x2c
    ShotPart parts[2];      // 0x30 (2 * 12 = 24 bytes)
    int f48;                // 0x48 (72)
    int f4c;                // 0x4c (76)
    u8 f50;                 // 0x50 (80)
    u8 f51[2];              // 0x51 (81..82)
    u8 f53;                 // 0x53 (83)
    u8 f54;                 // 0x54 (84)
    u8 f55;                 // 0x55 (85)
    u8 pad56[2];            // 0x56
    int f58;                // 0x58 (88)
    int pad5c;              // 0x5c
};

struct CommonFrame {
    Vec4 v;
    float f0, f1, f2, f3, f4, f5;
    short s0;
    u16 s1;
} __attribute__((aligned(16)));

struct ReplayShot {
    u8 pad0[48];            // 0x00..0x2f
    u8 f30;                 // 0x30 (48)
    u8 f31;                 // 0x31 (49)
    u8 pad32[6];            // 0x32..0x37
    int f38;                // 0x38 (56)
    u8 pad3c[4];            // 0x3c..0x3f
    int f40;                // 0x40 (64)
    u8 pad44[12];           // 0x44..0x4f
    Vec4 vec_50;            // 0x50..0x5f (80)
    u8 pad60;               // 0x60 (96)
    u8 f61;                 // 0x61 (97)
    u8 pad62[14];           // 0x62..0x6f
};                          // 0x70 (112 bytes)

struct PartElement {
    int s;                  // 0x00
    u8 pad4[4];             // 0x04
    u8 b;                   // 0x08
    u8 pad9[11];            // 0x09
    float f;                // 0x14 (20)
    u8 pad18[28];           // 0x18..0x33 (20 + 4 + 28 = 52 bytes)
};

struct PartData {
    u8 pad0[240];           // 0x00
    PartElement elements[2];// 0xf0
};

struct SubPlayer {
    u8 pad0[88];            // 0x00
    PartData *parts;        // 0x58 (88)
};

struct Player {
    u8 pad0[360];           // 0x00
    int f168;               // 0x168 (360)
    u8 pad16c[996];         // 0x16c (364 + 996 = 1360 = 0x550)
    Matrix4 mtx;            // 0x550 (1360..1423, 64 bytes)
    u8 pad590[16];          // 0x590 (1424..1439, 16 bytes)
    Vec4 scale;             // 0x5a0 (1440..1455, 16 bytes)
    u8 f5b0[2];             // 0x5b0 (1456)
    u8 pad5b2[2];           // 0x5b2
    int f5b4;               // 0x5b4 (1460)
    u8 f5b8;                // 0x5b8 (1464)
    char f5b9;              // 0x5b9 (1465)
    u8 pad5ba[2];           // 0x5ba
    SubPlayer *sub;         // 0x5bc (1468)
};

struct MatchData {
    u8 pad0[112];           // 0x00
    Player *players[4];     // 0x70 (112)
};

struct CameraData {
    u8 pad0[2416];          // 0x00
    CommonFrame frame;      // 0x970 (2416)
    inline void RecordTo(CommonFrame &dst) const {
        dst = frame;
    }
};

struct ReplayObj {
    void *vtable;           // 0x00
    u8 pad4[60];            // 0x04
    u8 f40;                 // 0x40 (64)
    u8 pad41[23];           // 0x41
    int f58;                // 0x58 (88)
    int f5c;                // 0x5c (92)
    u8 pad60[4];            // 0x60
    int f64;                // 0x64 (100)
    u8 pad68[4];            // 0x68
    int f6c;                // 0x6c (108)
    u8 pad70[16];           // 0x70
    ReplayShot shots[9];    // 0x80 (128)
    PlayerFrame *player_replays[4][9]; // 0x470 (1136)
    CommonFrame *camera_replays[9];    // 0x500 (1280)
};

extern "C" MatchData *DAT_003386b0;
extern "C" CameraData *DAT_0034b5dc;
extern "C" const char DAT_002d9400[];
extern "C" const char DAT_002d9410[];

extern "C" static void FUN_00116f98(...)
{
}

extern "C" void FUN_00093a90(Vec4 *, SubPlayer *);
extern "C" void FUN_00022434(Vec4 *, const void *, const void *);
extern "C" void FUN_00022450(Vec4 *, const void *, const void *);
extern "C" void FUN_0001d3ac(Quat *, const Matrix4 *);
extern "C" void FUN_000ae5a0(Player *, PlayerFrame *);
extern "C" u8 FUN_000b21cc(Player *, int *, u8 *);

static inline Vec4 SubVec(const void *shot, const Vec4 *pos) {
    Vec4 r;
    FUN_00022434(&r, shot, pos);
    return r;
}

extern "C" void FUN_001173f8(ReplayObj *self) {
    Vec4 pos;
    Matrix4 mtx;
    Vec4 scale;
    Quat rot;

    int i;
    PlayerFrame *pf;
    Player *player;
    int offset;
    char *p_replay;
    ReplayShot *shot;

    if (self->f5c < 0) {
        return;
    }
    shot = &self->shots[self->f5c];
    if (self->f6c == 0) {
        if (shot->f61 != 0 || (((int)(signed char)shot->f31 << 26) >> 27) == 1) {
            shot->vec_50.x = 0.0f;
            shot->vec_50.y = 0.0f;
            shot->vec_50.z = 0.0f;
            shot->vec_50.w = 0.0f;
        } else {
            Player *p0 = DAT_003386b0->players[((int)(signed char)shot->f30 << 26) >> 29];
            FUN_00093a90(&pos, p0->sub);
            shot->vec_50 = SubVec(shot, &pos);
            scale = p0->scale;
            FUN_00022450(&shot->vec_50, &shot->vec_50, &scale);
            shot->vec_50.y = 0.0f;
            shot->vec_50.w = 0.0f;
        }
    }
    if (self->f6c < 150) {
        for (i = 0, offset = 0, p_replay = (char *)self; i < 4; offset += 4, p_replay += 36, i++) {
            player = *(Player **)((char *)DAT_003386b0 + offset + 112);
            if (player != 0) {
                pf = *(PlayerFrame **)(p_replay + self->f5c * 4 + 1136) + self->f6c;
                pf->f48 = player->f168;
                mtx = player->mtx;
                FUN_0001d3ac(&rot, &mtx);
                pf->rot = rot;
                pf->pos = *(Vec4 *)&mtx.m[3][0];
                pf->f20 = 0;
                pf->flags.b0 = 0;
                pf->flags.b1 = 0;
                pf->flags.b2 = 0;
                pf->flags.b3 = 0;
                pf->flags.b4 = 0;
                pf->flags.b5 = 0;
                pf->flags.b6 = 7;
                pf->f24 = 0;
                pf->f28 = 0;
                FUN_000ae5a0(player, pf);
                SubPlayer *sub = player->sub;
                for (unsigned int t = 0; t < 2; t++) {
                    pf->f51[t] = player->f5b0[t];
                    float f = sub->parts ? sub->parts->elements[t].f : 0.0f;
                    u8 b = sub->parts ? sub->parts->elements[t].b : 0;
                    short s = sub->parts ? sub->parts->elements[t].s : -1;
                    pf->parts[t].s = s;
                    pf->parts[t].b = b;
                    pf->parts[t].f = f;
                }
                pf->f4c = player->f5b4;
                pf->f50 = player->f5b8;
                pf->f54 = FUN_000b21cc(player, &pf->f58, &pf->f53);
                pf->f55 = player->f5b9;
            }
        }
        DAT_0034b5dc->RecordTo(self->camera_replays[self->f5c][self->f6c]);
        self->f6c++;
        shot->f40 = self->f6c;
    } else {
        if (!self->f40) {
            self->f40 = 1;
            FUN_00116f98(DAT_002d9400, DAT_002d9410, 397);
        }
    }
    self->shots[self->f64].f38 = -self->f58;
    self->f58++;
}
