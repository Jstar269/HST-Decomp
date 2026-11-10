/* HSTMATCH module=EBOOT.BIN start=0x001f2264 end=0x001f25a4 symbol=FUN_001f2264 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Slot {
    unsigned char done;
    char pad[0x120 - 1];
};

struct Pos {
    char pad[0x60];
    float x;
    float y;
};

struct Layer {
    Pos *pos;
};

struct Ctx {
    char pad[4];
    Layer *layer;
    char pad08[0x74 - 8];
    float sx;
    float sy;
};

struct Fade {
    char pad[0xc];
    float level;
};

struct Board {
    char pad[0x98];
    unsigned char shown;
};

struct Screen {
    char pad00[0x44];
    int started;
    char pad48[0x3cc - 0x48];
    int slot;
    char pad3d0[0x510 - 0x3d0];
    void *view;
    char pad514[0x560 - 0x514];
    void *effect;
    char pad564[0x584 - 0x564];
    Board *board;
    char pad588[0x5a0 - 0x588];
    unsigned char win;
    unsigned char draw;
    unsigned char skip;
    char pad5a3[0x819c - 0x5a3];
    int state;
    int count;
};

extern "C" {
extern Slot DAT_003418bc[];
extern void *DAT_003415d4;
extern Ctx *DAT_003415ac;
extern Fade *DAT_00331b84;
void FUN_001e7bc0(void *);
void FUN_001fa35c(Screen *);
void FUN_001faa10(Screen *);
void FUN_001f3770(Screen *);
void FUN_001ff668(void);
void FUN_001e1614(void *);
void FUN_001905b8(void *, int);
int FUN_00190724(void *);
void FUN_00063b20(float, Fade *, int);
void FUN_00063c14(float, Fade *, int);
void FUN_001f4bbc(Screen *);
void FUN_001e075c(void *);
void FUN_00216de0(void *);
void FUN_001f41e8(Screen *);
void FUN_001e09dc(void *, int);
void FUN_001e2668(void *);
void FUN_001e7c3c(void *);
void FUN_001f4bfc(Screen *);
void FUN_001f483c(Screen *);
}

static inline void SetPos(float y)
{
    Ctx *c = DAT_003415ac;
    c->layer->pos->x = 240.0f;
    c->layer->pos->y = y;
}

extern "C" void FUN_001f2264(Screen *self)
{
    if (self->started == 0) {
        FUN_001e7bc0(self->view);
        bool fresh = true;
        if (DAT_003418bc[self->slot].done != 0)
            fresh = false;
        if (fresh) {
            FUN_001fa35c(self);
            FUN_001faa10(self);
        }
        if (self->skip != 0 || !fresh) {
            FUN_001f3770(self);
            self->state = 2;
            self->count = 0;
        } else {
            FUN_001ff668();
            FUN_001e1614(self->view);
            FUN_001905b8(DAT_003415d4, 3);
            FUN_001ff668();
            SetPos(168.0f);
            self->state = 0;
            self->count = 0;
        }
    }
    switch (self->state) {
    case 0:
        self->state = 1;
        self->count = 0;
        break;
    case 1:
        if (FUN_00190724(DAT_003415d4) != 0) {
            Ctx *c = DAT_003415ac;
            if (c != 0) {
                c->sx = 1.0f;
                c->sy = 1.0f;
                SetPos(136.0f);
            }
            if (self->win != 0 || self->draw != 0) {
                FUN_00063b20(65535.0f, DAT_00331b84, 5);
                self->state = 3;
                self->count = 0;
            } else {
                FUN_001f3770(self);
                self->state = 2;
                self->count = 0;
            }
        }
        break;
    case 2:
        break;
    case 3:
        self->count++;
        if (DAT_00331b84->level == 0.0f && self->count > 6) {
            FUN_001f4bbc(self);
            FUN_001e075c(self->view);
            self->board->shown = 1;
            FUN_001f3770(self);
            FUN_00216de0(self->effect);
            FUN_001ff668();
            self->state = 2;
            self->count = 0;
            FUN_00063c14(65535.0f, DAT_00331b84, 5);
            FUN_001f41e8(self);
            FUN_001e09dc(self->view, 2);
            FUN_001e2668(self->view);
            FUN_001e7c3c(self->view);
        }
        break;
    }
    FUN_001f4bfc(self);
    FUN_001f483c(self);
}
