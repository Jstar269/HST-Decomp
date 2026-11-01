/* HSTMATCH module=EBOOT.BIN start=0x0026ce14 end=0x0026d224 symbol=FUN_0026ce14 lang=c++ rodata=0x002bc410 */

typedef unsigned int uint;

extern "C" {
extern void *DAT_00309bdc;
extern void *DAT_00309bf8;
extern int DAT_00331b84;
extern int DAT_003386c8;
extern int DAT_0034179c;
extern int DAT_003415ac;

void FUN_00063478(int, float);
void FUN_001b2ee0(int *);
int FUN_0008d994(int);
void FUN_00103540(int, float);
void FUN_00293ae0(void *, int);
void FUN_0005ba00(void *, int);
void FUN_0027a2f4(void *, void *, void *, void *);
void FUN_0027a5e0(void *, int);
void FUN_0027a530(void *, int);
void FUN_00102ea4(void *, int);
void FUN_0001f0f0(void *, int);
void FUN_00000a00(void *);
}

struct Item {
    virtual ~Item();
};

struct Member27a530 {
    ~Member27a530() {
        if (this) {
            FUN_0027a530(this, 0);
        }
    }
    char pad[12];
};

struct Member27a5e0 {
    ~Member27a5e0() {
        if (this) {
            FUN_0027a5e0(this, 0);
        }
    }
    char pad[12];
};

struct Member27a2f4 {
    char pad[12];
    char *p_e8;
    int f_ec;
    ~Member27a2f4() {
        if (this) {
            char *sp20 = p_e8;
            int sp24 = f_ec;
            char *sp28;
            FUN_0027a2f4(&sp28, this, &sp24, &sp20);
        }
    }
};

struct Mat4 {
    float m[4][4];
};

static const Mat4 DAT_002bc410 = {{{0}}};

extern "C" void *FUN_0026ce14(void *self, short flags)
{
    if (self != 0) {
        *(void **)self = &DAT_00309bdc;
        *(void **)((char *)self + 60) = &DAT_00309bf8;

        FUN_00063478(DAT_00331b84, 203000.0f);

        void *p = *(void **)((char *)self + 240);
        if (p != 0) {
            *(void **)((char *)self + 240) = 0;
            delete (Item *)p;
            *(void **)((char *)self + 240) = 0;
        }

        p = *(void **)((char *)self + 252);
        if (p != 0) {
            *(void **)((char *)self + 252) = 0;
            delete (Item *)p;
            *(void **)((char *)self + 252) = 0;
        }

        p = *(void **)((char *)self + 2572);
        if (p != 0) {
            *(void **)((char *)self + 2572) = 0;
            delete (Item *)p;
            *(void **)((char *)self + 2572) = 0;
        }

        p = *(void **)((char *)self + 256);
        if (p != 0) {
            *(void **)((char *)self + 256) = 0;
            delete (Item *)p;
            *(void **)((char *)self + 256) = 0;
        }

        p = *(void **)((char *)self + 244);
        if (p != 0) {
            *(void **)((char *)self + 244) = 0;
            delete (Item *)p;
            *(void **)((char *)self + 244) = 0;
        }

        p = *(void **)((char *)self + 2620);
        if (p != 0) {
            if (p != 0) {
                *(void **)((char *)self + 2620) = 0;
                delete (Item *)p;
                *(void **)((char *)self + 2620) = 0;
            }
        }

        FUN_001b2ee0(&DAT_0034179c);

        int obj = FUN_0008d994(DAT_003386c8);
        if (obj != 0) {
            FUN_00103540(obj, 100000.0f);
        }

        char *v0 = (char *)DAT_003415ac;
        char *a1 = *(char **)(v0 + 4);
        char *dst = *(char **)a1;
        if (dst != 0) {
            dst += 48;
        }
        *(Mat4 *)dst = DAT_002bc410;

        char *a3 = *(char **)(*(char **)(v0 + 4));
        *(float *)(a3 + 96) = 240.0f;
        *(float *)(a3 + 100) = 136.0f;
        *(float *)(a3 + 96) = *(float *)(a3 + 96);
        *(float *)(a3 + 100) = *(float *)(a3 + 100);
        *(float *)(v0 + 116) = 1.0f;
        *(float *)(v0 + 120) = 1.0f;

        FUN_00293ae0((char *)self + 2336, -1);
        FUN_0005ba00((char *)self + 400, -1);

        char *m = (char *)self + 228;
        if (m != 0) {
            char *sp20 = (char *)self + 232;
            int sp24 = *(int *)((char *)self + 236);
            char *sp28;
            FUN_0027a2f4(&sp28, (char *)self + 216, &sp24, &sp20);
        }

        FUN_0027a5e0((char *)self + 204, 0);
        FUN_0027a530((char *)self + 192, 0);
        FUN_0027a530((char *)self + 180, 0);
        FUN_0027a530((char *)self + 168, 0);
        FUN_0027a530((char *)self + 156, 0);
        FUN_0027a530((char *)self + 144, 0);
        FUN_0027a530((char *)self + 60, 0);
        FUN_00102ea4((char *)self + 60, 0);
        FUN_0001f0f0(self, 0);

        if (flags > 0) {
            FUN_00000a00(self);
        }
    }
    return self;
}
// Near miss: 246 words differ out of 260 words.
// Deleting destructor with primary vtable at 0(self) and secondary at 60(self).
// Deletes 6 sub-objects at offsets 240, 252, 2572, 256, 244, 2620 with intermediate zeroing.
// Writes 4x4 zero matrix from rodata (0x002bc410) into *(DAT_003415ac + 4) + 48.
// Member destructors run at 216 (FUN_0027a2f4), 204 (FUN_0027a5e0), 192..144 (FUN_0027a530), followed by base destructors 0x102ea4 and 0x1f0f0.
// near miss from lane dsn5 (unreviewed)
