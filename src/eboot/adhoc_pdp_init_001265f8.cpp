/* HSTMATCH module=EBOOT.BIN start=0x001265f8 end=0x00126864 symbol=FUN_001265f8 lang=c++ */
/* Hand-written reconstruction from the disassembly (types from the lane ag4 draft); exact match verified; names and types await human review. */

struct Stream {
    virtual ~Stream();
    char pad[16];
    Stream(int cap);
};

struct ThreadArgs {
    int pdp_id;      // 0x00 (24(sp))
    unsigned short *p_u16; // 0x04 (28(sp))
    unsigned char *p_u8;   // 0x08 (32(sp))
    int semaid;      // 0x0c (36(sp))
    int pad0;        // 0x10 (40(sp))
    int pad1;        // 0x14 (44(sp))
    int pad2;        // 0x18 (48(sp))
    unsigned int param_3; // 0x1c (52(sp))
    Stream *s1;      // 0x20 (56(sp))
    Stream *s2;      // 0x24 (60(sp))
};

struct Obj {
    char pad[4];
    int thid;        // 0x04
    int semaid;      // 0x08
    int pdp_id;      // 0x0c
    unsigned short *p_u16; // 0x10
    unsigned char *p_u8;   // 0x14
    Stream *s1;      // 0x18
    Stream *s2;      // 0x1c
};

extern "C" {
    int FUN_002b7e10(int, unsigned short, int, unsigned char);
    void *__nw__FUi(unsigned int);
    void __dl__FPv(void *);
    Stream *FUN_000ff4fc(void *, int);
    int FUN_002b7890(const char *, int, int, int, int);
    void FUN_001264f8(const char *, ...);
    int FUN_002b7920(const char *, void (*)(), int, int, int, int);
    void FUN_0012630c();
    int FUN_002b78d8(int, int, void *);
    int FUN_002b79a8(int);
    int FUN_002b78e8(int);
    extern const char DAT_002da2cc;
    extern const char DAT_002da2dc;
    extern const char DAT_002da308;
    extern const char DAT_002da318;
    extern const char DAT_002da344;
    extern const char DAT_002da370;
    extern const char DAT_002da284;
}

extern "C" int FUN_001265f8(Obj *self, int param_2, unsigned int param_3, int param_4, int param_5)
{
    ThreadArgs args;

    args.pdp_id = self->pdp_id = FUN_002b7e10(param_2, (unsigned short)param_3, param_4, param_5);

    unsigned short *w = new unsigned short(0);
    self->p_u16 = w;
    args.p_u16 = w;
    unsigned char *b = new unsigned char(0);
    self->p_u8 = b;
    args.p_u8 = b;
    Stream *s = (Stream *)__nw__FUi(0x14);
    if (s != 0) {
        s = FUN_000ff4fc(s, 0x4000);
    }
    self->s1 = s;
    args.s1 = s;
    s = (Stream *)__nw__FUi(0x14);
    if (s != 0) {
        s = FUN_000ff4fc(s, 0x4000);
    }
    self->s2 = s;
    args.s2 = s;

    args.param_3 = param_3;
    int sema = FUN_002b7890(&DAT_002da2cc, 0, 1, 1, 0);
    if (sema <= 0) {
        FUN_001264f8(&DAT_002da2dc, sema);
    } else {
        args.semaid = self->semaid = sema;
        int th = FUN_002b7920(&DAT_002da308, FUN_0012630c, 0x29, 0x2000, 0, 0);
        if (th <= 0) {
            FUN_001264f8(&DAT_002da318, th);
        } else {
            self->thid = th;
            int ret = FUN_002b78d8(th, 0x28, &args);
            if (ret != 0) {
                FUN_001264f8(&DAT_002da344, ret);
            } else {
                return 1;
            }
        }
    }

    if (self->thid > 0) {
        int ret = FUN_002b79a8(self->thid);
        if (ret != 0) {
            FUN_001264f8(&DAT_002da370, ret);
        }
        self->thid = -1;
    }
    if (self->semaid > 0) {
        int ret = FUN_002b78e8(self->semaid);
        if (ret != 0) {
            FUN_001264f8(&DAT_002da284, ret);
        }
        self->semaid = -1;
    }
    if (self->s2) {
        delete self->s2;
        self->s2 = 0;
    }
    if (self->s1) {
        delete self->s1;
        self->s1 = 0;
    }
    if (self->p_u8) {
        delete self->p_u8;
        self->p_u8 = 0;
    }
    if (self->p_u16) {
        delete self->p_u16;
        self->p_u16 = 0;
    }
    return 0;
}
