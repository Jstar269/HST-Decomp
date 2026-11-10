/* HSTMATCH module=EBOOT.BIN start=0x00291d40 end=0x00292e38 symbol=FUN_00291d40 lang=c++ bss=0x0034a824 */
struct Color {
    union {
        struct {
            unsigned char r, g, b, a;
        } c;
        unsigned int rgba;
    } u;
    Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
    {
        u.c.r = r;
        u.c.g = g;
        u.c.b = b;
        u.c.a = a;
    }
};

struct Tex {
    char pad0[0x18];
    float z;
};

struct Fade {
    char pad0[4];
    float duration;
    char pad8[0x14 - 0x8];
    Tex *tex;
    char pad18[0x24 - 0x18];
    Tex *tex2;
    char pad28[0x3c - 0x28];
    float step;
    unsigned char active;
    unsigned char started;
    char pad42[2];
    int dir;
    int type;
    void DrawSplitV(Color c, int x, int y, int r);
    void DrawSplitH(Color c, int x, int y, int r);
};

extern "C" {
extern int DAT_002ffad8[];
extern float DAT_002ffaf4, DAT_002ffaf8;
extern int DAT_002ffafc;
extern float DAT_002ffb00, DAT_002ffb04, DAT_002ffb08, DAT_002ffb0c;
extern int DAT_002ffb10, DAT_002ffb14, DAT_002ffb18, DAT_002ffb1c;
extern float DAT_002ffb20, DAT_002ffb24, DAT_002ffb28, DAT_002ffb2c, DAT_002ffb30, DAT_002ffb34, DAT_002ffb38;
extern float DAT_002ffb3c[8];
extern float DAT_002ffb5c, DAT_002ffb60, DAT_002ffb64, DAT_002ffb68, DAT_002ffb6c, DAT_002ffb70, DAT_002ffb74;
extern char DAT_002ffb78[];
extern float DAT_0034a82c[8];
extern float DAT_0034a834, DAT_0034a838;
extern char DAT_0034179c[];
extern char DAT_00311170[];
float FUN_00292e38(Fade *);
void FUN_001b28f0(void *, char *);
void FUN_000503d0(char *, float);
Tex *FUN_00050534(char *);
void FUN_00051398(Tex *, float);
void FUN_00051490(Tex *, int, int, int, int, int, int);
void FUN_00051894(Tex *, Color, float, float, float, float, float, float, float, float, float);
void FUN_00051da0(Tex *, Color, Color, Color, float, float, float, float, float, float, float, float, float, float,
                  float, float, float, float, float);
void FUN_0005185c(Tex *);
}

inline void Fade::DrawSplitV(Color c, int x, int y, int r)
{
    FUN_00051894(tex, c, 0.0f, 0.0f, (float)(x - r), (float)(y - r), 127.0f, 255.0f, (float)x, (float)(y + r), 65535.0f);
    FUN_00051894(tex, c, 127.0f, 0.0f, (float)x, (float)(y - r), 0.0f, 255.0f, (float)(x + r), (float)(y + r), 65535.0f);
}

inline void Fade::DrawSplitH(Color c, int x, int y, int r)
{
    FUN_00051894(tex, c, 0.0f, 0.0f, (float)(x - r), (float)(y - r), 255.0f, 127.0f, (float)(x + r), (float)y, 65535.0f);
    FUN_00051894(tex, c, 0.0f, 127.0f, (float)(x - r), (float)y, 255.0f, 0.0f, (float)(x + r), (float)(y + r), 65535.0f);
}

static inline void DrawScreen(Tex *t, Color c, float x, float y, float scale)
{
    float u = 0.0f;
    for (int i = 0; i < 4; i++) {
        FUN_00051894(t, c, u, 0.0f, x, y, u + 120.0f, 272.0f, x + 120.0f * scale, y + 272.0f * scale, 65534.0f);
        u += 120.0f;
        x += 120.0f * scale;
    }
}

static inline void DrawWhite(Tex *t, Color c)
{
    float u = 0.0f;
    for (int i = 0; i < 4; i++) {
        FUN_00051894(t, c, u, 0.0f, u, 0.0f, u + 120.0f, 272.0f, u + 120.0f, 272.0f, 65534.0f);
        u += 120.0f;
    }
}

