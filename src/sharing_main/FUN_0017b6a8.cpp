/* HSTMATCH module=hstp_trans_main.prx start=0x0017b6a8 end=0x0017bbbc symbol=FUN_0017b6a8 lang=c++ */
/* Clone of src/eboot/FUN_0016f45c.cpp (hstclone.py tag dsn45): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern char DAT_00387b3c;
extern char DAT_00387b48;
extern char DAT_00387b64;
extern char DAT_00387b8c;
extern char DAT_00387bb8;
extern char DAT_00387be4;
extern int DAT_003b7ebc;
extern int DAT_003eaad8;
extern void *DAT_003f468c[];
extern int DAT_003f39a8;
extern int DAT_003e1a08;

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
void FUN_0001c5f4(void *, void *);
void FUN_0001c73c(void *);
void FUN_00212b30(void);
void FUN_0018753c(void *, void *, void *, int);
void FUN_00187954(void *, void *, void *, int);
void FUN_00066260(void *, void *, int, int, void *);
void FUN_001c0ec0(void *);
int FUN_00178cac(void *, int, int, int, int, int, void *, void *, int);
int FUN_00177328(void);
void FUN_001782b8(int, void *, int, int, int, int, int, int);
void FUN_00185fe8(void *, unsigned int);
void FUN_00185ef0(void *, const Elem *const *, Tag);
void FUN_001787ec(void *, void *, int, int);
void FUN_001791d8(void *, void *);
void FUN_0017bbbc(void *, int);
int FUN_0020d694(void);
void FUN_0017c7f8(void *, void *, int);
void FUN_0018c6e8(int);
void FUN_001884d4(int);
void FUN_002924a0(int);
}

void *operator new(unsigned int) throw();

extern "C" int FUN_0017b6a8(unsigned char *self, Tag, Tag, Tag, Tag)
{
    Elem *local_4;
    Elem *local_8;

    FUN_00000020(&DAT_00387b3c);
    FUN_0001c5f4(&DAT_003b7ebc, &DAT_00387b48);
    FUN_00212b30();
    FUN_0018753c(self + 0x2a4, self + 0x214, &DAT_00387b64, 0x78);
    FUN_00187954(self + 0x31c, self + 0x244, &DAT_00387b8c, 0xcc);
    FUN_00066260(self + 0x274, &DAT_00387bb8, 1, 0, &DAT_00387be4);
    FUN_001c0ec0(&DAT_003eaad8);
    FUN_0001c73c(&DAT_003b7ebc);

    int uVar1 = ((int (*)(void))(*(int *)(self + 0x540)))();
    FUN_00178cac(self + 0x4fc, uVar1, self[0x44], self[0x46], 0, 0, self + 0x4f0, self + 0x80, 0);

    uVar1 = FUN_00177328();
    FUN_00178cac(self + 0x520, uVar1, self[0x44], self[0x46], self[0x40], 1, self + 0x514, self + 0x80, 1);

    FUN_001782b8(5, self + 0x1a4, 0, self[0x44], self[0x46], 0, 0, 0);
    FUN_001782b8(1, self + 0x1d8, 1, self[0x44], self[0x46], 0, 0, 0);
    FUN_001782b8(2, self + 0x1f0, 1, self[0x44], self[0x46], 0, 0, 0);
    FUN_001782b8(3, self + 0x208, 0, self[0x44], self[0x46], 0, 0, 0);

    if (*(unsigned int *)(self + 0x1ec) < *(unsigned int *)(self + 0x1dc)) {
        FUN_00185fe8(self + 0x1e4, *(unsigned int *)(self + 0x1dc));
    }
    for (int i = 0; (unsigned int)i < *(unsigned int *)(self + 0x1dc); i++) {
        local_4 = new Elem();
        if (*(unsigned int *)(self + 0x1e8) < *(unsigned int *)(self + 0x1ec)) {
            int idx = *(unsigned int *)(self + 0x1e8) + 1;
            *(int *)(self + 0x1e8) = idx;
            *(Elem **)(*(int *)(self + 0x1e4) + idx * 4 - 4) = local_4;
        } else {
            FUN_00185ef0(self + 0x1e4, &local_4, Tag());
        }
    }

    if (*(unsigned int *)(self + 0x204) < *(unsigned int *)(self + 500)) {
        FUN_00185fe8(self + 0x1fc, *(unsigned int *)(self + 500));
    }
    for (int i = 0; (unsigned int)i < *(unsigned int *)(self + 500); i++) {
        local_8 = new Elem();
        if (*(unsigned int *)(self + 0x200) < *(unsigned int *)(self + 0x204)) {
            int idx = *(unsigned int *)(self + 0x200) + 1;
            *(int *)(self + 0x200) = idx;
            *(Elem **)(*(int *)(self + 0x1fc) + idx * 4 - 4) = local_8;
        } else {
            FUN_00185ef0(self + 0x1fc, &local_8, Tag());
        }
    }

    FUN_001787ec(self + 0x1b0, self + 0x1a4, 2, self[0x46]);
    FUN_001787ec(self + 0x1bc, self + 0x1a4, 1, self[0x46]);
    FUN_001787ec(self + 0x1c8, self + 0x1a4, 0, self[0x46]);
    FUN_001791d8(self + 0x508, self + 0x4fc);
    FUN_001791d8(self + 0x52c, self + 0x520);

    if (self[0x3f] != 0) {
        int v2 = ((int (*)(void))(*(int *)(self + 0x540)))();
        int v3 = FUN_0020d694();
        FUN_0017bbbc(self, *(short *)(v3 + 0x40));
        if (v2 != ((int (*)(void))(*(int *)(self + 0x540)))()) {
            if (self[0x4f] != 0) {
                self[0x4f] = 0;
                if (DAT_003f468c[*(int *)(self + 0x58)] != 0) {
                    delete (BaseObj *)DAT_003f468c[*(int *)(self + 0x58)];
                    DAT_003f468c[*(int *)(self + 0x58)] = 0;
                }
            }
        }
    } else {
        int v2 = ((int (*)(void))(*(int *)(self + 0x540)))();
        FUN_0017bbbc(self, -1);
        if (v2 != ((int (*)(void))(*(int *)(self + 0x540)))()) {
            if (self[0x4e] != 0) {
                self[0x4e] = 0;
                if (DAT_003f468c[*(int *)(self + 0x54)] != 0) {
                    delete (BaseObj *)DAT_003f468c[*(int *)(self + 0x54)];
                    DAT_003f468c[*(int *)(self + 0x54)] = 0;
                }
            }
        }
    }

    uVar1 = ((int (*)(void))(*(int *)(self + 0x540)))();
    FUN_0017c7f8(self, self + 0x4f0, uVar1);
    FUN_0018c6e8(*(int *)(self + 0x174));
    FUN_001884d4(*(int *)(self + 0x178));

    switch (DAT_003f39a8) {
    case 38:
    case 45:
        if (DAT_003e1a08 != 0) {
            FUN_002924a0(*(int *)(DAT_003e1a08 + 0x40));
        }
        break;
    }

    *(int *)(self + 0x6c) = 1;
    FUN_00000028();
    return 1;
}
