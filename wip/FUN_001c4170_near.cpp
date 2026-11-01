/* HSTMATCH module=EBOOT.BIN start=0x001c4170 end=0x001c4704 symbol=FUN_001c4170 lang=c++ */

#pragma inline_depth(8)

struct Tag {};

extern "C" {
extern void *DAT_00309320;
extern char DAT_00341d10;
extern void *DAT_0034a118;
extern void *DAT_0034a114;
extern void *DAT_0034b340;
extern void *DAT_0034b344;
extern void *DAT_0034b34c;
extern void *DAT_00310fc4;

void FUN_002a4938();
void FUN_00092ff8(void *);
void *FUN_00000bcc(unsigned int);
void *FUN_00091830(void *);
void FUN_0009191c(void *, void *);
void *FUN_00288a84(void *);
void FUN_000009e4(void *);
void FUN_00289e84(void *);
void FUN_00127788();
void FUN_0012d45c();
void FUN_000429a0(void *);
void FUN_0028aeb4(void *, int);
void FUN_00155ed8(void *, int);
void FUN_00289cf4(void *, int);
void FUN_001ce748(void *, int);
void FUN_0005da44(void *, int);
void FUN_0001f0f0(void *, int);
void FUN_00000a00(void *);
}

struct ItemA {
    virtual ~ItemA();
    virtual void _2();
    virtual void _3();
    virtual void _4();
    virtual void _5();
    virtual void _6();
    virtual void _7();
    virtual void _8();
    virtual void _9();
    virtual void _10();
    virtual void _11();
    virtual int func34();
    char pad[48];
    int f34;
};

template <class T> struct It {
    T *p;
    It() {}
    It(T *x) : p(x) {}
    It& operator++() { ++p; return *this; }
    T& operator*() const { return *p; }
};
template <class T> static inline int operator!=(It<T> a, It<T> b) {
    return (a.p == b.p) ^ 1;
}

template <class T> struct RevIt {
    int unused_;
    It<T> current_;
    explicit RevIt(It<T> i) : current_(i) {}
    T& operator*() const { return current_.p[-1]; }
    RevIt& operator++() { --current_.p; return *this; }
};
template <class T> static inline int operator!=(RevIt<T> a, RevIt<T> b) {
    return (a.current_.p == b.current_.p) ^ 1;
}

template <class T> struct Vec {
    T *data;
    int size;
    int cap;

    It<T> begin() { return It<T>(data); }
    It<T> end() { return It<T>(data + size); }
    RevIt<T> rbegin() { return RevIt<T>(It<T>(data + size)); }
    RevIt<T> rend() { return RevIt<T>(It<T>(data)); }
};

struct Self {
    void *vtable;
    char pad0[56];
    void *member_60;        // 60
    Vec<ItemA *> v64;       // 64
    Vec<void *> v76;        // 76
    Vec<void *> v88;        // 88
    Vec<void *> v100;       // 100
    Vec<void *> v112;       // 112
    Vec<void *> v124;       // 124
    Vec<void *> v136;       // 136
    char pad1[4];           // 148
    int field_98;           // 152
    char pad2[192];         // 156..348
    ItemA *member_348;      // 348
    ItemA *member_352;      // 352
    ItemA *member_356;      // 356
    char pad3[16];          // 360..376
    ItemA *member_376;      // 376
    void *member_380;       // 380
    char pad4[4];           // 384
};

static inline void destroy_vec_1ce748(void *p) {
    if (p) FUN_001ce748(p, 0);
}
static inline void destroy_vec_5da44(void *p) {
    if (p) FUN_0005da44(p, 0);
}

