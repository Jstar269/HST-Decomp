/* HSTMATCH module=hstp_trans_main.prx start=0x00131868 end=0x00131ad4 symbol=FUN_00131868 lang=c++ */
/* Clone of src/eboot/adhoc_pdp_init_001265f8.cpp (hstclone.py tag c211507): addresses substituted; exact match verified; names and types await human review. */
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
    int FUN_002c7b98(int, unsigned short, int, unsigned char);
    void *__nw__FUi(unsigned int);
    void __dl__FPv(void *);
    Stream *FUN_00104d94(void *, int);
    int FUN_002c7630(const char *, int, int, int, int);
    void FUN_00131768(const char *, ...);
    int FUN_002c76c0(const char *, void (*)(), int, int, int, int);
    void FUN_00131540();
    int FUN_002c7678(int, int, void *);
    int FUN_002c7748(int);
    int FUN_002c7688(int);
    extern const char DAT_0038134c;
    extern const char DAT_0038135c;
    extern const char DAT_00381388;
    extern const char DAT_00381398;
    extern const char DAT_003813c4;
    extern const char DAT_003813f0;
    extern const char DAT_00381304;
}

extern "C" int FUN_00131868(Obj *self, int param_2, unsigned int param_3, int param_4, int param_5)
{
    ThreadArgs args;

    args.pdp_id = self->pdp_id = FUN_002c7b98(param_2, (unsigned short)param_3, param_4, param_5);

    unsigned short *w = new unsigned short(0);
    self->p_u16 = w;
    args.p_u16 = w;
    unsigned char *b = new unsigned char(0);
    self->p_u8 = b;
    args.p_u8 = b;
    Stream *s = (Stream *)__nw__FUi(0x14);
    if (s != 0) {
        s = FUN_00104d94(s, 0x4000);
    }
    self->s1 = s;
    args.s1 = s;
    s = (Stream *)__nw__FUi(0x14);
    if (s != 0) {
        s = FUN_00104d94(s, 0x4000);
    }
    self->s2 = s;
    args.s2 = s;

    args.param_3 = param_3;
    int sema = FUN_002c7630(&DAT_0038134c, 0, 1, 1, 0);
    if (sema <= 0) {
        FUN_00131768(&DAT_0038135c, sema);
    } else {
        args.semaid = self->semaid = sema;
        int th = FUN_002c76c0(&DAT_00381388, FUN_00131540, 0x29, 0x2000, 0, 0);
        if (th <= 0) {
            FUN_00131768(&DAT_00381398, th);
        } else {
            self->thid = th;
            int ret = FUN_002c7678(th, 0x28, &args);
            if (ret != 0) {
                FUN_00131768(&DAT_003813c4, ret);
            } else {
                return 1;
            }
        }
    }

    if (self->thid > 0) {
        int ret = FUN_002c7748(self->thid);
        if (ret != 0) {
            FUN_00131768(&DAT_003813f0, ret);
        }
        self->thid = -1;
    }
    if (self->semaid > 0) {
        int ret = FUN_002c7688(self->semaid);
        if (ret != 0) {
            FUN_00131768(&DAT_00381304, ret);
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
