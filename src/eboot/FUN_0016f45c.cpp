/* HSTMATCH module=EBOOT.BIN start=0x0016f45c end=0x0016f970 symbol=FUN_0016f45c lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern char DAT_002e0534;
extern char DAT_002e0540;
extern char DAT_002e055c;
extern char DAT_002e0584;
extern char DAT_002e05b0;
extern char DAT_002e05dc;
extern int DAT_0030ebf4;
extern int DAT_0034179c;
extern void *DAT_0034b340[];
extern int DAT_0034a678;
extern int DAT_003386c8;

struct Tag {};

struct BaseObj {
    virtual ~BaseObj();
};

struct Elem {
    int f0;
    int f4;
    char f8;
    char f9;
    char fa;
    char fb;
    char fc;
    int f10;

    Elem() {
        f4 = 0;
        f0 = 0;
        f10 = 0;
        f8 = 0;
        f9 = 0;
        fa = 0;
        fb = 0;
        fc = 0;
    }
};

extern "C" {
void FUN_00000020(void *);
void FUN_00000028(void);
void FUN_0001ca58(void *, void *);
void FUN_0001cb88(void *);
void FUN_002042c8(void);
void FUN_0017ae48(void *, void *, void *, int);
void FUN_0017b260(void *, void *, void *, int);
void FUN_000651b8(void *, void *, int, int, void *);
void FUN_001b2d90(void *);
int FUN_0016ca34(void *, int, int, int, int, int, void *, void *, int);
int FUN_0016b164(void);
void FUN_0016c040(int, void *, int, int, int, int, int, int);
void FUN_001798f4(void *, unsigned int);
void FUN_001797fc(void *, const Elem *const *, Tag);
void FUN_0016c574(void *, void *, int, int);
void FUN_0016cf60(void *, void *);
void FUN_0016f970(void *, int);
int FUN_001feec4(void);
void FUN_001705ac(void *, void *, int);
void FUN_0017fff4(int);
void FUN_0017bde0(int);
void FUN_00281128(int);
}

void *operator new(unsigned int) throw();

extern "C" int FUN_0016f45c(unsigned char *self, Tag, Tag, Tag, Tag)
{
    Elem *local_4;
    Elem *local_8;

    FUN_00000020(&DAT_002e0534);
    FUN_0001ca58(&DAT_0030ebf4, &DAT_002e0540);
    FUN_002042c8();
    FUN_0017ae48(self + 0x2a4, self + 0x214, &DAT_002e055c, 0x78);
    FUN_0017b260(self + 0x31c, self + 0x244, &DAT_002e0584, 0xcc);
    FUN_000651b8(self + 0x274, &DAT_002e05b0, 1, 0, &DAT_002e05dc);
    FUN_001b2d90(&DAT_0034179c);
    FUN_0001cb88(&DAT_0030ebf4);

    int uVar1 = ((int (*)(void))(*(int *)(self + 0x540)))();
    FUN_0016ca34(self + 0x4fc, uVar1, self[0x44], self[0x46], 0, 0, self + 0x4f0, self + 0x80, 0);

    uVar1 = FUN_0016b164();
    FUN_0016ca34(self + 0x520, uVar1, self[0x44], self[0x46], self[0x40], 1, self + 0x514, self + 0x80, 1);

    FUN_0016c040(5, self + 0x1a4, 0, self[0x44], self[0x46], 0, 0, 0);
    FUN_0016c040(1, self + 0x1d8, 1, self[0x44], self[0x46], 0, 0, 0);
    FUN_0016c040(2, self + 0x1f0, 1, self[0x44], self[0x46], 0, 0, 0);
    FUN_0016c040(3, self + 0x208, 0, self[0x44], self[0x46], 0, 0, 0);

    if (*(unsigned int *)(self + 0x1ec) < *(unsigned int *)(self + 0x1dc)) {
        FUN_001798f4(self + 0x1e4, *(unsigned int *)(self + 0x1dc));
    }
    for (int i = 0; (unsigned int)i < *(unsigned int *)(self + 0x1dc); i++) {
        local_4 = new Elem();
        if (*(unsigned int *)(self + 0x1e8) < *(unsigned int *)(self + 0x1ec)) {
            int idx = *(unsigned int *)(self + 0x1e8) + 1;
            *(int *)(self + 0x1e8) = idx;
            *(Elem **)(*(int *)(self + 0x1e4) + idx * 4 - 4) = local_4;
        } else {
            FUN_001797fc(self + 0x1e4, &local_4, Tag());
        }
    }

    if (*(unsigned int *)(self + 0x204) < *(unsigned int *)(self + 500)) {
        FUN_001798f4(self + 0x1fc, *(unsigned int *)(self + 500));
    }
    for (int i = 0; (unsigned int)i < *(unsigned int *)(self + 500); i++) {
        local_8 = new Elem();
        if (*(unsigned int *)(self + 0x200) < *(unsigned int *)(self + 0x204)) {
            int idx = *(unsigned int *)(self + 0x200) + 1;
            *(int *)(self + 0x200) = idx;
            *(Elem **)(*(int *)(self + 0x1fc) + idx * 4 - 4) = local_8;
        } else {
            FUN_001797fc(self + 0x1fc, &local_8, Tag());
        }
    }

    FUN_0016c574(self + 0x1b0, self + 0x1a4, 2, self[0x46]);
    FUN_0016c574(self + 0x1bc, self + 0x1a4, 1, self[0x46]);
    FUN_0016c574(self + 0x1c8, self + 0x1a4, 0, self[0x46]);
    FUN_0016cf60(self + 0x508, self + 0x4fc);
    FUN_0016cf60(self + 0x52c, self + 0x520);

    if (self[0x3f] != 0) {
        int v2 = ((int (*)(void))(*(int *)(self + 0x540)))();
        int v3 = FUN_001feec4();
        FUN_0016f970(self, *(short *)(v3 + 0x40));
        if (v2 != ((int (*)(void))(*(int *)(self + 0x540)))()) {
            if (self[0x4f] != 0) {
                self[0x4f] = 0;
                if (DAT_0034b340[*(int *)(self + 0x58)] != 0) {
                    delete (BaseObj *)DAT_0034b340[*(int *)(self + 0x58)];
                    DAT_0034b340[*(int *)(self + 0x58)] = 0;
                }
            }
        }
    } else {
        int v2 = ((int (*)(void))(*(int *)(self + 0x540)))();
        FUN_0016f970(self, -1);
        if (v2 != ((int (*)(void))(*(int *)(self + 0x540)))()) {
            if (self[0x4e] != 0) {
                self[0x4e] = 0;
                if (DAT_0034b340[*(int *)(self + 0x54)] != 0) {
                    delete (BaseObj *)DAT_0034b340[*(int *)(self + 0x54)];
                    DAT_0034b340[*(int *)(self + 0x54)] = 0;
                }
            }
        }
    }

    uVar1 = ((int (*)(void))(*(int *)(self + 0x540)))();
    FUN_001705ac(self, self + 0x4f0, uVar1);
    FUN_0017fff4(*(int *)(self + 0x174));
    FUN_0017bde0(*(int *)(self + 0x178));

    switch (DAT_0034a678) {
    case 38:
    case 45:
        if (DAT_003386c8 != 0) {
            FUN_00281128(*(int *)(DAT_003386c8 + 0x40));
        }
        break;
    }

    *(int *)(self + 0x6c) = 1;
    FUN_00000028();
    return 1;
}