static inline void DrawJet(Tex *t3, Color c3, int x, int y, float s1, float s2)
{
    FUN_00051da0(t3, c3, c3, c3,
                 32.0f, 32.0f, (float)(x - 128) + DAT_0034a834, (float)(y - 128) + DAT_0034a838, 65535.0f,
                 0.0f, 32.0f, -1.0f, (float)(y - 128) + DAT_0034a838, 65535.0f,
                 0.0f, 0.0f, -1.0f,
                 (float)(y - 128) + DAT_0034a838 + s1 * ((float)(x - 128) + DAT_0034a834), 65535.0f);
    FUN_00051da0(t3, c3, c3, c3,
                 0.0f, 0.0f, (float)(x - 128) + DAT_0034a834, (float)(y - 128) + DAT_0034a838, 65535.0f,
                 0.0f, 32.0f, (float)(x - 128) + DAT_0034a834, 273.0f, 65535.0f,
                 32.0f, 32.0f,
                 (float)(x - 128) + DAT_0034a834 + s2 * (272.0f - ((float)(y - 128) + DAT_0034a838)),
                 273.0f, 65535.0f);
    FUN_00051894(t3, c3, 0.0f, 0.0f, -8.0f, (float)(y - 128) + DAT_0034a838 - 2.0f, 32.0f, 32.0f,
                 (float)(x - 128) + DAT_0034a834 + 2.0f, 280.0f, 65535.0f);
}

static inline float Clamp(const float &v, const float &lo, const float &hi)
{
    return v < lo ? lo : (!(v > hi) ? v : hi);
}

