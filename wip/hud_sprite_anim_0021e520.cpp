/* HSTMATCH module=EBOOT.BIN start=0x0021e520 end=0x0021e90c symbol=FUN_0021e520 lang=c++ */
struct Sprite {
    unsigned int color;
    short x0;
    short y0;
    short x1;
    short y1;
};

struct Anim {
    virtual void a0();
    virtual void a1();
    virtual void Update();
};

struct Item {
    virtual void i0();
    virtual void Show(int);
};

struct Row {
    void *label;
    void *value;
    char pad[0x10];
};

struct Hud {
    char pad0[0x60];
    Row rows[3];
    char pad1[4];
    Item *items[3];
    Sprite *spr[9];
    int left[9];
    int right[9];
    int top[9];
    int bottom[9];
    char pad2[0x174 - 0x16c];
    Anim anim;
    char pad3[0x1c8 - 0x178];
    int sel;
    int rank;
    char pad4[0x1e8 - 0x1d0];
    int frame;
};

extern "C" {
extern const float DAT_002f087c;
extern const float DAT_002f0880;
extern const float DAT_002f0884;
extern const float DAT_002f0888;
extern const int DAT_002f088c[16];
extern const float DAT_002f08cc;
extern const float DAT_002f08d0;
void FUN_000ffb3c(void *, int);
}

inline unsigned int Rgba(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    return r | (g << 8 | (a << 24 | b << 16));
}

inline int Curve(int k)
{
    if (k < 16)
        return DAT_002f088c[k];
    return 128;
}


extern "C" void FUN_0021e520(Hud *self)
{
    self->anim.Update();
    self->frame++;
    if (self->frame >= DAT_002f087c) {
        self->items[self->sel]->Show(1);
        FUN_000ffb3c(self->rows[self->sel].label, self->rank < 10);
        FUN_000ffb3c(self->rows[self->sel].value, 0);
    } else {
        int i = self->sel * 3;
        float t = self->frame;
        int k = (int)(t * DAT_002f08cc);
        int fade = (int)(t * DAT_002f08d0);
        float scale = k < 16 ? DAT_002f088c[k] / 128.0f : 1.0f;
        float grow = -DAT_002f0880 + DAT_002f0880 * scale;
        int c0 = k < 16 ? DAT_002f088c[k] : 128;
        int a = (int)(float)(k < 16 ? DAT_002f088c[k] : 128);
        float w = grow * 15.0f / 2.0f;
        self->spr[i]->x0 = (short)(self->left[i] - w);
        float h = grow * 19.0f / 2.0f;
        self->spr[i]->x1 = (short)(self->right[i] + w);
        self->spr[i]->y0 = (short)(self->top[i] - h);
        self->spr[i]->y1 = (short)(self->bottom[i] + h);
        unsigned int col = Rgba(c0, c0, c0, a);
        self->spr[i]->color = col;
        float s = DAT_002f0884 * fade / DAT_002f087c;
        self->spr[i + 1]->x0 = (short)(self->left[i + 1] - w);
        self->spr[i + 1]->x1 = (short)(self->right[i + 1] + w);
        self->spr[i + 1]->y0 = (short)(self->top[i + 1] - h);
        self->spr[i + 1]->y1 = (short)(self->bottom[i + 1] + h);
        self->spr[i + 1]->color = col;
        w = s * 95.0f / 2.0f;
        h = s * 60.0f / 2.0f;
        float alpha = DAT_002f0888 - DAT_002f0888 * fade / DAT_002f087c;
        self->spr[i + 2]->x0 = (short)(self->left[i + 2] - w);
        self->spr[i + 2]->x1 = (short)(self->right[i + 2] + w);
        self->spr[i + 2]->y0 = (short)(self->top[i + 2] - h);
        self->spr[i + 2]->y1 = (short)(self->bottom[i + 2] + h);
        self->spr[i + 2]->color = (self->spr[i + 2]->color & 0xffffff) | ((unsigned char)(unsigned int)alpha << 24);
    }
}
// NEAR 121 (lane w4f1, was 164): integer registers and both branch shapes now match retail up to +0x138
// (compare on self->frame directly with the int->float conversion CSE'd fixed the top; ai preassigned
// with Rgba(u8,u8,u8,u8) gives the beqzl/andi placement). What differs: FP register colouring after
// the third ternary (ours grow=f8, (float)ai cvt'd immediately into f6; retail grow=f6, cvt late into f5,
// DAT_087c/0884/0888 in f7/f4/f3), which then reorders the whole sprite block schedule.
// Ruled out: ternary vs if/else for ai, Curve() inline for the table reads, late/early (float)ai,
// s/alpha computed at top or late, w2/h2 new locals, h before w, DAT globals read into locals (194+),
// float fade local (165).
// improved by lane w4f1 (unreviewed): 164 -> 121 differing words
