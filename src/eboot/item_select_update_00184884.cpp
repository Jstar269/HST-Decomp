/* HSTMATCH module=EBOOT.BIN start=0x00184884 end=0x00184afc symbol=FUN_00184884 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

typedef int (*Func)(int);
struct Mgr {
    char pad[0x43];
    unsigned char f43;
    char pad2[0x548 - 0x44];
    Func func548;
    int pad3;
    Func func550;
};
extern "C" Mgr *FUN_0016bc70();

inline int helper2(int x) {
    Mgr *mgr = FUN_0016bc70();
    if (x != 0x7fff && x != 0x7ffe) return mgr->func548(x);
    return 0;
}

struct Item {
    char pad[0x24];
    int f24;
    int f28;
};

struct ArrElem {
    Item *item;
    char pad[0x14 - 4];
};

struct Sub774 {
    void *f0;
    int pad[3];
    int f10;
};

struct Sub794 {
    char pad[68];
    Item *item;
};

struct Self {
    char pad_00[4];
    int f4;
    char pad_08[0x58 - 8];
    void *f58;
    char pad_5c[0x6c - 0x5c];
    void *f6c;
    char pad_70[0x760 - 0x70];
    void *f760;
    char pad_764[0x774 - 0x764];
    Sub774 s774;
    char pad_784[0x794 - 0x774 - sizeof(Sub774)];
    Sub794 s794;
    char pad_798[0x828 - 0x794 - sizeof(Sub794)];
    void *f828;
    char pad_82c[0x1180 - 0x82c];
    void *f1180;
};

extern "C" {
int FUN_0016d110(int);
void FUN_0010b980(void *, int, int);
void FUN_000ffb3c(void *, int);
void FUN_000ffc70(void *, int);
void FUN_0018379c(Self *, int, int, int);
void FUN_00184884(Self *self, unsigned int p2, int param_3);
}

void FUN_00184884(Self *self, unsigned int p2, int param_3) {
    int param_2 = (int)p2;
    int i;
    int sel;
    Mgr *m = FUN_0016bc70();
    if (param_2 != 0x7fff && param_2 != 0x7ffe) sel = m->func550(param_2);
    else sel = 0;

    if (param_3 != 0) {
        FUN_0010b980(self->f1180, sel, 0);
    } else {
        FUN_0010b980(self->f1180, 4, 0);
    }

    Sub774 *s1 = &self->s774;
    void **s0 = &self->f760;
    void **s2 = &self->f828;
    Mgr *mgr2 = FUN_0016bc70();

    if (mgr2->f43 != 0) {
        FUN_000ffb3c(s1->f0, 1);
        FUN_000ffb3c(*s0, 1);
        FUN_000ffb3c(*s2, 1);
    } else {
        FUN_000ffb3c(s1->f0, 0);
        FUN_000ffb3c(*s0, 0);
        FUN_000ffb3c(*s2, 0);
        if (param_3 != 0) {
            FUN_000ffc70(s1->f0, (int)0x80808080);
        } else {
            FUN_000ffc70(s1->f0, s1->f10);
        }
    }

    int s1_lim = FUN_0016d110(param_2);
    i = 0;
    int s2_off = 0;
    do {
        Sub794 *sub = (Sub794 *)((char *)self + (s2_off + 1940));
        Item **p = &sub->item;
        if (i < s1_lim) {
            if (i == sel) {
                FUN_000ffb3c(*p, 0);
                (*p)->f24 = (*p)->f28;
            } else {
                FUN_000ffb3c(*p, 1);
            }
        } else {
            FUN_000ffb3c(*p, 0);
            (*p)->f24 = (*p)->f28 + 1;
        }
        i++;
        s2_off += 20;
    } while (i < 4);

    void **v0 = &self->f58;
    void **s0_ptr = &self->f6c;
    if (param_3 != 0) {
        FUN_000ffb3c(*v0, 0);
        FUN_000ffb3c(*s0_ptr, 0);
    } else {
        FUN_000ffb3c(*v0, 1);
        FUN_000ffb3c(*s0_ptr, 1);
    }

    if (self->f4 != 0) {
        FUN_0018379c(self, helper2(param_2), 1, param_3);
    }
}