extern "C" void FUN_00291d40(Fade *self)
{
    int type = self->type;
    bool is4 = type == 4;
    bool is1 = type == 1;
    bool is6 = type == 6;
    if (self->tex != 0 && self->duration > 0.0f && FUN_00292e38(self) <= 60.0f) {
        float k = 1.0f;
        Color col(128, 128, 128, 128);
        if (self->active && !is4 && !is1) {
            static Color dim(128, 128, 128, 1);
            col = dim;
        }
        int max = DAT_002ffad8[self->type];
        int r = 0;
        int x = 240;
        int y = 136;
        float t = FUN_00292e38(self);
        if (t <= 31.0f) {
            float c = Clamp((31.0f - t) / self->duration, DAT_002ffaf4, DAT_002ffaf8);
        } else if (is4) {
            float c;
            if (t <= 45.0f) {
                c = Clamp((t - 31.0f) / (self->duration / 2.0f), DAT_002ffb00, DAT_002ffb04);
                x = (int)(240.0f - (float)DAT_002ffafc * (1.0f - c * c));
            } else {
                c = Clamp((61.0f - t) / (self->duration / 2.0f), DAT_002ffb08, DAT_002ffb0c);
                x = (int)(240.0f + (float)DAT_002ffafc * (1.0f - c * c));
            }
            r = (int)((float)max * (c * c));
        } else if (is1) {
            float c = Clamp((t - 31.0f) / self->duration, DAT_002ffb28, DAT_002ffb2c);
            x = (int)((float)DAT_002ffb10 + (float)DAT_002ffb18 * c);
            y = (int)((float)DAT_002ffb14 + (float)DAT_002ffb1c * c);
            k = DAT_002ffb20 + DAT_002ffb24 * c;
            r = (int)(128.0f * k);
            if (t < DAT_002ffb30 && t + self->step >= DAT_002ffb30)
                FUN_001b28f0(DAT_0034179c, DAT_002ffb78);
        } else {
            float c = Clamp((t - 31.0f) / self->duration, DAT_002ffb34, DAT_002ffb38);
            r = (int)((float)max * (c * c));
        }
        if (r != max || self->active) {
            FUN_00051490(self->tex, 0, 0, 480, 272, 0, 0);
            if (is4) {
                if (r > 0)
                    FUN_00051894(self->tex, col, 0.0f, 0.0f, (float)(x - r), (float)(y - r), 127.0f, 127.0f,
                                 (float)(x + r), (float)(y + r), 65535.0f);
            } else if (is1) {
                if (r > 0)
                    FUN_00051894(self->tex, col, 0.0f, 0.0f, (float)(x - r), (float)(y - r), 255.0f, 255.0f,
                                 (float)(x + r), (float)(y + r), 65535.0f);
            } else {
                if (r > 0) {
                    switch (self->dir) {
                    case 0:
                        FUN_00051894(self->tex, col, 0.0f, 0.0f, (float)(x - r), (float)(y - r), 255.0f, 255.0f,
                                     (float)(x + r), (float)(y + r), 65535.0f);
                        break;
                    case 1:
                        self->DrawSplitV(col, x, y, r);
                        break;
                    case 2:
                        self->DrawSplitH(col, x, y, r);
                        break;
                    }
                }
                int e = y - r + 1;
                if (e > 0)
                    FUN_00051894(self->tex, col, 0.0f, 0.0f, 0.0f, 0.0f, 16.0f, 16.0f, 480.0f, (float)e, 65535.0f);
                e = y + r - 1;
                if (e < 272)
                    FUN_00051894(self->tex, col, 0.0f, 0.0f, 0.0f, (float)e, 16.0f, 16.0f, 480.0f, 272.0f, 65535.0f);
                e = x - r + 1;
                if (e > 0)
                    FUN_00051894(self->tex, col, 0.0f, 0.0f, 0.0f, 0.0f, 16.0f, 16.0f, (float)e, 272.0f, 65535.0f);
                e = x + r - 1;
                if (e < 480)
                    FUN_00051894(self->tex, col, 0.0f, 0.0f, (float)e, 0.0f, 16.0f, 16.0f, 480.0f, 272.0f, 65535.0f);
            }
            FUN_0005185c(self->tex);
        }
        if (self->active && !(is4 && t > 45.0f)) {
            if (!self->started) {
                FUN_000503d0(DAT_00311170, (float)(is4 || is1 ? -1010 : 0) + self->tex->z);
                self->started = 1;
            }
            Color white(128, 128, 128, 128);
            Tex *t2 = FUN_00050534(DAT_00311170);
            FUN_00051398(t2, (float)(is4 || is1 ? -1000 : 1000) + self->tex->z);
            if (t2) {
                FUN_00051490(t2, 0, 0, 480, 272, 0, 0);
                DrawWhite(t2, white);
                FUN_0005185c(t2);
            }
        }
        if (is1) {
            Color c3(128, 128, 128, 1);
            Tex *t3 = self->tex2;
            FUN_00051398(t3, self->tex->z - 10000.0f);
            float s1 = -(DAT_002ffb3c[3] - DAT_002ffb3c[1]) / (DAT_002ffb3c[2] - DAT_002ffb3c[0]);
            float s2 = (DAT_002ffb3c[6] - DAT_002ffb3c[4]) / (DAT_002ffb3c[7] - DAT_002ffb3c[5]);
            for (int i = 0; i < 8; i++)
                DAT_0034a82c[i] = 128.0f + k * (DAT_002ffb3c[i] - 128.0f);
            FUN_00051490(t3, 0, 0, 480, 272, 0, 0);
            DrawJet(t3, c3, x, y, s1, s2);
            FUN_0005185c(t3);
        }
    } else if (is6) {
        float t = FUN_00292e38(self);
        float c;
        if (t <= 31.0f)
            c = Clamp((31.0f - t) / (self->duration * DAT_002ffb64), DAT_002ffb68, DAT_002ffb6c);
        else
            c = Clamp((t - 31.0f) / (self->duration * DAT_002ffb64), DAT_002ffb70, DAT_002ffb74);
        float a = 1.0f + DAT_002ffb5c * c;
        float b = 1.0f + DAT_002ffb60 * c;
        if (!self->started) {
            FUN_000503d0(DAT_00311170, 203010.0f);
            self->started = 1;
        }
        Color col(128, 128, 128, 128.0f * b);
        Tex *tex = FUN_00050534(DAT_00311170);
        FUN_00051398(tex, 204010.0f);
        if (tex) {
            float x0 = 240.0f * (1.0f - a);
            float y0 = 136.0f * (1.0f - a);
            FUN_00051490(tex, 0, 0, 480, 272, 0, 0);
            DrawScreen(tex, col, x0, y0, a);
            FUN_0005185c(tex);
        }
    }
}
// NEAR MISS (lane own2): 111 of 1086 words differ (hstmatch). Structure, calls, branches and the float clamps match.
// What still differs: (1) frame 160 vs retail 128: with the 8-float lerp written as a loop (the only spelling that
//   gives retail's loads-before-stores schedule), every by-value Color parameter of the inline helpers
//   (DrawSplitV/H, DrawWhite, DrawJet, DrawScreen) keeps a 4-byte slot (sp offsets of all colours +32);
//   explicit statements (scalar externs, s1 first, s2 between stores) keep frame 128 but interleave loads/stores (161).
// (2) is6 tail: x0 = 240*(1-a) is computed after FUN_00051490 (retail before, with y0), registers f20/f21/f22 swapped.
// (3) white loop / DrawJet: the colour copy `lw s0` is scheduled before FUN_00051490 (retail after), arg-setup order.
// (4) lerp block: lui register naming and the B4c/B54 load order.
// Ruled out: const source table (s1/s2 sunk past the call, 4 extra saved FPRs), struct/array source (1088 words),
//   local struct copy, Vec2-style loop (119), loop in an inline helper (1102 words), scalar unsigned colour params
//   (copy-propagated back to per-call stack loads), helpers that contain FUN_00051490 (166).
// The colour must reach the draws through by-value Color params of inline helpers (only spelling found that keeps it in
//   s0/s2 across calls); Color needs the union with an unsigned int so copies are lw/sw.
// near miss from lane own2 (unreviewed)