extern "C" Self *FUN_001c4170(Self *self, short deleting_flag) {
    if (self != 0) {
        self->vtable = &DAT_00309320;
        if (DAT_00341d10 != '\0') {
            char *s3 = &DAT_00341d10;
            FUN_002a4938();
            if (self->field_98 == 0) {
                FUN_00092ff8(DAT_0034a118);
                void *mem = FUN_00000bcc(224);
                if (mem != 0) mem = FUN_00091830(mem);
                DAT_0034b340 = mem;
                FUN_0009191c(mem, DAT_0034a118);
            }
            for (It<ItemA *> it = self->v64.begin(); it != self->v64.end(); ++it) {
                ItemA *s5 = *it;
                void *s4 = FUN_00288a84(s5);
                if (s4 != 0) {
                    if (s5->func34() == 3) {
                        if (DAT_0034b344 == 0 && *(short *)(s3 + 54) == s5->f34) {
                            FUN_00092ff8(s4);
                            void *mem = FUN_00000bcc(224);
                            if (mem != 0) mem = FUN_00091830(mem);
                            DAT_0034b344 = mem;
                            FUN_0009191c(mem, s4);
                        }
                        if (DAT_0034b34c == 0 && *(short *)(s3 + 58) == s5->f34) {
                            FUN_00092ff8(s4);
                            void *mem = FUN_00000bcc(224);
                            if (mem != 0) mem = FUN_00091830(mem);
                            ItemA *s5_adj = (s5 != 0) ? (ItemA *)((char *)s5 - 224) : 0;
                            DAT_0034b34c = mem;
                            FUN_0009191c(mem, s5_adj);
                        }
                    }
                }
            }
        }

        for (RevIt<void *> it = self->v100.rbegin(); it != self->v100.rend(); ++it) {
            FUN_000009e4(*it);
        }

        for (RevIt<ItemA *> it = self->v64.rbegin(); it != self->v64.rend(); ++it) {
            ItemA *item = *it;
            if (item != 0) {
                delete item;
            }
        }

        self->v100.size = 0;
        self->v76.size = 0;
        self->v88.size = 0;
        self->v112.size = 0;
        self->v124.size = 0;
        self->v136.size = 0;
        self->v64.size = 0;

        FUN_00289e84((char *)self + 384);
        FUN_00127788();
        FUN_0012d45c();
        FUN_000429a0(DAT_00310fc4);

        if (self->member_380 != 0) {
            FUN_0028aeb4(self->member_380, 1);
            self->member_380 = 0;
        }
        if (self->member_376 != 0) {
            delete self->member_376;
            self->member_376 = 0;
        }
        if (self->member_356 != 0) {
            delete self->member_356;
            self->member_356 = 0;
        }
        if (self->member_352 != 0) {
            delete self->member_352;
            self->member_352 = 0;
        }
        if (self->member_348 != 0) {
            delete self->member_348;
            self->member_348 = 0;
        }
        if (self->member_60 != 0) {
            FUN_00155ed8(self->member_60, 1);
            self->member_60 = 0;
        }

        DAT_0034a118 = 0;
        DAT_0034a114 = 0;

        FUN_00289cf4((char *)self + 384, -1);

        if (&self->v136) destroy_vec_1ce748(&self->v136);
        if (&self->v124) destroy_vec_1ce748(&self->v124);
        if (&self->v112) destroy_vec_1ce748(&self->v112);
        if (&self->v100) destroy_vec_5da44(&self->v100);
        if (&self->v88) destroy_vec_1ce748(&self->v88);
        if (&self->v76) destroy_vec_1ce748(&self->v76);
        if (&self->v64) destroy_vec_1ce748(&self->v64);

        FUN_0001f0f0(self, 0);

        if (deleting_flag > 0) {
            FUN_00000a00(self);
        }
    }
    return self;
}

// 357/357 words, 181 differ, first diff at +0x0 (frame size 160 vs 176).
// Destructor tail (0x1c4634..0x1c46fc) matches byte-for-byte: 7 vector member destructors + base class dtor.
// Blocked by MSL reverse iterator temporaries allocation order and stack frame padding (16 bytes).
// near miss from lane dsn6 (unreviewed)
