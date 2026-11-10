/* HSTMATCH module=EBOOT.BIN start=0x000ef564 end=0x000ef978 symbol=FUN_000ef564 lang=c++ rodata=0x002c0b18 */
struct Vec4 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct Snd {
    char pad0[0xc];
    int vol;
    char pad1[0x52 - 0x10];
    unsigned char on;
};

struct Coin {
    char pad0[0x55];
    unsigned char playing;
    char pad1[0x78 - 0x56];
    void *head;
    void *tail;
    void *btn0;
    void *btn1;
    char pad2[0x90 - 0x88];
    void *anim;
    void *anims[2];
    char pad3[0xa0 - 0x9c];
    float angle;
    float period;
    float speed;
    float offset;
    unsigned char heads;
    char pad4[0xb8 - 0xb1];
    int state;
    int timer;
    char pad5[0xc5 - 0xc0];
    unsigned char call;
};

struct Rand {
    unsigned seed;
    Rand(unsigned s) : seed(s) {}
    int Next()
    {
        seed = seed * 0x343fd + 0x269ec3;
        return (seed >> 16) & 0x7fff;
    }
};

extern "C" {
extern Snd *DAT_00331b84;
extern unsigned char DAT_003386b8;
extern unsigned DAT_003417c4;
extern unsigned DAT_00310fc8;
extern float DAT_002d725c;
extern short DAT_002d71d4;
extern float DAT_002d71c4;
extern float DAT_002d71c8;
extern float DAT_002d71cc;
extern float DAT_00338bb8;
extern float DAT_00338bbc;
extern char DAT_0034179c;
void FUN_000ffb3c(void *, int);
void FUN_00063830(Snd *, Vec4 *, Vec4 *, int, int, int, float, float);
void FUN_001b1c44(void *, int);
void FUN_00058f38(float, void *, int);
void FUN_00059084(void *, int);
float FUN_0000bd2c(float);
}

inline int GlobalRand()
{
    DAT_00310fc8 = DAT_00310fc8 * 0x343fd + 0x269ec3;
    return (DAT_00310fc8 >> 16) & 0x7fff;
}

inline const float &Min(const float &a, const float &b)
{
    return a < b ? a : b;
}

extern "C" void FUN_000ef564(Coin *self, int state)
{
    float a[2];
    float b;
    self->state = state;
    self->timer = 0;
    switch (self->state) {
    case 0: {
        FUN_000ffb3c(self->btn0, 1);
        FUN_000ffb3c(self->btn1, 1);
        FUN_000ffb3c(self->head, 1);
        FUN_000ffb3c(self->tail, 1);
        FUN_000ef564(self, 1);
        Vec4 a;
        Vec4 b;
        b.w = DAT_002d725c;
        a.w = DAT_002d725c;
        b.x = 0.0f;
        b.y = 0.0f;
        b.z = 0.0f;
        a.x = 0.0f;
        a.y = 0.0f;
        a.z = 0.0f;
        FUN_00063830(DAT_00331b84, &a, &b, -1, 0, 0, DAT_002d71d4, -1.0f);
        self->playing = 1;
        break;
    }
    case 1:
        if (DAT_003386b8 != 0)
            self->heads = self->call != Rand(DAT_003417c4).Next() % 2;
        else
            self->heads = self->call != GlobalRand() % 2;
        self->angle = 0.0f;
        self->period = DAT_002d71c4;
        self->offset = 0.0f;
        self->speed = 0.0f;
        FUN_001b1c44(&DAT_0034179c, 27);
        self->anim = self->anims[!self->heads];
        FUN_00058f38(0.0f, self->anim, -1);
        FUN_00059084(self->anim, -1);
        break;
    case 2: {
        float d = (self->heads ? 720.0f : 540.0f) - self->angle;
        a[0] = 1.0f;
        b = 1.0f;
        if (d > 540.0f)
            d -= 360.0f;
        a[0] = d / self->period * 2.0f;
        b = DAT_002d71c8 + (DAT_002d71cc - DAT_002d71c8) * ((unsigned)GlobalRand() * (1.0f / 32768.0f));
        float n = FUN_0000bd2c(FUN_0000bd2c(Min(a[0], b)) / 2.0f) * 2.0f;
        float k = 1.0f + n;
        float o = DAT_00338bb8;
        o += (d - k * self->period / 2.0f) / self->period;
        float m = k + DAT_00338bbc;
        self->offset = o;
        self->speed = self->period / m;
        break;
    }
    case 3:
        FUN_001b1c44(&DAT_0034179c, 28);
        FUN_000ffb3c(self->btn0, 0);
        FUN_000ffb3c(self->btn1, 0);
        FUN_000ffb3c(self->head, (self->heads != 0) ^ 1);
        FUN_000ffb3c(self->tail, self->heads);
        break;
    case 4:
        FUN_000ffb3c(self->btn0, 1);
        FUN_000ffb3c(self->btn1, 1);
        break;
    case 5: {
        Snd *s = DAT_00331b84;
        s->vol = 0;
        s->on = 0;
        self->playing = 0;
        break;
    }
    }
}
// NEAR 11 (lane pn1b 2026-11-10, was 15): `float o = DAT_00338bb8; o += (d - k * period / 2.0f) / period;` (compound add) gives
// retail's case-2 schedule (bb8 loaded before period, k + bbc after the offset chain); left (9 words) is only FPU numbering:
// retail n f2 / bb8 f1 / period f3, ours n f1 / bb8 f3 / period f2. Ruled out: `o` loaded before k (15) or before n (258),
// no `n` local (11), `k = n + 1.0f` / `k = n; k += 1.0f` (11), a named `t` for the numerator (11), offset/speed stored without
// locals (18), speed computed after the offset store (18).
// Case 3 (2 words): retail loads `heads` (lbu a1) before the `tail` pointer (lw a0 in the jal slot); ruled out: unsigned char/bool
// prototypes (37-39), `!= 0` (37), an int/uchar getter (11), an int local (11).
// improved by lane pn1b (unreviewed): 15 -> 11 differing words
