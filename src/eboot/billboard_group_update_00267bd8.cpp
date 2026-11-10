/* HSTMATCH module=EBOOT.BIN start=0x00267bd8 end=0x00267f24 symbol=FUN_00267bd8 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Vec4 {
    float x, y, z, w;
};

union Mat4 {
    Vec4 r[4];
    float m[16];
};

struct Cam {
    char pad[0x1d8];
    float height;
    char pad1dc[0x1e0 - 0x1dc];
    Vec4 fwd;
    Vec4 up;
};

struct Item {
    char pad[0x10];
    Vec4 pos;
    char pad20[4];
    void *anim;
};

struct Sys {
    char pad[0x8c];
    int f8c;
};

struct Group {
    char pad[0x4c];
    unsigned char on;
    char pad4d[3];
    Item **items;
    unsigned int count;
    char pad58[4];
    float lo;
    float hi;
    unsigned char hidden;
};

extern "C" {
extern Mat4 DAT_002bc410;
extern Cam *DAT_003313c4;
extern Sys *DAT_00340284;
extern int DAT_00338714;
void FUN_000224f8(Vec4 *, Vec4 *, Vec4 *);
void FUN_00022518(Vec4 *, Vec4 *);
void FUN_00055044(void *, Mat4 *);
int FUN_00131cf8(Sys *);
void FUN_0005f1e8(void *, int);
}

#pragma opt_strength_reduction off
extern "C" void FUN_00267bd8(Group *self)
{
    Mat4 m;
    m.m[0] = DAT_002bc410.m[0];
    m.m[1] = DAT_002bc410.m[1];
    m.m[2] = DAT_002bc410.m[2];
    m.m[3] = DAT_002bc410.m[3];
    m.m[4] = DAT_002bc410.m[4];
    m.m[5] = DAT_002bc410.m[5];
    m.m[6] = DAT_002bc410.m[6];
    m.m[7] = DAT_002bc410.m[7];
    m.m[8] = DAT_002bc410.m[8];
    m.m[9] = DAT_002bc410.m[9];
    m.m[10] = DAT_002bc410.m[10];
    m.m[11] = DAT_002bc410.m[11];
    m.m[12] = DAT_002bc410.m[12];
    m.m[13] = DAT_002bc410.m[13];
    m.m[14] = DAT_002bc410.m[14];
    m.m[15] = DAT_002bc410.m[15];
    m.r[1] = DAT_003313c4->up;
    m.r[2] = DAT_003313c4->fwd;
    Vec4 c;
    FUN_000224f8(&c, &m.r[1], &m.r[2]);
    m.r[0] = c;
    m.r[1].x = 0.0f;
    m.r[1].y = 1.0f;
    m.r[1].z = 0.0f;
    m.r[1].w = 1.0f;
    m.r[0].y = 0.0f;
    m.r[2].y = 0.0f;
    Vec4 n0;
    FUN_00022518(&n0, &m.r[0]);
    n0.w = 0.0f;
    m.r[0] = n0;
    Vec4 n2;
    FUN_00022518(&n2, &m.r[2]);
    n2.w = 0.0f;
    m.r[2] = n2;
    if (self->on == 0)
        return;
    for (unsigned int i = 0; i < self->count; i++) {
        m.r[3] = self->items[i]->pos;
        FUN_00055044(self->items[i]->anim, &m);
        if (DAT_00340284->f8c == 0 || FUN_00131cf8(DAT_00340284) != 0) {
            if (DAT_003313c4->height > self->lo) {
                if (self->items[i]->pos.z > self->lo) {
                    self->hidden = 1;
                } else {
                    FUN_0005f1e8(self->items[i]->anim, DAT_00338714);
                    self->hidden = 0;
                }
            } else if (DAT_003313c4->height < self->hi) {
                if (self->items[i]->pos.z < self->hi) {
                    self->hidden = 1;
                } else {
                    FUN_0005f1e8(self->items[i]->anim, DAT_00338714);
                    self->hidden = 0;
                }
            } else {
                FUN_0005f1e8(self->items[i]->anim, DAT_00338714);
                self->hidden = 0;
            }
        } else {
            FUN_0005f1e8(self->items[i]->anim, DAT_00338714);
            self->hidden = 0;
        }
    }
}
